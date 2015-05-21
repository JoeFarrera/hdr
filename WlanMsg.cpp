#include "stdafx.h"
#include "tpc.h"
#include "konstant.h"
#include "komdat.h"
#include "zs_bed.h"
#include "AgvClientSocket.h"
#include "WlanMsg.h"
#include "msgMacro.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(WlanMsg, CObject)

WlanMsg::WlanMsg(void)
{
	Init();
}

WlanMsg::~WlanMsg(void)
{
}


/////////////////////////////////////////////////////////////////////////////
// CMsg Operations

void WlanMsg::Init()
{
	m_bClose = FALSE;
	m_strText = _T("");
	m_art = 0;
	m_msgListIn.RemoveAll();
	m_msgListOut.RemoveAll();
	m_ptrCliSock = NULL;
}

void WlanMsg::SetClientSocket(CAgvClientSocket * pSocket) {
	// MSGAVISO2 ("SetClientSocket: old: %p, new: %p", m_ptrCliSock, pSocket);
	m_ptrCliSock = pSocket;
}

/**
* Read in all there is waiting at the port.
* Put it in <m_strTeleRecibir>
*/
int WlanMsg::GetMessageFromPort (CArchive& ar) {
		DWORD dwBytes;
		TCHAR szIn [MAXBLOCKIN];
		// Assuming will always be starting a message


		// See how many bytes are waiting to be read
		BOOL result = m_ptrCliSock->IOCtl(FIONREAD, &dwBytes);
		if (result && dwBytes < MAXBLOCKIN)
		{
			MSGTRACE1 ("Leido del socket nLength:%d", dwBytes);

			memset (szIn, ' ', MAXBLOCKIN);
			ar.Read(szIn, dwBytes);
			szIn [dwBytes] = '\0';
			m_strTeleRecibir += szIn;
			return (int)dwBytes;
		}
		else
		{
			if (dwBytes < MAXBLOCKIN)
			{
				MSGERROR ("WlanMsg::GetMessageFromPort: SocketError");
			}
			else
			{
				MSGERROR1 ("WlanMsg::GetMessageFromPort: SocketError bytes to be read (%d) exceeds buffer size", dwBytes);
			}
			return 0;
		}

}


void WlanMsg::PutMessageOnQueue () 
{
	int bytesLeft = m_strTeleRecibir.GetLength();

	while (bytesLeft > 0)
	{
		LPTSTR p = m_strTeleRecibir.GetBuffer(bytesLeft);
		CString strText;
		int nLectura = 0;
		bool fStart = false;
		int i;
		int startPos = 0;
		int endPos = 0;
		for (i = 0; i < bytesLeft && nLectura == 0; i++)
		  {
			// Read your data from your CArchive object
			if (*p == STX) {
				if (fStart)
				{
					MSGERROR ("STX in telegram from port");
					startPos = i;
				}
				else
				{
					fStart = true;
					startPos = i;
				}

			}
			else if (*p == ETX)
			{
				if (fStart)
				{
					endPos = i;
					break;
				}
				else
				{
					MSGERROR ("ETX in telegram without STX");
				}

			}
			p++;

		}
		m_strTeleRecibir.ReleaseBuffer();
		if (endPos > 0) {
			endPos ++;	// 0 indexed
			strText = m_strTeleRecibir.Mid(startPos, endPos - startPos);
			m_strTeleRecibir.Delete(0, endPos);
			m_msgListIn.AddTail(strText);
			MSGAVISO1("WLAN << %s", strText);
			bytesLeft = bytesLeft - endPos;
		}
		else
		{

			// no se ha acabado el mensaje
			bytesLeft = 0;
		}
	}
}

bool WlanMsg::GetNextMessageIn (CString * ptrStr) {
	bool bReturn = false;
	// MSGTRACE ("WlanMsg::GetNextMessageIn Locking");
	CSingleLock inListLock(&m_CsIn);
	inListLock.Lock();

	if (m_msgListIn.GetCount() > 0)
	{
		*ptrStr = m_msgListIn.RemoveHead();
		bReturn = true;
	}
	inListLock.Unlock();
	// MSGTRACE ("WlanMsg::GetNextMessageIn Unlocked");
	return bReturn;

}

bool WlanMsg::GetNextMessageOut (CString * ptrStr) {
	bool bReturn = false;
	// MSGTRACE ("WlanMsg::GetNextMessageOut Locking");
	CSingleLock outListLock(&m_CsOut);
	outListLock.Lock();

	if (m_msgListOut.GetCount() > 0)
	{
		*ptrStr = m_msgListOut.RemoveHead();
		bReturn = true;
	}
	outListLock.Unlock();
	// MSGTRACE ("WlanMsg::GetNextMessageOut Unlocked");
	return bReturn;
}



void WlanMsg::QueueMessage (CString strMessage){
	MSGTRACE ("WlanMsg::GetNextMessageOut Locking");
	CSingleLock outListLock(&m_CsOut);
	outListLock.Lock();

	m_msgListOut.AddTail(strMessage);

	outListLock.Unlock();
	MSGTRACE ("WlanMsg::GetNextMessageOut Unlocked");

}



/////////////////////////////////////////////////////////////////////////////
// CMsg serialization

void WlanMsg::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{
		TRACE ("Serialize (out) m_ptrCliSock %p\n", m_ptrCliSock);
		int len = m_strText.GetLength();
		LPTSTR p = m_strText.GetBuffer(len);
		ar.Write(p, len);

	}
	else
	{
		TRACE ("Serialize (in) m_ptrCliSock %p\n", m_ptrCliSock);
		int lenRead = GetMessageFromPort(ar);
		if (!lenRead) {
			MSGERROR("Message not read in serialize. TODO");
			return;
		}

		PutMessageOnQueue();
	}
}


void WlanMsg::SendNextMsg () 
{
	
	CString strMsg;

	while (GetNextMessageOut(&m_strText))
	{
		m_ptrCliSock->SendMsg(this);
		MSGAVISO1 ("WLAN >>: %s", m_strText);
		if (m_bClose)
			break;
	}
}

CString WlanMsg::ToStringTelegrama () {
	CString str;
	str.Format("%c : %s", m_art, m_strText);
	return str;
}

#ifdef _DEBUG
void WlanMsg::AssertValid() const
{
	CObject::AssertValid();
}

void WlanMsg::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG

