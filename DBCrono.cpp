
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
           char  filnam[27];
};
static const struct sqlcxp sqlfpn =
{
    26,
    "f:\\projects\\tpc\\dbcrono.pc"
};


static unsigned int sqlctx = 465738451;


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

 static const char *sq0001 = 
"select crono ,'T' ,codtren  from tren where CRONO is  not null  union select\
 crono ,'R' ,codope  from OPERAD where CRONO is  not null            ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4130,178,0,0,
5,0,0,1,145,0,9,91,0,0,0,0,0,1,0,
20,0,0,1,0,0,13,95,0,0,3,0,0,1,0,2,3,0,0,2,1,0,0,2,5,0,0,
47,0,0,1,0,0,15,104,0,0,0,0,0,1,0,
62,0,0,2,90,0,6,148,0,0,4,4,0,1,0,2,3,0,0,1,9,0,0,1,9,0,0,3,3,0,0,
};


// DBCrono.cpp: implementation of the CDBCrono class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "DBCrono.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The ORACA=YES option must be specified to enable use of the ORACA
/* EXEC ORACLE OPTION (ORACA=YES); */ 


// Variables Oracle
typedef char asciz[80];
/* EXEC SQL TYPE asciz IS STRING(80) REFERENCE; */ 


/* EXEC SQL BEGIN DECLARE SECTION; */ 


struct dbCliente
{
	int crono;
	char tipo;
	char codigo [10];
};

struct idbCliente
{
	short crono;
	short tipo;
	short codigo;
};

struct dbCliente * ptrDBCliente;
struct idbCliente * ptriDBCliente;

struct dbCliente * listaClientes [MAX_CLIENTES_TPC];



/* EXEC SQL END DECLARE SECTION; */ 





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBCrono::CDBCrono(void * ptrContext)
{
	m_NumClientes = 0;
	m_ptrContext = ptrContext;
	oraca.orastxtf = ORASTFERR;
}


CDBCrono::~CDBCrono()
{

}



void CDBCrono::InitClientes(void * ptrCtx)
{

    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("CDBCrono::InitClientes():", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO InitClientesNoHayMas; */ 


	sql_context * ptrContext = (sql_context *)ptrCtx;
	/* EXEC SQL CONTEXT USE :*ptrContext; */ 



	/* EXEC SQL DECLARE curClientes CURSOR FOR
		SELECT crono, 'T', codtren
		FROM tren
		WHERE CRONO IS NOT NULL
		UNION
		SELECT crono, 'R', codope
		FROM OPERAD
		WHERE CRONO IS NOT NULL; */ 


	ptriDBCliente = new struct idbCliente;

	/* EXEC SQL OPEN curClientes; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrContext)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 0;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0001;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlcxt(&(*(ptrContext)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("CDBCrono::InitClientes():",sqlca,oraca);
}

	int i;
	for (i = 0; i < MAX_CLIENTES_TPC; )
	{
		ptrDBCliente = new struct dbCliente;
		/* EXEC SQL FETCH curClientes INTO :ptrDBCliente:ptriDBCliente; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrContext)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 3;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )20;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (           int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (         void  *)&ptrDBCliente->crono;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)&ptriDBCliente->crono;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&ptrDBCliente->tipo;
  sqlstm.sqhstl[1] = (unsigned int  )1;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)&ptriDBCliente->tipo;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)ptrDBCliente->codigo;
  sqlstm.sqhstl[2] = (unsigned int  )10;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)&ptriDBCliente->codigo;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&(*(ptrContext)), &sqlctx, &sqlstm, &sqlfpn);
  if (sqlca.sqlcode == 1403) goto InitClientesNoHayMas;
  if (sqlca.sqlcode < 0) plsql_error("CDBCrono::InitClientes():",sqlca,oraca);
}


		listaClientes [i++] = ptrDBCliente;
		

	}


InitClientesNoHayMas:

	/* EXEC SQL CLOSE curClientes; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrContext)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 3;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )47;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrContext)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("CDBCrono::InitClientes():",sqlca,oraca);
}



	m_NumClientes = i;
	return;
}


void CDBCrono::UpdateClientes(CString * ptrStrRadio, CString * ptrStrTren)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 
 	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	/* VARCHAR dbCodigo [30]; */ 
struct { unsigned short len; unsigned char arr[30]; } dbCodigo;

	/* VARCHAR dbTipo [2]; */ 
struct { unsigned short len; unsigned char arr[2]; } dbTipo;

	int dbCrono;
	short idbCrono;
	int dbRetVal;
	
	/* EXEC SQL END DECLARE SECTION; */ 


	// Si no ha sido inicializado, now is the time
	if (m_NumClientes == 0)
		InitClientes (ptrCtx);

	CString strTemp;

	ptrStrTren->Empty ();
	ptrStrRadio->Empty ();

    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("CDBCrono::UpdateClientes ():", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO UpdateClientesNotFound; */ 


	for (int i = 0; i < m_NumClientes; i++)
	{
		strcpy ((char *)dbCodigo.arr, listaClientes [i]->codigo);
		dbCodigo.len = strlen ((char *)dbCodigo.arr);
		dbTipo.arr [0] = listaClientes [i]->tipo;
		dbTipo.len = 1;
		dbCrono = listaClientes [i]->crono;
		idbCrono = 0;	// Para nulls


		/* EXEC SQL EXECUTE
		begin
			:dbRetVal := map.EstaConectado (:dbCodigo, :dbTipo, :dbCrono:idbCrono);
		end;
		END-EXEC; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 4;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "begin :dbRetVal := map . EstaConectado ( :dbCodigo , :dbTip\
o , :dbCrono:idbCrono ) ; end ;";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )62;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&dbRetVal;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&dbCodigo;
  sqlstm.sqhstl[1] = (unsigned int  )32;
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&dbTipo;
  sqlstm.sqhstl[2] = (unsigned int  )4;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)&dbCrono;
  sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)&idbCrono;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqphsv = sqlstm.sqhstv;
  sqlstm.sqphsl = sqlstm.sqhstl;
  sqlstm.sqphss = sqlstm.sqhsts;
  sqlstm.sqpind = sqlstm.sqindv;
  sqlstm.sqpins = sqlstm.sqinds;
  sqlstm.sqparm = sqlstm.sqharm;
  sqlstm.sqparc = sqlstm.sqharc;
  sqlstm.sqpadto = sqlstm.sqadto;
  sqlstm.sqptdso = sqlstm.sqtdso;
  sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
  if (sqlca.sqlcode == 1403) goto UpdateClientesNotFound;
  if (sqlca.sqlcode < 0) plsql_error("CDBCrono::UpdateClientes ():",sqlca,oraca);
}





		strTemp.Format ("%s:%c ", dbCodigo.arr, dbRetVal == 1 ? 'S':'N');

		if (idbCrono != -1)
			listaClientes [i]->crono = dbCrono;
		else
			listaClientes [i]->crono = 0;

		if (dbTipo.arr [0] == 'T')
			*ptrStrTren = *ptrStrTren + strTemp;
		else
			*ptrStrRadio = *ptrStrRadio + strTemp;
	}

	return;

UpdateClientesNotFound:

	MSGERROR ("CDBCrono::UpdateClientes not found");

}
