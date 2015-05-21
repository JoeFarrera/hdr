// tpcView.h : interface of the CTpcView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TPCVIEW_H__D917B8A2_9D53_485B_8980_A7E078115C20__INCLUDED_)
#define AFX_TPCVIEW_H__D917B8A2_9D53_485B_8980_A7E078115C20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




class CTpcView : public CListView
{
protected: // create from serialization only
	CTpcView();
	DECLARE_DYNCREATE(CTpcView)

// Attributes
public:
	CTpcDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpcView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:

	void Message(UINT type, LPCSTR file, UINT linea, LPCSTR modulo, LPCSTR strMensaje);
	static void BorrarLogs(void);
	virtual ~CTpcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CImageList m_ImageList;
	CStdioFile * GetFicheroLog();
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

struct log_tag {
		CTime timeAct;
		int hora;
		CStdioFile * pFile;
		int horaLog;
		CString strText;
		CString strLogPath;
} m_log;


// Generated message map functions
protected:
	//{{AFX_MSG(CTpcView)
	afx_msg void OnVerFiltroTrace();
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in tpcView.cpp
inline CTpcDoc* CTpcView::GetDocument()
   { return (CTpcDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPCVIEW_H__D917B8A2_9D53_485B_8980_A7E078115C20__INCLUDED_)
