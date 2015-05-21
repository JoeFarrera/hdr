#if !defined(AFX_TESTGENORDDLG_H__4933AAF3_B336_4E82_B19A_225042BCA49E__INCLUDED_)
#define AFX_TESTGENORDDLG_H__4933AAF3_B336_4E82_B19A_225042BCA49E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestGenOrdDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestGenOrdDlg dialog

class CTestGenOrdDlg : public CDialog
{
// Construction
public:
	CTestGenOrdDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestGenOrdDlg)
	enum { IDD = IDD_TESTGENORD };
	CString	m_tipest;
	CString	m_codest;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestGenOrdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestGenOrdDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTGENORDDLG_H__4933AAF3_B336_4E82_B19A_225042BCA49E__INCLUDED_)
