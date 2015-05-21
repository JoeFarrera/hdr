#if !defined(AFX_SOLICITARCARGABAT_H__90DC4B05_BC7A_4528_A46B_7FC36A7E260F__INCLUDED_)
#define AFX_SOLICITARCARGABAT_H__90DC4B05_BC7A_4528_A46B_7FC36A7E260F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SolicitarCargaBat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSolicitarCargaBat dialog

class CSolicitarCargaBat : public CDialog
{
// Construction
public:
	CSolicitarCargaBat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSolicitarCargaBat)
	enum { IDD = IDD_CARGABAT };
	int		m_cargador;
	UINT	m_nAgv;
	BOOL	m_fCargar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSolicitarCargaBat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSolicitarCargaBat)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOLICITARCARGABAT_H__90DC4B05_BC7A_4528_A46B_7FC36A7E260F__INCLUDED_)
