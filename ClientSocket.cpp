// ClientSocket.cpp : implementation of the CClientSocket class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "IMsg.h"
#include "ClientSocket.h"
#include "TpcDoc.h"
#include "tpc.h"

#include "msg.h"
#include "ISocketSink.h"

/////////////////////////////////////////////////////////////////////////////
// CClientSocket Construction

CClientSocket::CClientSocket(ISocketSink * socketSink)
{
	m_pSocketSink = socketSink;
	m_nMsgCount = 0;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
}

CClientSocket::CClientSocket()
{
	CClientSocket(NULL);

}

/////////////////////////////////////////////////////////////////////////////
// CClientSocket Operations

void CClientSocket::Init()
{
	m_pFile = new CSocketFile(this);
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);
	m_pArchiveOut = new CArchive(m_pFile,CArchive::store);
}

void CClientSocket::Abort()
{
	if (m_pArchiveOut != NULL)
	{
		m_pArchiveOut->Abort();
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}
	MSGERROR("** Conexión BuB abortada **");
}

void CClientSocket::SendMsg(IMsg* pMsg)
{
	if (!this->IsAborted())
	{
		g_Container.m_pMainWnd->PostMessage (WM_USER_SOCKET_WRITE, 0, (LPARAM)"Write");
		pMsg->Serialize(*m_pArchiveOut);
		m_pArchiveOut->Flush();	// M2015 TODO: Seems to block here 
		g_Container.m_pMainWnd->PostMessage (WM_USER_SOCKET_WRITE, 0, (LPARAM)"[----]");
	}
}


void CClientSocket::ReceiveMsg(IMsg* pMsg)
{
	g_Container.m_pMainWnd->PostMessage (WM_USER_SOCKET_READ, 0, (LPARAM)"[Read]");

	pMsg->Serialize(*m_pArchiveIn);
	g_Container.m_pMainWnd->PostMessage (WM_USER_SOCKET_READ, 0, (LPARAM)"[----]");


	pMsg->SetClientSocket(this);
}

/////////////////////////////////////////////////////////////////////////////
// CClientSocket Overridable callbacks

void CClientSocket::OnReceive(int nErrorCode)
{
	TRACE(_T("%s: CClientSocket::OnReceive(%d)\n"), AfxGetApp()->m_pszAppName, nErrorCode);
	CSocket::OnReceive(nErrorCode);

	if (nErrorCode != ERROR_SUCCESS)
		nErrorCode = nErrorCode;

	m_pSocketSink ->ProcessPendingRead(this);
	TRACE(_T("After %s: CClientSocket::OnReceive(%d)\n"), AfxGetApp()->m_pszAppName, nErrorCode);

}


/////////////////////////////////////////////////////////////////////////////
// CSocket Implementation

CClientSocket::~CClientSocket()
{
	// TODO - this asserts, as the socket is invalid
	if (m_pArchiveOut != NULL)
		delete m_pArchiveOut;

	if (m_pArchiveIn != NULL)
		delete m_pArchiveIn;

	if (m_pFile != NULL)
		delete m_pFile;
}

#ifdef _DEBUG
void CClientSocket::AssertValid() const
{
	CSocket::AssertValid();
}

void CClientSocket::Dump(CDumpContext& dc) const
{
	CSocket::Dump(dc);
}
#endif //_DEBUG

IMPLEMENT_DYNAMIC(CClientSocket, CSocket)


void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);
	if (m_pSocketSink)
		m_pSocketSink->CloseSocket(this);
}
