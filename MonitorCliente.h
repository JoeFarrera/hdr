// MonitorCliente.h: interface for the CMonitorCliente class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITORCLIENTE_H__F9A3B17E_4DEA_438D_87B6_E629A3F4B538__INCLUDED_)
#define AFX_MONITORCLIENTE_H__F9A3B17E_4DEA_438D_87B6_E629A3F4B538__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMonitorCliente  
{
public:
	CMonitorCliente();
	virtual ~CMonitorCliente();

private:
typedef struct TCliente
	{
		UINT crono;
		UCHAR tipo;
		CString strCodigo;
	};
int m_numClientes;



};

#endif // !defined(AFX_MONITORCLIENTE_H__F9A3B17E_4DEA_438D_87B6_E629A3F4B538__INCLUDED_)
