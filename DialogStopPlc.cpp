// DialogStopPlc.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "DialogStopPlc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogStopPlc dialog


CDialogStopPlc::CDialogStopPlc(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogStopPlc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogStopPlc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CDialogStopPlc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogStopPlc)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogStopPlc, CDialog)
	//{{AFX_MSG_MAP(CDialogStopPlc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogStopPlc message handlers

void CDialogStopPlc::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();

}
