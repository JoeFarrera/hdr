// tpc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "tpc.h"

#include "MainFrm.h"
#include "TpcDoc.h"
#include "LeftView.h"
#include "TpcView.h"
#include "GestorAGV.h"
#include "GestorOrden.h"
//#include "Block.h"		JLac
#include "typedef.h"
#include "TraceLevelDlg.h"
#include "PLCThread.h"
#include "CCourseLoader.h"
#include "BuBThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTpcApp

BEGIN_MESSAGE_MAP(CTpcApp, CWinApp)
	//{{AFX_MSG_MAP(CTpcApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	// ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	// ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

CContainer g_Container;	// Global
//CBlock g_Block;
CGestorOrden g_GestorOrden; // Gestor de ordenes

CCourseData * g_pCourseData;




/////////////////////////////////////////////////////////////////////////////
// CTpcApp construction

CTpcApp::CTpcApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTpcApp object

CTpcApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTpcApp initialization

BOOL CTpcApp::InitInstance()
{
	// Michael: Necesario para sockets...
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	//Enable3dControls();	JLac			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("INDUMAT"));

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Run process at high priority:	JLac, 08.05.2015
	HANDLE currentHandle = GetCurrentProcess();
	DWORD currentPriorityClass = GetPriorityClass(currentHandle);
	int newPriorityClass = HIGH_PRIORITY_CLASS;
	bool success = SetPriorityClass(currentHandle,newPriorityClass) == TRUE;


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTpcDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CTpcView));
	AddDocTemplate(pDocTemplate);
/* M2015 Testing TODO
	CMultiDocTemplate * p_MDocTemplate2 = new CMultiDocTemplate(
            IDR_MAINFRAME,
            RUNTIME_CLASS(CTpcDoc),
            RUNTIME_CLASS(CMainFrame),
            RUNTIME_CLASS(CTpcView));
      AddDocTemplate(p_MDocTemplate2 );

*/


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	g_Container.m_pMainWnd = AfxGetApp ()->GetMainWnd ();


	

	g_Container.GetRegisterVariables ();
	// El thread, si sale solo, se arrancará de nuevo en el CBackgroundProcess::run ()
	// MSGAVISO ("NOT Starting PLC thread");
	g_Container.m_pPLCThread = (CPLCThread *)AfxBeginThread(RUNTIME_CLASS(CPLCThread));
	g_Container.m_pBuBThread = (CBuBThread *)AfxBeginThread(RUNTIME_CLASS(CBuBThread));

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTpcApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTpcApp message handlers



BOOL CTpcApp::OnIdle(LONG lCount) 
{

	if (g_Container.m_fSiemensFatalError && ! g_Container.m_fSimulateCom)
	{
		AfxMessageBox (IDS_SIEMENS_FATAL, MB_ICONSTOP);
		exit (0);
	}

	g_GestorOrden.UpdateEstado ();

	// Actualizar el BuB
	// M2015 done from BuBThread g_Container.m_pDoc->SendUpdateToBuB ();

	// Michael 29.01.2002 Añadido condición de en funcionamiento para generar órdenes
	if (g_Container.m_fRequestUpdateSiemens && g_Container.IsEnFuncionamiento ())
	{
		CDBGestorOrden::ForzarOrden ();
		g_Container.m_fRequestUpdateSiemens = false;


	}

	if (g_Container.m_fQuizasAnularDelivery)
	{
		g_GestorOrden.QuizasEnviarRechazo ();
		g_Container.m_fQuizasAnularDelivery = false;
	}


	return CWinApp::OnIdle(lCount);
}

