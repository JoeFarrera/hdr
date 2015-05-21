// DBUtil.h: interface for the CDBUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBUTIL_H__88C6C153_982C_4438_8B5A_FECA37B5CDA7__INCLUDED_)
#define AFX_DBUTIL_H__88C6C153_982C_4438_8B5A_FECA37B5CDA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void plsql_error(char *msg, T_sqlca sqlca, T_oraca oraca);
#define	MAX_PROCESSES	30	// TODO: Should combine

typedef struct { CString strThread; void * ptrCtx; } CTX_ID;
typedef CArray<CTX_ID, CTX_ID&> CCtx_Array;



class CDBUtil  
{
public:
	void * GetOraContext();
	T_sqlca sqlca;
	T_oraca oraca;
	void DisconnectOracle();
	bool IsConnected() { return (m_fConectado); };
	void * ConectarOracle(LPCTSTR sThread);
	CDBUtil();
	virtual ~CDBUtil();
	void * GetOracleContext (LPCTSTR sThread);


private:
	void * GetAndDeleteOraContext();
	int GetThreadIndex();
	void InitThreadArray();
	UINT m_nThread;	// Número de threads conectados
	void SetOraContext (void * ptrCtx);
	bool m_fConectado;	// Indicar si la sesión esta conectado a Oracle
	void * m_ptrContext;
	struct {
		DWORD nThreadID;
		void * ptrCtx;
	} m_CtxArray [MAX_PROCESSES];	
	CCtx_Array m_ctx_array;

};



#endif // !defined(AFX_DBUTIL_H__88C6C153_982C_4438_8B5A_FECA37B5CDA7__INCLUDED_)
