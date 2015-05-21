// TCPServerSocket.h : interface of the TCPServerSocket class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __TCPSOCK_H__
#define __TCPSOCK_H__


class ISocketServer;


class CTCPServerSocket : public CSocket
{
        DECLARE_DYNAMIC(CTCPServerSocket);

// Construction
public:
	BOOL m_bClose;
    CTCPServerSocket(ISocketServer * pSocketServer);

// Operations
public:
	BOOL TryListening();
	void TimerID(UINT nID) {m_nTimerID = nID;}
	UINT TimerID() {return m_nTimerID;}
	CSocket * m_pSockRecv;	// Socket object for Accept member function

// Implementation
protected:
	virtual void OnAccept (int nErrorCode);
	UINT m_nTimerID;
	BOOL m_bTryConnect;
	CTime m_startConnect;

	virtual void OnClose(int nErrorCode);
	virtual BOOL OnMessagePending();

private:
	ISocketServer * m_pSocketServer;
};

#endif // __TCPSOCK_H__
