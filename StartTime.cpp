// StartTime.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "StartTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStartTime dialog


CStartTime::CStartTime(CWnd* pParent /*=NULL*/)
	: CDialog(CStartTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStartTime)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStartTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStartTime)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStartTime, CDialog)
	//{{AFX_MSG_MAP(CStartTime)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStartTime message handlers
