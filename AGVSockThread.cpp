// AGVSockThread.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "AgvClientSocket.h"
#include "WlanMsg.h"
#include "Agv.h"
#include "AGVSockThread.h"




// CAGVSockThread

IMPLEMENT_DYNCREATE(CAGVSockThread, CWinThread)

CAGVSockThread::CAGVSockThread()
{
}

CAGVSockThread::~CAGVSockThread()
{
	if(m_pMsg)
	{
		if(m_pMsg->m_ptrCliSock)
		{
			// This is already done delete m_pMsg->m_ptrCliSock;
			m_pMsg->m_ptrCliSock = NULL;
		}

		delete m_pMsg;
		m_pMsg = NULL;
	}

	if (m_pSocket)
		delete m_pSocket;


}



BOOL CAGVSockThread::InitInstance()
{
	CString str;
	m_pSocket = new CAgvClientSocket(m_pAgv);
	m_pSocket->Attach(m_hSocket);	// What if m_hSocket not initiated
	m_pSocket->Init();

	str.Format("Agv%dThread", m_pAgv->m_id);
	g_Container.m_dbutil.ConectarOracle (str);

	m_fRun = false;


	m_pMsg = m_pSocket->GetPtrWlanMsg();

	m_pAgv->SetClientConnection (m_pMsg);
	
	m_fRun = true;
	m_fTerminate = false;

	m_nLastMessageCount = 0;

	m_timerId = ::SetTimer(NULL,0,1000,NULL);

	return TRUE;
}

void CAGVSockThread::SetAgvPtr (CAgv * ptrAgv) 
{
	m_pAgv = ptrAgv;
}


int CAGVSockThread::ExitInstance()
{
	MSGERROR1 ("Cerrando thread de comunicación con AGV %d", m_pAgv->m_id);
	return CWinThread::ExitInstance();
}

void CAGVSockThread::OnTimer(WPARAM wParam, LPARAM /*lParam*/)
{
	if (m_fTerminate)
	{
		m_pSocket->DoClose();
		::PostQuitMessage(0);
		return;
	}

	if (m_pSocket->IsAborted())
	{
		// Not exiting as exit occurs when next one starts
		return;
	}

	// If there haven't been any incoming messages since the last OnTimer(), perhaps the
	// socket is losing OnReceive events
	if (m_pSocket->GetIncomingMessageCount() == m_nLastMessageCount)
		m_pSocket->HasDataIncoming();

	m_nLastMessageCount = m_pSocket->GetIncomingMessageCount();

if	(wParam == m_timerId)
{
	CString strEstado;
	if (m_pAgv->HasBeenPolled())
		strEstado = m_pAgv->GetStringEstado();
	else
		strEstado = "Sin Comunicar";

	MSGAVISO2 ("Update timer for AGV:%d Estado: %s ", m_pAgv->m_id, m_pAgv->GetStringEstado());
	if (m_hSocket != INVALID_SOCKET)
	{
		if (m_pAgv != NULL && m_fRun)
		{
			m_pMsg->SendNextMsg();
			//if (m_pAgv->HasBeenPolled())
			//{
			//	if (m_pAgv->IsBlocked())
			//	{
			//		m_pAgv->UpdateEstadoGrossStatusWlan();
			//		if (m_pAgv->IsWlanUpdateEstado())
			//		{
			//			m_pAgv->MaybeSendQuittum();
			//			m_pAgv->ResetWlanUpdateEstado();
			//		}
			//	}
			//	m_pAgv->UpdateEstado();
			//	m_pAgv->MaybeUnblockDrive();
	
				
			
		}
	}
	else 
	{
		MSGERROR1 ("Agv %d thread socket closed!", m_pAgv->m_id);
	}

	// Timer repeats - not necessary ::SetTimer(NULL,0,5000,NULL);
	
}
}



BEGIN_MESSAGE_MAP(CAGVSockThread, CWinThread)
	ON_THREAD_MESSAGE(WM_TIMER,	OnTimer)	
END_MESSAGE_MAP()


// CAGVSockThread message handlers
