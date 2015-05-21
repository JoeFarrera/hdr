#if !defined(AFX_TESTSRDLG_H__9A804738_8C2C_4CE2_8241_D7ECA6A21361__INCLUDED_)
#define AFX_TESTSRDLG_H__9A804738_8C2C_4CE2_8241_D7ECA6A21361__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestSRDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestSRDlg dialog

class CTestSRDlg : public CDialog
{
// Construction
public:
	CTestSRDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestSRDlg)
	enum { IDD = IDD_TESTSR };
	CString	m_tipoTel;
	BYTE	m_Edit [19];
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestSRDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestSRDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTSRDLG_H__9A804738_8C2C_4CE2_8241_D7ECA6A21361__INCLUDED_)
