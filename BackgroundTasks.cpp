// BackgroundTasks.cpp: implementation of the CBackgroundTasks class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "BackgroundTasks.h"
#include "msgMacro.h"
#include "tpcDoc.h"
#include "tpcView.h"
#include "DBEstacion.h"
#include "DBEstadoPLC.h"
#include "PLCThread.h"
#include "DBCrono.h"
#include "DBEstadistica.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


UINT ThreadBackgroundTasks (LPVOID pParam);	// Start Thread prototype

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBackgroundTasks::CBackgroundTasks()
{
    m_hTimers [0] = CreateWaitableTimer(NULL, TRUE, "UpdatePtoComTimer");
	ASSERT (m_hTimers [0]);

    m_hTimers [1] = CreateWaitableTimer(NULL, TRUE, "UpdateEstadoPLC");
	ASSERT (m_hTimers [1]);

}

CBackgroundTasks::~CBackgroundTasks()
{

}


// Global callback arrancado por AfxBeginThread
UINT ThreadBackgroundTasks (LPVOID pParam)
{
	CBackgroundTasks * pTask = (CBackgroundTasks *) pParam;

	TRACE ("CBackgroundTasks arrancado (desde thread)");
	
	// Connect to Oracle
	SleepEx (5000, false);
	pTask->m_OraContext = g_Container.m_dbutil.ConectarOracle ("BackgroundTasks");

	pTask->run ();
	return (0);
}


void CBackgroundTasks::run()
{

	LARGE_INTEGER lTimer;
	lTimer.QuadPart = 20000 * -10000; // Millisegundos a Nanosegundos
	UINT updateSiemens;	
	UINT updateEstado;	// Petición periodica de Estado al PLC
	CDBEstadoPLC estadoPLC;
	
	CDBCrono dbCrono (m_OraContext);


	for (updateSiemens = 0, updateEstado = 0;;updateSiemens++, updateEstado++)
	{

		// Michael Mods 2009 fin

		CDBEstacion::SetPuntosEspera ();
		estadoPLC.SendEstadoPLC ();


		// Michael Mods 2009
		estadoPLC.UpdateInstanceTimestamp();



		SetWaitableTimer (m_hTimers [0],
			&lTimer,
			0,
			NULL,
			NULL,
			false);

		if (updateSiemens >= 2)
		{
			// Cosas menos criticas
			// Actualizar los descansos para el BuB
			CDBEstacion::UpdateBuBDescansos ();
			updateSiemens = 0;

			if (NULL == g_Container.m_wPLCThreadId)
				// Arrancamos el thread de nuevo
				// MSGAVISO ("NOT RESTARTING Plc Thread");
				g_Container.m_pPLCThread = (CPLCThread *)AfxBeginThread(RUNTIME_CLASS(CPLCThread));

			// Actualizar visualización de clientes conectados para BuB
			dbCrono.UpdateClientes (&g_Container.m_strRadio, &g_Container.m_strTren);
// M2015			g_Container.m_fSendSystemStatusToBuB = true;

			// Michael 28.02.2002 Archivar órdenes de transporte
			CDBEstadistica::ArchivarOrdTran ();

		}
		
		// Provocar que se mire de lanzar órdenes en el thread principal
		g_Container.m_fRequestUpdateSiemens = true;

		// Provocar que se miren las órdenes por entregar en el thread principal
		g_Container.m_fQuizasAnularDelivery = true;	


		MSGTRACE ("Background task waiting...");

		DWORD dwResult = WaitForMultipleObjects (2, m_hTimers, FALSE, INFINITE);
		if (dwResult != WAIT_OBJECT_0 && dwResult != WAIT_OBJECT_0 + 1)
		{
			MSGERROR1 ("CBackgroundTasks::run () WaitForMultipleObjects failed LastError (%d)", GetLastError ());
		}
		else
		{
			MSGTRACE1 ("Timer was signalled for %s", 
				dwResult == WAIT_OBJECT_0 ? "UpdatePtoComTimer" : "???");
		}

		// Michael 30.07.2002 Pedir estado al PLC cada 20 minutos
		if (updateEstado >= 60)
		{
			updateEstado = 0;
		    g_Container.m_pMainWnd->PostMessage (WM_USER_GETESTADOPLC, 0, NULL);
		}


		// Michael 13.01.2002 Mirar de arrancar de nuevo
		if (!g_Container.IsEnFuncionamiento () && !g_Container.IsSincronizacion ())
			if (g_Container.m_fAutoStart)
			{
				CTime curTime = CTime::GetCurrentTime ();

				if (g_Container.m_StartTime < curTime)
				{
					MSGAVISO ("Arrancando sistema por hora programada de arranque");
					g_Container.SetSystemState (BETRIEBSBEREIT);
				}
			}

	}





}

void CBackgroundTasks::ArrancarThread()
{
	CBackgroundTasks * ptrTasks = new CBackgroundTasks;	
	CWinThread* pThread = AfxBeginThread (ThreadBackgroundTasks, ptrTasks);
	
	return ;

}
