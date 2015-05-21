// TraceLevelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "TraceLevelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTraceLevelDlg dialog


CTraceLevelDlg::CTraceLevelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTraceLevelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTraceLevelDlg)
	m_fAviso = FALSE;
	m_fError = FALSE;
	m_fFatal = FALSE;
	m_fTrace = FALSE;
	m_fWarning = FALSE;
	m_fAvisoFichero = FALSE;
	m_fErrorFichero = FALSE;
	m_fFatalFichero = FALSE;
	m_fTraceFichero = FALSE;
	m_fWarningFichero = FALSE;
	//}}AFX_DATA_INIT
}


void CTraceLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTraceLevelDlg)
	DDX_Check(pDX, IDC_AVISO, m_fAviso);
	DDX_Check(pDX, IDC_ERROR, m_fError);
	DDX_Check(pDX, IDC_FATAL, m_fFatal);
	DDX_Check(pDX, IDC_TRACE, m_fTrace);
	DDX_Check(pDX, IDC_WARNING, m_fWarning);
	DDX_Check(pDX, IDC_AVISO2, m_fAvisoFichero);
	DDX_Check(pDX, IDC_ERROR2, m_fErrorFichero);
	DDX_Check(pDX, IDC_FATAL2, m_fFatalFichero);
	DDX_Check(pDX, IDC_TRACE2, m_fTraceFichero);
	DDX_Check(pDX, IDC_WARNING2, m_fWarningFichero);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTraceLevelDlg, CDialog)
	//{{AFX_MSG_MAP(CTraceLevelDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTraceLevelDlg message handlers
