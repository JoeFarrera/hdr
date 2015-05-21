#if !defined(AFX_TIEMPOORDENDLG_H__A403A653_7FD6_47E1_83D9_12E834F8A3FD__INCLUDED_)
#define AFX_TIEMPOORDENDLG_H__A403A653_7FD6_47E1_83D9_12E834F8A3FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TiempoOrdenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTiempoOrdenDlg dialog

class CTiempoOrdenDlg : public CDialog
{
// Construction
public:
	CTiempoOrdenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTiempoOrdenDlg)
	enum { IDD = IDD_TIEMPOORDEN };
	int		m_augmento;
	int		m_augmentoText;
	int		m_frecuencia;
	UINT	m_partida;
	int		m_frecuenciaText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTiempoOrdenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTiempoOrdenDlg)
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIEMPOORDENDLG_H__A403A653_7FD6_47E1_83D9_12E834F8A3FD__INCLUDED_)
