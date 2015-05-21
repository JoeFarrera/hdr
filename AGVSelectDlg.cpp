// AGVSelectDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "tpc.h"
#include "AGVSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CAGVSelectDlg 


CAGVSelectDlg::CAGVSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAGVSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAGVSelectDlg)
	m_nFz = 1;
	//}}AFX_DATA_INIT
}


void CAGVSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAGVSelectDlg)
	DDX_Text(pDX, IDC_EDIT_SELAGV, m_nFz);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAGVSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CAGVSelectDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CAGVSelectDlg 

void CAGVSelectDlg::OnCancel() 
{
	// TODO: Zusätzlichen Bereinigungscode hier einfügen
	m_nFz = 0;

	CDialog::OnCancel();
}

