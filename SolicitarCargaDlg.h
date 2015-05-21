#if !defined(AFX_SOLICITARCARGADLG_H__23188D9B_2451_4D45_9608_6F5F7582A471__INCLUDED_)
#define AFX_SOLICITARCARGADLG_H__23188D9B_2451_4D45_9608_6F5F7582A471__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SolicitarCargaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSolicitarCargaDlg dialog

class CSolicitarCargaDlg : public CDialog
{
// Construction
public:
	CSolicitarCargaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSolicitarCargaDlg)
	enum { IDD = IDD_CARGABAT };
	UINT	m_nAgv;
	BOOL	m_fCargarBat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSolicitarCargaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSolicitarCargaDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOLICITARCARGADLG_H__23188D9B_2451_4D45_9608_6F5F7582A471__INCLUDED_)
