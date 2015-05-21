#if !defined(AFX_DIALOGSTOPPLC_H__ADAA8354_5C60_4EA7_83B8_4D32460FBC2C__INCLUDED_)
#define AFX_DIALOGSTOPPLC_H__ADAA8354_5C60_4EA7_83B8_4D32460FBC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogStopPlc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogStopPlc dialog

class CDialogStopPlc : public CDialog
{
// Construction
public:
	CDialogStopPlc(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogStopPlc)
	enum { IDD = IDD_DIALOG_STOP_PLC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogStopPlc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogStopPlc)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSTOPPLC_H__ADAA8354_5C60_4EA7_83B8_4D32460FBC2C__INCLUDED_)
