// GestorAGV.cpp: implementation of the CGestorAGV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "tpc.h"

#include "GestorAGV.h"
#include "msgMacro.h"
#include "tpcDoc.h"
#include "tpcView.h"
#include "DBEstacion.h"
#include "DBEstadoPLC.h"
#include "Orden.h"
#include "DBGestorOrden.h"
#include "GestorOrden.h"
#include "SituationSet.h"
#include "V_BLOCK.H"
#include "tabsetfv.h"
#include "macro.h"


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


CGestorAGV::CGestorAGV()
{


	// Insert some AGVs
	fzdat = new TYP_FZDAT [KKFZMAX_PLUS1];



    m_hTimer = CreateWaitableTimer(NULL, TRUE, "AgvPollTimer");

	ASSERT (m_hTimer);
	

	m_lpHandles [0] = m_hTimer;
	m_lpHandles [1] = m_evtOrden;



	for (int i = 0 /* XXX */; i < KKFZMAX; i++)
	{
		m_ptrAgv [i] = new CAgv (i + 1, &m_SerCom);
		m_ptrAgv [i]->m_pFzdat = fzdat + i + 1;
		m_ptrAgv [i]->Init ();
	}

}

CGestorAGV::~CGestorAGV()
{
	// delete agvs
	for (int i = 0; i < KKFZMAX; i++)
	{
		delete m_ptrAgv [i];
	}
 
	if(fzdat != NULL)
	{
		delete [] fzdat;
	}

}

// Arrancar el thread de comunicación con AGVs
void CGestorAGV::ArrancarComThread()
{
	g_pGestor = new CGestorAGV;	// Crear Gestion de AGVs
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

		LARGE_INTEGER uNextPoll;

		m_NextPoll.QuadPart = 0;



		for (int i = 0; i < KKFZMAX; i++)

		// Scan through AGVs to see if there are telegramas to be sent
		{
			CAgv * ptrAgv = m_ptrAgv [i];
			int estado = ptrAgv->GetEstado ();
			if (estado != AGVFUERASISTEMA /* && estado != AGVPERDIDO */)
			{
				uNextPoll = ptrAgv->Poll ();
				// Determinar siguiente poll más urgente
				if (uNextPoll.QuadPart > 0 && m_NextPoll.QuadPart == 0)
					m_NextPoll.QuadPart = uNextPoll.QuadPart;
				else
					if (uNextPoll.QuadPart < m_NextPoll.QuadPart)
						m_NextPoll.QuadPart = uNextPoll.QuadPart;
			}

		}

		if (m_NextPoll.QuadPart == 0)
		{
			// No hay Agvs
			m_NextPoll = CAgv::GetTimePlusInterval (NEXTPOLLPERDIDO);
		}

		SetSemaforos ();

		SetEnCircuito ();	// Poner Agvs en Circuito si no hay

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
	if (nAgv > KKFZMAX)
		return NULL;
	else
		return m_ptrAgv [nAgv - 1];

}

// Ver si un knot esta ocupado por otro Agv o No
UINT CGestorAGV::IsOccupiedKnot(UINT nAgv, UINT knot)
{

	for (int i = 0; i < KKFZMAX; i++)

	{
		CAgv * ptrAgv = m_ptrAgv [i];
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

	for (int i = 0; i < KKFZMAX; i++)
	{
		CAgv * ptrAgv = m_ptrAgv [i];
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

						timeToPickup = ptrAgv->GetTimeToPickup (ptrAgv->m_pOrden->m_at.hz.nr);
						timeToPickupThief = ptrAgvThief->GetTimeToPickup (ptrAgv->m_pOrden->m_at.hz.nr);
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
					ptrAgvConOrden->m_pOrden->m_at.hz.nr,
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
	TYP_EINZELZIEL * ptrZiel;

	if (ptrOrden = CDBGestorOrden::GetOrden (DELIVER, nAgv))
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

				ptrAgv->SetZiel (&ptrAgv->m_pFzdat->auftziel, ptrZiel->nr,
					ptrZiel->pos,
					ptrZiel->lage,
					ptrZiel->ebene);

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

	for (int i = 0; i < KKFZMAX; i++)
	{
		CAgv * ptrAgv = m_ptrAgv [i];
		if (ptrAgv->IsEnCircuito () && FZ_IM_SYSTEM (*ptrAgv->m_pFzdat))
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
			for (int i = KKFZMAX; i > 0 && numAgvsEnCircuito < numAgvsAllowed; i--)
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
									ptrAgv->m_nCargador = 0;	// Para que salga
							case AGVSINDATOS:
							case IRPUNTOESP:
							case PUNTOESP:
								if (IsOkPoner55_56 (ptrAgv))
								{
									ptrAgv->SetEnCircuito ();
									numAgvsEnCircuito ++;
								}
							}
						}
					}
				}
			}
			g_Container.m_fSendSystemStatusToBuB = true;
		}
	}
	// Si no está en funcionamiento, no elabora num. agvs en circuito...
}


// Buscar un Agv en un punto, o que tiene destino el punto
CAgv * CGestorAGV::GetAgvEnPunto(UINT ptoCom)
{
	for (int i = 0; i < KKFZMAX; i++)
	{
		CAgv * ptrAgv = GetAgvPtr (i + 1);
		if (AGVFUERASISTEMA !=ptrAgv->GetEstado ())
		{
			if (ptrAgv->m_pFzdat->mp == ptoCom
				|| ptrAgv->m_pFzdat->auftziel.nr == ptoCom)
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


	for (int i = 0; i < KKFZMAX; i++)
	// Para cada AGV ver si necesita semáforo
	{
		CAgv * ptrAgv = m_ptrAgv [i];
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

	for (int i = 0; i < KKFZMAX; i++)
	{
		CAgv * ptrAgv = m_ptrAgv [i];
		if (ptrAgv->m_id != nAgvQuery && ptrAgv->IsEnCircuito () && FZ_IM_SYSTEM (*ptrAgv->m_pFzdat))
		{
			switch (ptrAgv->GetEstado ())
			{
				case IRPUNTOESP:
					switch (ptrAgv->m_pFzdat->auftziel.nr)
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
		for (int i = 0; i < KKFZMAX; i++)
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

void CGestorAGV::AnularCargarBat()
{
	for (int i = 0; i < KKFZMAX; i++)

	// Anula la carga de bateria en cada AGV
	{
		CAgv * ptrAgv = m_ptrAgv [i];
		ptrAgv->m_nCargador = 0;
		ptrAgv->SetCargaBateriaBuB (false);
		ptrAgv->SetCargado();
	}
}