#pragma once


class CClientSocket;
class WlanMsg;
class CAgvClientSocket;


// CAGVSockThread


class CAGVSockThread : public CWinThread
{
	DECLARE_DYNCREATE(CAGVSockThread)

protected:
	CAGVSockThread();           // protected constructor used by dynamic creation
	CAgvClientSocket * m_pSocket;
	

public:
	void SetTerminate () { m_fTerminate = true; };
	virtual ~CAGVSockThread();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void SetAgvPtr (CAgv * ptrAgv);
	void OnTimer(WPARAM wParam, LPARAM /*lParam*/);

	SOCKET m_hSocket;


private:
	bool m_fTerminate;
	WlanMsg * m_pMsg;	// Message reference pointer
	CAgv * m_pAgv;
	bool m_fRun;
	UINT m_timerId;
	int m_nLastMessageCount;


protected:
	DECLARE_MESSAGE_MAP()
};


