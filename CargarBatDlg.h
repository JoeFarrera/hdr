#if !defined(AFX_CARGARBATDLG_H__46141761_45FA_4182_8436_8B99D2534EE2__INCLUDED_)
#define AFX_CARGARBATDLG_H__46141761_45FA_4182_8436_8B99D2534EE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CargarBatDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCargarBatDlg dialog

class CCargarBatDlg : public CDialog
{
// Construction
public:
	CCargarBatDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCargarBatDlg)
	enum { IDD = IDD_CARGABAT };
	UINT	m_nAgv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCargarBatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCargarBatDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARGARBATDLG_H__46141761_45FA_4182_8436_8B99D2534EE2__INCLUDED_)
