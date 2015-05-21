#if !defined(AFX_STARTTIME_H__E43D11F6_E2AB_429B_AC4E_9E177B8673C3__INCLUDED_)
#define AFX_STARTTIME_H__E43D11F6_E2AB_429B_AC4E_9E177B8673C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StartTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStartTime dialog

class CStartTime : public CDialog
{
// Construction
public:
	CStartTime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStartTime)
	enum { IDD = IDD_TIMEDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStartTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStartTime)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STARTTIME_H__E43D11F6_E2AB_429B_AC4E_9E177B8673C3__INCLUDED_)
