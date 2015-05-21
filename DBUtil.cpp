
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned int magic;
  };
  typedef struct sql_cursor sql_cursor;
  typedef struct sql_cursor SQL_CURSOR;
#endif /* SQL_CRSR */

/* Thread Safety */
typedef void * sql_context;
typedef void * SQL_CONTEXT;

/* Object support */
struct sqltvn
{
  unsigned char *tvnvsn; 
  unsigned short tvnvsnl; 
  unsigned char *tvnnm;
  unsigned short tvnnml; 
  unsigned char *tvnsnm;
  unsigned short tvnsnml;
};
typedef struct sqltvn sqltvn;

struct sqladts
{
  unsigned int adtvsn; 
  unsigned short adtmode; 
  unsigned short adtnum;  
  sqltvn adttvn[1];       
};
typedef struct sqladts sqladts;

static struct sqladts sqladt = {
  1,1,0,
};

/* Binding to PL/SQL Records */
struct sqltdss
{
  unsigned int tdsvsn; 
  unsigned short tdsnum; 
  unsigned char *tdsval[1]; 
};
typedef struct sqltdss sqltdss;
static struct sqltdss sqltds =
{
  1,
  0,
};

/* File name & Package Name */
struct sqlcxp
{
  unsigned short fillen;
           char  filnam[40];
};
static const struct sqlcxp sqlfpn =
{
    39,
    "c:\\netprojects\\tpc_new_vs2008\\dbutil.pc"
};


static unsigned int sqlctx = 187540811;


static struct sqlexd {
   unsigned int   sqlvsn;
   unsigned int   arrsiz;
   unsigned int   iters;
   unsigned int   offset;
   unsigned short selerr;
   unsigned short sqlety;
   unsigned int   occurs;
      const short *cud;
   unsigned char  *sqlest;
      const char  *stmt;
   sqladts *sqladtp;
   sqltdss *sqltdsp;
            void  **sqphsv;
   unsigned int   *sqphsl;
            int   *sqphss;
            void  **sqpind;
            int   *sqpins;
   unsigned int   *sqparm;
   unsigned int   **sqparc;
   unsigned short  *sqpadto;
   unsigned short  *sqptdso;
   unsigned int   sqlcmax;
   unsigned int   sqlcmin;
   unsigned int   sqlcincr;
   unsigned int   sqlctimeout;
   unsigned int   sqlcnowait;
              int   sqfoff;
   unsigned int   sqcmod;
   unsigned int   sqfmod;
            void  *sqhstv[4];
   unsigned int   sqhstl[4];
            int   sqhsts[4];
            void  *sqindv[4];
            int   sqinds[4];
   unsigned int   sqharm[4];
   unsigned int   *sqharc[4];
   unsigned short  sqadto[4];
   unsigned short  sqtdso[4];
} sqlstm = {12,4};

// Prototypes
extern "C" {
  void sqlcxt (void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlcx2t(void **, unsigned int *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlbuft(void **, char *);
  void sqlgs2t(void **, char *);
  void sqlorat(void **, unsigned int *, void *);
}

// Forms Interface
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern "C" { void sqliem(char *, int *); }

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4130,178,0,0,
5,0,0,1,0,0,32,80,0,0,0,0,0,1,0,
20,0,0,0,0,0,60,102,0,0,0,0,0,1,0,
35,0,0,0,0,0,58,136,0,0,1,1,0,1,0,3,109,0,0,
54,0,0,2,0,0,27,178,0,0,4,4,0,1,0,1,5,0,0,1,10,0,0,1,10,0,0,1,10,0,0,
85,0,0,3,0,0,32,234,0,0,0,0,0,1,0,
};


// DBUtil.cpp: implementation of the CDBUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"


#include "DBUtil.h"

#include "TpcDoc.h"
#include "TpcView.h"
#include "Container.h"
#include "msgmacro.h"
#include "Alarma.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// The ORACA=YES option must be specified to enable use of the ORACA
/* EXEC ORACLE OPTION (ORACA=YES); */ 




/* EXEC SQL BEGIN DECLARE SECTION; */ 


sql_context ctxPLCThread;
sql_context ctxGestorAGVThread;
sql_context ctxTPCDocThread;
sql_context ctxBackgroundTasks;
sql_context ctxAgv2Thread;
sql_context ctxAgv10Thread;
sql_context ctxAgv11Thread;

sql_context ctxs [20];

/* EXEC SQL END DECLARE SECTION; */ 



void plsql_error(char *msg, T_sqlca sqlca, T_oraca oraca)
{
	MSGFATAL (msg);
    sqlca.sqlerrm.sqlerrmc[sqlca.sqlerrm.sqlerrml] = '\0';
    oraca.orastxt.orastxtc[oraca.orastxt.orastxtl] = '\0';
    oraca.orasfnm.orasfnmc[oraca.orasfnm.orasfnml] = '\0';


	

    MSGFATAL (sqlca.sqlerrm.sqlerrmc);
	MSGFATAL1 ("in %s", oraca.orastxt.orastxtc);
	MSGFATAL2 ("on line %d of %s",
			oraca.oraslnr,
			oraca.orasfnm.orasfnmc);

	CString strTemp;
	strTemp.Format ("%s %s.\nLa aplicación no puede continuar y parará.",
				msg,
				sqlca.sqlerrm.sqlerrmc);

	CAlarma * ptrAlarma = new CAlarma;
	ptrAlarma->AvisarAlarma (6, 1, 0, 0,0,0,0,0,0,strTemp);


	// Dejará que el mensaje de alarma salga ??
	AfxMessageBox (strTemp, MB_ICONSTOP);
			
/* Disable ORACLE error checking to avoid an infinite loop
 * should another error occur within this routine.
 */
    /* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 




  // Roll back any pending changes and disconnect from Oracle.
    /* EXEC SQL ROLLBACK RELEASE; */ 

{
    struct sqlexd sqlstm;
    sqlorat((void **)0, &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 0;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )5;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}



	
    // errorExit ();
	exit (0);
	return;
}




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CDBUtil::CDBUtil()
{
	m_fConectado = false;
	m_nThread = 0;
	oraca.orastxtf = ORASTFERR;
	/* EXEC SQL ENABLE THREADS; */ 

{
 struct sqlexd sqlstm;
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 0;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )20;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


	


}

CDBUtil::~CDBUtil()
{

}




// conectar a Oracle según usuario/clave@db definido en variable global :username
void * CDBUtil::ConectarOracle(LPCTSTR sThread)
{



	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	char    *username;

	/* EXEC SQL END DECLARE SECTION; */ 


	CString strThread = sThread;

	m_ptrContext = GetOracleContext(strThread);
	if (m_ptrContext == NULL)
	{
		int i = m_ctx_array.GetCount();
		i++;
		// EXEC SQL CONTEXT ALLOCATE :ctxAgv2Thread;
		/* EXEC SQL CONTEXT ALLOCATE :ctxs[i]; */ 

{
  struct sqlexd sqlstm;
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 1;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )35;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&ctxs[i];
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(void *);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt((void **)0, &sqlctx, &sqlstm, &sqlfpn);
}


		// m_ptrContext = &ctxAgv2Thread;
		m_ptrContext = &ctxs [i];
		CTX_ID id = { strThread, &ctxs [i] };
		m_ctx_array.Add (id);
	}
	MSGAVISO2 ("Connect Thread %s id: %d", strThread, AfxGetThread()->m_nThreadID);


	sql_context * ptrContext = (sql_context *) m_ptrContext;

	SetOraContext (m_ptrContext);

	/* EXEC SQL CONTEXT USE :*ptrContext; */ 

	MSGAVISO2 ("Using context: %d en thread %d", ptrContext, 
		AfxGetThread()->m_nThreadID);

  // Call sql_error() function on any error in an embedded SQL statement
    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("ConectarOracle: ", sqlca, oraca); */ 



  // Save text of SQL current statement in the ORACA if an error occurs.
    oraca.orastxtf = ORASTFERR;

  // Connect to Oracle.

   CString strUserConnect = AfxGetApp ()->GetProfileString ("Settings", "Usuario");

   if (strUserConnect.IsEmpty ())
		{
			CString strTemp;
			strTemp.LoadString (IDS_NOORACLEUSER);
			AfxMessageBox (strTemp);
			username = "hdr/hdr";
			AfxGetApp ()->WriteProfileString ("Settings", "Usuario", "hdr/hdr");
			
		}
		else 
			username = strUserConnect.GetBuffer (100);


#if ! defined (FOREXPORTTOINDUMAT)
    /* EXEC SQL CONNECT :username; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&(*(ptrContext)), &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 4;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.iters = (unsigned int  )10;
    sqlstm.offset = (unsigned int  )54;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)username;
    sqlstm.sqhstl[0] = (unsigned int  )0;
    sqlstm.sqhsts[0] = (         int  )0;
    sqlstm.sqindv[0] = (         void  *)0;
    sqlstm.sqinds[0] = (         int  )0;
    sqlstm.sqharm[0] = (unsigned int  )0;
    sqlstm.sqadto[0] = (unsigned short )0;
    sqlstm.sqtdso[0] = (unsigned short )0;
    sqlstm.sqphsv = sqlstm.sqhstv;
    sqlstm.sqphsl = sqlstm.sqhstl;
    sqlstm.sqphss = sqlstm.sqhsts;
    sqlstm.sqpind = sqlstm.sqindv;
    sqlstm.sqpins = sqlstm.sqinds;
    sqlstm.sqparm = sqlstm.sqharm;
    sqlstm.sqparc = sqlstm.sqharc;
    sqlstm.sqpadto = sqlstm.sqadto;
    sqlstm.sqptdso = sqlstm.sqtdso;
    sqlstm.sqlcmax = (unsigned int )100;
    sqlstm.sqlcmin = (unsigned int )2;
    sqlstm.sqlcincr = (unsigned int )1;
    sqlstm.sqlctimeout = (unsigned int )0;
    sqlstm.sqlcnowait = (unsigned int )0;
    sqlcxt(&(*(ptrContext)), &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) plsql_error("ConectarOracle: ",sqlca,oraca);
}

 // IDENTIFIED BY :password;
    MSGAVISO1 ("ConectarOracle(): Connected to Oracle as %s",
				 username);
	/* EXEC SQL CONTEXT USE DEFAULT; */ 


	m_fConectado = true;
#else

	m_fConectado = false;
#endif

	return (m_ptrContext);

}

void * CDBUtil::GetOracleContext (LPCTSTR sThread)
{
	for (int i = 0; i < m_ctx_array.GetCount(); i++)
	{
		CTX_ID id = m_ctx_array.GetAt(i);
		if (id.strThread == sThread)
		{
			return id.ptrCtx;
		}
	}
	// Got here - not found
	return NULL;
	



}



// Desconectar de la base de datos
void CDBUtil::DisconnectOracle()
{	

	// Michael 27.04.2009 sql_context * ptrContext = (sql_context *) m_ptrContext;
	sql_context * ptrContext = (sql_context *)GetAndDeleteOraContext();


	// Michael 23.04.2009
	if (!m_ptrContext)
	{
		MSGERROR ("m_ptrContext es null!");
		return;
	}




	/* EXEC SQL CONTEXT USE :*ptrContext; */ 

	
    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("DisconnectOracle: ", sqlca, oraca); */ 

	/* EXEC SQL ROLLBACK WORK RELEASE; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrContext)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 4;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )85;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrContext)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("DisconnectOracle: ",sqlca,oraca);
}



	MSGAVISO ("Desconectado de Oracle");
}

// Guardar sql_context para el thread
void CDBUtil::SetOraContext(void *ptrCtx)
{
	int i = GetThreadIndex();
	m_CtxArray [i].nThreadID = AfxGetThread()->m_nThreadID;
	m_CtxArray [i].ptrCtx = ptrCtx;
}

// Buscar sql_context para el thread
void * CDBUtil::GetOraContext()
{
	DWORD dwThreadID = AfxGetThread()->m_nThreadID;
	for (UINT i = 0; i < m_nThread; i++)
		if (m_CtxArray [i].nThreadID == dwThreadID)
			return (m_CtxArray [i].ptrCtx);

	// Si llega aquí - no va	
		
	ASSERT (FALSE);

	return (NULL);	

}



int CDBUtil::GetThreadIndex()
{
	if (m_nThread < MAX_PROCESSES)
	{
		return (m_nThread++);
	}
	else
	{
		for (int i = 0; i < MAX_PROCESSES; i++)
		{
			if (m_CtxArray [i].nThreadID == 0)
				return i;
		}
	}

	ASSERT (FALSE);

	return (0);	

}


void CDBUtil::InitThreadArray()
{
	for (int i = 0; i < MAX_PROCESSES; i++)
	{
		m_CtxArray [i].nThreadID = 0;
	}

}

void * CDBUtil::GetAndDeleteOraContext()
{
	DWORD dwThreadID = AfxGetThread()->m_nThreadID;
	for (UINT i = 0; i < m_nThread; i++)
		if (m_CtxArray [i].nThreadID == dwThreadID)
		{
			m_CtxArray [i].nThreadID = 0;
			return (m_CtxArray [i].ptrCtx);
		}

	// Si llega aquí - no va	
		
	ASSERT (FALSE);

	return (NULL);	



}
