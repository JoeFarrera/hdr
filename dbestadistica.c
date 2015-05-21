
/* Result Sets Interface */
#ifndef SQL_CRSR
#  define SQL_CRSR
  struct sql_cursor
  {
    unsigned int curocn;
    void *ptr1;
    void *ptr2;
    unsigned long magic;
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
           char  filnam[33];
};
static const struct sqlcxp sqlfpn =
{
    32,
    "c:\\projects\\tpc\\dbestadistica.pc"
};


static unsigned long sqlctx = 1889997939;


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
            void  *sqhstv[13];
   unsigned int   sqhstl[13];
            int   sqhsts[13];
            void  *sqindv[13];
            int   sqinds[13];
   unsigned int   sqharm[13];
   unsigned int   *sqharc[13];
   unsigned short  sqadto[13];
   unsigned short  sqtdso[13];
} sqlstm = {10,13};

// Prototypes
extern "C" {
  void sqlcxt (void **, unsigned long *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlcx2t(void **, unsigned long *,
               struct sqlexd *, const struct sqlcxp *);
  void sqlbuft(void **, char *);
  void sqlgs2t(void **, char *);
  void sqlorat(void **, unsigned long *, void *);
}

// Forms Interface
static const int IAPSUCC = 0;
static const int IAPFAIL = 1403;
static const int IAPFTL  = 535;
extern "C" { void sqliem(char *, int *); }

 static const char *sq0012 = 
"select CODCLIND ,CODMES ,TEXTO  from DEFMESA ,DEFTEXT where ((DEFMESA.ZONA=:\
b0 and DEFMESA.IND=:b1) and DEFTEXT.BIT=:b2)           ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{10,4138,0,0,0,
5,0,0,1,221,0,3,87,0,0,10,10,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,
0,0,1,3,0,0,1,3,0,0,1,1,0,0,1,3,0,0,
60,0,0,2,47,0,5,122,0,0,1,1,0,1,0,1,3,0,0,
79,0,0,3,110,0,5,131,0,0,4,4,0,1,0,1,1,0,0,1,3,0,0,1,3,0,0,1,3,0,0,
110,0,0,4,47,0,5,142,0,0,1,1,0,1,0,1,3,0,0,
129,0,0,5,0,0,29,150,0,0,0,0,0,1,0,
144,0,0,6,244,0,3,226,0,0,13,13,0,1,0,1,3,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,
1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,3,0,0,
211,0,0,7,0,0,29,260,0,0,0,0,0,1,0,
226,0,0,8,39,0,6,292,0,0,0,0,0,1,0,
241,0,0,9,105,0,5,340,0,0,4,4,0,1,0,1,1,0,0,1,3,0,0,1,3,0,0,1,3,0,0,
272,0,0,10,108,0,3,352,0,0,4,4,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,1,1,0,0,
303,0,0,11,0,0,29,369,0,0,0,0,0,1,0,
318,0,0,12,131,0,9,422,0,0,3,3,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,
345,0,0,12,0,0,13,424,0,0,3,0,0,1,0,2,5,0,0,2,5,0,0,2,5,0,0,
372,0,0,12,0,0,15,433,0,0,0,0,0,1,0,
387,0,0,13,36,0,6,452,0,0,0,0,0,1,0,
};


// DBEstadistica.cpp: implementation of the CDBEstadistica class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "DBEstadistica.h"
#include "Agv.h"
#include "Orden.h"
#include "macro.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

// The ORACA=YES option must be specified to enable use of the ORACA
/* EXEC ORACLE OPTION (ORACA=YES); */ 




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBEstadistica::CDBEstadistica()
{

}

CDBEstadistica::~CDBEstadistica()
{

}

// Grabar o marcar final de un error de Agv
void CDBEstadistica::LogError(CAgv * ptrAgv, UINT nErr, bool fFin, bool fAbort)
{
	T_sqlca sqlca;	// This is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

	
	 int dbNagv;
	 int dbNummod;
	 int dbNumerr;
	 int dbMp;
	 int dbNmp;
	 int dbWegstr;
	 int dbPosdes;
	 char dbLadodes;
	 int dbPtocomdes;
	 char dbTipfin;
	 int dbCodord;

	/* EXEC SQL END DECLARE SECTION; */ 


	dbNagv = ptrAgv->m_id;
	dbNummod = ptrAgv->m_pFzdat->errofz[nErr].modul_nr;
	dbNumerr = ptrAgv->m_pFzdat->errofz[nErr].fehler_nr;

	
	/* EXEC SQL WHENEVER SQLERROR GOTO LogErrorSqlError; */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	if (ptrAgv->m_pOrden)
		dbCodord = atoi ((char *)ptrAgv->m_pOrden->m_at.id_nr); // Any chance of this failing ?
	else
		dbCodord = 0;

	if (!fFin)
	{
		// Aparece el error
		dbMp = ptrAgv->m_pFzdat->mp;
		dbNmp = ptrAgv->m_pFzdat->nmp;
		dbWegstr = ptrAgv->m_pFzdat->wegstr;
		dbPtocomdes = ptrAgv->m_pFzdat->auftziel.nr;
		dbPosdes = ptrAgv->m_pFzdat->auftziel.pos;
		dbLadodes = ptrAgv->m_pFzdat->auftziel.lage;

		/* EXEC SQL INSERT INTO ERRAGV 
			(ERRID,      
			 NAGV,      
			 NUMMOD,    
			 NUMERR,    
			 MP,        
			 NMP,       
			 WEGSTR,    
			 PTOCOMDES, 
			 POSDES,    
			 LADODES,   
			 FECCRE,    
			 FECFIN,    
			 TIPFIN,    
			 CODORD)
			SELECT
			ERRAGV_SEQ.NEXTVAL,
			:dbNagv,
			:dbNummod,
			:dbNumerr,
			:dbMp,
			:dbNmp,
			:dbWegstr,
			:dbPtocomdes,
			:dbPosdes,
			:dbLadodes,
			sysdate,
			null,
			'C',	-- En curso
			:dbCodord
			FROM DUAL; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 10;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "insert into ERRAGV (ERRID,NAGV,NUMMOD,NUMERR,MP,NMP,WEGSTR,\
PTOCOMDES,POSDES,LADODES,FECCRE,FECFIN,TIPFIN,CODORD)select ERRAGV_SEQ.nextval\
  ,:b0 ,:b1 ,:b2 ,:b3 ,:b4 ,:b5 ,:b6 ,:b7 ,:b8 ,sysdate ,null  ,'C' ,:b9  from\
 DUAL ";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )5;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&dbNagv;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&dbNummod;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&dbNumerr;
  sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)&dbMp;
  sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)&dbNmp;
  sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         void  *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned int  )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (         void  *)&dbWegstr;
  sqlstm.sqhstl[5] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         void  *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned int  )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (         void  *)&dbPtocomdes;
  sqlstm.sqhstl[6] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         void  *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned int  )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (         void  *)&dbPosdes;
  sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         void  *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned int  )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (         void  *)&dbLadodes;
  sqlstm.sqhstl[8] = (unsigned int  )1;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         void  *)0;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned int  )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (         void  *)&dbCodord;
  sqlstm.sqhstl[9] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         void  *)0;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned int  )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
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
  if (sqlca.sqlcode < 0) goto LogErrorSqlError;
}



			if (dbCodord)
			{
				// Marcar la orden en error
				/* EXEC SQL UPDATE ordtran
				SET ERRORD = 'S'
				WHERE CODORD = :dbCodord; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 10;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update ordtran  set ERRORD='S' where CODORD=:b0";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )60;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)&dbCodord;
    sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
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
    sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) goto LogErrorSqlError;
}


			}
	}
	else
	{
		dbTipfin = fAbort ? 'R' : 'N';

		/* EXEC SQL UPDATE ERRAGV
			SET FECFIN = sysdate,
			TIPFIN = :dbTipfin
			WHERE NAGV = :dbNagv
			AND NUMMOD = :dbNummod
			AND NUMERR = :dbNumerr
			AND TIPFIN = 'C'; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 10;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "update ERRAGV  set FECFIN=sysdate,TIPFIN=:b0 where (((NAGV=\
:b1 and NUMMOD=:b2) and NUMERR=:b3) and TIPFIN='C')";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )79;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&dbTipfin;
  sqlstm.sqhstl[0] = (unsigned int  )1;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&dbNagv;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&dbNummod;
  sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)&dbNumerr;
  sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
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
  if (sqlca.sqlcode < 0) goto LogErrorSqlError;
}



			if (dbCodord)
			{
				// Quitar el error de la orden
				/* EXEC SQL UPDATE ordtran
				SET ERRORD = 'N'
				WHERE CODORD = :dbCodord; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
    sqlstm.sqlvsn = 10;
    sqlstm.arrsiz = 10;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "update ordtran  set ERRORD='N' where CODORD=:b0";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )110;
    sqlstm.cud = sqlcud0;
    sqlstm.sqlest = (unsigned char  *)&sqlca;
    sqlstm.sqlety = (unsigned short)256;
    sqlstm.occurs = (unsigned int  )0;
    sqlstm.sqhstv[0] = (         void  *)&dbCodord;
    sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
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
    sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
    if (sqlca.sqlcode < 0) goto LogErrorSqlError;
}


			}


	}

	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 10;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )129;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) goto LogErrorSqlError;
}



	return;

LogErrorSqlError:
	MSGERROR4 ("Imposible %s log de errores para Agv: %d NumMod: %d NumErr: %d",
		fFin ? "actualizar" : "insertar",
		dbNagv,
		dbNummod,
		dbNumerr);

	return;
}

// Inserta un registro en el histórico de AGVs
void CDBEstadistica::ArchivarHistAgv(CAgv * ptrAgv)
{
	TRY
	{
		int iArchivarHistAgv = AfxGetApp ()->GetProfileInt ("Settings", "ArchivarHistAgv", -1);
		if (iArchivarHistAgv == -1)
		{
			AfxGetApp ()->WriteProfileInt ("Settings", "ArchivarHistAgv", 0);
		}
		
		if (iArchivarHistAgv == 1)
		{	
			T_sqlca sqlca;	// This is static
			T_oraca oraca;
			oraca.orastxtf = ORASTFERR;
			sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
			/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

		 
			/* EXEC SQL BEGIN DECLARE SECTION; */ 

			
			int dbNagv;
			int dbEstado;
			char * dbBlockTraffic;
			char * dbBlockBub;
			char * dbEnCircuito;
			int dbMp;
			int dbNmp;
			int dbDestino;
			int dbCodord;
			char * dbMesa1;
			char * dbMesa2;
			char * dbCaminoAlt;
			int dbTiempo;

			/* EXEC SQL END DECLARE SECTION; */ 


			/* EXEC SQL WHENEVER SQLERROR GOTO ArchivarHistAgvSqlError; */ 

			/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


			dbNagv = ptrAgv->m_id;
			dbEstado = ptrAgv->GetEstado();
			dbBlockTraffic = (ptrAgv->IsBlocked() ? "S" : "N");
			dbBlockBub = (ptrAgv->IsDriveForbiddenByBuB() ? "S" : "N");
			dbEnCircuito = (ptrAgv->IsEnCircuito() ? "S": "N");
			dbMp = ptrAgv->m_pFzdat->mp;
			dbNmp = ptrAgv->m_pFzdat->nmp;
			dbDestino = ptrAgv->m_pFzdat->auftziel.nr;
			if (ptrAgv->m_pOrden)
				dbCodord = atoi ((char *)ptrAgv->m_pOrden->m_at.id_nr); // Any chance of this failing ?
			else
				dbCodord = 0;
			dbMesa1 = (MESA1CARGADA (*ptrAgv->m_pFzdat) ? "S" : "N");
			dbMesa2 = (MESA2CARGADA (*ptrAgv->m_pFzdat) ? "S" : "N");
			dbCaminoAlt = (ptrAgv->CaminoAltActivado() ? "S" : "N");
			if (ptrAgv->tiempo != NULL)
			{
				CTimeSpan elapsedTime = CTime::GetCurrentTime() - ptrAgv->tiempo;
				dbTiempo = (int)((long)elapsedTime.GetTotalSeconds());
			}
			else
				dbTiempo = 0;
			/* EXEC SQL INSERT INTO HISTAGV 
				(REGID,      
					AGV,      
					ESTADO,    
					BLOCKTRAFFIC,    
					BLOCKBUB,
					ENCIRCUITO,
					MP,        
					NMP,       
					DESTINO,    
					ORDEN, 
					MESA1,    
					MESA2,   
					FECREG,    
					CAMINOALT,    
					TIEMPO)
				SELECT
				HISTAGV_SEQ.NEXTVAL,
				:dbNagv,
				:dbEstado,
				:dbBlockTraffic,
				:dbBlockBub,
				:dbEnCircuito,
				:dbMp,
				:dbNmp,
				:dbDestino,
				:dbCodord,
				:dbMesa1,
				:dbMesa2,
				sysdate,
				:dbCaminoAlt,
				:dbTiempo
				FROM DUAL; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 13;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "insert into HISTAGV (REGID,AGV,ESTADO,BLOCKTRAFFIC,BLOCKBU\
B,ENCIRCUITO,MP,NMP,DESTINO,ORDEN,MESA1,MESA2,FECREG,CAMINOALT,TIEMPO)select H\
ISTAGV_SEQ.nextval  ,:b0 ,:b1 ,:b2 ,:b3 ,:b4 ,:b5 ,:b6 ,:b7 ,:b8 ,:b9 ,:b10 ,s\
ysdate ,:b11 ,:b12  from DUAL ";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )144;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&dbNagv;
   sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)&dbEstado;
   sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)dbBlockTraffic;
   sqlstm.sqhstl[2] = (unsigned int  )0;
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         void  *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned int  )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (         void  *)dbBlockBub;
   sqlstm.sqhstl[3] = (unsigned int  )0;
   sqlstm.sqhsts[3] = (         int  )0;
   sqlstm.sqindv[3] = (         void  *)0;
   sqlstm.sqinds[3] = (         int  )0;
   sqlstm.sqharm[3] = (unsigned int  )0;
   sqlstm.sqadto[3] = (unsigned short )0;
   sqlstm.sqtdso[3] = (unsigned short )0;
   sqlstm.sqhstv[4] = (         void  *)dbEnCircuito;
   sqlstm.sqhstl[4] = (unsigned int  )0;
   sqlstm.sqhsts[4] = (         int  )0;
   sqlstm.sqindv[4] = (         void  *)0;
   sqlstm.sqinds[4] = (         int  )0;
   sqlstm.sqharm[4] = (unsigned int  )0;
   sqlstm.sqadto[4] = (unsigned short )0;
   sqlstm.sqtdso[4] = (unsigned short )0;
   sqlstm.sqhstv[5] = (         void  *)&dbMp;
   sqlstm.sqhstl[5] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[5] = (         int  )0;
   sqlstm.sqindv[5] = (         void  *)0;
   sqlstm.sqinds[5] = (         int  )0;
   sqlstm.sqharm[5] = (unsigned int  )0;
   sqlstm.sqadto[5] = (unsigned short )0;
   sqlstm.sqtdso[5] = (unsigned short )0;
   sqlstm.sqhstv[6] = (         void  *)&dbNmp;
   sqlstm.sqhstl[6] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[6] = (         int  )0;
   sqlstm.sqindv[6] = (         void  *)0;
   sqlstm.sqinds[6] = (         int  )0;
   sqlstm.sqharm[6] = (unsigned int  )0;
   sqlstm.sqadto[6] = (unsigned short )0;
   sqlstm.sqtdso[6] = (unsigned short )0;
   sqlstm.sqhstv[7] = (         void  *)&dbDestino;
   sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[7] = (         int  )0;
   sqlstm.sqindv[7] = (         void  *)0;
   sqlstm.sqinds[7] = (         int  )0;
   sqlstm.sqharm[7] = (unsigned int  )0;
   sqlstm.sqadto[7] = (unsigned short )0;
   sqlstm.sqtdso[7] = (unsigned short )0;
   sqlstm.sqhstv[8] = (         void  *)&dbCodord;
   sqlstm.sqhstl[8] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[8] = (         int  )0;
   sqlstm.sqindv[8] = (         void  *)0;
   sqlstm.sqinds[8] = (         int  )0;
   sqlstm.sqharm[8] = (unsigned int  )0;
   sqlstm.sqadto[8] = (unsigned short )0;
   sqlstm.sqtdso[8] = (unsigned short )0;
   sqlstm.sqhstv[9] = (         void  *)dbMesa1;
   sqlstm.sqhstl[9] = (unsigned int  )0;
   sqlstm.sqhsts[9] = (         int  )0;
   sqlstm.sqindv[9] = (         void  *)0;
   sqlstm.sqinds[9] = (         int  )0;
   sqlstm.sqharm[9] = (unsigned int  )0;
   sqlstm.sqadto[9] = (unsigned short )0;
   sqlstm.sqtdso[9] = (unsigned short )0;
   sqlstm.sqhstv[10] = (         void  *)dbMesa2;
   sqlstm.sqhstl[10] = (unsigned int  )0;
   sqlstm.sqhsts[10] = (         int  )0;
   sqlstm.sqindv[10] = (         void  *)0;
   sqlstm.sqinds[10] = (         int  )0;
   sqlstm.sqharm[10] = (unsigned int  )0;
   sqlstm.sqadto[10] = (unsigned short )0;
   sqlstm.sqtdso[10] = (unsigned short )0;
   sqlstm.sqhstv[11] = (         void  *)dbCaminoAlt;
   sqlstm.sqhstl[11] = (unsigned int  )0;
   sqlstm.sqhsts[11] = (         int  )0;
   sqlstm.sqindv[11] = (         void  *)0;
   sqlstm.sqinds[11] = (         int  )0;
   sqlstm.sqharm[11] = (unsigned int  )0;
   sqlstm.sqadto[11] = (unsigned short )0;
   sqlstm.sqtdso[11] = (unsigned short )0;
   sqlstm.sqhstv[12] = (         void  *)&dbTiempo;
   sqlstm.sqhstl[12] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[12] = (         int  )0;
   sqlstm.sqindv[12] = (         void  *)0;
   sqlstm.sqinds[12] = (         int  )0;
   sqlstm.sqharm[12] = (unsigned int  )0;
   sqlstm.sqadto[12] = (unsigned short )0;
   sqlstm.sqtdso[12] = (unsigned short )0;
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
   if (sqlca.sqlcode < 0) goto ArchivarHistAgvSqlError;
}



			/* EXEC SQL COMMIT; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
   sqlstm.sqlvsn = 10;
   sqlstm.arrsiz = 13;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )211;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
   if (sqlca.sqlcode < 0) goto ArchivarHistAgvSqlError;
}


			ptrAgv->tiempo = CTime::GetCurrentTime ();

			return;

		ArchivarHistAgvSqlError:
			MSGERROR1 ("Imposible insertar archivo historico para Agv: %d",
				dbNagv);

			return;
	}
	}
	CATCH(CException, ex)
	{	
		ex->ReportError();
	}
	END_CATCH
}


// Mover órdenes de transporte al histórico
void CDBEstadistica::ArchivarOrdTran()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 
 	T_sqlca sqlca;	// This is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;
	/* EXEC SQL WHENEVER SQLERROR GOTO ArchivarOrdTranError; */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL EXECUTE
	begin
		map.ArchivarHistOrdTran;
	end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 13;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin map . ArchivarHistOrdTran ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )226;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) goto ArchivarOrdTranError;
}


	// El commit es realizado por el procedure


	return;
	
ArchivarOrdTranError:
	MSGERROR ("Error al archivar Ordenes de Transporte");

	return;
}


// Grabar o quitar un defecto
void CDBEstadistica::LogDefecto(UINT nZona, UINT nIndMesa, UINT nBit, bool fFin, bool fAbort)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 	T_sqlca sqlca;	// This is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;
	/* EXEC SQL BEGIN DECLARE SECTION; */ 

	
	int dbZona;
	int dbInd;
	int dbBit;
	char dbTipFin;

	/* EXEC SQL END DECLARE SECTION; */ 


	dbZona = nZona;
	dbInd = nIndMesa;
	dbBit = nBit;

    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("LogDefecto: ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	if (fFin)
	{
		if (fAbort)
			dbTipFin = 'R';
		else
			dbTipFin = 'N';

		/* EXEC SQL UPDATE DEFECTO
			SET fecfin = sysdate,
				tipfin = :dbTipFin
			WHERE ZONA = :dbZona
			AND IND = :dbInd
			AND BIT = :dbBit
			AND TIPFIN = 'C'; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 13;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "update DEFECTO  set fecfin=sysdate,tipfin=:b0 where (((ZONA\
=:b1 and IND=:b2) and BIT=:b3) and TIPFIN='C')";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )241;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&dbTipFin;
  sqlstm.sqhstl[0] = (unsigned int  )1;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&dbZona;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&dbInd;
  sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)&dbBit;
  sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
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
  if (sqlca.sqlcode < 0) plsql_error("LogDefecto: ",sqlca,oraca);
}


		
	}
	else
		{
		dbTipFin = 'C';	// En curso
		/* EXEC SQL INSERT INTO DEFECTO 
			(ZONA,
			IND, 	
			BIT,
			FECCRE,
			FECFIN, 	
			TIPFIN)
		SELECT
			:dbZona,
			:dbInd,
			:dbBit,
			sysdate,
			null,
			:dbTipFin
			FROM dual; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 10;
  sqlstm.arrsiz = 13;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "insert into DEFECTO (ZONA,IND,BIT,FECCRE,FECFIN,TIPFIN)sele\
ct :b0 ,:b1 ,:b2 ,sysdate ,null  ,:b3  from dual ";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )272;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&dbZona;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&dbInd;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&dbBit;
  sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)&dbTipFin;
  sqlstm.sqhstl[3] = (unsigned int  )1;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
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
  if (sqlca.sqlcode < 0) plsql_error("LogDefecto: ",sqlca,oraca);
}


		}
	
	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 13;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )303;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("LogDefecto: ",sqlca,oraca);
}



}


bool CDBEstadistica::GetStringsDefecto(UINT nZona, UINT nIndMesa, UINT nBit, LPTSTR strCodMes, LPTSTR strCodClInd, LPTSTR strDefecto)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 	T_sqlca sqlca;	// This is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;
    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("GetStringsDefecto: ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO GetStringsDefectoNotFound; */ 


	bool fRetVal = false;

	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	
	char * dbCodClInd;
	int dbZona;
	int dbInd;
	char * dbCodMes;
	char * dbTexto;
	int dbBit;

	short idbCodClInd = -1;
	short idbCodMes = -1;
	short idbTexto = -1;


	/* EXEC SQL END DECLARE SECTION; */ 




	dbZona = nZona;
	dbInd = nIndMesa;
	dbBit = nBit;
	dbCodClInd = strCodClInd;

	dbCodMes = strCodMes;
	dbTexto = strDefecto;

	/* EXEC SQL DECLARE CURDEF CURSOR FOR
		SELECT CODCLIND,
				CODMES,
				TEXTO
		FROM DEFMESA, DEFTEXT
		WHERE DEFMESA.ZONA = :dbZona
		AND DEFMESA.IND = :dbInd
		AND DEFTEXT.BIT = :dbBit; */ 


	/* EXEC SQL OPEN CURDEF; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 13;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0012;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )318;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&dbZona;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbInd;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&dbBit;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
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
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetStringsDefecto: ",sqlca,oraca);
}



	/* EXEC SQL FETCH CURDEF INTO 
		:dbCodClInd:idbCodClInd,
		:dbCodMes:idbCodMes,
		:dbTexto:idbTexto; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 13;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )345;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)dbCodClInd;
 sqlstm.sqhstl[0] = (unsigned int  )0;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)&idbCodClInd;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)dbCodMes;
 sqlstm.sqhstl[1] = (unsigned int  )0;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)&idbCodMes;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)dbTexto;
 sqlstm.sqhstl[2] = (unsigned int  )0;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)&idbTexto;
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
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode == 1403) goto GetStringsDefectoNotFound;
 if (sqlca.sqlcode < 0) plsql_error("GetStringsDefecto: ",sqlca,oraca);
}



	fRetVal = true;

GetStringsDefectoNotFound:

	/* EXEC SQL CLOSE CURDEF; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 13;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )372;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetStringsDefecto: ",sqlca,oraca);
}


	
	return (fRetVal);
}



void CDBEstadistica::BorrarHistoricos()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 	T_sqlca sqlca;	// This is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;
	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("CDBEstadistica::BorrarHistoricos-> ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 

	
	MSGTRACE ("BorrarHistoricos");
	
	/* EXEC SQL EXECUTE
	begin
		map.BorrarHistoricos;		
	end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 10;
 sqlstm.arrsiz = 13;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin map . BorrarHistoricos ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )387;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("CDBEstadistica::BorrarHistoricos-> ",sqlca,oraca);
}




}
