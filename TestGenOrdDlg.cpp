// TestGenOrdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "TestGenOrdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestGenOrdDlg dialog


CTestGenOrdDlg::CTestGenOrdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestGenOrdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestGenOrdDlg)
	m_tipest = _T("");
	m_codest = _T("");
	//}}AFX_DATA_INIT
}


void CTestGenOrdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestGenOrdDlg)
	DDX_Text(pDX, IDC_TIPEST, m_tipest);
	DDV_MaxChars(pDX, m_tipest, 1);
	DDX_Text(pDX, IDC_CODEST, m_codest);
	DDV_MaxChars(pDX, m_codest, 7);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestGenOrdDlg, CDialog)
	//{{AFX_MSG_MAP(CTestGenOrdDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestGenOrdDlg message handlers
