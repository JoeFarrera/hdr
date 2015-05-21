// SolicitarCargaBat.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "SolicitarCargaBat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSolicitarCargaBat dialog


CSolicitarCargaBat::CSolicitarCargaBat(CWnd* pParent /*=NULL*/)
	: CDialog(CSolicitarCargaBat::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSolicitarCargaBat)
	m_cargador = -1;
	m_nAgv = 0;
	m_fCargar = FALSE;
	//}}AFX_DATA_INIT
}


void CSolicitarCargaBat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSolicitarCargaBat)
	DDX_Radio(pDX, IDC_RADIO1, m_cargador);
	DDX_Text(pDX, IDC_NUMAGV, m_nAgv);
	DDV_MinMaxUInt(pDX, m_nAgv, 1, 8);
	DDX_Check(pDX, IDC_CARGARCHECK, m_fCargar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSolicitarCargaBat, CDialog)
	//{{AFX_MSG_MAP(CSolicitarCargaBat)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSolicitarCargaBat message handlers
