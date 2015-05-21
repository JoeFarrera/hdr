// SerialCom.h: interface for the CSerialCom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALCOM_H__D304FB51_1EF5_11D5_9926_0010B547EBE6__INCLUDED_)
#define AFX_SERIALCOM_H__D304FB51_1EF5_11D5_9926_0010B547EBE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Flow control flags

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

// ascii definitions

#define ASCII_XON       0x11
#define ASCII_XOFF      0x13
#define LF				0x0a




// Tamaño maximo del bloque a leer
#define MAXBLOCK        200

class CSerialCom
{
public:
	void ResetReceiveBuffer();
	DWORD ReceiveTelegrama (CString& telegrama);
	bool SendTelegrama (LPCTSTR telegrama);
	CSerialCom();
	virtual ~CSerialCom();
	bool WriteCommBlock (LPCTSTR lpByte , DWORD dwBytesToWrite);

private:
	UINT m_uErrorCount;	// Contador de errores para resetear la comunicación
	CString m_strTeleEnviar;
	CString m_strTeleRecibir;
	bool CloseConnection();
	OVERLAPPED m_osWrite;
	OVERLAPPED m_osRead;
	int ReadCommBlock(LPSTR lpszBlock, int nMaxLength);
	bool m_fConnected;
	bool SetupConnection();
	bool OpenConnection();
	HANDLE m_hComDev;
	BYTE m_bPort;
	DWORD m_dwBaudRate;
	BYTE m_bParity;
	BYTE m_bStopBits;
	BYTE m_bByteSize;


};

#endif // !defined(AFX_SERIALCOM_H__D304FB51_1EF5_11D5_9926_0010B547EBE6__INCLUDED_)
