#if !defined(AFX_PLCTHREAD_H__56EE4F17_A05C_4124_82A5_35B23EC1C219__INCLUDED_)
#define AFX_PLCTHREAD_H__56EE4F17_A05C_4124_82A5_35B23EC1C219__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PLCThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CPLCThread thread

class CPLCThread : public CWinThread
{
	DECLARE_DYNCREATE(CPLCThread)
protected:
	CPLCThread();           // protected constructor used by dynamic creation
	void OnUserMessageSiemens (WPARAM wParam, LPARAM lParam);
	void OnUserMessageSiemensSend (WPARAM wParam, LPARAM lParam);
	void OnUserTestUIThread (WPARAM wParam, LPARAM lParam);

// Attributes
public:
	struct sqlca sqlca;
	struct oraca oraca;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPLCThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPLCThread();

	// Generated message map functions
	//{{AFX_MSG(CPLCThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLCTHREAD_H__56EE4F17_A05C_4124_82A5_35B23EC1C219__INCLUDED_)
