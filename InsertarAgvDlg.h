#if !defined(AFX_INSERTARAGVDLG_H__4865220A_22B1_42C1_96BB_58A42554DAE3__INCLUDED_)
#define AFX_INSERTARAGVDLG_H__4865220A_22B1_42C1_96BB_58A42554DAE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InsertarAgvDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsertarAgvDlg dialog

class CInsertarAgvDlg : public CDialog
{
// Construction
public:
	CInsertarAgvDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInsertarAgvDlg)
	enum { IDD = IDD_INSERTARAGVDLG };
	UINT	m_Mp;
	UINT	m_nAgv;
	UINT	m_Pos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsertarAgvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInsertarAgvDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSERTARAGVDLG_H__4865220A_22B1_42C1_96BB_58A42554DAE3__INCLUDED_)
