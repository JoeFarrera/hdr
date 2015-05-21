// BuBThread.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "ClientSocket.h"
#include "Msg.h"
#include "TCPServerSocket.h"
#include "ClientSocket.h"
#include "GestorAgv.h"
#include "Orden.h"
#include "GestorOrden.h"
#include "dbutil.h"
#include "dbestacion.h"
#include "tpcView.h"

// #include "BackgroundTasks.h"
// #include "GestorEstacionBuB.h"

#include "TabSetFV.h"	//JLac

// #include "Bed_zs.h"
#include "Macro.h"
#include "DBEstadistica.h"

#include "MsgMacro.h"

// #include "AutomaticTrafficControl.h"


#include "BuBThread.h"


// CBuBThread

IMPLEMENT_DYNCREATE(CBuBThread, CWinThread)

CBuBThread::CBuBThread()
{
	m_pMsg = NULL;
	m_fConnected = false;
}

CBuBThread::~CBuBThread()
{
}

BOOL CBuBThread::InitInstance()
{
	StartSocketServer();
	// TODO:  perform and per-thread initialization here
	m_fRun = false;
	//m_fRun = true;
	g_Container.m_dbutil.ConectarOracle ("BuBThread");

	m_timerId = ::SetTimer(NULL,0,5000,NULL);

	g_Container.m_wBuBThreadId = GetCurrentThreadId ();


	m_iKeepAlive = 0;

	return TRUE;
}

int CBuBThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

// Procesar Peticion de Conexión de Clientes por sockets
void CBuBThread::ProcessPendingAccept(CSocket * pServerSocket)
{
	// Puede ser que estuviera conectado ya... ???
	// TODO: Look at case where connection is already made, as appears to not happen correctly

	SOCKADDR_IN sockAddr;
	int sockAddrLen = sizeof (SOCKADDR_IN);
	CClientSocket* pSocket = new CClientSocket(this);

	if (pServerSocket->Accept(*pSocket, (SOCKADDR *)&sockAddr, &sockAddrLen))
	{
		pSocket->Init();

		m_connectionList.AddTail(pSocket);

		wsprintf (m_strConnected.GetBuffer (20), "%d.%d.%d.%d", 
			sockAddr.sin_addr.S_un.S_un_b.s_b1,
			sockAddr.sin_addr.S_un.S_un_b.s_b2,
			sockAddr.sin_addr.S_un.S_un_b.s_b3,
			sockAddr.sin_addr.S_un.S_un_b.s_b4);
		m_strConnected.ReleaseBuffer ();
		CString strTemp;
		strTemp.Format ("Connection de cliente (BuB): %s", m_strConnected);
		MSGAVISO (strTemp);

		// Initialize reference connection
		m_pMsg = new CMsg;
		m_pMsg->m_ptrCliSock = pSocket;
		// Look at doing this for all connections. Use list
		m_fConnected = true;
	

	}
	else
	{
		delete pSocket;
		int nLast = GetLastError ();
		TRACE (_T("AcceptConnection: GetLastError is: %d\n"), nLast);
	}
}

void CBuBThread::StartSocketServer()
{
	TCHAR rgchServer [MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwLen = MAX_COMPUTERNAME_LENGTH;
	CString strConnect	= "Server";
	int nPort = 5678;	// M2015 TODO should be - take this out to parameter init
	BOOL fRetVal;
	
	m_pSocket = new CTCPServerSocket (this);


	if (! GetComputerName (rgchServer, &dwLen))
		strcpy_s (rgchServer, sizeof ("???"), "???");

	
	if ( m_pSocket->Create (nPort))
	{
		// SetTitle(strConnect + " " + "[" + rgchServer + "]");
		if (m_pSocket->TryListening ())
		{
			fRetVal = TRUE;
		}
		else
			fRetVal = FALSE;
	}
	else
	{
		CString strTemp;
		strTemp.Format ("Error de inicialización del Server Socket %d", GetLastError ());
		AfxMessageBox (strTemp);
		fRetVal = TRUE;
	}
	if (!fRetVal)
		AfxMessageBox ("Error de inicialización del Server Socket %d", GetLastError ());


}


// Procesar una peticion de lectura de un socket cliente
void CBuBThread::ProcessPendingRead(CClientSocket *pClientSocket)
{
	do
	{
		CMsg* pMsg = ReadMsg(pClientSocket);

		if (pMsg->m_bClose)
		{
			CloseSocket(pClientSocket);
			break;
		}
	}
	while (!pClientSocket->m_pArchiveIn->IsBufferEmpty());

	TRACE ("CBuBThread::ProcessPendingRead. Mensages en m_msgList: %d\n", m_msgList.GetCount());
	ProcessMessages ();
}

// Lectura de mensajes en el socket cliente pSocket
// Insertar mensajes leidos en m_msgList
CMsg * CBuBThread::ReadMsg(CClientSocket* pSocket)
{
	CMsg * ptrMsg = new CMsg;

	TRY
	{
		pSocket->ReceiveMsg((IMsg *)ptrMsg);
		
		MSGTRACE1 ("<< %s", ptrMsg->m_strText);

		//m_cs.Lock ();
		m_msgList.AddTail(ptrMsg);
		//m_cs.Unlock ();
	}
	CATCH(CFileException, e)
	{
		CString strTemp;
		if (strTemp.LoadString(IDS_READERROR))
			MSGERROR (strTemp);

		ptrMsg->m_bClose = TRUE;
		pSocket->Abort();
	}
	END_CATCH

	return ptrMsg;
}

void CBuBThread::CloseSocket(CClientSocket *pSocket)
{
	m_fConnected = false;
	pSocket->Abort();

	POSITION pos,temp;
	for(pos = m_connectionList.GetHeadPosition(); pos != NULL;)
	{
		temp = pos;
		CClientSocket* pSock = (CClientSocket*)m_connectionList.GetNext(pos);
		if (pSock == pSocket)
		{
			m_connectionList.RemoveAt(temp);
			break;
		}
	}

	delete pSocket;

}

void CBuBThread::ProcessMessages()
{
	for (; m_msgList.GetCount (); )
	{
		// m_cs.Lock ();
		CMsg * ptrMsg = (CMsg *)m_msgList.RemoveHead ();
		// m_cs.Unlock ();
		
		ASSERT (ptrMsg->m_ptrCliSock == m_pMsg->m_ptrCliSock);
		
		// Reset keepalive
		m_iKeepAlive = 0;

		//m_cs.Unlock ();
		if (ptrMsg->m_Tele.nData [0] == 'S' &&
			ptrMsg->m_Tele.nData [1] == 'Y')
		{
			// Synchro tele
			g_Container.SetSystemState(SYNCHRONISATION);
			SendSystemStatus (ptrMsg);
			SendSystemConstants (ptrMsg);

			// When to send this...XXX
			SendAllStations (ptrMsg, true);
			SendAllOrders (ptrMsg, true);
			SendAllAgvs (ptrMsg, true);
			
			// M2015 If Status is synchronizing, then set status to Working and send Again//
			g_Container.SetSystemState(BETRIEBSBEREIT);
			SendSystemStatus (ptrMsg);
		}
		else if (ptrMsg->m_Tele.nData [0] == 'T' &&
			ptrMsg->m_Tele.nData [1] == 'T')
		{
			// Usar como trigger de esta funcion. Mover... XXX
			MSGAVISO ("TT Telegram");
			// M2015 Do nothing!!
			// M2015 SendAllAgvs (ptrMsg, false);	// solo los que han sido actualizado
			
			// Michael 05.04.2015 M2015 Seems that this is the only reset to look in the database for orders
			// reset in CGestorOrden:UpdateEstado()
		
		}
		else if (ptrMsg->m_Tele.nData [0] == 'T' &&
			ptrMsg->m_Tele.nData [1] == 'I')
		{
			// M2015
			MSGTRACE ("TI Telegram");
			SendTITelegram(ptrMsg);
		}


		else if (ptrMsg->m_Tele.nData [0] == 'S' &&
			ptrMsg->m_Tele.nData [1] == 'K')
		{
			// System status command de BuB
			MSGAVISO ("SK Telegram");
			ProcessSKTelegram (ptrMsg);
		}
		else if (ptrMsg->m_Tele.nData [0] == 'A' &&
			ptrMsg->m_Tele.nData [1] == 'T')
		{
			// Orden de transporte nuevo o modificación de existente
			ProcessATTelegram (ptrMsg);
		}
		else if (ptrMsg->m_Tele.nData [0] == 'F' &&
			ptrMsg->m_Tele.nData [1] == 'K')
		{
			ProcessFKTelegram (ptrMsg);
		}
		else if (ptrMsg->m_Tele.nData [0] == 'M' &&
			ptrMsg->m_Tele.nData [1] == 'A')
		{
			ProcessMATelegram (ptrMsg);
		}

		else
		{
			MSGWARNING2 ("BuB telegram: %c %c desconocido", 
				ptrMsg->m_Tele.nData [0],
				ptrMsg->m_Tele.nData [1]);

		}
		delete ptrMsg;

	}

}




// Enviar el estado del sistema al cliente que lo solicita
void CBuBThread::SendSystemStatus(CMsg *ptrMsg)
{
	empf_z0_tele tele  = {'0'};   
	
	memset (&tele, 0, sizeof (tele));


	
	/* Telegrammkennung eintragen */
	tele.TKopf.ucId[0]    = 'Z';
	tele.TKopf.ucId[1]    = '0';
	tele.TKopf.usType     = 0;
	tele.TKopf.usValid    = 1;
	tele.TKopf.usIndex    = 0;
	tele.TKopf.usSize     = sizeof(TYP_SYSTEMDATEN);
	tele.TKopf.usMaxItems = 1;

    tele.TZ0.anla_zust = g_Container.GetSystemState ();	
	tele.TZ0.anla_fahrt = g_Container.GetSystemDrive (); 
	tele.TZ0.betr_vaha = g_Container.GetSystemAuto (); 
	tele.TZ0.betr_art_host = g_Container.GetSystemHostConnection (); 
	tele.TZ0.FzSammelStoerung = g_Container.GetNumAgvError ();  
	tele.TZ0.StationsSammelStoerung = g_Container.GetNumStationError ();
	tele.TZ0.numAgvAllowed = g_Container.GetTotalAgvsAllowed ();
	tele.TZ0.numAgvEnCircuito = g_pGestor->GetAgvsEnCircuito ();
	tele.TZ0.augmentoTiempoOrden = g_GestorOrden.GetAugmentoTiempo ();
	tele.TZ0.tiempoPartidaOrden = g_GestorOrden.GetPartidaOrden ();
	tele.TZ0.frecuenciaOrden = g_GestorOrden.GetFrecuenciaOrden ();
	tele.TZ0.startTime = (UINT)g_Container.m_StartTime.GetTime();
	tele.TZ0.fAutoStart = g_Container.m_fAutoStart;
	tele.TZ0.fCaminoAlt = g_Container.m_fCaminoAlternativo;
	tele.TZ0.fCaminoAlt203 = g_Container.m_fCaminoAlternativo203;
	tele.TZ0.fCaminoAlt303 = g_Container.m_fCaminoAlternativo303;
	strncpy (tele.TZ0.szRadio, g_Container.m_strRadio, 30); 
	strncpy (tele.TZ0.szTren, g_Container.m_strTren, 30);
	tele.TZ0.fPLCEnLinea = g_Container.m_fSiemensFatalError; // Maybe not. Look at host connection


	CMsg msg;

	msg.m_Tele.nTeleLaenge = sizeof (empf_z0_tele);
	msg.m_Tele.nRetStatus = 1;
	msg.m_Tele.nOnOff = 1;
	msg.m_Tele.nPcNr = 1;

	int	sizethis = sizeof (tTeleKopf);
		sizethis = sizeof (TYP_SYSTEMDATEN);

	sizethis = sizeof (TYP_2008);

	sizethis = sizeof (CTime);

	memcpy (msg.m_Tele.nData, &tele, sizeof (empf_z0_tele));

	TRY
	{
		ptrMsg->m_ptrCliSock->SendMsg((IMsg *)&msg);
	}
	CATCH(CFileException, e)
	{
		ptrMsg->m_ptrCliSock->Abort();

		CString strTemp;
		if (strTemp.LoadString(IDS_SENDERROR))
			MSGERROR (strTemp);
	}
	END_CATCH
}


void CBuBThread::SendTITelegram(CMsg *ptrMsg)
{
	CMsg msg;
	empf_ti_hdr_tele tele  = {'0'};   
	
	memset (&tele, 0, sizeof (tele));


	
	/* Telegrammkennung eintragen */
	tele.TKopf.ucId[0]    = 'T';
	tele.TKopf.ucId[1]    = 'I';
	tele.TKopf.usType     = 0;
	tele.TKopf.usValid    = 1;
	tele.TKopf.usIndex    = 0;
	tele.TKopf.usSize     = sizeof(TYP_DATUM);
	tele.TKopf.usMaxItems = 1;

	SYSTEMTIME sysTime;
	CTime theTime = CTime::GetCurrentTime();
	

	// Zeitelegramm
		
	theTime.GetAsSystemTime(sysTime);



	//msg.m_Tele.nTeleLaenge = 11;
	//msg.m_Tele.nRetStatus  = 0;
	//msg.m_Tele.nOnOff = 1;
	//msg.m_Tele.nPcNr = 1;
	//msg.m_Tele.nData[0]  = 'T';
	//msg.m_Tele.nData[1]  = 'I';
	//msg.m_Tele.nData[2]  = 0;
	//msg.m_Tele.nData[3]  = 0;
	//msg.m_Tele.nData[4]  = sysTime.wYear & 0xFF;
	//msg.m_Tele.nData[5]  = sysTime.wYear>>8 & 0xFF;
	//msg.m_Tele.nData[6]  = (unsigned char) sysTime.wMonth;
	//msg.m_Tele.nData[7]  = (unsigned char) sysTime.wDay;
	//msg.m_Tele.nData[8]  = (unsigned char) sysTime.wHour;
	//msg.m_Tele.nData[9]  = (unsigned char) sysTime.wMinute;
	//msg.m_Tele.nData[10] = (unsigned char) sysTime.wSecond;

	
	tele.date.jahr = sysTime.wYear;
	tele.date.monat  = (unsigned char) sysTime.wMonth;
	tele.date.tag = (unsigned char) sysTime.wDay;
	tele.date.stunde  = (unsigned char) sysTime.wHour;
	tele.date.minute  = (unsigned char) sysTime.wMinute;
	tele.date.sekunde = (unsigned char) sysTime.wSecond;


	msg.m_Tele.nTeleLaenge = sizeof (empf_ti_hdr_tele);
	msg.m_Tele.nRetStatus = 0;
	msg.m_Tele.nOnOff = 1;
	msg.m_Tele.nPcNr = 1;

	memcpy (msg.m_Tele.nData, &tele, sizeof (empf_ti_hdr_tele));

	TRY
	{
		ptrMsg->m_ptrCliSock->SendMsg((IMsg *)&msg);
	}
	CATCH(CFileException, e)
	{
		ptrMsg->m_ptrCliSock->Abort();

		CString strTemp;
		if (strTemp.LoadString(IDS_SENDERROR))
			MSGERROR (strTemp);
	}
	END_CATCH

}


void CBuBThread::SendSystemConstants(CMsg *ptrMsg)
{
	empf_ak_tele tele = {'0'};
	
	tele.TKopf.ucId[0]    = 'A';
	tele.TKopf.ucId[1]    = 'K';
	tele.TKopf.usType     = 0;
	tele.TKopf.usValid    = 1;
	tele.TKopf.usIndex    = 0;
	tele.TKopf.usSize     = sizeof(TYP_ANLAGEKONSTANTEN);
	tele.TKopf.usMaxItems = 1;
	
	//tele.TAK.nAnzFZ         = KKFZMAX;
	//tele.TAK.nAnzFZTypen    = 1;
	//tele.TAK.nMaxMP         = KKMAXMP;
	//tele.TAK.nAnzMP         = KKMAX_STO;
	//tele.TAK.nAnzStationen  = KMAX_STATIONEN;
	CCourseAGVType* pCourse = g_pCourseData->GetPtrCourseAGVType(1);
	tele.TAK.nAnzFZ = g_pCourseData->GetNumberAGVs();
	tele.TAK.nAnzFZTypen = 1;
	tele.TAK.nMaxMP = g_pCourseData->GetMaxAIDCp();
	tele.TAK.nAnzMP = g_pCourseData->GetNumberAIDCps();
	tele.TAK.nAnzStationen = g_pCourseData->GetNumberAIDStations();

	// TODO Michael 2015.02.12

	tele.TAK.nAnzAusgangsByte = 0;
	tele.TAK.nAnzEingangsByte = 0;
	tele.TAK.nAnzMerkerByte = 0;
	strcpy_s(tele.TAK.ProjektName, sizeof(tele.TAK.ProjektName), g_pCourseData->GetProjectName().GetBuffer());

	CMsg msg;
	msg.m_Tele.nTeleLaenge = sizeof (empf_ak_tele);
	msg.m_Tele.nRetStatus = 1;
	msg.m_Tele.nOnOff = 1;
	msg.m_Tele.nPcNr = 1;

	memcpy (msg.m_Tele.nData, &tele, sizeof (empf_ak_tele));

	TRY
	{
		ptrMsg->m_ptrCliSock->SendMsg((IMsg *)&msg);
	}
	CATCH(CFileException, e)
	{
		ptrMsg->m_ptrCliSock->Abort();

		CString strTemp;
		if (strTemp.LoadString(IDS_SENDERROR))
			MSGERROR (strTemp);
	}
	END_CATCH

}



void CBuBThread::SendAllStations(CMsg *ptrMsg, bool sendAll)
{

	empf_s0_tele tele = {'0'};

	if (ptrMsg == NULL)
		ptrMsg = m_pMsg;	

	if (ptrMsg == NULL)	// m_pMsg is null
	{
		MSGERROR ("SendAllStations (): No hay cliente BuB conectado");
		return;
	}

	CMsg msg;
#if !defined (FOREXPORTTOINDUMAT)
	CDBEstacion dbEstacion;
#endif

	msg.m_Tele.nTeleLaenge = sizeof (empf_s0_tele);
	msg.m_Tele.nRetStatus = 1;
	msg.m_Tele.nOnOff = 1;
	msg.m_Tele.nPcNr = 1;

	UINT numStations = g_Container.m_GestorEstBuB.m_EstacionPtrArray.GetSize ();

	tele.TKopf.ucId[0]    = 'S';
	tele.TKopf.ucId[1]    = '0';
	tele.TKopf.usType     = 0;
	tele.TKopf.usValid    = 1;
	// M2015 tele.TKopf.usSize     = sizeof(TYP_STATION);
	tele.TKopf.usSize     = sizeof(TYPE_AID_STATION);
	tele.TKopf.usMaxItems = numStations;


	for (UINT myi = 0; myi < numStations; myi ++)
	{
		CEstacionBuB * ptrEstBuB =  g_Container.m_GestorEstBuB.m_EstacionPtrArray.GetAt (myi);
		if (ptrEstBuB->SetTelegramBuB (&tele, sendAll))
		{
			tele.TKopf.usIndex = myi;
			memcpy (msg.m_Tele.nData, &tele, sizeof (empf_s0_tele));
			
			TRY
			{
				ptrMsg->m_ptrCliSock->SendMsg((IMsg *)&msg);
			}
			CATCH(CFileException, e)
			{
				ptrMsg->m_ptrCliSock->Abort();
				
				CString strTemp;
				if (strTemp.LoadString(IDS_SENDERROR))
					MSGERROR (strTemp);
			}
			END_CATCH
		}
	}
	return;



/*
	int AnzahlPosLage = 0;	// Number of positions ?
	for (int i = 1; i <= KMAX_STATIONEN; i++)
	{
		AnzahlPosLage += statdat[i].pos_anz_r;
		AnzahlPosLage += statdat[i].pos_anz_l;
	}

	tele.TKopf.ucId[0] = 'S';
	tele.TKopf.ucId[1] = '0';
	tele.TKopf.usType = 1;
	tele.TKopf.usValid = 1;
	tele.TKopf.usSize = sizeof(TYP_STATION);
	tele.TKopf.usMaxItems = AnzahlPosLage;


	for (int i = 1; i <= KMAX_STATIONEN; i++)
	{
		// XXX dbEstacion.InsertarEstacion (i);
		// Lefthand side
		int pos;
		for (pos = 0; pos < statdat[i].pos_anz_l; pos++)
		{
			tele.TKopf.usIndex = i;
			tele.TStation.mp = statdat[i].mp; // Ziel.nr;
			tele.TStation.pos = pos + 1; //?? Ziel.pos;
			tele.TStation.lage = 'L'; // Ziel.lage;
			tele.TStation.typ = (unsigned char)e_typ_tab[z_typ_tab[zt_ver_tab[statdat[i].zieltyp_l]].index].ziel_art; // ermittle_ziel_typ(Ziel);
			tele.TStation.gesp = (unsigned char)statdat[i].dyn.gesp_l; // ziel_gesperrt(Ziel);
			tele.TStation.gest = (unsigned char)statdat[i].dyn.gest_l; // ziel_gestoert(Ziel);
			tele.TStation.prio = statdat[i].dyn.prio;
			tele.TStation.last = 0;	// Loaded or not
			tele.TStation.anz_ebenen = 1; // Number of layers at the position
			tele.TStation.fz_typ = 0;	// Agv type is always 0 in HdR

			wsprintf((char *)tele.TStation.KundenNr, (LPTSTR)&kn_tab[statdat[i].ziel_index_l + pos]);

			memcpy(msg.m_Tele.nData, &tele, sizeof (empf_s0_tele));

			TRY
			{
				ptrMsg->m_ptrCliSock->SendMsg(&msg);
			}
			CATCH(CFileException, e)
			{
				ptrMsg->m_ptrCliSock->Abort();

				CString strTemp;
				if (strTemp.LoadString(IDS_SENDERROR))
					MSGERROR(strTemp);
			}
			END_CATCH
		}
		// right side
		for (pos = 0; pos < statdat[i].pos_anz_r; pos++)
		{
			tele.TKopf.usIndex = i;
			tele.TStation.mp = statdat[i].mp; // Ziel.nr;
			tele.TStation.pos = pos + 1; // Michael 15.09.2001 statdat [i].pos_anz_r; //?? Ziel.pos;
			tele.TStation.lage = 'R'; // Ziel.lage;
			tele.TStation.typ = (unsigned char)e_typ_tab[z_typ_tab[zt_ver_tab[statdat[i].zieltyp_r]].index].ziel_art;

			tele.TStation.gesp = (unsigned char)statdat[i].dyn.gesp_r; // ziel_gesperrt(Ziel);
			tele.TStation.gest = (unsigned char)statdat[i].dyn.gest_r; // ziel_gestoert(Ziel);
			tele.TStation.prio = statdat[i].dyn.prio;
			tele.TStation.last = 0;	// ??
			tele.TStation.anz_ebenen = 1; // ?? AnzahlEbenen(Ziel);
			wsprintf((char *)tele.TStation.KundenNr, (LPTSTR)&kn_tab[statdat[i].ziel_index_r + pos]);

			memcpy(msg.m_Tele.nData, &tele, sizeof (empf_s0_tele));

			TRY
			{
				ptrMsg->m_ptrCliSock->SendMsg(&msg);
			}
			CATCH(CFileException, e)
			{
				ptrMsg->m_ptrCliSock->Abort();

				CString strTemp;
				if (strTemp.LoadString(IDS_SENDERROR))
					MSGERROR(strTemp);
			}
			END_CATCH
		}
	} // for
*/	
}

// Enviar telegrama de AGV
void CBuBThread::SendF0Telegram(CMsg *ptrMsg, UINT nAgv, bool sendAll)
{
	if (g_pGestor != NULL)
	{
		empf_f0_tele tele = {'0'};
		
		CAgv * ptrAgv = g_pGestor->GetAgvPtr (nAgv);
		
		if (ptrAgv->m_fBuB || sendAll)
		{
			// Enviar solo si tiene datos validos
			if (true /* ptrAgv->GetEstado () != AGVFUERASISTEMA /* && FZ_IM_SYSTEM(*(ptrAgv->m_pFzdat))*/ )
			{
				ptrAgv->ResetBuB ();
				// Enviar solo si hay cambios o si ha sido indicado
				tele.TKopf.ucId[0]    = 'F';
				tele.TKopf.ucId[1]    = '0';
				tele.TKopf.usType     = 0;
				tele.TKopf.usValid    = 1;
				tele.TKopf.usIndex    = nAgv - 1;
				tele.TKopf.usSize     = sizeof(TYP_FZDAT);
				//tele.TKopf.usMaxItems = KKFZMAX;
				tele.TKopf.usMaxItems = g_pCourseData->GetNumberAGVs();

				
				memcpy ((void *)&tele.FzDat, ptrAgv->GetFzdatPtr (), sizeof (TYP_FZDAT));
				// Copy Agv estado for display purposes in the BuB.
				// NOTE: This is duplicating data - but otherwise requires extensive mods.
				tele.FzDat.uEstado = ptrAgv->GetEstado ();
				tele.FzDat.fCargaBateriaBuB = ptrAgv->IsCargaBateriaBuB ();
				tele.FzDat.fEnCircuito = ptrAgv->IsEnCircuito ();
				
				CMsg msg;
				msg.m_Tele.nTeleLaenge = sizeof (empf_f0_tele);
				msg.m_Tele.nRetStatus = 1;
				msg.m_Tele.nOnOff = 1;
				msg.m_Tele.nPcNr = 1;
				
				
				memcpy (msg.m_Tele.nData, &tele, sizeof(empf_f0_tele));
				
				TRY
				{
					//JLac, 21.04.2015:
					TRACE(
						"BuBThread: Sending F0: AGV%d: Track Cp%dNCp%d, driven %dmm\n",
						nAgv,
						(int)tele.FzDat.mp,
						(int)tele.FzDat.nmp,
						(int)tele.FzDat.wegstr);

					ptrMsg->m_ptrCliSock->SendMsg((IMsg *)&msg);
				}
				CATCH(CFileException, e)
				{
					ptrMsg->m_ptrCliSock->Abort();
					
					CString strTemp;
					if (strTemp.LoadString(IDS_SENDERROR))
						MSGERROR (strTemp);
				}
				END_CATCH
			} // AGVFUERASISTEMA
		} // sendAll
	} // g_pGestor
}

// System status command de BuB
void CBuBThread::ProcessSKTelegram(CMsg *ptrMsg)
{

	send_sk_tele * ptrTele;


	ptrTele = (send_sk_tele *)&ptrMsg->m_Tele.nData;

	CString strTemp;
	strTemp.Format (_T("SK tele: aktion: %d wert: %d..."), 
			ptrTele->aktion, ptrTele->wert);
	MSGTRACE (strTemp);

	switch (ptrTele->aktion)
		{
  		case TK_SK_SYSTEM:
			/* Michael 17.04.2002
			if (ptrTele->wert == SYNCHRONISATION ||
				ptrTele->wert == WECKEN)
			{
				g_Container.SetSystemState (BETRIEBSBEREIT);
			}
			else
			{	
			*/
				if (FEIERABEND == (unsigned char)ptrTele->wert)
				{
					CTime curTime = CTime::GetCurrentTime ();
					if (g_Container.m_fAutoStart && g_Container.m_StartTime < curTime)
						{
						// Desactivar autostart ya que la hora es anterior al actual
						g_Container.m_fAutoStart = false;
						}
					// Fin de turno: 

	
				}
				g_Container.SetSystemState ((unsigned char)ptrTele->wert);
			// Michael 17.04.2002 fin }
			break;
  		case TK_SK_FAHRSTATUS: 
			// Drive Status
			g_Container.SetSystemDrive ((unsigned char)ptrTele->wert);
			break;
	  	case TK_SK_BETRIEBSART_HOST:
			g_Container.SetSystemHostConnection ((unsigned char)ptrTele->wert);
			break;
		case TK_SK_SYSTEMZUSTAND:
			g_Container.SetSystemAuto ((unsigned char)ptrTele->wert);
			break;
		case TK_SK_TIEMPO_ORDEN:
			g_GestorOrden.SetAugmentoTiempo (ptrTele->augmentoTiempoOrden);
			g_GestorOrden.SetFrecuenciaOrden (ptrTele->frecuenciaOrden);
			g_GestorOrden.SetTiempoPartidaOrden (ptrTele->tiempoPartidaOrden);
			break;
		case TK_SK_PROG_START:
			g_Container.SetStartTime (ptrTele->startTime, ptrTele->fAutoStart);
			break;
		case TK_SK_CAMINO_ALT:
			g_Container.SetCaminoAlt (ptrTele->fCaminoAlt == TRUE ? true : false);
			g_Container.SetCaminoAlt203 (ptrTele->fCaminoAlt203 == TRUE ? true : false);
			g_Container.SetCaminoAlt303 (ptrTele->fCaminoAlt303 == TRUE ? true : false);
			break;
		// Anular la carga de bateria para todos los AGVs 08.11.2005
		case TK_SK_NO_CARGAR_BAT:
			g_pGestor->AnularCargarBat();
			break;
		default:
			strTemp.Format (_T("SK tele: aktion: %d wert: %d... No interpretado !!"), 
					ptrTele->aktion, ptrTele->wert);
			MSGWARNING (strTemp);
			break;

			
	}
	

	SendClientQuitum (ptrMsg, ptrTele->TKopf.kenn, 
		ptrTele->TKopf.kenn, 
		0 /* ErrorCode */);

	SendSystemStatus (ptrMsg);

	// Michael 15.07.2002 Realizar limpieza si es fin de turno
	if (TK_SK_SYSTEM == ptrTele->aktion && FEIERABEND == (unsigned char)ptrTele->wert)
	{
		// Purgar ficheros
		CTpcView::BorrarLogs();
		// Purgar historicos
// Michael 31.08.2007 TODO Realizar fuera de la aplicación?		CDBEstadistica::BorrarHistoricos();
	}

}

// Send Quitum al BuB
void CBuBThread::SendClientQuitum(CMsg *ptrMsg,
							   unsigned char * kenn,
							   unsigned char * qkenn,
							   UINT errorCode)
{
	send_qt_tele tele = {'0'};
	memset (&tele, ' ', sizeof (tele));
	
	
	/* Telegrammkennung eintragen */
	tele.TKopf.ucId[0]    = 'Q';
	tele.TKopf.ucId[1]    = 'T';
	tele.TKopf.usType     = 0;
	tele.TKopf.usValid    = 1;
	tele.TKopf.usIndex    = 0;
	tele.TKopf.usSize     = sizeof (TYP_QUITTUNG);
	tele.TKopf.usMaxItems = 1;
	
	tele.TQuitt.kenn       = SwapBytes (qkenn);
	tele.TQuitt.qkenn      = SwapBytes (kenn);
	tele.TQuitt.error_code = errorCode; 

	CMsg msg;
	msg.m_Tele.nTeleLaenge = sizeof (send_qt_tele);
	msg.m_Tele.nRetStatus = 1;
	msg.m_Tele.nOnOff = 1;
	msg.m_Tele.nPcNr = ptrMsg->m_Tele.nPcNr;


	memcpy (msg.m_Tele.nData, &tele, sizeof(send_qt_tele));
	
	TRY
	{
		MSGAVISO ("Sending QT Telegram");
		ptrMsg->m_ptrCliSock->SendMsg((IMsg *)&msg);
		MSGAVISO ("QT Telegram sent");
	}
	CATCH(CFileException, e)
	{
		ptrMsg->m_ptrCliSock->Abort();

		CString strTemp;
		if (strTemp.LoadString(IDS_SENDERROR))
			MSGERROR (strTemp);
	}
	END_CATCH

}

unsigned short CBuBThread::SwapBytes(unsigned char * us)
{
	unsigned char *pc = us;
	// return (pc[0] << 8) + pc[1];
	return (pc[1] << 8) + pc[0];
}

// Enviar telegrama al BuB de las órdenes
void CBuBThread::SendA0Telegram(CMsg *ptrMsg, TYP_AT_DAT *atPtr, int index)
{
	empf_a0_tele tele = {'0'};
	
	tele.TKopf.ucId[0]    = 'A';
	tele.TKopf.ucId[1]    = '0';
	tele.TKopf.usType     = 0;
	tele.TKopf.usValid    = 1;
	tele.TKopf.usIndex    = index;	// XXX How to indicate indexes
	tele.TKopf.usSize     = sizeof(TYP_AT_DAT);
	tele.TKopf.usMaxItems = KMAX_AUFTRAG;	// XXX How main order items ?
	

	memcpy ((void *)&tele.AtDat, atPtr, sizeof (TYP_AT_DAT));

	CMsg msg;
	msg.m_Tele.nTeleLaenge = sizeof (empf_a0_tele);
	msg.m_Tele.nRetStatus = 1;
	msg.m_Tele.nOnOff = 1;
	msg.m_Tele.nPcNr = 1;

	
	memcpy (msg.m_Tele.nData, &tele, sizeof(empf_a0_tele));
	
	TRY
	{
		ptrMsg->m_ptrCliSock->SendMsg((IMsg *)&msg);
	}
	CATCH(CFileException, e)
	{
		ptrMsg->m_ptrCliSock->Abort();

		CString strTemp;
		if (strTemp.LoadString(IDS_SENDERROR))
			MSGERROR (strTemp);
	}
	END_CATCH

}

// Orden de transporte nuevo o modificación de existente
void CBuBThread::ProcessATTelegram(CMsg *ptrMsg)
{
	send_at_tele * ptrTele;
	bool fUpdateAll = false;	// True si hay que actualizar todas del BuB
	
	ptrTele = (send_at_tele *)&ptrMsg->m_Tele.nData;
	
	MSGTRACE1 (_T("AT tele: ident: %s"), ptrTele->id_nr);
	
	//	SendClientQuitum (ptrMsg, ptrTele->kenn, ptrTele->qkenn);
	
	TYP_AT_DAT at_neu;
	
	memset (&at_neu,0,sizeof(at_neu));
	
	at_neu.herkunft     = 'M';	// Manual Order
    
	at_neu.prio_org     = ptrTele->prioritaet; // Original Priority
    
	memcpy(&at_neu.hz,&ptrTele->hz,sizeof(at_neu.hz)); // Collection
   	memcpy(&at_neu.bz,&ptrTele->bz,sizeof(at_neu.bz)); // Delivery
    memcpy(&at_neu.id_nr[0], &ptrTele->id_nr[0], KMAXID_AT); // Identificación
    // XXX lasttyp Load type ??? at_neu.lasttyp = ptrTele->lasttyp;	// Load type (Load 1, Load 2, 1+2)

	// Revise all this initialization
	at_neu.af_status = 0;
	at_neu.akt_org = 0;	// Original Action
	at_neu.akt_z.nLayer= 0;
	at_neu.akt_z.ucSide = 0;
	at_neu.akt_z.nDest = 0;
	at_neu.akt_z.nPos = 0;
	at_neu.aktion = 1;
	at_neu.art = 2; // Not used
	at_neu.at_lauf_nr = 1;
	at_neu.bnr = 1;

	at_neu.fMesa1 = ptrTele->fMesa1;
	at_neu.fMesa2 = ptrTele->fMesa2;
	at_neu.fSimCarga = ptrTele->fSimCarga;

	// XXX Michael: See TYP_AT_DAT definition host_ss.c file for this action
    switch (ptrTele->auftr_aktion)
	{
	case TK_TA_NEU:
		{
			COrden * pOrden = new COrden;	// la insertamos
			memcpy ((void *)&(pOrden->m_at), &at_neu, sizeof (at_neu));
			g_GestorOrden.InsertarOrden (pOrden);
		}
		break;
	case TK_TA_AENDERN:
		{
			// Puede haber modificado la prioridad y el destino
			g_GestorOrden.BuBUpdateOrden (*ptrTele);

		}
		break;
	case TK_TA_LOESCHEN:
		{
			// Delete Order
			g_GestorOrden.BuBDeleteOrden (ptrTele->id_nr, false);
			fUpdateAll = true;
		}
		break;
	case TK_TA_QUITTIEREN:
		{
			// Finalizar telegrama como normalmente
			g_GestorOrden.BuBDeleteOrden (ptrTele->id_nr, true); 
			fUpdateAll = true;
		}
		break;
	case TK_TA_SPERREN:
		{
			// Suspend order
			g_GestorOrden.BuBSuspendReleaseOrden (*ptrTele);
			break;
		}
	case TK_TA_FREIGEBEN:
		{
			// Release Order
			g_GestorOrden.BuBSuspendReleaseOrden (*ptrTele);
			break;
		}
	default:
		{
			;
		}
	}
	
	
	SendClientQuitum (ptrMsg, ptrTele->TKopf.kenn, 
		ptrTele->TKopf.kenn, 
		0 /* ErrorCode: See what could go here... */);
	
	
	SendAllOrders (ptrMsg);	// Michael 25.10.2001 SendAllOrders (ptrMsg, fUpdateAll);
}


// Enviar todas las ordenes al cliente
void CBuBThread::SendAllOrders(CMsg *ptrMsg, bool SendAll)
{
	// M2015 TODO: This may not be thread safe and may require a critical section
	CTypedPtrArray <CObArray, COrden*> tpaLocalOrd;
	
	tpaLocalOrd.Copy(g_GestorOrden.m_OrdenPtrArray);

	int nOrd = tpaLocalOrd.GetSize();

	
	COrden dummyOrden;
	COrden * pOrden;

	dummyOrden.m_at.bnr = 0;	// Para que el BuB lo ignore
	dummyOrden.m_at.herkunft = 'A'; // Orden automático normal
	dummyOrden.m_at.fznr = 0;	// To delete AGV orders
	dummyOrden.m_at.id_nr [0] = '\0';
	dummyOrden.m_at.BzString [0] = '\0';

	SendAll = SendAll || g_GestorOrden.m_fUpdateRequired;

	g_GestorOrden.m_fUpdateRequired = false;

	int nOrdSend = SendAll ? KMAX_AUFTRAG : nOrd; // Enviar max. o solo existentes
	

	if (SendAll && nOrd == 0)
	{
		// Si no hay ordenes 
		for (int i = 0; i < KMAX_AUFTRAG; i++)
		{
			SendA0Telegram (ptrMsg, &dummyOrden.m_at, i);
			SendM0Telegram (ptrMsg, &dummyOrden.m_at, i);
		}
	
	}


	for (int i = 0;i < nOrdSend; i++)
	{
		if (i < nOrd)
			pOrden = tpaLocalOrd.GetAt (i);
		else
		{
			pOrden = &dummyOrden;
		}

		 if (SendAll || pOrden->m_fBuB)
		 {
			 if ('S' != pOrden->m_at.herkunft)
			 {
				SendA0Telegram (ptrMsg, &pOrden->m_at, pOrden->m_uBuBIndex);
			 }
			 else
			 {
				 // Es una orden manual
				SendM0Telegram (ptrMsg, &pOrden->m_at, pOrden->m_uBuBIndex);
			 }
			 pOrden->ResetBuB ();

		 }
	}
}

// Enviar todos los AGVs al cliente
void CBuBThread::SendAllAgvs(CMsg *ptrMsg, bool sendAll)
{
	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	{
		SendF0Telegram (ptrMsg, i + 1, sendAll);
	}


}

// Modificar AGV drives. AGV in / out of System
void CBuBThread::ProcessFKTelegram(CMsg *ptrMsg)
{

	send_fk_tele * ptrTele;

	UINT errorCode = 0;


	
	ptrTele = (send_fk_tele *)&ptrMsg->m_Tele.nData;

	CString strTemp;
	strTemp.Format (_T("FK tele: fznr: %d aktion: %d"), 
			ptrTele->fznr, ptrTele->aktion);
	MSGTRACE (strTemp);

	CAgv * ptrAgv = g_pGestor->GetAgvPtr (ptrTele->fznr);


	switch (ptrTele->aktion)
	{
	  	case TK_FZ_AUS_SYSTEM:
			{
			// take agv out of system
			CSingleLock lockAgv (&ptrAgv->m_cs);	// Bloquear acceso al AGV...
			MSGTRACE1 ("ProcessFKTelegram () Esperando Aceso AGV %d\n", ptrAgv->m_id);
			lockAgv.Lock ();
			MSGTRACE1 ("ProcessFKTelegram () Aceso Agv %d granted...\n", ptrAgv->m_id);
			// Michael 15.06.2005 T5K Mirar si se quitan las órdenes manuales aquí
			if (ptrAgv->m_pOrden)
			{
				int index;
				COrden * ptrOrden =  g_GestorOrden.GetOrdenPtr (ptrAgv->m_pOrden->m_at.id_nr, &index);
				if (ptrOrden)
				{
					g_GestorOrden.DeleteOrden (index);
				}
				ptrAgv->SetDeleted ();
			}
			ptrAgv->SetEstado (AGVFUERASISTEMA); 
			
			
			lockAgv.Unlock ();
			}
			break;
		case TK_FZ_IN_SYSTEM:
			// Insertar Agv in System
			{
				switch (ptrAgv->GetEstado ())
				{
				case AGVFUERASISTEMA:
				case AGVPERDIDO:
					// Michael 23.05.2007 Agv9
					int startPoint;
					if (ptrAgv->m_id > 8)
					{
						startPoint = 151 + (ptrAgv->m_id - 8);
					}
					else 
					{
						startPoint = 152 + ptrAgv->m_id;
					}

					ptrAgv->InsertarAGV (startPoint, 0);
					ptrAgv->SetEstado (AGVSINDATOS);

				}
			}
			break;
		case TK_FZ_TESTBETRIEB:
			// Use Agv for BuB testing
			ptrAgv->m_pFzdat->FVvonBedienung = !ptrAgv->m_pFzdat->FVvonBedienung;
			break;
		case TK_FZ_FAHRVERBOT:
			//Xavi, 02/11/05:Si canvia l'estat de bloqueig pel bub, insertem un registre a histagv
			if (ptrAgv->m_pFzdat->FVvonBedienung != 1)
				CDBEstadistica::ArchivarHistAgv(ptrAgv);
			//Fi xavi
			ptrAgv->m_pFzdat->FVvonBedienung = 1;	// Drive forbidden by BuB
			SET_AGV_STOP (*ptrAgv->m_pFzdat);
			break;
  		case TK_FZ_FAHRERLAUBNIS:
			//Xavi, 02/11/05:Si canvia l'estat de bloqueig pel bub, insertem un registre a histagv
			if (ptrAgv->m_pFzdat->FVvonBedienung != 0)
				CDBEstadistica::ArchivarHistAgv(ptrAgv);
			ptrAgv->m_pFzdat->FVvonBedienung = 0;
			SET_AGV_DRIVE (*ptrAgv->m_pFzdat);
			break;
		case TK_FZ_CARGARBAT:
			if (ptrAgv->m_pFzdat->mp != ptrTele->uCargador 
					&& CAgv::IsOccupiedCP (ptrTele->uCargador))
					errorCode = 1;
			else
			{
				ptrAgv->m_nCargador = ptrTele->uCargador;
				ptrAgv->SetCargaBateriaBuB (true);
			}
			break;
		// Afegit Xavi, 13/06/05: Tractament del missatge per enviar a manteniment
			// :TODO Michael para pruebas ***
		case TK_FZ_MANTENIMIENTO:
			ptrAgv->m_nMantenimiento = ptrTele->uMantenimiento;
			break;
		case TK_FZ_QUITARMANTENIMIENTO:
			ptrAgv->m_nMantenimiento = 0;
			break;
		// Fi afegit Xavi, 13/06/05
		case TK_FZ_QUITARCARGA:
			{
				if (ptrAgv->m_nCargador != 0)
				{
					ptrAgv->m_nCargador = 0;
					ptrAgv->SetCargaBateriaBuB (false);
				}
				else
					errorCode = 1;
			}
			break;
	}
	SendClientQuitum (ptrMsg, ptrTele->TKopf.kenn, 
		ptrTele->TKopf.kenn, 
		errorCode);

	SendF0Telegram (ptrMsg, ptrTele->fznr, true);
	
}


void CBuBThread::SendUpdateToBuB()
{

	if (!g_Container.IsBlockingTableCopied())
		return;

	if (m_pMsg != NULL)
	{
		// M2015 Agvs should update on their own SendAllAgvs (m_pMsg, false);
		SendAllOrders (m_pMsg);
		SendAllStations (m_pMsg);
		SendAllAlarmas ();
		if (g_Container.m_fSendSystemStatusToBuB)
		{
			SendSystemStatus (m_pMsg);
			g_Container.m_fSendSystemStatusToBuB = false;
		}
	}

}



// MA Telegram: Orden manual para AGV
void CBuBThread::ProcessMATelegram(CMsg *ptrMsg)
{
	send_ma_tele * ptrTele;
	bool fUpdateAll = false;	// True si hay que actualizar todas del BuB
	
	ptrTele = (send_ma_tele *)&ptrMsg->m_Tele.nData;
	

	MSGTRACE1 (_T("MA tele: ident: %s"), ptrTele->id_nr);
	
	TYP_AT_DAT at_neu;

	memset (&at_neu,0,sizeof(at_neu));

	at_neu.af_status = 0;
	at_neu.akt_org = 0;	// Original Action
	at_neu.akt_z.nLayer = 0;
	at_neu.akt_z.ucSide = 0;
	at_neu.akt_z.nDest = 0;
	at_neu.akt_z.nPos = 0;
	at_neu.aktion = 1;
	at_neu.art = 2; // Not used
	at_neu.at_lauf_nr = 1;
	at_neu.bnr = 1;
	

	// M2015 todo
	//ptrTele->az.nDest = 44;
	//ptrTele->az.nLayer = 0;
	//ptrTele->az.ucSide = 'L';
	//ptrTele->az.nPos = 1;
	//ptrTele->fMesa1 = true;
	//ptrTele->fMesa2 = false;
	//ptrTele->aktion = 0;
	// M2015 todo


	at_neu.herkunft = 'S';  // Manual AGV order !!!
	at_neu.prio_org = 1;
	at_neu.fznr     = (unsigned char)ptrTele->fznr;
	at_neu.akt_org  = ptrTele->aktion;
	memcpy(&at_neu.az, &ptrTele->az,sizeof(at_neu.az));
	
	if (at_neu.az.nDest && at_neu.az.nPos == 0)
	{
		at_neu.az.nDest   =  0;
		at_neu.az.ucSide = '0';
	}

	memcpy(at_neu.id_nr, ptrTele->id_nr, KMAXID_AT);

	at_neu.fMesa1 = ptrTele->fMesa1;
	at_neu.fMesa2 = ptrTele->fMesa2;
	at_neu.fSimCarga = ptrTele->fSimCarga;
	
	switch (ptrTele->auftr_aktion)
	{
	case TK_TA_NEU:
		{
			// La insertamos...
			// Get the description string
			strncpy((char *)at_neu.BzString, (const char *)ptrTele->szPSName, ptrTele->nLenPSName);

			COrden * pOrden = new COrden;	// la insertamos
			
			memcpy ((void *)&(pOrden->m_at), &at_neu, sizeof (at_neu));
			g_GestorOrden.GetNewOrdenId (pOrden->m_at.id_nr);
			pOrden->SetOrderInsertTime ();
			COrden * pYaOrden = g_GestorOrden.ExisteOrden (pOrden);
			if (pYaOrden)
			{
				// La orden ya existe..
				MSGERROR1 ("La orden %s ya existe", pOrden->m_at.id_nr);
				delete pOrden;
			}
			else
			{
				pOrden->GetDatosEstacion ();	// Recuper datos de la estación
				g_GestorOrden.InsertarOrdenPtrManual (pOrden);
			}
			
			break;
		}
	case TK_TA_AENDERN:
		g_GestorOrden.BuBUpdateOrden (*ptrTele);
		break;
	case TK_TA_LOESCHEN:
		g_GestorOrden.BuBDeleteOrden (ptrTele->id_nr, false);
		break;
	default:
		{
			MSGERROR1 ("ProcessMATelegram () auftr:aktion %d desconocido",
				ptrTele->auftr_aktion);
		}
	}
	
	SendClientQuitum (ptrMsg, ptrTele->TKopf.kenn, 
		ptrTele->TKopf.kenn, 
		0 /* ErrorCode */);
	
	
	SendAllOrders (ptrMsg, fUpdateAll);	// XXX Should only send new / modified orders

}



// TI Telegram: BuB Requests time
void CBuBThread::ProcessTITelegram(CMsg *ptrMsg)
{

	
	SendClientQuitum (ptrMsg, 0, 
		0, 
		0 /* ErrorCode */);
	
	

}

// Enviar telegrama al BuB de las ordenes de AGVs
void CBuBThread::SendM0Telegram(CMsg *ptrMsg, TYP_AT_DAT *atPtr, int index)
{
	empf_m0_tele tele  = {'0'};
	
	tele.TKopf.ucId[0]    = 'M';
	tele.TKopf.ucId[1]    = '0';
	tele.TKopf.usType     = 0;
	tele.TKopf.usValid    = 1;
	tele.TKopf.usIndex    = index;	// XXX How to indicate indexes
	tele.TKopf.usSize     = sizeof(TYP_MA_DAT);
	tele.TKopf.usMaxItems = KMAX_AUFTRAG;	// XXX How main order items ?
	


	tele.MaDat.fznr = atPtr->fznr;
     tele.MaDat.status = atPtr->Status;       
     tele.MaDat.aktiv = atPtr->akt_org;
	 tele.MaDat.ds_status = 0; // Disposition ???
     tele.MaDat.aktion = atPtr->akt_org;
	 CAgv::SetZiel (&tele.MaDat.akt_z, atPtr->az.nDest, 
					(unsigned char)atPtr->az.nPos,			//JLac
					atPtr->az.ucSide, 
					atPtr->az.nLayer);
	 strcpy_s ((char *)tele.MaDat.id_nr, sizeof(atPtr->id_nr), (char *)atPtr->id_nr); 
	 tele.MaDat.datum.tag = atPtr->datum.tag;  
     tele.MaDat.datum.monat = atPtr->datum.monat;
     tele.MaDat.datum.stunde = atPtr->datum.stunde;
     tele.MaDat.datum.minute = atPtr->datum.minute;

	 strcpy_s ((char *)tele.MaDat.szStationName, sizeof(tele.MaDat.szStationName), 
		 (char *)atPtr->BzString); 

	CMsg msg;

	msg.m_Tele.nTeleLaenge = sizeof (empf_m0_tele);
	msg.m_Tele.nRetStatus = 1;
	msg.m_Tele.nOnOff = 1;
	msg.m_Tele.nPcNr = 1;

	
	memcpy (msg.m_Tele.nData, &tele, sizeof(empf_m0_tele));
	
	TRY
	{
		ptrMsg->m_ptrCliSock->SendMsg((IMsg *)&msg);
	}
	CATCH(CFileException, e)
	{
		ptrMsg->m_ptrCliSock->Abort();

		CString strTemp;
		if (strTemp.LoadString(IDS_SENDERROR))
			MSGERROR (strTemp);
	}
	END_CATCH

}


// Enviar mensaje de borrar orden de AGV
void CBuBThread::SendM0DeleteTelegram(int index)
{
	if (! m_pMsg)
		return;

	
	empf_m0_tele tele = {'0'};
	
	tele.TKopf.ucId[0]    = 'M';
	tele.TKopf.ucId[1]    = '0';
	tele.TKopf.usType     = 0;
	tele.TKopf.usValid    = 1;
	tele.TKopf.usIndex    = index;	// XXX How to indicate indexes
	tele.TKopf.usSize     = sizeof(TYP_MA_DAT);
	tele.TKopf.usMaxItems = KMAX_AUFTRAG;	// XXX How main order items ?
	


	memset (&tele.MaDat, 0, sizeof (tele.MaDat));
	tele.MaDat.fznr = 0;	// para que se borre

	CMsg msg;

	msg.m_Tele.nTeleLaenge = sizeof (empf_m0_tele);
	msg.m_Tele.nRetStatus = 1;
	msg.m_Tele.nOnOff = 1;
	msg.m_Tele.nPcNr = 1;

	
	memcpy (msg.m_Tele.nData, &tele, sizeof(empf_m0_tele));
	
	TRY
	{
		m_pMsg->m_ptrCliSock->SendMsg((IMsg *)&msg);
	}
	CATCH(CFileException, e)
	{
		m_pMsg->m_ptrCliSock->Abort();

		CString strTemp;
		if (strTemp.LoadString(IDS_SENDERROR))
			MSGERROR (strTemp);
	}
	END_CATCH

}

void CBuBThread::SendA0DeleteTelegram(int index)
{
	if (! m_pMsg)
		return;

	empf_a0_tele tele = {'0'};
	
	tele.TKopf.ucId[0]    = 'A';
	tele.TKopf.ucId[1]    = '0';
	tele.TKopf.usType     = 0;
	tele.TKopf.usValid    = 1;
	tele.TKopf.usIndex    = index;	
	tele.TKopf.usSize     = sizeof(TYP_AT_DAT);
	tele.TKopf.usMaxItems = KMAX_AUFTRAG;	
	
	memset (&tele.AtDat, 0, sizeof (tele.AtDat));
	tele.AtDat.bnr = 0;	// Para que se borre la orden el BuB

	CMsg msg;
	msg.m_Tele.nTeleLaenge = sizeof (empf_a0_tele);
	msg.m_Tele.nRetStatus = 1;
	msg.m_Tele.nOnOff = 1;
	msg.m_Tele.nPcNr = 1;

	
	memcpy (msg.m_Tele.nData, &tele, sizeof(empf_a0_tele));
	
	TRY
	{
		m_pMsg->m_ptrCliSock->SendMsg((IMsg *)&msg);
	}
	CATCH(CFileException, e)
	{
		m_pMsg->m_ptrCliSock->Abort();

		CString strTemp;
		if (strTemp.LoadString(IDS_SENDERROR))
			MSGERROR (strTemp);
	}
	END_CATCH


}

// Enviar avisos de alarmas
void CBuBThread::SendAllAlarmas()
{
	if (! m_pMsg)
		return;

	// M2015 TODO
	return;

	empf_dt_tele tele  = {'0'};

	tele.TKopf.ucId[0]    = 'D';
	tele.TKopf.ucId[1]    = 'T';
	tele.TKopf.usType     = 1;
	tele.TKopf.usValid    = 1;
	tele.TKopf.usIndex    = 0;
	tele.TKopf.usSize     = sizeof(TYP_SYSLOG) + sizeof(TYP_DATUM);
	tele.TKopf.usMaxItems = 30;

	CMsg msg;
	msg.m_Tele.nTeleLaenge = sizeof (empf_dt_tele);
	msg.m_Tele.nRetStatus = 1;
	msg.m_Tele.nOnOff = 1;
	msg.m_Tele.nPcNr = 1;


	CSingleLock lockAlarma (&g_Container.m_csAlarma);
	lockAlarma.Lock ();

	UINT numAlarmas = g_Container.m_AlarmaArray.GetSize ();
	for (UINT i = numAlarmas; i > 0; i--)
	{
		CAlarma * ptrAlarma = g_Container.m_AlarmaArray.GetAt (i - 1);
		if (ptrAlarma->SetTelegramaBuB (&tele))
		{

		SetInsertTime (&tele.TDatum);
		memcpy (msg.m_Tele.nData, &tele, sizeof(empf_dt_tele));

		TRY
		{
			m_pMsg->m_ptrCliSock->SendMsg((IMsg *)&msg);
			g_Container.m_AlarmaArray.RemoveAt (i - 1);
			delete ptrAlarma;

		}
		CATCH(CFileException, e)
		{
			m_pMsg->m_ptrCliSock->Abort();

			CString strTemp;
			if (strTemp.LoadString(IDS_SENDERROR))
				MSGERROR (strTemp);
		}
		END_CATCH
		}
	}

			
	lockAlarma.Unlock ();


}

// Poner la hora en una estructura datum
void CBuBThread::SetInsertTime(TYP_DATUM *ptrDatum)
{
	CTime time = CTime::GetCurrentTime ();
	int year = time.GetYear ();

	ptrDatum->jahr  = (unsigned short) year; 
	ptrDatum->monat = (unsigned char)time.GetMonth ();
	ptrDatum->tag = (unsigned char)time.GetDay ();
	ptrDatum->stunde = (unsigned char)time.GetHour ();
	ptrDatum->minute  = (unsigned char)time.GetMinute ();
	ptrDatum->sekunde = (unsigned char)time.GetSecond ();


}



void CBuBThread::OnTimer(WPARAM wParam, LPARAM /*lParam*/)
{
	//if (m_fTerminate)
	//	ExitInstance();
if	(wParam == m_timerId)
{
	TRACE ("** Update timer for BuB.. **\n");
	if (IsConnected()) 
	{
		if (++m_iKeepAlive > 5)
		{
			MSGERROR ("NO INCOMING ACTIVITY ON BuB SOCKET");
			// m_pMsg->m_ptrCliSock->Close();
			// M2015 TODO Michael 25.04.2015 Sometimes this is called and there is no incoming data.
			// The socket blocks indefinitely on the read. Look at possibility of peeking at data on the socket
			// ProcessPendingRead(m_pMsg->m_ptrCliSock);
//			if (!m_pMsg->m_ptrCliSock->m_pArchiveIn->IsBufferEmpty())
//			{
				TCHAR c;
				MSGAVISO ("Pinging BuB socket");
		
				m_pMsg->m_ptrCliSock->Receive(&c, 1, MSG_PEEK);// Stimulates on receive?
				MSGAVISO ("BuB socket pinged");
//			}
				m_iKeepAlive = 0;
		}
		else
		{
			g_GestorOrden.SetGetOrdenesDB ();	
			SendUpdateToBuB ();
			SendDeleteTelegrams();
		}

	}
	else
	{
		MSGERROR ("BuB thread socket closed!");
	}

	::SetTimer(NULL,0,1000,NULL);
	
}
}

bool CBuBThread::GetNextOrderIndex(BuB_Order_Index & index)
{
	bool retVal;
	m_cs.Lock();
	if 	 (!m_ListDeleteOrders.IsEmpty())
	{
		index = m_ListDeleteOrders.GetHead();
		retVal = true;
	}
	else
		retVal = false;
	m_cs.Unlock();
	return retVal;
	
}

void CBuBThread::DeleteFirstOrderIndex()
{
	m_cs.Lock();
	m_ListDeleteOrders.RemoveHead();
	m_cs.Unlock();
}

void CBuBThread::SendDeleteTelegrams () 
{
	BuB_Order_Index orderIndex;
	while (GetNextOrderIndex(orderIndex))
	{
		if ('S' == orderIndex.type)
			SendM0DeleteTelegram (orderIndex.bubIndex);
		else
			SendA0DeleteTelegram (orderIndex.bubIndex);
		DeleteFirstOrderIndex();

	}

}


void CBuBThread::AddOrderToDelete(UCHAR tipo, UCHAR accion, UINT index)
{
	BuB_Order_Index orderIndex = { tipo, accion, index };
	m_cs.Lock();
	m_ListDeleteOrders.AddTail(orderIndex);
	m_cs.Unlock();
}

// Send Agv update to BuB


void CBuBThread::OnAgvUpdateBuB(WPARAM wParam, LPARAM lParam)
{
	UINT id = (UINT)lParam;
	if (IsConnected())
		this->SendF0Telegram(m_pMsg, id, true);
	// If there is no bub, there is no need
}


bool CBuBThread::IsConnected(void)
{
	if (m_fConnected)
	{
		return (m_pMsg != NULL && m_pMsg->m_ptrCliSock != NULL && 
			m_pMsg->m_ptrCliSock->m_hSocket != INVALID_SOCKET);
	}
	else 
		return false;
}


BEGIN_MESSAGE_MAP(CBuBThread, CWinThread)
		ON_THREAD_MESSAGE(WM_TIMER,	OnTimer)
		ON_THREAD_MESSAGE(WM_USER_AGV_UPDATE_BUB, OnAgvUpdateBuB)
END_MESSAGE_MAP()





