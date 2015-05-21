#if !defined(AFX_TESTESTACIONDLG_H__0D4DFF60_27B8_45BC_B1A0_6BF69B9C8446__INCLUDED_)
#define AFX_TESTESTACIONDLG_H__0D4DFF60_27B8_45BC_B1A0_6BF69B9C8446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestEstacionDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTestEstacionDLG dialog

class CTestEstacionDLG : public CDialog
{
// Construction
public:
	CTestEstacionDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTestEstacionDLG)
	enum { IDD = IDD_TESTGETESTACION };
	CString	m_CodClIND;
	UINT	m_nLado;
	UINT	m_nPosicion;
	UINT	m_nPtoCom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestEstacionDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTestEstacionDLG)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTESTACIONDLG_H__0D4DFF60_27B8_45BC_B1A0_6BF69B9C8446__INCLUDED_)
