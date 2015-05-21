// TCPServerSocket.cpp : implementation file
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

#include <stddef.h>

#include "stdafx.h"
#include "tpc.h"
#include "TCPServerSocket.h"
#include "ISocketSink.h"
#include "TpcDoc.h"



#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CTCPServerSocket, CSocket)

CTCPServerSocket::CTCPServerSocket(ISocketServer* pSocketServer)
{
	// Michael: Asociar documento con el socket
	m_bClose = FALSE;
	m_nTimerID = 0;
	m_bTryConnect = FALSE;
	m_pSocketServer = pSocketServer;
	m_pSockRecv = NULL;
}


void CTCPServerSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);
	m_bClose = TRUE;
	

}


BOOL CTCPServerSocket::OnMessagePending()
{
	if (m_bTryConnect) {
		if (CTime::GetCurrentTime() > m_startConnect + CTimeSpan(0, 0, 0, 5)) {
			CancelBlockingCall();
			return FALSE;
		}
	}

	MSG msg;
	if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::DispatchMessage(&msg);
		return FALSE;
	}
	
	return CSocket::OnMessagePending();

}

// Realizar listen del socket
BOOL CTCPServerSocket::TryListening()
{
	if (!Listen ())
	{
		int nLast = GetLastError ();
		TRACE (_T("TryListening: GetLastError is: %d\n"), nLast);
		return (false);
	}
	else
		return (true);

}


void CTCPServerSocket::OnAccept(int nErrorCode)
{
	m_pSocketServer->ProcessPendingAccept (this);
		

}
