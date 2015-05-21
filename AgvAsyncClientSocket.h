// AgvAsyncClientSocket.h : 

#ifndef __AGVCLNTSOCK_H__
#define __AGVCLNTSOCK_H__

class CMsg;
class CTpcDoc;
class ISocketSink;
class IMsg;
class WlanMsg;
class CAgv;

class CAgvAsyncClientSocket : public CAsyncSocket
{
	DECLARE_DYNAMIC(CAgvAsyncClientSocket);
private:
	CAgvAsyncClientSocket(const CAgvAsyncClientSocket& rSrc);         // no implementation
	void operator=(const CAgvAsyncClientSocket& rSrc);  // no implementation


// Construction
public:
	CAgvAsyncClientSocket(CAgv * pAgv);
	CAgvAsyncClientSocket();

// Attributes
private:
	int m_nMsgCount;
	ISocketSink * m_pSocketSink;
	bool m_bAborted;
	WlanMsg * m_pMsg;
	CAgv * m_pAgv;
	typedef enum {L0, L1, L2, L3, DATA, ERRORSTATE} ReceiveState;
	ReceiveState m_State;
    union {
       int ByteCount;
       BYTE data[4];
    } Header;
	void SetDataState();
	CByteArray m_recvBuff;
	CByteArray m_SendBuff;

	INT_PTR m_bytesRead;






// Operations
public:
	void Init();
	void SendMsg(WlanMsg * pMsg);
	bool IsAborted () { return m_bAborted; };
	void DoClose();
	int GetIncomingMessageCount() { return m_nMsgCount; };
	void HasDataIncoming();



private:
	void ToStringAgv(char * str);
	void IncrementIncomingMessageCount();
	void ProcessPacket();
	void ProcessReceive(CByteArray & data);
	int	Send(LPCVOID Buf, int nBufLen, int nFlags);

// Overridable callbacks
protected:
	virtual void OnReceiveKnowingLength(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

// Implementation
public:
	virtual ~CAgvAsyncClientSocket();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void OnClose(int nErrorCode);
};

#endif // __AGVCLNTSOCK_H__
