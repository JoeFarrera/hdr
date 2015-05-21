// TestSRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "TestSRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestSRDlg dialog


CTestSRDlg::CTestSRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestSRDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestSRDlg)
	m_tipoTel = _T("");
	for (int i = 0; i < 19; i++)
		m_Edit [i] = 0;
	//}}AFX_DATA_INIT
}


void CTestSRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestSRDlg)
	DDX_Text(pDX, IDC_TIPOTEL, m_tipoTel);
	DDV_MaxChars(pDX, m_tipoTel, 1);
	DDX_Text(pDX, IDC_EDIT1, m_Edit [0]);
	DDV_MinMaxByte(pDX, m_Edit [0], 0, 3);
	DDX_Text(pDX, IDC_EDIT2, m_Edit [1]);
	DDV_MinMaxByte(pDX, m_Edit [1], 0, 3);
	DDX_Text(pDX, IDC_EDIT3, m_Edit [2]);
	DDV_MinMaxByte(pDX, m_Edit [2], 0, 3);
	DDX_Text(pDX, IDC_EDIT4, m_Edit [3]);
	DDV_MinMaxByte(pDX, m_Edit [3], 0, 3);
	DDX_Text(pDX, IDC_EDIT5, m_Edit [4]);
	DDV_MinMaxByte(pDX, m_Edit [4], 0, 3);
	DDX_Text(pDX, IDC_EDIT6, m_Edit [5]);
	DDV_MinMaxByte(pDX, m_Edit [5], 0, 3);
	DDX_Text(pDX, IDC_EDIT7, m_Edit [6]);
	DDV_MinMaxByte(pDX, m_Edit [6], 0, 3);
	DDX_Text(pDX, IDC_EDIT8, m_Edit [7]);
	DDV_MinMaxByte(pDX, m_Edit [7], 0, 3);
	DDX_Text(pDX, IDC_EDIT9, m_Edit [8]);
	DDV_MinMaxByte(pDX, m_Edit [8], 0, 3);
	DDX_Text(pDX, IDC_EDIT10, m_Edit [9]);
	DDV_MinMaxByte(pDX, m_Edit [9], 0, 3);
	DDX_Text(pDX, IDC_EDIT11, m_Edit [10]);
	DDV_MinMaxByte(pDX, m_Edit [10], 0, 3);
	DDX_Text(pDX, IDC_EDIT12, m_Edit [11]);
	DDV_MinMaxByte(pDX, m_Edit [11], 0, 3);
	DDX_Text(pDX, IDC_EDIT13, m_Edit [12]);
	DDV_MinMaxByte(pDX, m_Edit [12], 0, 3);
	DDX_Text(pDX, IDC_EDIT14, m_Edit [13]);
	DDV_MinMaxByte(pDX, m_Edit [13], 0, 3);
	DDX_Text(pDX, IDC_EDIT15, m_Edit [14]);
	DDV_MinMaxByte(pDX, m_Edit [14], 0, 3);
	DDX_Text(pDX, IDC_EDIT16, m_Edit [15]);
	DDV_MinMaxByte(pDX, m_Edit [15], 0, 3);
	DDX_Text(pDX, IDC_EDIT17, m_Edit [16]);
	DDV_MinMaxByte(pDX, m_Edit [16], 0, 3);
	DDX_Text(pDX, IDC_EDIT18, m_Edit [17]);
	DDV_MinMaxByte(pDX, m_Edit [17], 0, 3);
	DDX_Text(pDX, IDC_EDIT19, m_Edit [18]);
	DDV_MinMaxByte(pDX, m_Edit [18], 0, 3);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestSRDlg, CDialog)
	//{{AFX_MSG_MAP(CTestSRDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestSRDlg message handlers
