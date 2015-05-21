// CargarBatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "CargarBatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCargarBatDlg dialog


CCargarBatDlg::CCargarBatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCargarBatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCargarBatDlg)
	m_nAgv = 0;
	//}}AFX_DATA_INIT
}


void CCargarBatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCargarBatDlg)
	DDX_Text(pDX, IDC_NUMAGV, m_nAgv);
	DDV_MinMaxUInt(pDX, m_nAgv, 1, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCargarBatDlg, CDialog)
	//{{AFX_MSG_MAP(CCargarBatDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCargarBatDlg message handlers
