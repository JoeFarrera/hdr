// DBCrono.h: interface for the CDBCrono class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCRONO_H__988BD535_EFB9_4257_A251_148FF898BF4B__INCLUDED_)
#define AFX_DBCRONO_H__988BD535_EFB9_4257_A251_148FF898BF4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_CLIENTES_TPC 8

class CDBCrono  
{
public:
	void UpdateClientes (CString * ptrStrRadio, CString * ptrStrTren);
	int m_NumClientes;	// Número de dispositivos controlados
	CDBCrono(void * ptrContext);
	virtual ~CDBCrono();
	T_sqlca sqlca;	// code area de Oracle
	T_oraca oraca;

private:
	void InitClientes(void * ptrCtx);
	void * m_ptrContext;


};

#endif // !defined(AFX_DBCRONO_H__988BD535_EFB9_4257_A251_148FF898BF4B__INCLUDED_)
