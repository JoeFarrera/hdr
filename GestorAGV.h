// GestorAGV.h: interface for the CGestorAGV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GESTORAGV_H__EBD7D77B_227E_11D5_992A_0010B547EBE6__INCLUDED_)
#define AFX_GESTORAGV_H__EBD7D77B_227E_11D5_992A_0010B547EBE6__INCLUDED_

#include "SerialCom.h"	// Added by ClassView
#include "Agv.h"	// Added by ClassView
//#include "a1_konst.c86"	// Added by Michael
#include "ISocketSink.h"
#include "ISocketServer.h"
#include "ClientSocket.h"
#include "AgvSockThread.h"
#include "AgvSockType.h"



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTpcView;
class CTCPServerSocket;
class CClientSocket;
class WlanMsg;





class CGestorAGV : public ISocketServer, ISocketSink
{
public:
	void StartSocketServer(void);
	void ProcessPendingAccept (CSocket * pServerSocket);
	void InitAGVs(CDistanceReqWayTeleMap* pmapDistancesReqWayTele);
	void AnularCargarBat();
	UINT GetAgvsEnCircuitoIdle (UINT nAgvQuery = 0);
	CStdioFile * GetWriteHistoryFile(enum T_Hist fileExt);
	UINT GetAgvsEnCircuito();
	void GetAgvConOrden(UINT nAgv);
	bool StealOrden (CAgv * ptrAgv);
	UINT IsOccupiedKnot (UINT nAgv, UINT knot);
	HANDLE m_hTimer;	// para temporizador
	HANDLE m_lpHandles [2];	// Para esperar...
	CAgv * GetAgvPtr (UINT nAgv);
	CCriticalSection m_FVAusBITab;	// Protect access to data while blocking function operates	
	void AutomataGestor();
	static void ArrancarComThread(CDistanceReqWayTeleMap* pmapDistancesReqWayTele);
	CEvent m_evtTeleOut;	// Señalar que hay telegramas salientes
	CEvent m_evtOrden;		// Señalar que hay órdenes a transmitir a los Agvs
	CGestorAGV(CDistanceReqWayTeleMap* pmapDistancesReqWayTele);
	virtual ~CGestorAGV();

	// CList<class CAgv, class CAgv&> m_list;	// Lista de AGVs
	//CAgv * m_ptrAgv [KKFZMAX];	// Array de Agvs
	CAgv ** m_pptrAgv;	// Array de Agvs		//JLac better CPtrArray!
	LARGE_INTEGER m_NextPoll;	// Nanosegundos hasta next poll

private:
	bool IsOkPoner55_56(CAgv * ptrAgv);
	void SetSemaforos();
	LPCSTR GetStringHistFileExtension (enum T_Hist fileExt);
	void WriteAgvHistoryFile();
	void ReadSitStopAndGoFiles();
	CAgv * GetAgvEnPunto (UINT ptoCom);
	void SetEnCircuito();
	CSerialCom m_SerCom;	// Comunicaciones Port Serie
	LARGE_INTEGER m_LastPoll;	// Nanosegundos desde last poll
	CDBEstadoPLC m_estadoPLC;	
	CTCPServerSocket * m_pSocket; // Socket del servidor
	// TODO M2015 See if needed here
	CPtrList m_connectionList;	// Lista de sockets cliente
	CString m_strConnected;		// Identificar cliente conectado
	WlanMsg * m_pMsg;	// Message reference pointer
	CAgv * GetAgvFromIP (CString *pIPAddress);

	CAgvSockTypeArray m_agvSockArray;




};

extern CGestorAGV* g_pGestor;

#endif // !defined(AFX_GESTORAGV_H__EBD7D77B_227E_11D5_992A_0010B547EBE6__INCLUDED_)
