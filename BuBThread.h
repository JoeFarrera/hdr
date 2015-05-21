#pragma once

#include "ISocketSink.h"
#include "ISocketServer.h"




// CBuBThread

typedef struct {
		UCHAR type;	// Type of order: 'A' | 'M'
		UCHAR action;	// 'D' Delete
		UINT bubIndex;	// Index en el bub
	} BuB_Order_Index;

typedef CList<BuB_Order_Index, BuB_Order_Index& >CList_BuB_Order_Index;


class CBuBThread : public CWinThread, ISocketServer, ISocketSink
{
	DECLARE_DYNCREATE(CBuBThread)

protected:
	void ProcessPendingAccept(CSocket * pServerSocket);
	CBuBThread();           // protected constructor used by dynamic creation
	virtual ~CBuBThread();


public:
	void OnTimer(WPARAM wParam, LPARAM /*lParam*/);
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void ProcessPendingRead(CClientSocket *pClientSocket);

	void SendUpdateToBuB();
	void SendA0Telegram(CMsg *ptrMsg, TYP_AT_DAT *atPtr, int index = 0);
	void SendAllAlarmas ();
	void SendAllStations (CMsg *ptrMsg, bool sendAll = false);
	void SendA0DeleteTelegram (int index);
	void SendM0DeleteTelegram (int index);



	SOCKET m_hSocket;
	CMsg * m_pMsg;	// Message reference pointer


private:
	void StartSocketServer();
	void CloseSocket(CClientSocket *pSocket);
	void ProcessMessages();
	void SetInsertTime (TYP_DATUM * ptrDatum);
	void SendM0Telegram (CMsg *ptrMsg, TYP_AT_DAT *atPtr, int index);
	void ProcessMATelegram (CMsg *ptrMsg);
	void ProcessFKTelegram (CMsg *ptrMsg);
	void ProcessTITelegram (CMsg *ptrMsg);
	void SendAllAgvs (CMsg * ptrMsg, bool sendAll);
	unsigned short SwapBytes (unsigned char * us);
	void SendClientQuitum (CMsg* ptrMsg,
		unsigned char * kenn,
		unsigned char * qkenn,
		UINT errorCode);
	void SendTITelegram (CMsg *ptrMsg);
	void ProcessSKTelegram (CMsg *ptrMsg);
	void ProcessATTelegram(CMsg *ptrMsg);
	void SendF0Telegram (CMsg *ptrMsg, UINT nAgv, bool sendAll);
	void SendSystemConstants (CMsg *ptrMsg);
	void SendSystemStatus (CMsg * ptrMsg);
	void SendAllOrders (CMsg *ptrMsg, bool sendAll = false);
	void SendDeleteTelegrams ();
	bool GetNextOrderIndex(BuB_Order_Index &ptrIndex);
	void DeleteFirstOrderIndex();


	bool m_fRun;
	CMsg* ReadMsg(CClientSocket* pSocket);
	UINT m_timerId;
	CTCPServerSocket * m_pSocket; // Socket del servidor
	CPtrList m_connectionList;	// Lista de sockets cliente
	CString m_strConnected;		// Identificar cliente conectado
	CPtrList m_msgList;			// Lista de mensajes - es correcto ?
	CCriticalSection m_cs;
	CList_BuB_Order_Index m_ListDeleteOrders;
	int m_iKeepAlive;
	bool m_fConnected;








protected:
	DECLARE_MESSAGE_MAP()
public:
	// Add orders to be communicated as deleted to the BuB
	void AddOrderToDelete(UCHAR tipo, UCHAR accion, UINT index);
	// Send Agv update to BuB
	void OnAgvUpdateBuB(WPARAM wParam, LPARAM lParam);
	bool IsConnected(void);
	
};


