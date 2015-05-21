// tpcView.cpp : implementation of the CTpcView class
//

#include "stdafx.h"
#include "tpc.h"

#include "tpcDoc.h"
#include "tpcView.h"
#include "TraceLevelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIRECTORIOLOG "C:\\projects\\tpc\\log\\"
#define BUFFER MAX_PATH 

/////////////////////////////////////////////////////////////////////////////
// CTpcView

IMPLEMENT_DYNCREATE(CTpcView, CListView)

BEGIN_MESSAGE_MAP(CTpcView, CListView)
	//{{AFX_MSG_MAP(CTpcView)
	ON_COMMAND(ID_VER_FILTROTRACE, OnVerFiltroTrace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpcView construction/destruction

CTpcView::CTpcView()
{
	m_log.pFile = NULL;
	m_log.strLogPath.Empty ();

	m_ImageList.Create (IDB_BITMAP3, 16, 1, RGB (255, 0, 255));





	// Inicializar flags de nivel de mensajes para funcion Message
	m_fAviso = 	AfxGetApp ()->GetProfileInt ("Settings", "ShowAviso", 1) == 1;
	m_fError = AfxGetApp ()->GetProfileInt ("Settings", "ShowError", 1) == 1;
	m_fFatal = AfxGetApp ()->GetProfileInt ("Settings", "ShowFatal", 1) == 1;
	m_fTrace = AfxGetApp ()->GetProfileInt ("Settings", "ShowTrace", 1) == 1;
	m_fWarning = AfxGetApp ()->GetProfileInt ("Settings", "ShowWarning", 1) == 1;
	m_fAvisoFichero = 	AfxGetApp ()->GetProfileInt ("Settings", "ShowAvisoFichero", 1) == 1;
	m_fErrorFichero = AfxGetApp ()->GetProfileInt ("Settings", "ShowErrorFichero", 1) == 1;
	m_fFatalFichero = AfxGetApp ()->GetProfileInt ("Settings", "ShowFatalFichero", 1) == 1;
	m_fTraceFichero = AfxGetApp ()->GetProfileInt ("Settings", "ShowTraceFichero", 1) == 1;
	m_fWarningFichero = AfxGetApp ()->GetProfileInt ("Settings", "ShowWarningFichero", 1) == 1;


}

CTpcView::~CTpcView()
{
	if(m_log.pFile!= NULL)
	{
		delete m_log.pFile;	
	}

}

BOOL CTpcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= LVS_REPORT;	// Empezamos en estilo report

	g_Container.SetPtrView (this);	// Inicializar container view

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTpcView drawing

void CTpcView::OnDraw(CDC* pDC)
{
	CTpcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CListCtrl& refCtrl = GetListCtrl();
	refCtrl.InsertItem(0, "Item!");
	// TODO: add draw code for native data here
}

void CTpcView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl & listCtrl = GetListCtrl ();
	CString strR;

	// Michael 25.07.2002 Añadir iconos a la lista
	listCtrl.SetImageList (&m_ImageList,LVSIL_SMALL);

	// Columnas de la lista
	strR.LoadString (IDS_MSGTYPE);
	listCtrl.InsertColumn (0, strR, LVCFMT_LEFT, 50);
	strR.LoadString (IDS_MSGTIME);
	listCtrl.InsertColumn (1, strR, LVCFMT_LEFT, 110);
	strR.LoadString (IDS_MSGMESSAGE);
	listCtrl.InsertColumn (2, strR, LVCFMT_LEFT, 250);
	strR.LoadString (IDS_MSGMODULE);
	listCtrl.InsertColumn (3, strR, LVCFMT_LEFT, 96);
	strR.LoadString (IDS_MSGFILE);
	listCtrl.InsertColumn (4, strR, LVCFMT_LEFT, 200);
	strR.LoadString (IDS_MSGLINE);
	listCtrl.InsertColumn (5, strR, LVCFMT_LEFT, 50);
	
	Message (MTRACE,  __FILE__, __LINE__, "Arranque TPC", "tpcView");
	


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CTpcView diagnostics

#ifdef _DEBUG
void CTpcView::AssertValid() const
{
	CListView::AssertValid();
}

void CTpcView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CTpcDoc* CTpcView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTpcDoc)));
	return (CTpcDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTpcView message handlers
void CTpcView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window
}

// Display message on the list...
void CTpcView::Message(UINT type, LPCSTR file, UINT linea, LPCSTR modulo, LPCSTR strMensaje)
{	
	static TCHAR * tipo [] = { "Trace", "Info", "Aviso", "Error", "Fatal"};
	static UINT imagen [] = { 0, 1, 1, 2, 3};

	CListCtrl & listCtrl = GetListCtrl ();


	CString strLinea;
	strLinea.Format (_T("%d"), linea);

	CTime time = CTime::GetCurrentTime ();
	int nCount = listCtrl.GetItemCount();
	
	// Determinar si sacar el mensaje o no

	
	bool fDisplay = (type == MTRACE && m_fTrace) 
			|| (type ==	MAVISO && m_fAviso)
			|| (type ==	MWARNING && m_fWarning)
			|| (type ==	MERROR && m_fError)
			|| (type ==	MFATAL && m_fFatal);

	bool fFichero = (type == MTRACE && m_fTraceFichero) 
		|| (type ==	MAVISO && m_fAvisoFichero)
		|| (type ==	MWARNING && m_fWarningFichero)
		|| (type ==	MERROR && m_fErrorFichero)
		|| (type ==	MFATAL && m_fFatalFichero);

	if (fDisplay)
	{

	if (nCount > 1000)
	{
		// Borrar algunos mensajes
		// AfxMessageBox ("Deleting all items");
		listCtrl.DeleteAllItems ();
		nCount = 0;
	}

	// CString strTime = time.Format ("%c");

	// Michael 25.07.2002 Añadir iconos a la lista
//	listCtrl.InsertItem (nCount, tipo [type], 0);
	listCtrl.InsertItem (LVIF_IMAGE, nCount, NULL, 0, 0, imagen [type], 0);

	listCtrl.SetItemText (nCount, 1, time.Format ("%c"));
	listCtrl.SetItemText (nCount, 2, strMensaje);
	if (modulo != NULL)
		listCtrl.SetItemText (nCount, 3, modulo);
	listCtrl.SetItemText (nCount, 4, file);
	listCtrl.SetItemText (nCount, 5, strLinea);


	listCtrl.SetItem (nCount, 0, LVIF_IMAGE | LVIF_TEXT, tipo [type], imagen [type], 0, 0, 0); 
	if (g_Container.m_fScrollList)
		listCtrl.EnsureVisible(nCount, FALSE);


	}

	if (fFichero)
	{
		
		if (m_log.pFile == NULL || m_log.hora != time.GetHour ())
		{
			m_log.pFile = GetFicheroLog ();
		}
		m_log.strText.Format ("%-8s %s %-60s %-15s %-20s %s\n",
				tipo [type],
				time.Format ("%c"),
				strMensaje,
				modulo,
				file,
				strLinea);

		m_log.pFile->WriteString (m_log.strText);
		m_log.pFile->Flush ();
	}

}


CStdioFile * CTpcView::GetFicheroLog()
{
	CString strFichero;
	
	m_log.timeAct = CTime::GetCurrentTime ();

	m_log.hora = m_log.timeAct.GetHour ();

	if (m_log.strLogPath.IsEmpty ())
	{
		m_log.strLogPath = AfxGetApp ()->GetProfileString ("Settings", "LogPath");
		if (m_log.strLogPath.IsEmpty ())
		{
			CString strTemp;
			strTemp.LoadString (IDS_NOLOGPATHSET);
			AfxMessageBox (strTemp);
			AfxGetApp ()->WriteProfileString ("Settings", "LogPath", ".\\");
		}
		else if (m_log.strLogPath.Right (1) != "\\")
		{
			m_log.strLogPath += "\\";
		}
	}
	
	strFichero.Format ("%s%02d%02d%02d%02d.log",
		m_log.strLogPath,
		m_log.timeAct.GetYear (),
		m_log.timeAct.GetMonth (),
		m_log.timeAct.GetDay (),
		m_log.timeAct.GetHour ());

	if (m_log.pFile)
	{
		m_log.pFile->Close ();
		delete m_log.pFile;
	}

	// Michael XXX 04.09.2001: If the file doesn't exist, This asserts
	try
	{
	m_log.pFile = new CStdioFile (strFichero, 
							CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone);
	}
	catch (	CFileException e)
	{
		e.ReportError ();
	}

	ASSERT (m_log.pFile != NULL);

	m_log.pFile->SeekToEnd ();

	m_log.horaLog = m_log.timeAct.GetHour();

	return (m_log.pFile);

}



// Actualizar filtros de mensajes
void CTpcView::OnVerFiltroTrace() 
{
	CTraceLevelDlg traceDlg;
	traceDlg.m_fAviso = m_fAviso;
	traceDlg.m_fTrace = m_fTrace;
	traceDlg.m_fWarning = m_fWarning;
	traceDlg.m_fError = m_fError;
	traceDlg.m_fFatal = m_fFatal;
	traceDlg.m_fAvisoFichero = m_fAvisoFichero;
	traceDlg.m_fTraceFichero = m_fTraceFichero;
	traceDlg.m_fWarningFichero = m_fWarningFichero;
	traceDlg.m_fErrorFichero = m_fErrorFichero;
	traceDlg.m_fFatalFichero = m_fFatalFichero;

	if (IDOK == traceDlg.DoModal ())
	{
		m_fAviso = traceDlg.m_fAviso;
		m_fTrace = traceDlg.m_fTrace;
		m_fWarning = traceDlg.m_fWarning;
		m_fError = traceDlg.m_fError;
		m_fFatal = traceDlg.m_fFatal;
		m_fAvisoFichero = traceDlg.m_fAvisoFichero;
		m_fTraceFichero = traceDlg.m_fTraceFichero;
		m_fWarningFichero = traceDlg.m_fWarningFichero;
		m_fErrorFichero = traceDlg.m_fErrorFichero;
		m_fFatalFichero = traceDlg.m_fFatalFichero;

		AfxGetApp ()->WriteProfileInt ("Settings", "ShowAviso", m_fAviso ? 1 : 0);
		AfxGetApp ()->WriteProfileInt ("Settings", "ShowError", m_fError ? 1 : 0);
		AfxGetApp ()->WriteProfileInt ("Settings", "ShowFatal", m_fFatal ? 1 : 0);
		AfxGetApp ()->WriteProfileInt ("Settings", "ShowTrace", m_fTrace ? 1 : 0);
		AfxGetApp ()->WriteProfileInt ("Settings", "ShowWarning", m_fWarning ? 1 : 0);
		AfxGetApp ()->WriteProfileInt ("Settings", "ShowAvisoFichero", m_fAvisoFichero ? 1 : 0);
		AfxGetApp ()->WriteProfileInt ("Settings", "ShowErrorFichero", m_fErrorFichero ? 1 : 0);
		AfxGetApp ()->WriteProfileInt ("Settings", "ShowFatalFichero", m_fFatalFichero ? 1 : 0);
		AfxGetApp ()->WriteProfileInt ("Settings", "ShowTraceFichero", m_fTraceFichero ? 1 : 0);
		AfxGetApp ()->WriteProfileInt ("Settings", "ShowWarningFichero", m_fWarningFichero ? 1 : 0);
	}
}



// Borrar logs antiguos (7 dias)
void CTpcView::BorrarLogs() 
{
	SYSTEMTIME lpSystemTime;
	FILETIME ftSystemTime;
	WIN32_FIND_DATA fd;
	HANDLE hFind;
    DWORD cchCurDir; 
    LPTSTR lpszCurDir;
	TCHAR tchBuffer[BUFFER];
	CString strLogPath;

	strLogPath = AfxGetApp ()->GetProfileString ("Settings", "LogPath");
	if (strLogPath.IsEmpty ())
	{
		CString strTemp;
		strTemp.LoadString (IDS_NOLOGPATHSET);
		AfxMessageBox (strTemp);
		AfxGetApp ()->WriteProfileString ("Settings", "LogPath", ".\\");
	}


	//Generem la data minima a partir de la data del sistema
	GetSystemTime(&lpSystemTime);       
	if ((lpSystemTime.wDay - 7) >= 1)
		lpSystemTime.wDay = lpSystemTime.wDay - 7;
	else
	{
		if ((lpSystemTime.wMonth - 1) >= 1)
			lpSystemTime.wMonth = lpSystemTime.wMonth - 1;
		else
		{
			lpSystemTime.wYear = lpSystemTime.wYear - 1;
			lpSystemTime.wMonth = 12;
		}
		switch(lpSystemTime.wMonth)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			lpSystemTime.wDay = 31 - (lpSystemTime.wDay - 1);
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			lpSystemTime.wDay = 30 - (lpSystemTime.wDay - 1);
			break;
		case 2:
			lpSystemTime.wDay = 28 - (lpSystemTime.wDay - 1);
		}
	}

    SystemTimeToFileTime(&lpSystemTime,&ftSystemTime);
	
	//Fixem el directori inicial
	lpszCurDir = tchBuffer;
	cchCurDir = sizeof (tchBuffer);
    ::GetCurrentDirectory(cchCurDir, lpszCurDir); 
	::SetCurrentDirectory(strLogPath);
	
	hFind  = ::FindFirstFile(_T("*.*"),&fd);
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do
		{	
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{					
				if (CompareFileTime(&fd.ftCreationTime,&ftSystemTime)<0) 
				TRY
				{
				   CFile::Remove( fd.cFileName );
				}
				CATCH( CFileException, e )
				{
				}
				END_CATCH
			}  		   					
		}
		while (::FindNextFile(hFind,&fd));
		::FindClose (hFind);
	}
	::SetCurrentDirectory(lpszCurDir); 


}







