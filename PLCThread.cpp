// PLCThread.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "PLCThread.h"
#include "SiemensSR.h"
#include "SolicitarCargaBat.h"
#include "DBUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPLCThread

IMPLEMENT_DYNCREATE(CPLCThread, CWinThread)

/* Michael 09.01.2002
	Thread de tratamiento mensajes del PLC de Siemens
	El driver de siemens comunica con el thread principal mediante el mensaje
	de usuario WM_USER_SIEMENS_SR. El thread principal redirecciona el mensaje
	a la funcion de tratamiento CPLCThread::OnUserMessageSiemens */
CPLCThread::CPLCThread()
{
}

CPLCThread::~CPLCThread()
{


}

BOOL CPLCThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	g_Container.m_pPLCWnd = GetMainWnd ();

	CString strTitle;
	g_Container.m_pPLCWnd->GetWindowText(strTitle);


	g_Container.m_wPLCThreadId = GetCurrentThreadId ();

	MSGAVISO1 ("This CPLCThread::InitInstance () ThreadId: %d",
		g_Container.m_wPLCThreadId);

	g_Container.SetSystemHostConnection (HOST_ONLINE);


	// Arrancar conexión base de datos
	SleepEx (5000, false);


	// Si ya existe el contexto, no hace falta conectar:
	g_Container.m_dbutil.ConectarOracle ("PLCThread");

	// Arrancar Comunicación con el PLC Siemens
	if (NULL != g_Container.m_pSR)
	{
		// Por si estuviera ya
		delete g_Container.m_pSR;
		g_Container.m_pSR = NULL;
	}
 

	g_Container.m_pSR = new CSiemensSR ();

	// Michael 23.04.2009 this is called from the class initialization
	// g_Container.m_pSR->Process(DO_OPEN);


	return TRUE;
}

int CPLCThread::ExitInstance()
{
	// Michael 23.04.2009 Parece que este no tiene conexión con oracle...
//	CDBUtil dbUtil;

	// TODO:  perform any per-thread cleanup here
	g_Container.SetSystemHostConnection (HOST_OFFLINE);

	g_Container.m_dbutil.DisconnectOracle ();

	// Michael 09.06.2008 Disconectar Siemens
	if (g_Container.m_pSR)
	{
		MSGAVISO ("Closing Siemens S/R");
		g_Container.m_pSR->Process(DO_CLOSE);
		MSGAVISO ("Deleting SiemensSR");
		// Michael 23.04.2009 delete forces release of connection
		delete g_Container.m_pSR;
		g_Container.m_pSR = NULL;
		g_Container.m_wPLCThreadId = NULL;

		MSGAVISO ("SiemensSR Deleted");
	}
		// Michael 09.06.2008 Fin
	

	return CWinThread::ExitInstance();
}

// Respuesta de Siemens S/R
void CPLCThread::OnUserMessageSiemens(WPARAM wParam, LPARAM lParam)
{
#if ! defined (FOREXPORTTOINDUMAT)

	MSGAVISO ("This is CPLCThread::OnUserMessageSiemens ()");

	RB1 * ptrRB = (RB1 *) lParam;
	g_Container.m_pSR->Response(ptrRB);

	// Michael 23.04.2009
	if (!g_Container.m_pSR->IsConnected () && !g_Container.m_pSR->IsConnecting())
	{
		// Si se ha perdido la conexión, salir para empezar de nuevo
		MSGERROR ("Saliendo del PLCThread");
		::PostQuitMessage (0);
	}

#endif

}




// Enviar el estado al PLC
void CPLCThread::OnUserMessageSiemensSend(WPARAM wParam, LPARAM lParam)
{
#if ! defined (FOREXPORTTOINDUMAT)
	
	g_Container.m_pSR->SendEstado ((BYTE *)lParam);

	if (!g_Container.m_pSR->IsConnected () && !g_Container.m_pSR->IsConnecting ())
	{
		// Si se ha perdido la conexión, salir para empezar de nuevo
		MSGERROR ("Saliendo del PLCThread");
		// delete g_Container.m_pSR;
		g_Container.m_pSR->Process (DO_CLOSE);
		g_Container.m_wPLCThreadId = NULL;
		::PostQuitMessage (0);
	}

#endif
	// SendEstado will delete: delete (BYTE *)wParam;


}


void CPLCThread::OnUserTestUIThread(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox ("This is OnUserTestUIThread");
}

BEGIN_MESSAGE_MAP(CPLCThread, CWinThread)
	//{{AFX_MSG_MAP(CPLCThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE (WM_USER_SIEMENS_SR, OnUserMessageSiemens)
	ON_THREAD_MESSAGE (WM_USER_SIEMENS_SEND, OnUserMessageSiemensSend)
	ON_THREAD_MESSAGE (WM_USER_TESTUITHREAD, OnUserTestUIThread)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLCThread message handlers
