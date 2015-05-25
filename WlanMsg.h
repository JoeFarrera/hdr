#pragma once

#ifndef __WLANMSG_H
#define __WLANMSG_H

#include "afx.h"
#include "WLAN_FZ_Tele.h"
#include "IMsg.h"

#define STX	2
#define ETX 3
#define MAXBLOCKIN 1000


class CAgvClientSocket;
class IMsg;


class WlanMsg :
	public CObject, IMsg
{
public:
	WlanMsg(void);
	virtual ~WlanMsg(void);
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	void SetClientSocket(CAgvClientSocket * pSocket);
	void SendNextMsg ();
	bool IsClosed (){ return (m_bClose); };
	void SetClosed() { m_bClose = true; };
	CString ToStringTelegrama ();
	bool IsQuittum () { return (m_art == 'Q'); };
	bool IsUnknown () { return (m_art != 'L' && m_art != 'S' && m_art != 'Q' && m_art != 'E'); };
	UCHAR GetArt () { return (m_art); };
	CString GetString () { return (m_strText);};
	bool	GetNextMessageIn (CString * ptrStr);
	void QueueMessage (CString strMessage);
	bool HasIncoming(){ return (m_msgListIn.GetCount() > 0) ; };

protected:
	void Init();
	DECLARE_DYNCREATE(WlanMsg);

// Attributes
public:
	// M2015 CTele m_Tele;

	CAgvClientSocket * m_ptrCliSock;
	CString m_strText;
	CString m_strTeleRecibir;
	typ_tele_quitt_wlan tele_quitt;
	typ_tele_version_wlan m_tele_version;
	typ_tele_error_wlan m_tele_error;
	typ_tele_gs_wlan m_tele_gs;

private:
	bool m_bClose;

	TYP_WLAN_FZ_TELE tele;
	typ_tele_kopf_wlan m_tele_kopf;
	int GetMessageFromPort (CArchive& ar);
	void PutMessageOnQueue (); 
	CStringList m_msgListIn;
	CStringList m_msgListOut;
	CCriticalSection m_CsOut;
	CCriticalSection m_CsIn;
	bool GetNextMessageOut (CString * ptrStr);
	void RemoveHead();






	unsigned char m_art;



#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
#endif // __WLANMSG_H