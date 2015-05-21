// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__A7FD2D97_A2E2_4EC5_AF84_4A4A2B311C3B__INCLUDED_)
#define AFX_MAINFRM_H__A7FD2D97_A2E2_4EC5_AF84_4A4A2B311C3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTpcView;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
//	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	static void Mensaje (MESSAGEPARMS *pMsg, const char *format, ...);
	virtual ~CMainFrame();
	CTpcView* GetRightPane();
	void OnTimer(UINT nIDEvent);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar      m_wndDlgBar;

// Generated message map functions
protected:
	afx_msg LRESULT OnUserMessageGlobal (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMessage (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMessageSiemens (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMessageSiemensSend (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMessageTestUIThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMessageGetEstadoPLC(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMessageAgvUpdateBuB(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMessageSocketRead(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserMessageSocketWrite(WPARAM wParam, LPARAM lParam);


	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTestsr();
	afx_msg void OnTestEstacion();
	afx_msg void OnWriteAGVHistory();
	afx_msg void OnUpdateSimulatecom(CCmdUI* pCmdUI);
	afx_msg void OnSimulatecom();
	afx_msg void OnUpdateGeneratesimorders(CCmdUI* pCmdUI);
	afx_msg void OnGeneratesimorders();
	afx_msg void OnInsertarAgv();
	afx_msg void OnCargabat();
	afx_msg void OnTestsrQ();
	afx_msg void OnHerramientasTestgenordenes();
	afx_msg void OnTiempoOrden();
	afx_msg void OnClose();
	afx_msg void OnScrollList();
	afx_msg void OnUpdateScrollList(CCmdUI* pCmdUI);
	afx_msg void OnReadsitstop();
	afx_msg void OnTestStopPlcComs();
	afx_msg void OnAutotrafficcontrol();
	afx_msg void OnUpdateAutotrafficcontrol(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	afx_msg void OnUpdateSocketRead(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSocketWrite(CCmdUI *pCmdUI);

	DECLARE_MESSAGE_MAP()
private:
	CTpcView * m_pView;
	CString m_sRead;
	CString m_sWrite;
public:
	afx_msg void OnHerramientasGuardarposicionesagvs();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__A7FD2D97_A2E2_4EC5_AF84_4A4A2B311C3B__INCLUDED_)
