#if !defined(AFX_TRACELEVELDLG_H__A68FBB3F_CA43_4BFB_B96C_8E3B8F1EB0F6__INCLUDED_)
#define AFX_TRACELEVELDLG_H__A68FBB3F_CA43_4BFB_B96C_8E3B8F1EB0F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TraceLevelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTraceLevelDlg dialog

class CTraceLevelDlg : public CDialog
{
// Construction
public:
	CTraceLevelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTraceLevelDlg)
	enum { IDD = IDD_TRACELEVEL };
	BOOL	m_fAviso;
	BOOL	m_fError;
	BOOL	m_fFatal;
	BOOL	m_fTrace;
	BOOL	m_fWarning;
	BOOL	m_fAvisoFichero;
	BOOL	m_fErrorFichero;
	BOOL	m_fFatalFichero;
	BOOL	m_fTraceFichero;
	BOOL	m_fWarningFichero;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTraceLevelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTraceLevelDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACELEVELDLG_H__A68FBB3F_CA43_4BFB_B96C_8E3B8F1EB0F6__INCLUDED_)
