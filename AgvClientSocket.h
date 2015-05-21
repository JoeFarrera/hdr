// AgvClientSocket.h : 

#ifndef __AGVCLNTSOCK_H__
#define __AGVCLNTSOCK_H__

class CMsg;
class CTpcDoc;
class ISocketSink;
class IMsg;
class WlanMsg;
class CAgv;

class CAgvClientSocket : public CSocket
{
	DECLARE_DYNAMIC(CAgvClientSocket);
private:
	CAgvClientSocket(const CAgvClientSocket& rSrc);         // no implementation
	void operator=(const CAgvClientSocket& rSrc);  // no implementation


// Construction
public:
	CAgvClientSocket(CAgv * pAgv);
	CAgvClientSocket();

// Attributes
private:
	int m_nMsgCount;
	CSocketFile* m_pFile;
	CArchive* m_pArchiveIn;
	CArchive* m_pArchiveOut;
	ISocketSink * m_pSocketSink;
	bool m_bAborted;
	WlanMsg * m_pMsg;
	CAgv * m_pAgv;



// Operations
public:
	void Init();
	void SendMsg(WlanMsg * pMsg);
	bool IsAborted () { return m_bAborted; };
	void DoClose();
	int GetIncomingMessageCount() { return m_nMsgCount; };
	void HasDataIncoming();
	WlanMsg * GetPtrWlanMsg () { return m_pMsg; };



private:
	void ToStringAgv(char * str);
	void ProcessPendingRead();
	void ReceiveMsg();
	void ReadMsg();
	void IncrementIncomingMessageCount();

// Overridable callbacks
protected:
	virtual void OnReceive(int nErrorCode);

// Implementation
public:
	virtual ~CAgvClientSocket();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void OnClose(int nErrorCode);
};

#endif // __AGVCLNTSOCK_H__
