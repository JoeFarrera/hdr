// AgvClientSocket.cpp : implementation of the CAgvClientSocket class
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
#include "tpc.h"
#include "IMsg.h"
#include "WlanMsg.h"
#include "DBEstadoPLC.h"
#include "Agv.h"
#include "AgvClientSocket.h"
#include "TpcDoc.h"
#include "tpc.h"

#include "msg.h"
#include "ISocketSink.h"

/////////////////////////////////////////////////////////////////////////////
// CAgvClientSocket Construction

CAgvClientSocket::CAgvClientSocket(CAgv * pAgv)
{
	m_nMsgCount = 0;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
	m_pAgv = pAgv;
	m_pMsg = new WlanMsg();
	m_pMsg->SetClientSocket(this);
	m_bAborted = false;
}

CAgvClientSocket::CAgvClientSocket()
{
	CAgvClientSocket(NULL);

}

/////////////////////////////////////////////////////////////////////////////
// CAgvClientSocket Operations

void CAgvClientSocket::Init()
{
	m_pFile = new CSocketFile(this);
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);
	m_pArchiveOut = new CArchive(m_pFile,CArchive::store);
}

void CAgvClientSocket::SendMsg(WlanMsg * pMsg)
{
	TRY
	{
	if (!IsAborted())
	{
		// TODO g_Container.m_pMainWnd->PostMessage (WM_USER_SOCKET_WRITE, 0, (LPARAM)"Write");
		pMsg->Serialize(*m_pArchiveOut);
		m_pArchiveOut->Flush();	// M2015 TODO: Seems to block here 
		// TODO g_Container.m_pMainWnd->PostMessage (WM_USER_SOCKET_WRITE, 0, (LPARAM)"[----]");
	}
	}
	CATCH(CFileException, e)
	{
		CString strTemp;
		if (strTemp.LoadString(IDS_READERROR))
			MSGERROR (strTemp);
		{
		   TCHAR   szCause[255];

		   e->GetErrorMessage(szCause, 255);

		   MSGERROR2 ("FileException error %s en SendMsg del Agv: %d", szCause, m_pAgv->m_id);
		}
		pMsg->SetClosed();	// So caller knows message was not sent
		DoClose();
		
	}
	END_CATCH

}


void CAgvClientSocket::ReceiveMsg()
{
	//g_Container.m_pMainWnd->PostMessage (WM_USER_SOCKET_READ, 0, (LPARAM)"[Read]");

	m_pMsg->Serialize(*m_pArchiveIn);
	// g_Container.m_pMainWnd->PostMessage (WM_USER_SOCKET_READ, 0, (LPARAM)"[----]");


}

void CAgvClientSocket::IncrementIncomingMessageCount ()
{
	m_nMsgCount++;
	m_nMsgCount = (m_nMsgCount >= 9999999) ? 1 : m_nMsgCount;
}
// Ver si hay data en el buffer de entrada. Si la hay, provoca 
void CAgvClientSocket::HasDataIncoming()
{
	DWORD dwBytes;
	BOOL result = IOCtl(FIONREAD, &dwBytes);
	if (result && dwBytes > 0)
	{
		MSGWARNING2 ("CAgvClientSocket::HasDataIncoming: Hay %d bytes esperando.Agv: %d", dwBytes, m_pAgv->m_id);
		TCHAR c;
		Receive(&c, 1, MSG_PEEK);
	}


}

/////////////////////////////////////////////////////////////////////////////
// CAgvClientSocket Overridable callbacks

void CAgvClientSocket::OnReceive(int nErrorCode)
{

	TRACE(_T("%s: CAgvClientSocket::OnReceive(%d)\n"), AfxGetApp()->m_pszAppName, nErrorCode);
	CSocket::OnReceive(nErrorCode);

	if (nErrorCode != ERROR_SUCCESS)
	{
		MSGERROR2 ("Recibido ErrorCode: %d en OnReceive() Agv: %d. Cerrando conexión", 
			nErrorCode, m_pAgv);
		DoClose();
		return;
	}

	IncrementIncomingMessageCount();
	
	ProcessPendingRead();
	TRACE(_T("After %s: CAgvClientSocket::OnReceive(%d)\n"), AfxGetApp()->m_pszAppName, nErrorCode);

}

void CAgvClientSocket::ProcessPendingRead()
{
	do
	{
		// This would be reading more than once
		ReadMsg();

		if (m_pMsg->IsClosed())
		{
			// TODO CloseSocket();
			break;
		}
		else
		{
			// Leave message on queue
			MSGAVISO1("Wlan messages incoming Agv: %d", m_pAgv->m_id);
			// m_pAgv->ProcessMessages (m_pMsg);
		}
	}
	while (!m_pArchiveIn->IsBufferEmpty());
	

}

// Lectura de mensajes en el socket cliente pSocket
void CAgvClientSocket::ReadMsg()
{
	TRY
	{
		ReceiveMsg();	
	}
	CATCH(CFileException, e)
	{
		CString strTemp;
		if (strTemp.LoadString(IDS_READERROR))
			MSGERROR (strTemp);
		{
		   TCHAR   szCause[255];
		   CString strFormatted;

		   e->GetErrorMessage(szCause, 255);

		   strFormatted = _T("FileException error: ");
		   strFormatted += szCause;

		   MSGERROR (strFormatted);
		}
		m_pMsg->SetClosed();	// TODO - See how to control this
		DoClose();
	}
	END_CATCH

	return;
}

void CAgvClientSocket::ToStringAgv(char * str)
{
	if (m_pAgv)
	{
		_itoa_s(m_pAgv->m_id, str, 2, 10);
	}
	else
		str = "??";
}



/////////////////////////////////////////////////////////////////////////////
// CSocket Implementation

CAgvClientSocket::~CAgvClientSocket()
{
	DoClose(); // M2014 TODO 14.05.2015 Testing...
	// TODO - this asserts, as the socket is invalid
	if (m_pArchiveOut != NULL)
		delete m_pArchiveOut;

	if (m_pArchiveIn != NULL)
		delete m_pArchiveIn;

	if (m_pFile != NULL)
		delete m_pFile;
}

#ifdef _DEBUG
void CAgvClientSocket::AssertValid() const
{
	CSocket::AssertValid();
}

void CAgvClientSocket::Dump(CDumpContext& dc) const
{
	CSocket::Dump(dc);
}
#endif //_DEBUG

IMPLEMENT_DYNAMIC(CAgvClientSocket, CSocket)


void CAgvClientSocket::OnClose(int nErrorCode)
{
	MSGWARNING1(_T("CAgvClientSocket::OnClose() Agv: %d"), m_pAgv->m_id);
	    
    
    if(nErrorCode != ERROR_SUCCESS)
	{
		MSGERROR2 (_T("CAgvClientSocket::OnClose() Agv: %d ErrorCode: %d"), 
			m_pAgv->m_id,
			nErrorCode);
	} 
	CSocket::OnClose(nErrorCode);
	m_pAgv->SetSocketClosed();

    // passive close        
    DoClose();

}

void CAgvClientSocket::DoClose()
{
	MSGWARNING1 (_T("CAgvClientSocket::DoClose Agv: %d"), m_pAgv->m_id);

	if(m_hSocket == INVALID_SOCKET)
	{ 
		MSGAVISO1 (_T("CAgvClientSocket::DoClose Agv: %d - Socket already invalid"), m_pAgv->m_id);
		return;
	} 

	if (m_pArchiveOut != NULL)
	{
		m_pArchiveOut->Abort();
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}

	m_bAborted = true;

	ShutDown();
	Close();


} 