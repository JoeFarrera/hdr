// tpc.h : main header file for the TPC application
//


#if !defined(AFX_TPC_H__72ECFD04_E84C_4634_8047_B212582BDE67__INCLUDED_)
#define AFX_TPC_H__72ECFD04_E84C_4634_8047_B212582BDE67__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// #define FOREXPORTTOINDUMAT

#define SQLCA_STORAGE_CLASS extern
#include <sqlca.h>
#define ORACA_STORAGE_CLASS extern
#include <oraca.h>

typedef struct sqlca T_sqlca;
typedef struct oraca T_oraca;

#include "KONSTANT.H"
#include "KOMDAT.H"
#include "ZS_BED.H"

#include "CCourseLoader.h"

enum   T_Message {
	MTRACE,
	MAVISO,
	MWARNING,
	MERROR,
	MFATAL };


#include "resource.h"       // main symbols
#include "Container.h"	// Added by ClassView

#include "msgMacro.h"


/////////////////////////////////////////////////////////////////////////////
// CTpcApp:
// See tpc.cpp for the implementation of this class
//

extern CContainer g_Container;	// Global
extern TYP_FZDAT *fzdat;	// Global necessary for blocking functions

extern CCourseData * g_pCourseData;


class CTpcApp : public CWinApp
{
public:
	CTpcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpcApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTpcApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPC_H__72ECFD04_E84C_4634_8047_B212582BDE67__INCLUDED_)
