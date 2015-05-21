// GestorAGV.cpp: implementation of the CGestorAGV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "tpc.h"



#include "msgMacro.h"
#include "tpcDoc.h"
#include "tpcView.h"
#include "DBEstacion.h"
#include "DBEstadoPLC.h"
#include "Orden.h"
#include "DBGestorOrden.h"
#include "GestorOrden.h"
#include "SituationSet.h"
//#include "V_BLOCK.H"
#include "tabsetfv.h"
#include "macro.h"
#include "DBEstadistica.h"

#include "TCPServerSocket.h"
#include "ClientSocket.h"
#include "WlanMsg.h"
#include "GestorAGV.h"






#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define		NEXTPOLLTIMEOUT	500			// Millisegundos
#define		NEXTPOLLPERDIDO	5000
#define		NEXTPOLLNORMAL	0

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGestorAGV* g_pGestor;

TYP_FZDAT * fzdat;

UINT ThreadSerialCom (LPVOID pParam);	// Start Thread prototype


CGestorAGV::CGestorAGV(CDistanceReqWayTeleMap* pmapDistancesReqWayTele)
{
	this->m_pptrAgv = NULL;		//JLac
	this->InitAGVs(pmapDistancesReqWayTele);
	StartSocketServer();	// M2015 TODO - see which thread to run this in

}

CGestorAGV::~CGestorAGV()
{
	// delete agvs
	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	{
		AfxGetApp ()->WriteProfileInt ("Settings", "LastPosAgv_" + m_pptrAgv[i]->m_id, m_pptrAgv[i]->m_pFzdat->mp);
		delete m_pptrAgv [i];
	}
 
	if(fzdat != NULL)
	{
		delete [] fzdat;
	}

	if (this->m_pptrAgv != NULL)		//JLac
	{
		free(this->m_pptrAgv);
	}
}

void CGestorAGV::InitAGVs(CDistanceReqWayTeleMap* pmapDistancesReqWayTele)
{
	// Insert some AGVs
	//fzdat = new TYP_FZDAT [KKFZMAX_PLUS1];
	int nAGVs = g_pCourseData->GetNumberAGVs();

	this->m_pptrAgv = (CAgv**)malloc(nAGVs * sizeof(CAgv*));	//JLac

	fzdat = new TYP_FZDAT[nAGVs + 1];


    this->m_hTimer = CreateWaitableTimer(NULL, TRUE, "AgvPollTimer");

	ASSERT(this->m_hTimer);
	

	this->m_lpHandles[0] = this->m_hTimer;
	this->m_lpHandles[1] = this->m_evtOrden;

	//pCourseAGV = g_pCourseData->GetPtrCourseAGVType(1);

	//for (int i = 0 /* XXX */; i < KKFZMAX; i++)
	for (int i = 0 /* XXX */; i < nAGVs; i++)
	{
		this->m_pptrAgv[i] = new CAgv(i + 1, &this->m_SerCom, pmapDistancesReqWayTele);
		this->m_pptrAgv[i]->m_pFzdat = fzdat + i + 1;
		this->m_pptrAgv[i]->Init();

 	}



	CAgv::SetAGVTCourse(g_pCourseData->GetPtrCourseAGVType(1));
}

// Arrancar el thread de comunicación con AGVs
void CGestorAGV::ArrancarComThread(CDistanceReqWayTeleMap* pmapDistancesReqWayTele)
{
	CWinThread* pThread = AfxBeginThread (ThreadSerialCom, g_pGestor);
    TRACE ("ThreadSerialCom arrancado (desde main)...");
	g_pGestor->m_evtTeleOut.SetEvent();// Activar el event flag de coms...
	
	return ;

}

// Global callback arrancado por AfxBeginThread
UINT ThreadSerialCom (LPVOID pParam)
{
	CGestorAGV* pGestor = (CGestorAGV*) pParam;

	TRACE ("ThreadSerialCom arrancado (desde thread)");
	

	// Connect to Oracle
	SleepEx (5000, false);
	g_Container.m_dbutil.ConectarOracle ("GestorAGVThread");

	CDBEstacion::GetAgvEnCargador ();	// Inicializar los Agvs en Cargador
	CDBGestorOrden::IniciarSistema ();	// Iniciar órdenes etc.
	CDBEstacion::InitArrayCargadores (g_Container.m_uCargadorArray); // Inicializar array de cargadores

	// XXXXX Michael 28.11.2001 pGestor->GetAgvConOrden ();	// Recuperar órdenes en curso



	g_Container.SetArrancadoAgvThread ();


	pGestor->AutomataGestor ();

	return (0);
}

// Automata del gestor de AGVs
void CGestorAGV::AutomataGestor()
{
	//if (g_Container.m_fSimulateCom) {

	//	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	//	{
	//	CDBEstadistica::GetLastPositionAGV(m_pptrAgv[i]);
	//	}
	//}

	// M2015 TODO - take some agvs out for simulation
	if (g_Container.m_fSimulateCom)
	{
		//this->m_pptrAgv[1]->SetEstado (AGVFUERASISTEMA);
		//this->m_pptrAgv[2]->SetEstado (AGVFUERASISTEMA);
		//this->m_pptrAgv[3]->SetEstado (AGVFUERASISTEMA);
		//this->m_pptrAgv[4]->SetEstado (AGVFUERASISTEMA);
		//this->m_pptrAgv[5]->SetEstado (AGVFUERASISTEMA);
		//this->m_pptrAgv[6]->SetEstado (AGVFUERASISTEMA);
		//this->m_pptrAgv[7]->SetEstado (AGVFUERASISTEMA);
		//this->m_pptrAgv[8]->SetEstado (AGVFUERASISTEMA);
		//this->m_pptrAgv[9]->SetEstado (AGVFUERASISTEMA);

	}


	for (;;)
	{
		// 22.02.2002 Read Sit files...
		if (g_Container.m_fReadSitStopGoFiles)
		{
			ReadSitStopAndGoFiles ();
			g_Container.m_fReadSitStopGoFiles = false;
		}

		if (g_Container.m_fWriteAgvHistoryFile)
		{
			WriteAgvHistoryFile ();
			g_Container.m_fWriteAgvHistoryFile = false;
		}

		if (g_Container.m_fWriteAGVPosition) {
			CString format;

			//for (int i = 0; i < KKFZMAX; i++)
			for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
			{
				format.Format("%s%d", "LastPosAgv_", m_pptrAgv[i]->m_id);
				AfxGetApp ()->WriteProfileInt ("Settings", format, m_pptrAgv[i]->m_pFzdat->mp);
			}
			g_Container.m_fWriteAGVPosition = false;
		}

		LARGE_INTEGER uNextPoll;

		m_NextPoll.QuadPart = 0;



		bool bHayWlanAgvs = false;
		//for (int i = 0; i < KKFZMAX; i++)
		for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)

		// Scan through AGVs to see if there are telegramas to be sent
		{
			CAgv * ptrAgv = m_pptrAgv [i];
			// M2015 TODO Only 2 and 10, AGVs in Reutlingen
			//if (ptrAgv->m_id == 2 || ptrAgv->m_id == 10 || ptrAgv->m_id == 11)
			//{

			if (!ptrAgv->IsWlan())
			{
				int estado = ptrAgv->GetEstado ();
				if (estado != AGVFUERASISTEMA /* && estado != AGVPERDIDO */)
				{
					//CSingleLock pollLock (m_csPoll);
					//MSGTRACE1("PollLock for Agv: %d", ptrAgv->m_id);
					//pollLock.Lock();

					uNextPoll = ptrAgv->Poll ();

					//pollLock.Unlock();
					//MSGTRACE1("PollLock unlock for Agv: %d", ptrAgv->m_id);

					// Determinar siguiente poll más urgente
					if (uNextPoll.QuadPart > 0 && m_NextPoll.QuadPart == 0)
						m_NextPoll.QuadPart = uNextPoll.QuadPart;
					else
						if (uNextPoll.QuadPart < m_NextPoll.QuadPart)
							m_NextPoll.QuadPart = uNextPoll.QuadPart;
				}
			}
			else
			{
				int estado = ptrAgv->GetEstado ();
				ptrAgv->ProcessMessages();
				bHayWlanAgvs = true;

			}
		}

		if (m_NextPoll.QuadPart == 0 || bHayWlanAgvs)
		{
			// No hay Agvs
			if (!bHayWlanAgvs)
				m_NextPoll = CAgv::GetTimePlusInterval (NEXTPOLLPERDIDO);
			else
				m_NextPoll = CAgv::GetTimePlusInterval (200);
		}

		SetSemaforos ();

		SetEnCircuito ();	// Poner Agvs en Circuito si no hay

		
		// M2015 TODO
		// Sleep (300);

		SetWaitableTimer (m_hTimer,
			&m_NextPoll,
			0,
			NULL,
			NULL,
			false);
		DWORD dwResult = WaitForMultipleObjects (2, m_lpHandles, FALSE, INFINITE);
		if (dwResult != WAIT_OBJECT_0 && dwResult != WAIT_OBJECT_0 + 1)
		{
			MSGERROR1 ("WaitForMultipleObjects failed LastError (%d)", GetLastError ());
		}
		else
		{
			MSGTRACE1 ("Timer was signalled for %s", 
				dwResult == WAIT_OBJECT_0 ? "Poll de Agv" : "OrdenNuevo");
		}

		
	}
}




// Devolver puntero al Agv solicitado
CAgv * CGestorAGV::GetAgvPtr(UINT nAgv)
{
	//if (nAgv > KKFZMAX)
	if (nAgv > (UINT)g_pCourseData->GetNumberAGVs())
		return NULL;
	else
		return m_pptrAgv [nAgv - 1];

}

// Ver si un knot esta ocupado por otro Agv o No
UINT CGestorAGV::IsOccupiedKnot(UINT nAgv, UINT knot)
{

	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)

	{
		CAgv * ptrAgv = m_pptrAgv [i];
		int estado = ptrAgv->GetEstado ();
		if (estado != AGVFUERASISTEMA)
		{
			if (ptrAgv->m_knot == knot)
				return (ptrAgv->m_id);
		}

	}
	return (0);

}

// Intentar quitar una orden de otro Agv por cercanía
bool CGestorAGV::StealOrden(CAgv *ptrAgvThief)
{
	bool fStolen = false;
	UINT timeToPickup;
	UINT timeToPickupThief;
	UINT minTimeOrden = 9999999;
	CAgv * ptrAgvConOrden = NULL;
	TEstadoAGV estado;

	if (! ptrAgvThief->IsCandidateForOrder (&estado))
		return (false);

	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	{
		CAgv * ptrAgv = m_pptrAgv [i];
		switch (ptrAgv->GetEstado ())
		{
			case ORDENNUEVO:		
			case IRCARGARORD:
				// Michael 15.01.2002. Si está cargado es que pasa algo...
				// Michael 23.04.2002. Solo es candidato si se puede modificar el destino
				if (ptrAgv->IsModifiableDestino ())
				{
					if (! MESA1CARGADA (*ptrAgv->m_pFzdat) && ! MESA2CARGADA (*ptrAgv->m_pFzdat))
					{

						timeToPickup = ptrAgv->GetTimeToPickup (ptrAgv->m_pOrden->m_at.hz.nDest);
						timeToPickupThief = ptrAgvThief->GetTimeToPickup (ptrAgv->m_pOrden->m_at.hz.nDest);
						if (timeToPickupThief < timeToPickup && timeToPickupThief < minTimeOrden)
						{
							ptrAgvConOrden = ptrAgv;
							minTimeOrden = timeToPickupThief;
						}
					}
				}
		}
	}
	if (ptrAgvConOrden)
	{
		// Bloquear el Agv antes de continuar
		CSingleLock lockAgv (&ptrAgvConOrden->m_cs);
		lockAgv.Lock ();
		switch (ptrAgvConOrden->GetEstado ())
		{
			case ORDENNUEVO:		
			case IRCARGARORD:
				{
				CString strTemp;
				strTemp.Format ("Agv %d en mp: %d quita orden %s para mp: %d de Agv %d en mp: %d",
					ptrAgvThief->m_id,
					ptrAgvThief->m_pFzdat->mp,
					ptrAgvConOrden->m_pOrden->m_at.id_nr,
					ptrAgvConOrden->m_pOrden->m_at.hz.nDest,
					ptrAgvConOrden->m_id,
					ptrAgvConOrden->m_pFzdat->mp);
				MSGAVISO (strTemp);
				
				COrden * ptrOrden = ptrAgvConOrden->m_pOrden;
				ptrAgvConOrden->ResetOrden ();
				ptrAgvConOrden->SetEstado (AGVSINDATOS);
				ptrAgvThief->SetOrden (ptrOrden);
				ptrOrden->SetAssigned (ptrAgvThief->m_id);
				fStolen = true;
				}
				break;
			default:
				MSGTRACE2 ("CGestorAGV::StealOrden(): Agv %d estado: %s",
						ptrAgvConOrden->m_id,
						ptrAgvConOrden->GetStringEstado ());

		}
		lockAgv.Unlock ();
	}
	return (fStolen);
}


void CGestorAGV::GetAgvConOrden(UINT nAgv)
{
	COrden * ptrOrden;
	TYPE_STATION_DEF * ptrZiel;
	// M2015 27.04.2015 This call may clash with a call to GetOrden in the GestorOrden thread when the TPC starts
	// Hence the critical section 
	CSingleLock lockOrden (&g_Container.m_csAsignarOrden);
	MSGTRACE ("GetAgvConOrden () Esperando lock m_csAsignarOrden...");
	lockOrden.Lock ();
	MSGTRACE ("GetAgvConOrden () Got lock m_csAsignarOrden...");
	ptrOrden = CDBGestorOrden::GetOrden (DELIVER, nAgv);
	lockOrden.Unlock();
	MSGTRACE ("GetAgvConOrden () Unlocked lock m_csAsignarOrden...");

	if (ptrOrden)
	{
		// Nota: No se está mirando si el Agv esta realmente cargado o no
		CAgv * ptrAgv = GetAgvPtr (nAgv);
		if (ptrAgv)
		{
			MSGTRACE2 ("GetAgvConOrden: %s para Agv: %d",
					ptrOrden->m_at.id_nr,
					ptrAgv->m_id);

			bool fCargado = true;
			if (ptrOrden->m_at.fMesa1)
				if (! MESA1CARGADA (*ptrAgv->m_pFzdat))
					fCargado = false;
			if (fCargado && ptrOrden->m_at.fMesa2)
				if (! MESA2CARGADA (*ptrAgv->m_pFzdat))
					fCargado = false;

			// Solo procedemos si está cargado correctamente
			if (fCargado)
			{
				MSGTRACE2 ("GetAgvConOrden OK: %s para Agv: %d",
					ptrOrden->m_at.id_nr,
					ptrAgv->m_id);

				ptrAgv->SetOrden (ptrOrden);

				ptrZiel = &ptrOrden->m_at.bz;

				ptrAgv->SetZiel (&ptrAgv->m_pFzdat->auftziel, ptrZiel->nDest,
					(unsigned char)ptrZiel->nPos,
					ptrZiel->ucSide,
					ptrZiel->nLayer);

				// Michael 14.07.2002 ptrAgv->SetEstado (CARGARORD);	// Michael 28.12.2001: Se volverá a descontar de RESAGV al arrancar.
				ptrAgv->SetEstado (CARGARORD);

				g_GestorOrden.AddOrden (ptrOrden);	// Añadir al display del BuB
			}
			else
			{
				// No está el AGV cargado - damos por terminada la orden
				MSGTRACE2 ("GetAgvConOrden AGV no cargado: %s para Agv: %d",
				ptrOrden->m_at.id_nr,
				ptrAgv->m_id);
				ptrOrden->SetEstado (DELIVERED);
				delete ptrOrden;
			}

		}
	}

}



// Contar los Agvs que están en circuito
UINT CGestorAGV::GetAgvsEnCircuito()
{
	UINT numAgvs = 0;

	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	{
		CAgv * ptrAgv = m_pptrAgv [i];
		if (ptrAgv->IsEnCircuito () && TEST_FZ_IM_SYSTEM (*ptrAgv->m_pFzdat))
			numAgvs++;
	}
	return (numAgvs);
}


// Añadir Agvs al circuito
void CGestorAGV::SetEnCircuito()
{
	if (g_Container.IsEnFuncionamiento ())
	{
		UINT numAgvsEnCircuito = GetAgvsEnCircuito ();
		UINT numAgvsAllowed = g_Container.GetTotalAgvsAllowed ();

		if (numAgvsEnCircuito < numAgvsAllowed)
		{
			//for (int i = KKFZMAX; i > 0 && numAgvsEnCircuito < numAgvsAllowed; i--)
			for (int i = g_pCourseData->GetNumberAGVs(); i > 0 && numAgvsEnCircuito < numAgvsAllowed; i--)
			{
				CAgv * ptrAgv = GetAgvEnPunto (g_Container.m_uCargadorArray [i - 1]);
				if (ptrAgv)
				{
					if (!ptrAgv->IsEnCircuito ())
					{
						// Poner en circuito si se permite
						TEstadoAGV estado = ptrAgv->GetEstado ();
						if (estado == CARGARBAT || ptrAgv->HasBeenPolled ())
						{
							switch (estado)
							{
							case CARGARBAT:
								if (FZ_BATTERIE_LEER(*ptrAgv->m_pFzdat) || ptrAgv->IsCargaBateriaBuB ())
									/* No lo sacamos si no está cargado o ha sido enviado a cargar de forma
									manual - pero nos puede bloquear */
									break;
								else
								{
									ptrAgv->m_nCargador = 0;	// Para que salga
									g_Container.m_fSendSystemStatusToBuB = true;
								}
							case AGVSINDATOS:
							case IRPUNTOESP:
							case PUNTOESP:
								if (IsOkPoner55_56 (ptrAgv))
								{
									ptrAgv->SetEnCircuito ();
									numAgvsEnCircuito ++;
									g_Container.m_fSendSystemStatusToBuB = true;
								}
							}
						}
				
					}
				}
			}

		}
	}
	// Si no está en funcionamiento, no elabora num. agvs en circuito...
}


// Buscar un Agv en un punto, o que tiene destino el punto
CAgv * CGestorAGV::GetAgvEnPunto(UINT ptoCom)
{
	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	{
		CAgv * ptrAgv = GetAgvPtr (i + 1);
		if (AGVFUERASISTEMA !=ptrAgv->GetEstado ())
		{
			if (ptrAgv->m_pFzdat->mp == ptoCom
				|| ptrAgv->m_pFzdat->auftziel.nDest == ptoCom)
				return (ptrAgv);
		}
	}
	return (NULL);
}

// Leer los ficheros de sitStop and sitGo sin detener el sistema
void CGestorAGV::ReadSitStopAndGoFiles()
{
	CString strPath;
	strPath.LoadString(IDS_FILEPATH);
	CString strFile;
	

	// Read the sitStop.txt file
	tFVSituation.m_bSetFV = true;
	strFile.LoadString(IDS_FILE_SITUATION_FV);
	tFVSituation.Read(strPath + "\\" + strFile);
	
	// Read the sitGo.txt file
	tFESituation.m_bSetFV = false;
	strFile.LoadString(IDS_FILE_SITUATION_FE);
	tFESituation.Read(strPath + "\\" + strFile);


}

CAgv * CGestorAGV::GetAgvFromIP (CString *pIPAddress){
	TCHAR charAgv [6];
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	{
		CAgv * ptrAgv = m_pptrAgv [i];

		sprintf(charAgv, "AGV%d", ptrAgv->m_id);
		

		CString agvIp = AfxGetApp ()->GetProfileString ("Settings", charAgv);
		if (agvIp == *pIPAddress)
			return ptrAgv;
	}
	return (NULL);


}


void CGestorAGV::ProcessPendingAccept (CSocket * pServerSocket)
{

	// Puede ser que estuviera conectado ya... ???
	SOCKADDR_IN sockAddr;
	int sockAddrLen = sizeof (SOCKADDR_IN);
	// CClientSocket* pSocket = new CClientSocket(this);
	CSocket acceptSoc;


	TRACE("CGestorAGV::ProcessPendingAccept - Before Accept\n");
	int res = pServerSocket->Accept(acceptSoc, (SOCKADDR *)&sockAddr, &sockAddrLen);
	pServerSocket->Bind(4000);
	TRACE("CGestorAGV::ProcessPendingAccept - After Accept\n");

	if (res)
	{
		wsprintf (m_strConnected.GetBuffer (20), "%d.%d.%d.%d", 
			sockAddr.sin_addr.S_un.S_un_b.s_b1,
			sockAddr.sin_addr.S_un.S_un_b.s_b2,
			sockAddr.sin_addr.S_un.S_un_b.s_b3,
			sockAddr.sin_addr.S_un.S_un_b.s_b4);
		m_strConnected.ReleaseBuffer ();
		CString strTemp;

		// Look at doing this for all connections. Use list
		CAgv * ptrAgv = GetAgvFromIP(&m_strConnected);
		if (ptrAgv == NULL)
		{
			MSGWARNING1 ("Conexión de cliente %s no aceptado por no registrado", m_strConnected);
			// pSocket->Close();
			return;
		}

		strTemp.Format ("Conexión de AGV: %d de IP: %s", ptrAgv->m_id, m_strConnected);
		MSGAVISO(strTemp);

		int threads = m_agvSockArray.GetSize();
		bool exists = false;
		for (int i = 0; i < threads; i++)
		{
			CAgvSockType st = m_agvSockArray.GetAt(i);
			if (st.m_id == ptrAgv->m_id)
			{
				CAGVSockThread* pThread = st.m_pAgvSockThread;
				// pThread->SuspendThread();
				TRACE("CGestorAGV::ProcessPendingAccept - Before pThread->SetTerminate\n");
				pThread->SetTerminate();
				TRACE("CGestorAGV::ProcessPendingAccept - After pThread->SetTerminate\n");
				// Remove from list then
				m_agvSockArray.RemoveAt(i);
				// TODO: If the thread is stopped any other way, it will not be removed from this list
				
				// pThread->Delete();

				break;
			}
		}
			// Create a thread to handle the connection.
			// The thread is created suspended so that we can
			// set variables in CConnectThread before it starts executing.
			TRACE("CGestorAGV::ProcessPendingAccept - Before Creating new thread\n");
			CAGVSockThread* pThread =
				 (CAGVSockThread*)AfxBeginThread(
					RUNTIME_CLASS(CAGVSockThread),
					THREAD_PRIORITY_HIGHEST,	// M2015 Try: THREAD_PRIORITY_HIGHEST
					0,
					CREATE_SUSPENDED);
		   
			  // Pass the socket to the thread by passing the socket handle.
			  // You cannot pass a CSocket object across threads.
			  TRACE("CGestorAGV::ProcessPendingAccept - Before detach socket\n");
			  pThread->m_hSocket = acceptSoc.Detach();
			  pThread->SetAgvPtr(ptrAgv);


			  // Now start the thread.
			  TRACE("CGestorAGV::ProcessPendingAccept - Before ResumeThread\n");
			  pThread->ResumeThread();
			  CAgvSockType * st = new CAgvSockType (pThread, ptrAgv->m_id);
			  m_agvSockArray.Add (*st);
			  TRACE("CGestorAGV::ProcessPendingAccept - All done\n");

	}

	else
	{
		// delete pSocket;
		int nLast = GetLastError ();
		TRACE (_T("AcceptConnection: GetLastError is: %d\n"), nLast);
	}
}


// Escribir lista de Agvs a fichero
void CGestorAGV::WriteAgvHistoryFile()
{

	CStdioFile * pFile;
	CString    strFileName;



	pFile = GetWriteHistoryFile (g_Container.m_histFileType); // Abre el fichero

	POSITION pos = g_Container.m_listSelectAgvHist.GetHeadPosition ();
	while (pos != NULL)
	{
		CString strAgv = g_Container.m_listSelectAgvHist.GetNext (pos);
		WriteFzHistory(strAgv, pFile);
	}
	pFile->Close();
	g_Container.m_listSelectAgvHist.RemoveAll ();

}

CStdioFile * CGestorAGV::GetWriteHistoryFile(enum T_Hist fileExt)
{
	CString strFichero;

	CStdioFile * pFile;
	
	CTime timeAct = CTime::GetCurrentTime ();

	CString filePath;

	filePath = AfxGetApp ()->GetProfileString ("Settings", "HistFilePath");

	if (filePath.IsEmpty ())
		{
			CString strTemp;
			strTemp.LoadString (IDS_NOHISTFILEPATHSET);
			AfxMessageBox (strTemp);
			AfxGetApp ()->WriteProfileString ("Settings", "HistFilePath", ".\\");
			filePath = ".\\";
		}
		else if (filePath.Right (1) != "\\")
		{
			filePath += "\\";
		}
	
	strFichero.Format ("%s%02d%02d%02d%02d%02d.%s",
		filePath,
		timeAct.GetYear (),
		timeAct.GetMonth (),
		timeAct.GetDay (),
		timeAct.GetHour (),
		timeAct.GetMinute (),
		GetStringHistFileExtension (fileExt));


	// Michael XXX 04.09.2001: If the file doesn't exist, This asserts
	try
	{
	pFile = new CStdioFile (strFichero, 
							CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone);
	}
	catch (	CFileException e)
	{
		e.ReportError ();
	}

	ASSERT (pFile != NULL);


	return (pFile);

}

LPCSTR CGestorAGV::GetStringHistFileExtension(enum T_Hist fileExt)
{
	switch (fileExt)
	{
		case HIST_CRASH:	return "CSH";
		case HIST_DEADLOCK:		return "DLK";
		case HIST_ALLGEMEIN:		return "OTR";
		default:				return "???";
	}

}


// Mirar de actualizar los semaforos de paso
void CGestorAGV::SetSemaforos()
{
//	static UINT mpS1 [] = {211, 299, 215, 300, 301, 212, 223, 254, 214, 297, 255, 216, 222, 0}; // Siempre acabado en 0
//	static UINT mpS1 [] = {211, 299, 300, 301, 214, 0}; // Siempre acabado en 0
	static UINT mpS1 [] = {212, 215, 227, 232, 300, 301, 0}; // Siempre acabado en 0
	static UINT mpS2 [] = {40, 220, 290, 291, 0}; 

	int actS1, actS2, newS1, newS2;

	newS1 = 0;
	newS2 = 0;

	m_estadoPLC.GetEstadoSemaforo (&actS1, &actS2);


	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
		// Para cada AGV ver si necesita semáforo
	{
		CAgv * ptrAgv = m_pptrAgv [i];
		for (int j = 0; mpS1 [j] != 0 && newS1 == 0; j++)
			if ((ptrAgv->m_pFzdat->mp == mpS1 [j]
					/* || ptrAgv->m_pFzdat->nmp == mpS1 [j] */) && ptrAgv->GetEstado () != PUNTOESP )
			{
				newS1 = 1;
			}
		for (int k = 0; mpS2 [k] != 0 && newS2 == 0; k++)
			if ((ptrAgv->m_pFzdat->mp == mpS2 [k]
					/* || ptrAgv->m_pFzdat->nmp == mpS2 [k] */) && ptrAgv->GetEstado () != PUNTOESP )
			{
				newS2 = 1;
			}
	}
	if (newS1 != actS1 || newS2 != actS2)
	{
		m_estadoPLC.SetEstadoSemaforo (newS1, newS2);
	}


}
/* Devolver número de Agvs en el circuito, en posiciones diferente de
	punto de carga bateria, exceptuando el Agv que realiza la consulta
 */
UINT CGestorAGV::GetAgvsEnCircuitoIdle(UINT nAgvQuery)
{
	UINT numAgvs = 0;

	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	{
		CAgv * ptrAgv = m_pptrAgv [i];
		if (ptrAgv->m_id != nAgvQuery && ptrAgv->IsEnCircuito () && TEST_FZ_IM_SYSTEM (*ptrAgv->m_pFzdat))
		{
			switch (ptrAgv->GetEstado ())
			{
				case IRPUNTOESP:
					switch (ptrAgv->m_pFzdat->auftziel.nDest)
					{
						case 53:
						case 54:
						case 55:
						case 56:
						case 57:
						case 58:
						case 59:
							break;
						default:
							numAgvs++;
					}
					break;
				case PUNTOESP:
					switch (ptrAgv->m_pFzdat->mp)
					{
						case 53:
						case 54:
						case 55:
						case 56:
						case 57:
						case 58:
						case 59:
							break;
						default:
							numAgvs++;
					}
			}
		}
	}
	
	MSGAVISO1 ("Número de Agvs idle: %d", numAgvs);

	return (numAgvs);
}


/* Confirmar que se puede poner en el circuito un Agv en 55/56
	porqué no tiene Agv delante */
bool CGestorAGV::IsOkPoner55_56(CAgv * ptrAgv)
{
	bool bRetVal = true;
	if (ptrAgv->m_pFzdat->mp == 55 || ptrAgv->m_pFzdat->mp == 56)
	{
		UINT mp = ptrAgv->m_pFzdat->mp == 55 ? 53 : 54;
		//for (int i = 0; i < KKFZMAX; i++)
		for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
		{
			CAgv * ptrAgvDelante = GetAgvPtr (i + 1);
			if (AGVFUERASISTEMA != ptrAgvDelante->GetEstado ())
			{
				if (ptrAgvDelante->m_pFzdat->mp == mp
					&& ! ptrAgvDelante->IsEnCircuito ())
				{
					MSGAVISO2 ("No se pone Agv: %d en circuito por tener Agv: %d delante",
						ptrAgv->m_id, ptrAgvDelante->m_id);
					bRetVal = false;
					break;
				}
			}
		}
	}

	return (bRetVal);
}

/*
	Anular la carga de bateria de todos los AGVs 08.11.2005
 */
void CGestorAGV::AnularCargarBat()
{
	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)

	// Anula la carga de bateria en cada AGV
	{
		CAgv * ptrAgv = m_pptrAgv [i];
		// Sólo si la posición es de carga de bateria
		if (ptrAgv->IsCargadorCurPos(true))
		{

			ptrAgv->m_nCargador = 0;
			ptrAgv->SetCargaBateriaBuB (false);
			ptrAgv->SetCargado();
		}
	}
}
void CGestorAGV::StartSocketServer(void)
{
	
	TCHAR rgchServer [MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwLen = MAX_COMPUTERNAME_LENGTH;
	CString strConnect	= "Server";
	int nPort = 4000;	// M2015 TODO - take this out to parameter init
	BOOL fRetVal;
	
	m_pSocket = new CTCPServerSocket (this);


	if (! GetComputerName (rgchServer, &dwLen))
		strcpy (rgchServer, "???");

	
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


	return;
}

