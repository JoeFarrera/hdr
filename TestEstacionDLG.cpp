// TestEstacionDLG.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "TestEstacionDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestEstacionDLG dialog


CTestEstacionDLG::CTestEstacionDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CTestEstacionDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestEstacionDLG)
	m_CodClIND = _T("");
	m_nLado = 0;
	m_nPosicion = 0;
	m_nPtoCom = 0;
	//}}AFX_DATA_INIT
}


void CTestEstacionDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestEstacionDLG)
	DDX_Text(pDX, IDC_CODCLIND, m_CodClIND);
	DDX_Text(pDX, IDC_LADO, m_nLado);
	DDX_Text(pDX, IDC_POSICION, m_nPosicion);
	DDX_Text(pDX, IDC_PTOCOM, m_nPtoCom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestEstacionDLG, CDialog)
	//{{AFX_MSG_MAP(CTestEstacionDLG)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestEstacionDLG message handlers
