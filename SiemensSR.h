// SiemensSR.h: interface for the CSiemensSR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIEMENSSR_H__628D69C6_C73E_46D6_A846_D269CFC7A4AE__INCLUDED_)
#define AFX_SIEMENSSR_H__628D69C6_C73E_46D6_A846_D269CFC7A4AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sr.h"
#include "SR_trace.h"
#include "SR_util.h"

#define MAX_ACCESSP        81
#define MAX_MAC            18
#define MAX_TSAP            9
#define MAX_BUF          4096

#define DO_OPEN             0
#define DO_CONNECT          1
#define DO_LISTEN           2
#define DO_SEND_EOM         3
#define DO_RECEIVE          4
#define DO_CLOSE            5

#define	MAXESTADOSENDPLC	24

class CSiemensSR  
{
public:
	void SendClose();
	BOOL IsConnected () { return (m_fConnected); };
	BOOL IsConnecting () { return (m_fConnecting); };
	void SendEstado (BYTE * ptrDatos);
	void SendTelegrama (BYTE tipoTel, BYTE * ptrDatos, WORD len);
	CSiemensSR();
	virtual ~CSiemensSR();
	void Response (RB1 *rb);
	void Process(WORD opcode);

private:
	bool m_fReceivePend;		// Indicar si hay un receive pendiente
	LPCTSTR GetProcessString (WORD opcode);
	void VerMensajeRecibido();
	LPCTSTR GetResponseString (WORD resp);
	void InitCommunication();
	LPCTSTR GetOpcodeString(WORD opcode);
	UCHAR m_LocalTSAP[MAX_TSAP];  // local TSAP                         
	UCHAR m_RemoteTSAP[MAX_TSAP]; // remote TSAP                        
	HWND m_hWnd;                 // handle of main window              
	BYTE m_RemoteMAC[6];         // MAC Address of SIMATIC S5/S7       
	WORD m_OpenRef;              // reference for an opened connection 
	int  m_AccessPoint;			 // AccessPoint (PG/PC-Interface)      
	RB1  m_ReqBlock;			 // Request Block for General Operations
	RB1  m_ReqBlockSend;         // Request Block for Send Operations
	RB1	 m_ReqBlockRecv;		 // Request block for Receive Operations
	BYTE m_SendBuf[MAX_BUF];     // Send Buffer                        
	BYTE m_RecvBuf[MAX_BUF];     // Receive Buffer                     
	WORD m_DataLen;              // length of exchanged Data           
	int  m_Data;                 // data, that should be exchanged     
	BOOL m_fConnected;            // connection confirmed ?             
	BOOL m_fConnecting;			 // in the process of connecting
	BOOL m_SrSet;                // interface initialized?             
	char m_ErrorMsg[250];        // error message                      

};

#endif // !defined(AFX_SIEMENSSR_H__628D69C6_C73E_46D6_A846_D269CFC7A4AE__INCLUDED_)
