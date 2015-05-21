// InsertarAgvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "InsertarAgvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsertarAgvDlg dialog


CInsertarAgvDlg::CInsertarAgvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsertarAgvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsertarAgvDlg)
	m_Mp = 0;
	m_nAgv = 0;
	m_Pos = 0;
	//}}AFX_DATA_INIT
}


void CInsertarAgvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertarAgvDlg)
	DDX_Text(pDX, IDS_INSERTARAGVMP, m_Mp);
	DDV_MinMaxUInt(pDX, m_Mp, 1, 300);
	DDX_Text(pDX, IDC_NUMAGVINS, m_nAgv);
	// Michael 24.05.2007 AGV9 DDV_MinMaxUInt(pDX, m_nAgv, 1, 8);
	DDV_MinMaxUInt(pDX, m_nAgv, 1, 11);
	DDX_Text(pDX, IDC_POSINS, m_Pos);
	DDV_MinMaxUInt(pDX, m_Pos, 0, 4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsertarAgvDlg, CDialog)
	//{{AFX_MSG_MAP(CInsertarAgvDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertarAgvDlg message handlers
