// AgvClientSocket.cpp : implementation of the CAgvAsyncClientSocket class
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
#include "AgvAsyncClientSocket.h"
#include "TpcDoc.h"
#include "tpc.h"

#include "msg.h"
#include "ISocketSink.h"

/////////////////////////////////////////////////////////////////////////////
// CAgvAsyncClientSocket Construction

CAgvAsyncClientSocket::CAgvAsyncClientSocket(CAgv * pAgv)
{
	m_State = L0;
	m_nMsgCount = 0;
	m_pAgv = pAgv;
	m_pMsg = new WlanMsg();
	m_pMsg->SetClientSocket(this);
	//m_pMsg->SetClientSocket(this);
	m_bAborted = false;
}

CAgvAsyncClientSocket::CAgvAsyncClientSocket()
{
	CAgvAsyncClientSocket(NULL);

}

/////////////////////////////////////////////////////////////////////////////
// CAgvAsyncClientSocket Operations

void CAgvAsyncClientSocket::Init()
{
}


void CAgvAsyncClientSocket::IncrementIncomingMessageCount ()
{
	m_nMsgCount++;
	m_nMsgCount = (m_nMsgCount >= 9999999) ? 1 : m_nMsgCount;
}
// Ver si hay data en el buffer de entrada. Si la hay, provoca 
void CAgvAsyncClientSocket::HasDataIncoming()
{
	DWORD dwBytes;
	BOOL result = IOCtl(FIONREAD, &dwBytes);
	if (result && dwBytes > 0)
	{
		MSGWARNING2 ("CAgvAsyncClientSocket::HasDataIncoming: Hay %d bytes esperando.Agv: %d", dwBytes, m_pAgv->m_id);
		TCHAR c;
		Receive(&c, 1, MSG_PEEK);
	}
	else
	{
		MSGWARNING1 ("CAgvAsyncClientSocket::HasDataIncoming: No hay datos por leer.Agv: %d", m_pAgv->m_id);
	}


}

/////////////////////////////////////////////////////////////////////////////
// CAgvAsyncClientSocket Overridable callbacks

void CAgvAsyncClientSocket::OnReceiveKnowingLength(int nErrorCode)
{

	TRACE(_T("%s: CAgvAsyncClientSocket::OnReceive(%d)\n"), AfxGetApp()->m_pszAppName, nErrorCode);
	CAsyncSocket::OnReceive(nErrorCode);

	if (nErrorCode != ERROR_SUCCESS)
	{
		MSGERROR2 ("Recibido ErrorCode: %d en OnReceive() Agv: %d. Cerrando conexión", 
			nErrorCode, m_pAgv);
		DoClose();
		return;
	}

	IncrementIncomingMessageCount();
// --------------------

 
    int nRead = 0;

    switch(m_State)  
       { /* state */
        case ERRORSTATE: 
           break;
        case L0: 
           nRead = Receive(&Header.data[0], sizeof(int));
           switch(nRead)
              { /* L0 next state */
               case SOCKET_ERROR:
				  MSGERROR2 ("Recibido ErrorCode: %d en OnReceive() (LO) Agv: %d. Cerrando conexión", 
						::GetLastError(), m_pAgv);

                  DoClose();
                  break;            
               case 0:              
                  break;            
               case 1:              
                  m_State = L1;       
                  break;            
               case 2:              
                  m_State = L2;       
                  break;            
               case 3:              
                  m_State = L3;       
                  break;            
               case 4:              
                  SetDataState();   
                  break;            
              } /* L0 next state */ 
           break;                   
        case L1: // one byte already read  
           nRead = Receive(&Header.data[1], sizeof(int) - 1);  
           switch(nRead)            
              { /* L1 next state */ 
               case SOCKET_ERROR:   
				  MSGERROR2 ("Recibido ErrorCode: %d en OnReceive() (L1) Agv: %d. Cerrando conexión", 
						::GetLastError(), m_pAgv);
                  DoClose();
                  break;            
               case 0:              
                  break;            
               case 1:              
                  m_State = L2;       
                  break;            
               case 2:              
                  m_State = L3;       
                  break;            
               case 3:              
                  SetDataState();   
                  break;            
              } /* L1 next state */ 
           break;                   
        case L2:                    
           nRead = Receive(&Header.data[2], sizeof(int) - 2);
           switch(nRead)
              { /* L2 next state */
               case SOCKET_ERROR:  
				  MSGERROR2 ("Recibido ErrorCode: %d en OnReceive() (L2) Agv: %d. Cerrando conexión", 
						::GetLastError(), m_pAgv);

                  DoClose();
                  break;            
               case 0:              
                  break;            
               case 1:              
                  m_State = L3;       
                  break;            
               case 2:              
                  SetDataState();   
                  break;            
              } /* L2 next state */ 
           break;                   
        case L3:                    
           nRead = Receive(&Header.data[3], sizeof(int) - 3);
           switch(nRead)            
              { /* L3 next state */ 
               case SOCKET_ERROR:   
				  MSGERROR2 ("Recibido ErrorCode: %d en OnReceive() (L3) Agv: %d. Cerrando conexión", 
						::GetLastError(), m_pAgv);
                  DoClose();
                  break;            
               case 0:              
                  break;            
               case 1:              
                  SetDataState();   
                  break;            
              } /* L3 next state */ 
           break;                   
        case DATA:                  
           nRead = Receive(m_recvBuff.GetData() + m_bytesRead, Header.ByteCount);
           if(nRead == SOCKET_ERROR)       
              { /* unrecoverable error */ 
				  MSGERROR2 ("Recibido ErrorCode: %d en OnReceive() (DATA) Agv: %d. Cerrando conexión", 
						::GetLastError(), m_pAgv);
                  DoClose();
               break;                     
              } /* unrecoverable error */ 
           Header.ByteCount -= nRead;     
           if(Header.ByteCount == 0)      
              { /* all read */            
               ProcessPacket();
               m_State = L0; 
               break;      
              } /* all read */ 
           break;              
       } /* state */        
	TRACE(_T("After %s: CAgvAsyncClientSocket::OnReceive(%d)\n"), AfxGetApp()->m_pszAppName, nErrorCode);

}


void CAgvAsyncClientSocket::OnReceive(int nErrorCode)
{

	TRACE(_T("%s: CAgvAsyncClientSocket::OnReceive(%d)\n"), AfxGetApp()->m_pszAppName, nErrorCode);
	CAsyncSocket::OnReceive(nErrorCode);

	if (nErrorCode != ERROR_SUCCESS)
	{
		MSGERROR2 ("Recibido ErrorCode: %d en OnReceive() Agv: %d. Cerrando conexión", 
			nErrorCode, m_pAgv);
		DoClose();
		return;
	}

	IncrementIncomingMessageCount();
	
	DWORD dwBytes;
	BOOL result = IOCtl(FIONREAD, &dwBytes);
	m_recvBuff.SetSize(dwBytes);
   int nRead = Receive(m_recvBuff.GetData(), dwBytes);
   if(nRead == SOCKET_ERROR)       
	  { /* unrecoverable error */ 
		  MSGERROR2 ("Recibido ErrorCode: %d en OnReceive() (DATA) Agv: %d. Cerrando conexión", 
				::GetLastError(), m_pAgv);
		  DoClose();
	  } /* unrecoverable error */ 
   else
   {
	   ProcessPacket();
   }
	TRACE(_T("After %s: CAgvAsyncClientSocket::OnReceive(%d)\n"), AfxGetApp()->m_pszAppName, nErrorCode);

}


/****************************************************************************
*                        CServerSocket::ProcessReceive
* Inputs:
*       CByteArray & data: Raw data received
* Result: void
*       
* Effect: 
*       This processes the received data
****************************************************************************/

void CAgvAsyncClientSocket::ProcessReceive(CByteArray & data)
{
	// TODO - compare with Async server function
	CString strByte((LPCTSTR)data.GetData(), data.GetSize());
	m_pMsg->PutMessageOnQueue(strByte);	// CByteArray?
	m_pAgv->ProcessMessages (m_pMsg);



	//CString s = ConvertReceivedDataToString(data);
	//s = _T(">>>") + s + _T("<<<");

	//CByteArray msg;
	// ConvertStringToSendData(s, msg);
	//Send(msg, ::GetCurrentThreadId());
} // CServerSocket::ProcessReceive



void CAgvAsyncClientSocket::ProcessPacket()
    {
     if(Header.ByteCount > 0)
        { /* send partial packet */
         // This will truncate the buffer if we have a partial receipt
         m_recvBuff.SetSize(m_recvBuff.GetSize() - Header.ByteCount);
        } /* send partial packet */

     if(m_recvBuff.GetSize() == 0)
        return; // nothing to send

     ProcessReceive(m_recvBuff);

     m_recvBuff.SetSize(0);
     Header.ByteCount = 0;
    } // CConnectSoc::NotifyOwnerAboutPacket

/****************************************************************************
*                          CConnectSoc::SetDataState
* Result: void
*       
* Effect: 
*       Sets DATA state. Converts the byte count from Network Standard
*       Byte Order to Host Byte Order
****************************************************************************/

void CAgvAsyncClientSocket::SetDataState()
    {                                     
     Header.ByteCount = ntohl(Header.ByteCount);
     TRACE(_T("%s: CConnectSoc::SetDataState: ByteCount %d (0x%08x)\n"), AfxGetApp()->m_pszAppName, Header.ByteCount, Header.ByteCount);
     m_recvBuff.SetSize(Header.ByteCount);
     m_bytesRead = 0;                       
     m_State = DATA;                        
    } // CConnectSoc::SetDataState   





void CAgvAsyncClientSocket::ToStringAgv(char * str)
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

CAgvAsyncClientSocket::~CAgvAsyncClientSocket()
{

}

#ifdef _DEBUG
void CAgvAsyncClientSocket::AssertValid() const
{
	CAsyncSocket::AssertValid();
}

void CAgvAsyncClientSocket::Dump(CDumpContext& dc) const
{
	CAsyncSocket::Dump(dc);
}
#endif //_DEBUG

IMPLEMENT_DYNAMIC(CAgvAsyncClientSocket, CSocket)


void CAgvAsyncClientSocket::OnClose(int nErrorCode)
{
	MSGWARNING1(_T("CAgvAsyncClientSocket::OnClose() Agv: %d"), m_pAgv->m_id);
	    
    
    if(nErrorCode != ERROR_SUCCESS)
	{
		MSGERROR2 (_T("CAgvAsyncClientSocket::OnClose() Agv: %d ErrorCode: %d"), 
			m_pAgv->m_id,
			nErrorCode);
	} 
	CAsyncSocket::OnClose(nErrorCode);
	m_pAgv->SetSocketClosed();

    // passive close        
    DoClose();

}

int CAgvAsyncClientSocket::Send(LPCVOID Buf, int nBufLen, int nFlags) 
   {
    TRACE(_T("%s: CAgvAsyncClientSocket::Send(%p, %d, %x)\n"), AfxGetApp()->m_pszAppName, Buf, nBufLen, nFlags);
    return CAsyncSocket::Send(Buf, nBufLen, nFlags);
   }


void CAgvAsyncClientSocket::SendMsg(WlanMsg * pMsg)
   {
    TRACE(_T("%s: CConnectSoc::DoAsyncSendBuff() \n"), AfxGetApp()->m_pszAppName);

    //if(bytesSent == m_SendBuff.GetSize())
    //   { /* all sent */
    //    DoSendComplete();
    //    TRACE(_T("%s: CConnectSoc::DoAsyncSendBuff(): nothing to send\n"), AfxGetApp()->m_pszAppName);
    //    return;
    //   } /* all sent */

    //ASSERT(bytesSent >= 0);

    // LPBYTE p = m_SendBuff.GetData() + bytesSent;
	INT_PTR len = pMsg->m_strText.GetLength();
	LPBYTE p = (LPBYTE)pMsg->m_strText.GetBuffer(len); // TODO - no way!!



	// TODO see where sendbuf is coming from

	//TRACE(_T("%s: CAgvAsyncClientSocket::Send(%p + %d (=%p),  %d - %d (=%d), 0)\n"),
 //                  AfxGetApp()->m_pszAppName,
 //                  m_SendBuff.GetData(), bytesSent, p,
 //                  (int)m_SendBuff.GetSize(),
 //                  (int)bytesSent,
 //                  len);



    int result = Send(p, (int)len, 0);
    if(result == SOCKET_ERROR)
       { /* failed */
        DWORD err = ::GetLastError();
        if(err != WSAEWOULDBLOCK)
           { /* couldn't send */
           MSGERROR2 ("Recibido ErrorCode: %d en AsyncSend() Agv: %d. Cerrando conexión", 
			err, m_pAgv);
		   DoClose();
            return; // done for now, wait for next call
           } /* couldn't send */
       } /* failed */
    else
        { /* succeeded */
         TRACE(_T("%s : Send()=>%d\n"), AfxGetApp()->m_pszAppName, result);
         //bytesSent += result;
         //if(bytesSent == m_SendBuff.GetSize())
         //   DoSendComplete();
        } /* succeeded */
   } // CAgvAsyncClientSocket::DoAsyncSendBuff

void CAgvAsyncClientSocket::DoClose()
{
	MSGWARNING1 (_T("CAgvAsyncClientSocket::DoClose Agv: %d"), m_pAgv->m_id);

	if(m_hSocket == INVALID_SOCKET)
	{ 
		MSGWARNING1 (_T("CAgvAsyncClientSocket::DoClose Agv: %d - INVALID_SOCKET"), m_pAgv->m_id);
		return;
	} 

	m_bAborted = true;

	ShutDown();
	Close();


}



// Michael: Note these are not in the Class
CString ConvertReceivedDataToString(CByteArray & data)
   {
     // data is UTF-8 encoded
    CArray<WCHAR, WCHAR> wc;
     // First, compute the amount of space required.  n will include the
     // space for the terminal NUL character
    INT_PTR n = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)data.GetData(), (int)data.GetSize(), NULL, 0);
    if(n == 0)
       { /* failed */
        DWORD err = ::GetLastError();
        TRACE(_T("%s: MultiByteToWideChar (1) returned error %d\n"), AfxGetApp()->m_pszAppName, err);
        return CString(_T(""));
       } /* failed */
    else
       { /* success */
        wc.SetSize(n);
        n = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)data.GetData(), (int)data.GetSize(), (LPWSTR)wc.GetData(), (int)n);
        if(n == 0)
           { /* failed */
            DWORD err = ::GetLastError();
            TRACE(_T("%s: MultiByteToWideChar (2) returned error %d\n"), AfxGetApp()->m_pszAppName, err);
            return CString(_T(""));
           } /* failed */
       } /* success */     

     // Data is now in Unicode
     // If we are a Unicode app we are done
     // If we are an ANSI app, convert it back to ANSI

#ifdef _UNICODE
     // If this is a Unicode app we are done
    return CString(wc.GetData(), (int)wc.GetSize());
#else // ANSI
    // Invert back to ANSI
    CString s;
    n = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wc.GetData(), (int)wc.GetSize(), NULL, 0, NULL, NULL);
    if(n == 0)
       { /* failed */
        DWORD err = ::GetLastError();
        TRACE(_T("%s: WideCharToMultiByte (1) returned error %d\n"), AfxGetApp()->m_pszAppName, err);
        return CString("");
       } /* failed */
    else
       { /* success */
        LPSTR p = s.GetBuffer((int)n);
        n = ::WideCharToMultiByte(CP_ACP, 0, wc.GetData(), (int)wc.GetSize(), p, (int)n, NULL, NULL);
        if(n == 0)
           { /* conversion failed */
            DWORD err = ::GetLastError();
            TRACE(_T("%s: WideCharToMultiByte (2) returned error %d\n"), AfxGetApp()->m_pszAppName, err);
            s.ReleaseBuffer();
            return CString("");
           } /* conversion failed */
        s.ReleaseBuffer();
        return s;
       } /* success */
#endif
   } // ConvertReceivedDataToString

/****************************************************************************
*                           ConvertStringToSendData
* Inputs:
*       const CString & s: String to send
*       CByteArray & msg: Place to format message
* Result: BOOL
*       TRUE if successful
*       FALSE if error
* Effect: 
*       Converts the data to a byte stream for transmission
****************************************************************************/

BOOL ConvertStringToSendData(const CString & s, CByteArray & msg)
    {
#ifdef _UNICODE
     int n = ::WideCharToMultiByte(CP_UTF8, 0, s, -1, NULL, 0, NULL, NULL);
     if(n == 0)
        { /* failed */
         //DWORD err = ::GetLastError();
         msg.SetSize(0);
         return FALSE;
        } /* failed */
     else
        { /* success */
         msg.SetSize(n);
         n = ::WideCharToMultiByte(CP_UTF8, 0, s, -1, (LPSTR)msg.GetData(), n, NULL, NULL);
         if(n == 0)
            { /* conversion failed */
             DWORD err = ::GetLastError();
             msg.SetSize(0);
             return FALSE;
            } /* conversion failed */
         else
            { /* use multibyte string */
             msg.SetSize(n - 1);
             return TRUE;
            } /* use multibyte string */
        } /* success */
#else // ANSI
     CArray<WCHAR, WCHAR> wc;

     int n = ::MultiByteToWideChar(CP_ACP, 0, s, -1, NULL, 0);
     if(n == 0)
        { /* failed */
         DWORD err = ::GetLastError();
         msg.SetSize(0);
         return FALSE;
        } /* failed */
     else
        { /* success */
         wc.SetSize(n);
         n = ::MultiByteToWideChar(CP_ACP, 0, s, -1, wc.GetData(), n);
        } /* success */     

     n = ::WideCharToMultiByte(CP_UTF8, 0, wc.GetData(), -1, NULL, 0, NULL, NULL);
     if(n == 0)
        { /* failed */
         DWORD err = ::GetLastError();
         msg.SetSize(0);
         return FALSE;
        } /* failed */
     else
        { /* success */
         msg.SetSize(n);
         n = ::WideCharToMultiByte(CP_UTF8, 0, wc.GetData(), -1, (LPSTR)msg.GetData(), n, NULL, NULL);
         if(n == 0)
            { /* conversion failed */
             DWORD err = ::GetLastError();
             msg.SetSize(0);
             return FALSE;
            } /* conversion failed */
         else
            { /* use multibyte string */
             msg.SetSize(n - 1);
             return TRUE;
            } /* use multibyte string */
        } /* success */
#endif
    } // ConvertStringToSendData