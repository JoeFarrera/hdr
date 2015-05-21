// SolicitarCargaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "SolicitarCargaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSolicitarCargaDlg dialog


CSolicitarCargaDlg::CSolicitarCargaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSolicitarCargaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSolicitarCargaDlg)
	m_nAgv = 0;
	m_fCargarBat = FALSE;
	//}}AFX_DATA_INIT
}


void CSolicitarCargaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSolicitarCargaDlg)
	DDX_Text(pDX, IDC_NUMAGV, m_nAgv);
	DDV_MinMaxUInt(pDX, m_nAgv, 1, 8);
	DDX_Check(pDX, IDC_CARGARCHECK, m_fCargarBat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSolicitarCargaDlg, CDialog)
	//{{AFX_MSG_MAP(CSolicitarCargaDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSolicitarCargaDlg message handlers
