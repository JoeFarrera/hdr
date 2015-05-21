
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
           char  filnam[47];
};
static const struct sqlcxp sqlfpn =
{
    46,
    "f:\\netprojects\\tpc_new_vs2008\\dbgestororden.pc"
};


static unsigned int sqlctx = 1764698917;


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
            void  *sqhstv[33];
   unsigned int   sqhstl[33];
            int   sqhsts[33];
            void  *sqindv[33];
            int   sqinds[33];
   unsigned int   sqharm[33];
   unsigned int   *sqharc[33];
   unsigned short  sqadto[33];
   unsigned short  sqtdso[33];
} sqlstm = {12,33};

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

 static const char *sq0013 = 
"select CODORD ,AGV ,FECCRE ,FECASIG ,FECREC ,FECFIN ,ESTADO ,PRIINI ,PRIACT \
,PRIFIN ,CODART ,NUMPAL ,ESTDESTPC ,ESTORITPC ,ESTDESTPCINI ,PTOCOMORI ,POSORI\
 ,LADOORI ,PTOCOMDES ,POSDES ,LADODES ,PTOCOMDESORIG ,POSDESORIG ,LADODESORIG \
,PTOCOMACT ,POSACT ,LADOACT ,TIPORD ,ERRORD ,MESA1 ,MESA2 ,SIMCARGA ,ORDRET  f\
rom ordtran where (estado=:b0 and agv=:b1) order by priact asc ,feccre  for up\
date ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4130,178,0,0,
5,0,0,1,47,0,4,222,0,0,1,0,0,1,0,2,3,0,0,
24,0,0,2,502,0,3,227,0,0,28,28,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,
3,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,3,0,0,1,3,0,0,1,5,0,0,1,3,0,0,1,3,0,0,
1,5,0,0,1,3,0,0,1,3,0,0,1,5,0,0,1,3,0,0,1,3,0,0,1,5,0,0,1,5,0,0,1,5,0,0,1,5,0,
0,1,5,0,0,1,5,0,0,1,5,0,0,
151,0,0,3,58,0,5,298,0,0,2,2,0,1,0,1,3,0,0,1,5,0,0,
174,0,0,4,58,0,5,302,0,0,2,2,0,1,0,1,3,0,0,1,5,0,0,
197,0,0,5,0,0,29,306,0,0,0,0,0,1,0,
212,0,0,6,136,0,6,354,0,0,4,4,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,2,3,0,0,
243,0,0,7,132,0,6,366,0,0,3,3,0,1,0,1,3,0,0,1,3,0,0,2,3,0,0,
270,0,0,8,50,0,6,378,0,0,1,1,0,1,0,1,3,0,0,
289,0,0,9,131,0,6,391,0,0,3,3,0,1,0,1,3,0,0,1,3,0,0,2,3,0,0,
316,0,0,10,52,0,6,403,0,0,1,1,0,1,0,1,3,0,0,
335,0,0,11,140,0,6,429,0,0,4,4,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,2,3,0,0,
366,0,0,12,119,0,6,508,0,0,3,3,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,
393,0,0,13,393,0,9,595,0,0,2,2,0,1,0,1,3,0,0,1,3,0,0,
416,0,0,13,0,0,13,596,0,0,33,0,0,1,0,2,3,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,
5,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,
0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
563,0,0,14,47,0,46,604,0,0,1,1,0,1,0,1,3,0,0,
582,0,0,15,0,0,29,607,0,0,0,0,0,1,0,
597,0,0,13,0,0,15,610,0,0,0,0,0,1,0,
612,0,0,16,150,0,6,744,0,0,5,5,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,1,5,0,0,1,3,0,0,
647,0,0,17,47,0,6,772,0,0,0,0,0,1,0,
662,0,0,18,35,0,6,803,0,0,0,0,0,1,0,
677,0,0,19,0,0,31,821,0,0,0,0,0,1,0,
692,0,0,20,57,0,6,851,0,0,2,2,0,1,0,2,3,0,0,1,3,0,0,
};


// DBGestorOrden.cpp: implementation of the CDBGestorOrden class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "DBGestorOrden.h"
#include "Orden.h"
#include "DBEstacion.h"
#include "Agv.h"
#include "GestorAgv.h"
#include "macro.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

// The ORACA=YES option must be specified to enable use of the ORACA
/* EXEC ORACLE OPTION (ORACA=YES); */ 


// Variables Oracle



/* EXEC SQL BEGIN DECLARE SECTION; */ 




struct dbOrden
{
 int codord;
 int agv;
 char feccre [7];
 char fecasig [7];
 char fecrec [7];
 char fecfin [7];
 int estado;
 int priini;
 int priact;
 int prifin;
 char codart [11];
 int numpal;
 char estdestpc [8];
 char estoritpc [8];
 char estdestpcini [8];
 int ptocomori;
 int posori;
 char ladoori [2];
 int ptocomdes;
 int posdes;
 char ladodes [2];
 int ptocomdesorig;
 int posdesorig;
 char ladodesorig [2];
 int ptocomact;
 int posact;
 char ladoact [2];
 char tipord [2];
 char errord [2];
 char mesa1 [2];
 char mesa2 [2];
 char simcarga [2];
 char ordret [2];
};

struct idbOrden
{
 short codord;
 short agv;
 short feccre;
 short fecasig;
 short fecrec;
 short fecfin;
 short estado;
 short priini;
 short priact;
 short prifin;
 short codart;
 short numpal;
 short estdestpc;
 short estoritpc;
 short estdestpcini;
 short ptocomori;
 short posori;
 short ladoori;
 short ptocomdes;
 short posdes;
 short ladodes;
 short ptocomdesorig;
 short posdesorig;
 short ladodesorig;
 short ptocomact;
 short posact;
 short ladoact;
 short tipord;
 short errord;
 short mesa1;
 short mesa2;
 short simcarga;
 short ordret;
 };


/* EXEC SQL END DECLARE SECTION; */ 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBGestorOrden::CDBGestorOrden()
{
	oraca.orastxtf = ORASTFERR;

}

CDBGestorOrden::~CDBGestorOrden()
{

}

// Insertar una orden nueva: NOTA: Solo las órdenes del BuB pasan por aquí
void CDBGestorOrden::InsertarOrden(COrden *ptrOrden)
{

 	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("InsertarOrden: ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 

	
	CString strVar;


	
	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	struct dbOrden * ptrDBorden;

	/* EXEC SQL END DECLARE SECTION; */ 


	// Buscar los datos de las estaciones de la orden
	ptrOrden->m_ptrEstOri = new CDBEstacion;
	ptrOrden->m_ptrEstDes = new CDBEstacion;

#if defined (FOREXPORTTOINDUMAT)
return;
#endif

	ptrDBorden = new struct dbOrden;
	ptrDBorden->agv = 0;
	ptrDBorden->estado = NEWORDER;
	ptrDBorden->priini = 1;
	ptrDBorden->priact = 1;
	ptrDBorden->prifin = 1;
	ptrDBorden->numpal = 0;
	if (ptrOrden->m_at.fMesa1)
		ptrDBorden->numpal = 1;
	if (ptrOrden->m_at.fMesa2)
		ptrDBorden->numpal++;


	ptrDBorden->ptocomori = ptrOrden->m_at.hz.nDest;
	ptrDBorden->posori = ptrOrden->m_at.hz.nPos;
	sprintf (ptrDBorden->ladoori, "%c", ptrOrden->m_at.hz.ucSide);

	ptrDBorden->ptocomdes = ptrOrden->m_at.bz.nDest;
	// Si va al A3, envialo al A2
	if (3 == ptrDBorden->ptocomdes)
		ptrDBorden->ptocomdes = 2;
		
	ptrDBorden->posdes = ptrOrden->m_at.bz.nPos;
	sprintf (ptrDBorden->ladodes, "%c", ptrOrden->m_at.bz.ucSide);

	ptrDBorden->ptocomdesorig = ptrOrden->m_at.bz.nDest;
	ptrDBorden->posdesorig = ptrOrden->m_at.bz.nPos;
	sprintf (ptrDBorden->ladodesorig, "%c", ptrOrden->m_at.bz.ucSide);

	ptrDBorden->ptocomact = ptrOrden->m_at.hz.nDest;
	ptrDBorden->posact = ptrOrden->m_at.hz.nPos;
	sprintf (ptrDBorden->ladoact, "%c", ptrOrden->m_at.hz.ucSide);

	sprintf (ptrDBorden->tipord, "%c", ptrOrden->m_at.herkunft);

	sprintf (ptrDBorden->errord, "%c", 'N');

	sprintf (ptrDBorden->mesa1, "%c",
				ptrOrden->m_at.fMesa1 ? 'S' : 'N');
 	sprintf (ptrDBorden->mesa2, "%c",
				ptrOrden->m_at.fMesa2 ? 'S' : 'N');
 	sprintf (ptrDBorden->simcarga, "%c",
				ptrOrden->m_at.fSimCarga ? 'S' : 'N');

	sprintf (ptrDBorden->ordret, "%c", 'N');

	ptrOrden->m_ptrEstOri->GetEstAgv (ptrOrden->m_at.hz.nDest,
		ptrOrden->m_at.hz.nPos,
		ptrOrden->m_at.hz.ucSide);

	ptrOrden->m_ptrEstDes->GetEstAgv (ptrOrden->m_at.bz.nDest,
		ptrOrden->m_at.bz.nPos,
		ptrOrden->m_at.bz.ucSide);

	strcpy (ptrDBorden->estdestpcini, (char *)ptrOrden->m_ptrEstDes->GetCodEstPul ());
	strcpy (ptrDBorden->estoritpc, (char *)ptrOrden->m_ptrEstOri->GetCodEstPul ());
	
	// Si va al A3, envíalo al A2
	if (3 == ptrOrden->m_at.bz.nDest)
	{
		ptrOrden->m_at.bz.nDest = 2;

		ptrOrden->m_ptrEstDes->GetEstAgv (
			ptrOrden->m_at.bz.nDest,
			ptrOrden->m_at.bz.nPos,
			ptrOrden->m_at.bz.ucSide);

	}
	strcpy (ptrDBorden->estdestpc, (char *)ptrOrden->m_ptrEstDes->GetCodEstPul ());

	/* EXEC SQL SELECT ordtranseq.nextval
		INTO :ptrDBorden->codord
		FROM dual; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 1;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select ordtranseq.nextval  into :b0  from dual ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&(ptrDBorden->codord);
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
 if (sqlca.sqlcode < 0) plsql_error("InsertarOrden: ",sqlca,oraca);
}



	
	/* EXEC SQL INSERT INTO ORDTRAN
            (CODORD
            ,AGV
            ,FECCRE
            ,FECASIG
			,FECREC
            ,FECFIN
            ,ESTADO
            ,PRIINI
            ,PRIACT
            ,PRIFIN
            ,CODART
            ,NUMPAL
            ,ESTDESTPC
            ,ESTORITPC
            ,ESTDESTPCINI
            ,PTOCOMORI
            ,POSORI
            ,LADOORI
            ,PTOCOMDES
            ,POSDES
            ,LADODES
            ,PTOCOMDESORIG
            ,POSDESORIG
            ,LADODESORIG
            ,PTOCOMACT
            ,POSACT
            ,LADOACT
            ,TIPORD
            ,ERRORD
            ,MESA1
            ,MESA2
            ,SIMCARGA
            ,ORDRET)
		SELECT 
			  :ptrDBorden->codord
			 , :ptrDBorden->agv
			 , sysdate
			 , null
			 , null
			 , null
			 , :ptrDBorden->estado
			 , :ptrDBorden->priini
			 , :ptrDBorden->priact
			 , :ptrDBorden->prifin
			 , null
			 , :ptrDBorden->numpal
			 , :ptrDBorden->estdestpc
			 , :ptrDBorden->estoritpc
			 , :ptrDBorden->estdestpcini
			 , :ptrDBorden->ptocomori
			 , :ptrDBorden->posori
			 , :ptrDBorden->ladoori
			 , :ptrDBorden->ptocomdes
			 , :ptrDBorden->posdes
			 , :ptrDBorden->ladodes
			 , :ptrDBorden->ptocomdesorig
			 , :ptrDBorden->posdesorig
			 , :ptrDBorden->ladodesorig
			 , :ptrDBorden->ptocomact
			 , :ptrDBorden->posact
			 , :ptrDBorden->ladoact
			 , :ptrDBorden->tipord
			 , :ptrDBorden->errord
			 , :ptrDBorden->mesa1
			 , :ptrDBorden->mesa2
			 , :ptrDBorden->simcarga
			 , :ptrDBorden->ordret
			 from dual; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 28;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "insert into ORDTRAN (CODORD,AGV,FECCRE,FECASIG,FECREC,FECFIN\
,ESTADO,PRIINI,PRIACT,PRIFIN,CODART,NUMPAL,ESTDESTPC,ESTORITPC,ESTDESTPCINI,PT\
OCOMORI,POSORI,LADOORI,PTOCOMDES,POSDES,LADODES,PTOCOMDESORIG,POSDESORIG,LADOD\
ESORIG,PTOCOMACT,POSACT,LADOACT,TIPORD,ERRORD,MESA1,MESA2,SIMCARGA,ORDRET)sele\
ct :b0 ,:b1 ,sysdate ,null  ,null  ,null  ,:b2 ,:b3 ,:b4 ,:b5 ,null  ,:b6 ,:b7\
 ,:b8 ,:b9 ,:b10 ,:b11 ,:b12 ,:b13 ,:b14 ,:b15 ,:b16 ,:b17 ,:b18 ,:b19 ,:b20 ,\
:b21 ,:b22 ,:b23 ,:b24 ,:b25 ,:b26 ,:b27  from dual ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )24;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&(ptrDBorden->codord);
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&(ptrDBorden->agv);
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&(ptrDBorden->estado);
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&(ptrDBorden->priini);
 sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&(ptrDBorden->priact);
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)&(ptrDBorden->prifin);
 sqlstm.sqhstl[5] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)&(ptrDBorden->numpal);
 sqlstm.sqhstl[6] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)(ptrDBorden->estdestpc);
 sqlstm.sqhstl[7] = (unsigned int  )8;
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (         void  *)(ptrDBorden->estoritpc);
 sqlstm.sqhstl[8] = (unsigned int  )8;
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         void  *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned int  )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (         void  *)(ptrDBorden->estdestpcini);
 sqlstm.sqhstl[9] = (unsigned int  )8;
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         void  *)0;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned int  )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (         void  *)&(ptrDBorden->ptocomori);
 sqlstm.sqhstl[10] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         void  *)0;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned int  )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
 sqlstm.sqhstv[11] = (         void  *)&(ptrDBorden->posori);
 sqlstm.sqhstl[11] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[11] = (         int  )0;
 sqlstm.sqindv[11] = (         void  *)0;
 sqlstm.sqinds[11] = (         int  )0;
 sqlstm.sqharm[11] = (unsigned int  )0;
 sqlstm.sqadto[11] = (unsigned short )0;
 sqlstm.sqtdso[11] = (unsigned short )0;
 sqlstm.sqhstv[12] = (         void  *)(ptrDBorden->ladoori);
 sqlstm.sqhstl[12] = (unsigned int  )2;
 sqlstm.sqhsts[12] = (         int  )0;
 sqlstm.sqindv[12] = (         void  *)0;
 sqlstm.sqinds[12] = (         int  )0;
 sqlstm.sqharm[12] = (unsigned int  )0;
 sqlstm.sqadto[12] = (unsigned short )0;
 sqlstm.sqtdso[12] = (unsigned short )0;
 sqlstm.sqhstv[13] = (         void  *)&(ptrDBorden->ptocomdes);
 sqlstm.sqhstl[13] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[13] = (         int  )0;
 sqlstm.sqindv[13] = (         void  *)0;
 sqlstm.sqinds[13] = (         int  )0;
 sqlstm.sqharm[13] = (unsigned int  )0;
 sqlstm.sqadto[13] = (unsigned short )0;
 sqlstm.sqtdso[13] = (unsigned short )0;
 sqlstm.sqhstv[14] = (         void  *)&(ptrDBorden->posdes);
 sqlstm.sqhstl[14] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[14] = (         int  )0;
 sqlstm.sqindv[14] = (         void  *)0;
 sqlstm.sqinds[14] = (         int  )0;
 sqlstm.sqharm[14] = (unsigned int  )0;
 sqlstm.sqadto[14] = (unsigned short )0;
 sqlstm.sqtdso[14] = (unsigned short )0;
 sqlstm.sqhstv[15] = (         void  *)(ptrDBorden->ladodes);
 sqlstm.sqhstl[15] = (unsigned int  )2;
 sqlstm.sqhsts[15] = (         int  )0;
 sqlstm.sqindv[15] = (         void  *)0;
 sqlstm.sqinds[15] = (         int  )0;
 sqlstm.sqharm[15] = (unsigned int  )0;
 sqlstm.sqadto[15] = (unsigned short )0;
 sqlstm.sqtdso[15] = (unsigned short )0;
 sqlstm.sqhstv[16] = (         void  *)&(ptrDBorden->ptocomdesorig);
 sqlstm.sqhstl[16] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[16] = (         int  )0;
 sqlstm.sqindv[16] = (         void  *)0;
 sqlstm.sqinds[16] = (         int  )0;
 sqlstm.sqharm[16] = (unsigned int  )0;
 sqlstm.sqadto[16] = (unsigned short )0;
 sqlstm.sqtdso[16] = (unsigned short )0;
 sqlstm.sqhstv[17] = (         void  *)&(ptrDBorden->posdesorig);
 sqlstm.sqhstl[17] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[17] = (         int  )0;
 sqlstm.sqindv[17] = (         void  *)0;
 sqlstm.sqinds[17] = (         int  )0;
 sqlstm.sqharm[17] = (unsigned int  )0;
 sqlstm.sqadto[17] = (unsigned short )0;
 sqlstm.sqtdso[17] = (unsigned short )0;
 sqlstm.sqhstv[18] = (         void  *)(ptrDBorden->ladodesorig);
 sqlstm.sqhstl[18] = (unsigned int  )2;
 sqlstm.sqhsts[18] = (         int  )0;
 sqlstm.sqindv[18] = (         void  *)0;
 sqlstm.sqinds[18] = (         int  )0;
 sqlstm.sqharm[18] = (unsigned int  )0;
 sqlstm.sqadto[18] = (unsigned short )0;
 sqlstm.sqtdso[18] = (unsigned short )0;
 sqlstm.sqhstv[19] = (         void  *)&(ptrDBorden->ptocomact);
 sqlstm.sqhstl[19] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[19] = (         int  )0;
 sqlstm.sqindv[19] = (         void  *)0;
 sqlstm.sqinds[19] = (         int  )0;
 sqlstm.sqharm[19] = (unsigned int  )0;
 sqlstm.sqadto[19] = (unsigned short )0;
 sqlstm.sqtdso[19] = (unsigned short )0;
 sqlstm.sqhstv[20] = (         void  *)&(ptrDBorden->posact);
 sqlstm.sqhstl[20] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[20] = (         int  )0;
 sqlstm.sqindv[20] = (         void  *)0;
 sqlstm.sqinds[20] = (         int  )0;
 sqlstm.sqharm[20] = (unsigned int  )0;
 sqlstm.sqadto[20] = (unsigned short )0;
 sqlstm.sqtdso[20] = (unsigned short )0;
 sqlstm.sqhstv[21] = (         void  *)(ptrDBorden->ladoact);
 sqlstm.sqhstl[21] = (unsigned int  )2;
 sqlstm.sqhsts[21] = (         int  )0;
 sqlstm.sqindv[21] = (         void  *)0;
 sqlstm.sqinds[21] = (         int  )0;
 sqlstm.sqharm[21] = (unsigned int  )0;
 sqlstm.sqadto[21] = (unsigned short )0;
 sqlstm.sqtdso[21] = (unsigned short )0;
 sqlstm.sqhstv[22] = (         void  *)(ptrDBorden->tipord);
 sqlstm.sqhstl[22] = (unsigned int  )2;
 sqlstm.sqhsts[22] = (         int  )0;
 sqlstm.sqindv[22] = (         void  *)0;
 sqlstm.sqinds[22] = (         int  )0;
 sqlstm.sqharm[22] = (unsigned int  )0;
 sqlstm.sqadto[22] = (unsigned short )0;
 sqlstm.sqtdso[22] = (unsigned short )0;
 sqlstm.sqhstv[23] = (         void  *)(ptrDBorden->errord);
 sqlstm.sqhstl[23] = (unsigned int  )2;
 sqlstm.sqhsts[23] = (         int  )0;
 sqlstm.sqindv[23] = (         void  *)0;
 sqlstm.sqinds[23] = (         int  )0;
 sqlstm.sqharm[23] = (unsigned int  )0;
 sqlstm.sqadto[23] = (unsigned short )0;
 sqlstm.sqtdso[23] = (unsigned short )0;
 sqlstm.sqhstv[24] = (         void  *)(ptrDBorden->mesa1);
 sqlstm.sqhstl[24] = (unsigned int  )2;
 sqlstm.sqhsts[24] = (         int  )0;
 sqlstm.sqindv[24] = (         void  *)0;
 sqlstm.sqinds[24] = (         int  )0;
 sqlstm.sqharm[24] = (unsigned int  )0;
 sqlstm.sqadto[24] = (unsigned short )0;
 sqlstm.sqtdso[24] = (unsigned short )0;
 sqlstm.sqhstv[25] = (         void  *)(ptrDBorden->mesa2);
 sqlstm.sqhstl[25] = (unsigned int  )2;
 sqlstm.sqhsts[25] = (         int  )0;
 sqlstm.sqindv[25] = (         void  *)0;
 sqlstm.sqinds[25] = (         int  )0;
 sqlstm.sqharm[25] = (unsigned int  )0;
 sqlstm.sqadto[25] = (unsigned short )0;
 sqlstm.sqtdso[25] = (unsigned short )0;
 sqlstm.sqhstv[26] = (         void  *)(ptrDBorden->simcarga);
 sqlstm.sqhstl[26] = (unsigned int  )2;
 sqlstm.sqhsts[26] = (         int  )0;
 sqlstm.sqindv[26] = (         void  *)0;
 sqlstm.sqinds[26] = (         int  )0;
 sqlstm.sqharm[26] = (unsigned int  )0;
 sqlstm.sqadto[26] = (unsigned short )0;
 sqlstm.sqtdso[26] = (unsigned short )0;
 sqlstm.sqhstv[27] = (         void  *)(ptrDBorden->ordret);
 sqlstm.sqhstl[27] = (unsigned int  )2;
 sqlstm.sqhsts[27] = (         int  )0;
 sqlstm.sqindv[27] = (         void  *)0;
 sqlstm.sqinds[27] = (         int  )0;
 sqlstm.sqharm[27] = (unsigned int  )0;
 sqlstm.sqadto[27] = (unsigned short )0;
 sqlstm.sqtdso[27] = (unsigned short )0;
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
 if (sqlca.sqlcode < 0) plsql_error("InsertarOrden: ",sqlca,oraca);
}



		
	/* EXEC SQL update conest
			set resagv = resagv + :ptrDBorden->numpal 
			where codestpul = :ptrDBorden->estoritpc; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 28;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update conest  set resagv=(resagv+:b0) where codestpul=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )151;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&(ptrDBorden->numpal);
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)(ptrDBorden->estoritpc);
 sqlstm.sqhstl[1] = (unsigned int  )8;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
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
 if (sqlca.sqlcode < 0) plsql_error("InsertarOrden: ",sqlca,oraca);
}



	/* EXEC SQL update conest
			 set resagv = resagv + :ptrDBorden->numpal 
			where codestpul = :ptrDBorden->estdestpc; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 28;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update conest  set resagv=(resagv+:b0) where codestpul=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )174;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&(ptrDBorden->numpal);
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)(ptrDBorden->estdestpc);
 sqlstm.sqhstl[1] = (unsigned int  )8;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
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
 if (sqlca.sqlcode < 0) plsql_error("InsertarOrden: ",sqlca,oraca);
}


	
	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 28;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )197;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("InsertarOrden: ",sqlca,oraca);
}



	// Actualizar el identificador de la orden
	sprintf ((char *)ptrOrden->m_at.id_nr, "%*d", KMAXID_AT - 1, ptrDBorden->codord);
	delete ptrDBorden;
}

// Actualizar el estado de una orden
void CDBGestorOrden::UpdateEstadoOrden(COrden *ptrOrden, TEstadoOrden estadoNuevo)
{

	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
		/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbCodord;
	int dbAgv;
	int dbRetVal;
	int dbEstado;
	int dbRecogido;

	/* EXEC SQL END DECLARE SECTION; */ 


#if defined (FOREXPORTTOINDUMAT)
return;
#endif


	dbCodord = atoi ((char *)ptrOrden->m_at.id_nr); // Any chance of this failing ?
	dbAgv = ptrOrden->m_at.fznr;
	dbEstado = estadoNuevo;

// Michael 07.04.2003  EXEC SQL WHENEVER SQLERROR DO plsql_error("UpdateEstadoOrden: ");
	/* EXEC SQL WHENEVER SQLERROR GOTO UpdateEstadoOrdenError; */ 

	
	while (true)
	{

	MSGTRACE3 ("UpdateEstadoOrden: dbCodord: %d dbEstado: %d estadoNuevo: %d",
						dbCodord, dbEstado, estadoNuevo);
	if (estadoNuevo != ptrOrden->GetEstado ())
	{
	// Solo actualizar si el estado es diferente
	switch (estadoNuevo)
	{
		case PENDINGASSIGN:
		case COLLECT:
		// Michael : Este estado no modifica asignación 17.10.2001 case COLLECT:
			/* EXEC SQL EXECUTE
				begin
					if (map.AsignarOrdTran (:dbCodord, :dbEstado, :dbAgv)) then
						:dbRetVal := 1;
					else
						:dbRetVal := 0;
					end if;
					commit;
				end;
			END-EXEC; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 28;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "begin if ( map . AsignarOrdTran ( :dbCodord , :dbEstado , \
:dbAgv ) ) then :dbRetVal := 1 ; else :dbRetVal := 0 ; end if ; commit ; end ;";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )212;
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
   sqlstm.sqhstv[1] = (         void  *)&dbEstado;
   sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)&dbAgv;
   sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         void  *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned int  )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (         void  *)&dbRetVal;
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
   if (sqlca.sqlcode < 0) goto UpdateEstadoOrdenError;
}


			break;
		case DELIVER:
			/* EXEC SQL EXECUTE
				begin
					if (map.IniciarCargaOrdTran (:dbCodord, :dbEstado)) then
						:dbRetVal := 1;
					else
						:dbRetVal := 0;
					end if;
					commit;
				end;
			END-EXEC; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 28;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "begin if ( map . IniciarCargaOrdTran ( :dbCodord , :dbEsta\
do ) ) then :dbRetVal := 1 ; else :dbRetVal := 0 ; end if ; commit ; end ;";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )243;
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
   sqlstm.sqhstv[1] = (         void  *)&dbEstado;
   sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)&dbRetVal;
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
   if (sqlca.sqlcode < 0) goto UpdateEstadoOrdenError;
}


			if (g_Container.m_fSimulateCom)
			{
				/* EXEC SQL EXECUTE
				begin
					map.simload(:dbCodord);
					commit;
				end;
				END-EXEC; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 28;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "begin map . simload ( :dbCodord ) ; commit ; end ;";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )270;
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
    if (sqlca.sqlcode < 0) goto UpdateEstadoOrdenError;
}


			}
				
			break;
		case DELIVERED:
			// Michael 29.08.2002 Aquí se produce un bloqueo ?
			MSGTRACE1 ("CDBGestorOrden::UpdateEstadoOrden () antes FinDescargaOrdTran %s",
				ptrOrden->m_at.id_nr); 
			/* EXEC SQL EXECUTE
				begin
					if (map.FinDescargaOrdTran (:dbCodord, :dbEstado)) then
						:dbRetVal := 1;
					else
						:dbRetVal := 0;
					end if;
					commit;
				end;
			END-EXEC; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 28;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "begin if ( map . FinDescargaOrdTran ( :dbCodord , :dbEstad\
o ) ) then :dbRetVal := 1 ; else :dbRetVal := 0 ; end if ; commit ; end ;";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )289;
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
   sqlstm.sqhstv[1] = (         void  *)&dbEstado;
   sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)&dbRetVal;
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
   if (sqlca.sqlcode < 0) goto UpdateEstadoOrdenError;
}


			if (g_Container.m_fSimulateCom)
				{
				/* EXEC SQL EXECUTE
				begin
					map.simunload(:dbCodord);
					commit;
				end;
				END-EXEC; */ 

{
    struct sqlexd sqlstm;
    sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
    sqlstm.sqlvsn = 12;
    sqlstm.arrsiz = 28;
    sqlstm.sqladtp = &sqladt;
    sqlstm.sqltdsp = &sqltds;
    sqlstm.stmt = "begin map . simunload ( :dbCodord ) ; commit ; end ;";
    sqlstm.iters = (unsigned int  )1;
    sqlstm.offset = (unsigned int  )316;
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
    if (sqlca.sqlcode < 0) goto UpdateEstadoOrdenError;
}


				}
	
	
			// Michael 29.08.2002 Aquí se produce un bloqueo ?
			MSGTRACE1 ("CDBGestorOrden::UpdateEstadoOrden () despues FinDescargaOrdTran %s",
				ptrOrden->m_at.id_nr); 
			break;
		case CANCELLED:
			switch (ptrOrden->GetEstado ())
			{
				case NEWORDER:
				case PENDINGASSIGN:
				case COLLECT:
					// Orden no empezado
					dbRecogido = 0;
					break;
				default:
					dbRecogido = 1;
					break;
			}
			/* EXEC SQL EXECUTE
				begin
					if (map.AnularOrdTran (:dbCodord, :dbRecogido, :dbEstado)) then
						:dbRetVal := 1;
					else
						:dbRetVal := 0;
					end if;
					commit;
				end;
			END-EXEC; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 28;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "begin if ( map . AnularOrdTran ( :dbCodord , :dbRecogido ,\
 :dbEstado ) ) then :dbRetVal := 1 ; else :dbRetVal := 0 ; end if ; commit ; e\
nd ;";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )335;
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
   sqlstm.sqhstv[1] = (         void  *)&dbRecogido;
   sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[1] = (         int  )0;
   sqlstm.sqindv[1] = (         void  *)0;
   sqlstm.sqinds[1] = (         int  )0;
   sqlstm.sqharm[1] = (unsigned int  )0;
   sqlstm.sqadto[1] = (unsigned short )0;
   sqlstm.sqtdso[1] = (unsigned short )0;
   sqlstm.sqhstv[2] = (         void  *)&dbEstado;
   sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[2] = (         int  )0;
   sqlstm.sqindv[2] = (         void  *)0;
   sqlstm.sqinds[2] = (         int  )0;
   sqlstm.sqharm[2] = (unsigned int  )0;
   sqlstm.sqadto[2] = (unsigned short )0;
   sqlstm.sqtdso[2] = (unsigned short )0;
   sqlstm.sqhstv[3] = (         void  *)&dbRetVal;
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
   if (sqlca.sqlcode < 0) goto UpdateEstadoOrdenError;
}


			break;
	}
	}
	else
		{
		MSGAVISO2 ("UpdateEstadoOrden (): La orden %s tiene estado %d ya actualizado",
			ptrOrden->m_at.id_nr,
			estadoNuevo);
		}
	return;

// Michael 07.04.2003
UpdateEstadoOrdenError:
 switch(sqlca.sqlcode) 
	{
	case -60:	// Bloqueo mutuo
		MSGERROR1 ("UpdateEstadoOrden (): Oracle Bloque Mutuo orden %s", ptrOrden->m_at.id_nr);
		continue;
		break;
	default:	// otro error - se debe tratar
		MSGERROR1 ("UpdateEstadoOrden (): Oracle salca.sqlcode is: %d", sqlca.sqlcode);
		plsql_error("UpdateEstadoOrden: ",sqlca, oraca);
		return;
	}
	} // while (true)
			
}

/* Borrar una orden a peticion del BuB
   Solo se "borra" aquí si es que ha sido anulado. 
   Sino, se termina convencionalmente
 */
void CDBGestorOrden::DeleteOrden(COrden *ptrOrden)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
		/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbCodord;
	int dbEstado;
	int dbRecogido;

	/* EXEC SQL END DECLARE SECTION; */ 


    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("DeleteOrden: ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	dbCodord = atoi ((char *)ptrOrden->m_at.id_nr); // Any chance of this failing ?
	dbEstado = CANCELLED;

	switch (ptrOrden->GetEstado ())
	{
		case NEWORDER:
		case PENDINGASSIGN:
		case COLLECT:
			// Orden no empezado
			dbRecogido = 0;
			break;
		case DELIVER:
			dbRecogido = 1;
			break;
		default:
			dbRecogido = -1;	// No procede anularlo ya tratado por cambios anteriores

	}

	if (-1 != dbRecogido)
	{
		/* EXEC SQL EXECUTE
			declare
				bRetVal boolean;
			begin
			bRetVal := map.AnularOrdTran (:dbCodord, :dbRecogido, :dbEstado);
			commit;
			end;
		END-EXEC; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 28;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "declare bRetVal boolean ; begin bRetVal := map . AnularOrdT\
ran ( :dbCodord , :dbRecogido , :dbEstado ) ; commit ; end ;";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )366;
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
  sqlstm.sqhstv[1] = (         void  *)&dbRecogido;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&dbEstado;
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
  if (sqlca.sqlcode < 0) plsql_error("DeleteOrden: ",sqlca,oraca);
}


	}

}


// Devolver la orden más prioritaria según el estado pedido
COrden * CDBGestorOrden::GetOrden(int estado, UINT nAgv)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
		/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

	T_sqlca sqlca;	// For this is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;

	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	struct dbOrden * ptrDBorden;
	struct idbOrden * ptriDBorden;
	int dbEstado;
	int dbAgv;


	/* EXEC SQL END DECLARE SECTION; */ 


#if defined (FOREXPORTTOINDUMAT)
	return (NULL);
#endif

	COrden * ptrOrden = new COrden;	// El borrado de la orden se hara cuando se acabe

	ptrDBorden = new struct dbOrden;
	ptriDBorden = new struct idbOrden;
	
    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("GetOrden: ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO GetOrdenNoHay; */ 


	dbEstado = estado;
	dbAgv = nAgv;

	/* EXEC SQL DECLARE CURORDTRANNEW CURSOR FOR
		select 
			 CODORD
            ,AGV
            ,FECCRE
            ,FECASIG
			,FECREC
            ,FECFIN
            ,ESTADO
            ,PRIINI
            ,PRIACT
            ,PRIFIN
            ,CODART
            ,NUMPAL
            ,ESTDESTPC
            ,ESTORITPC
            ,ESTDESTPCINI
            ,PTOCOMORI
            ,POSORI
            ,LADOORI
            ,PTOCOMDES
            ,POSDES
            ,LADODES
            ,PTOCOMDESORIG
            ,POSDESORIG
            ,LADODESORIG
            ,PTOCOMACT
            ,POSACT
            ,LADOACT
            ,TIPORD
            ,ERRORD
            ,MESA1
            ,MESA2
            ,SIMCARGA
            ,ORDRET
			 from ordtran
		where estado = :dbEstado
		and (agv = :dbAgv)	-- Si si llama para una orden nueva, agv es 0	
		order by priact asc, feccre
		for update; */ 

	/* EXEC SQL OPEN CURORDTRANNEW; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 28;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0013;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )393;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (         void  *)&dbEstado;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbAgv;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
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
 if (sqlca.sqlcode < 0) plsql_error("GetOrden: ",sqlca,oraca);
}


	/* EXEC SQL FETCH CURORDTRANNEW INTO ptrDBorden:ptriDBorden; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 33;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )416;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqfoff = (           int )0;
 sqlstm.sqfmod = (unsigned int )2;
 sqlstm.sqhstv[0] = (         void  *)&ptrDBorden->codord;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)&ptriDBorden->codord;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&ptrDBorden->agv;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)&ptriDBorden->agv;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)ptrDBorden->feccre;
 sqlstm.sqhstl[2] = (unsigned int  )7;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)&ptriDBorden->feccre;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)ptrDBorden->fecasig;
 sqlstm.sqhstl[3] = (unsigned int  )7;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)&ptriDBorden->fecasig;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)ptrDBorden->fecrec;
 sqlstm.sqhstl[4] = (unsigned int  )7;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)&ptriDBorden->fecrec;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)ptrDBorden->fecfin;
 sqlstm.sqhstl[5] = (unsigned int  )7;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)&ptriDBorden->fecfin;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)&ptrDBorden->estado;
 sqlstm.sqhstl[6] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)&ptriDBorden->estado;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)&ptrDBorden->priini;
 sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)&ptriDBorden->priini;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (         void  *)&ptrDBorden->priact;
 sqlstm.sqhstl[8] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         void  *)&ptriDBorden->priact;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned int  )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (         void  *)&ptrDBorden->prifin;
 sqlstm.sqhstl[9] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         void  *)&ptriDBorden->prifin;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned int  )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (         void  *)ptrDBorden->codart;
 sqlstm.sqhstl[10] = (unsigned int  )11;
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         void  *)&ptriDBorden->codart;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned int  )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
 sqlstm.sqhstv[11] = (         void  *)&ptrDBorden->numpal;
 sqlstm.sqhstl[11] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[11] = (         int  )0;
 sqlstm.sqindv[11] = (         void  *)&ptriDBorden->numpal;
 sqlstm.sqinds[11] = (         int  )0;
 sqlstm.sqharm[11] = (unsigned int  )0;
 sqlstm.sqadto[11] = (unsigned short )0;
 sqlstm.sqtdso[11] = (unsigned short )0;
 sqlstm.sqhstv[12] = (         void  *)ptrDBorden->estdestpc;
 sqlstm.sqhstl[12] = (unsigned int  )8;
 sqlstm.sqhsts[12] = (         int  )0;
 sqlstm.sqindv[12] = (         void  *)&ptriDBorden->estdestpc;
 sqlstm.sqinds[12] = (         int  )0;
 sqlstm.sqharm[12] = (unsigned int  )0;
 sqlstm.sqadto[12] = (unsigned short )0;
 sqlstm.sqtdso[12] = (unsigned short )0;
 sqlstm.sqhstv[13] = (         void  *)ptrDBorden->estoritpc;
 sqlstm.sqhstl[13] = (unsigned int  )8;
 sqlstm.sqhsts[13] = (         int  )0;
 sqlstm.sqindv[13] = (         void  *)&ptriDBorden->estoritpc;
 sqlstm.sqinds[13] = (         int  )0;
 sqlstm.sqharm[13] = (unsigned int  )0;
 sqlstm.sqadto[13] = (unsigned short )0;
 sqlstm.sqtdso[13] = (unsigned short )0;
 sqlstm.sqhstv[14] = (         void  *)ptrDBorden->estdestpcini;
 sqlstm.sqhstl[14] = (unsigned int  )8;
 sqlstm.sqhsts[14] = (         int  )0;
 sqlstm.sqindv[14] = (         void  *)&ptriDBorden->estdestpcini;
 sqlstm.sqinds[14] = (         int  )0;
 sqlstm.sqharm[14] = (unsigned int  )0;
 sqlstm.sqadto[14] = (unsigned short )0;
 sqlstm.sqtdso[14] = (unsigned short )0;
 sqlstm.sqhstv[15] = (         void  *)&ptrDBorden->ptocomori;
 sqlstm.sqhstl[15] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[15] = (         int  )0;
 sqlstm.sqindv[15] = (         void  *)&ptriDBorden->ptocomori;
 sqlstm.sqinds[15] = (         int  )0;
 sqlstm.sqharm[15] = (unsigned int  )0;
 sqlstm.sqadto[15] = (unsigned short )0;
 sqlstm.sqtdso[15] = (unsigned short )0;
 sqlstm.sqhstv[16] = (         void  *)&ptrDBorden->posori;
 sqlstm.sqhstl[16] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[16] = (         int  )0;
 sqlstm.sqindv[16] = (         void  *)&ptriDBorden->posori;
 sqlstm.sqinds[16] = (         int  )0;
 sqlstm.sqharm[16] = (unsigned int  )0;
 sqlstm.sqadto[16] = (unsigned short )0;
 sqlstm.sqtdso[16] = (unsigned short )0;
 sqlstm.sqhstv[17] = (         void  *)ptrDBorden->ladoori;
 sqlstm.sqhstl[17] = (unsigned int  )2;
 sqlstm.sqhsts[17] = (         int  )0;
 sqlstm.sqindv[17] = (         void  *)&ptriDBorden->ladoori;
 sqlstm.sqinds[17] = (         int  )0;
 sqlstm.sqharm[17] = (unsigned int  )0;
 sqlstm.sqadto[17] = (unsigned short )0;
 sqlstm.sqtdso[17] = (unsigned short )0;
 sqlstm.sqhstv[18] = (         void  *)&ptrDBorden->ptocomdes;
 sqlstm.sqhstl[18] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[18] = (         int  )0;
 sqlstm.sqindv[18] = (         void  *)&ptriDBorden->ptocomdes;
 sqlstm.sqinds[18] = (         int  )0;
 sqlstm.sqharm[18] = (unsigned int  )0;
 sqlstm.sqadto[18] = (unsigned short )0;
 sqlstm.sqtdso[18] = (unsigned short )0;
 sqlstm.sqhstv[19] = (         void  *)&ptrDBorden->posdes;
 sqlstm.sqhstl[19] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[19] = (         int  )0;
 sqlstm.sqindv[19] = (         void  *)&ptriDBorden->posdes;
 sqlstm.sqinds[19] = (         int  )0;
 sqlstm.sqharm[19] = (unsigned int  )0;
 sqlstm.sqadto[19] = (unsigned short )0;
 sqlstm.sqtdso[19] = (unsigned short )0;
 sqlstm.sqhstv[20] = (         void  *)ptrDBorden->ladodes;
 sqlstm.sqhstl[20] = (unsigned int  )2;
 sqlstm.sqhsts[20] = (         int  )0;
 sqlstm.sqindv[20] = (         void  *)&ptriDBorden->ladodes;
 sqlstm.sqinds[20] = (         int  )0;
 sqlstm.sqharm[20] = (unsigned int  )0;
 sqlstm.sqadto[20] = (unsigned short )0;
 sqlstm.sqtdso[20] = (unsigned short )0;
 sqlstm.sqhstv[21] = (         void  *)&ptrDBorden->ptocomdesorig;
 sqlstm.sqhstl[21] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[21] = (         int  )0;
 sqlstm.sqindv[21] = (         void  *)&ptriDBorden->ptocomdesorig;
 sqlstm.sqinds[21] = (         int  )0;
 sqlstm.sqharm[21] = (unsigned int  )0;
 sqlstm.sqadto[21] = (unsigned short )0;
 sqlstm.sqtdso[21] = (unsigned short )0;
 sqlstm.sqhstv[22] = (         void  *)&ptrDBorden->posdesorig;
 sqlstm.sqhstl[22] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[22] = (         int  )0;
 sqlstm.sqindv[22] = (         void  *)&ptriDBorden->posdesorig;
 sqlstm.sqinds[22] = (         int  )0;
 sqlstm.sqharm[22] = (unsigned int  )0;
 sqlstm.sqadto[22] = (unsigned short )0;
 sqlstm.sqtdso[22] = (unsigned short )0;
 sqlstm.sqhstv[23] = (         void  *)ptrDBorden->ladodesorig;
 sqlstm.sqhstl[23] = (unsigned int  )2;
 sqlstm.sqhsts[23] = (         int  )0;
 sqlstm.sqindv[23] = (         void  *)&ptriDBorden->ladodesorig;
 sqlstm.sqinds[23] = (         int  )0;
 sqlstm.sqharm[23] = (unsigned int  )0;
 sqlstm.sqadto[23] = (unsigned short )0;
 sqlstm.sqtdso[23] = (unsigned short )0;
 sqlstm.sqhstv[24] = (         void  *)&ptrDBorden->ptocomact;
 sqlstm.sqhstl[24] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[24] = (         int  )0;
 sqlstm.sqindv[24] = (         void  *)&ptriDBorden->ptocomact;
 sqlstm.sqinds[24] = (         int  )0;
 sqlstm.sqharm[24] = (unsigned int  )0;
 sqlstm.sqadto[24] = (unsigned short )0;
 sqlstm.sqtdso[24] = (unsigned short )0;
 sqlstm.sqhstv[25] = (         void  *)&ptrDBorden->posact;
 sqlstm.sqhstl[25] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[25] = (         int  )0;
 sqlstm.sqindv[25] = (         void  *)&ptriDBorden->posact;
 sqlstm.sqinds[25] = (         int  )0;
 sqlstm.sqharm[25] = (unsigned int  )0;
 sqlstm.sqadto[25] = (unsigned short )0;
 sqlstm.sqtdso[25] = (unsigned short )0;
 sqlstm.sqhstv[26] = (         void  *)ptrDBorden->ladoact;
 sqlstm.sqhstl[26] = (unsigned int  )2;
 sqlstm.sqhsts[26] = (         int  )0;
 sqlstm.sqindv[26] = (         void  *)&ptriDBorden->ladoact;
 sqlstm.sqinds[26] = (         int  )0;
 sqlstm.sqharm[26] = (unsigned int  )0;
 sqlstm.sqadto[26] = (unsigned short )0;
 sqlstm.sqtdso[26] = (unsigned short )0;
 sqlstm.sqhstv[27] = (         void  *)ptrDBorden->tipord;
 sqlstm.sqhstl[27] = (unsigned int  )2;
 sqlstm.sqhsts[27] = (         int  )0;
 sqlstm.sqindv[27] = (         void  *)&ptriDBorden->tipord;
 sqlstm.sqinds[27] = (         int  )0;
 sqlstm.sqharm[27] = (unsigned int  )0;
 sqlstm.sqadto[27] = (unsigned short )0;
 sqlstm.sqtdso[27] = (unsigned short )0;
 sqlstm.sqhstv[28] = (         void  *)ptrDBorden->errord;
 sqlstm.sqhstl[28] = (unsigned int  )2;
 sqlstm.sqhsts[28] = (         int  )0;
 sqlstm.sqindv[28] = (         void  *)&ptriDBorden->errord;
 sqlstm.sqinds[28] = (         int  )0;
 sqlstm.sqharm[28] = (unsigned int  )0;
 sqlstm.sqadto[28] = (unsigned short )0;
 sqlstm.sqtdso[28] = (unsigned short )0;
 sqlstm.sqhstv[29] = (         void  *)ptrDBorden->mesa1;
 sqlstm.sqhstl[29] = (unsigned int  )2;
 sqlstm.sqhsts[29] = (         int  )0;
 sqlstm.sqindv[29] = (         void  *)&ptriDBorden->mesa1;
 sqlstm.sqinds[29] = (         int  )0;
 sqlstm.sqharm[29] = (unsigned int  )0;
 sqlstm.sqadto[29] = (unsigned short )0;
 sqlstm.sqtdso[29] = (unsigned short )0;
 sqlstm.sqhstv[30] = (         void  *)ptrDBorden->mesa2;
 sqlstm.sqhstl[30] = (unsigned int  )2;
 sqlstm.sqhsts[30] = (         int  )0;
 sqlstm.sqindv[30] = (         void  *)&ptriDBorden->mesa2;
 sqlstm.sqinds[30] = (         int  )0;
 sqlstm.sqharm[30] = (unsigned int  )0;
 sqlstm.sqadto[30] = (unsigned short )0;
 sqlstm.sqtdso[30] = (unsigned short )0;
 sqlstm.sqhstv[31] = (         void  *)ptrDBorden->simcarga;
 sqlstm.sqhstl[31] = (unsigned int  )2;
 sqlstm.sqhsts[31] = (         int  )0;
 sqlstm.sqindv[31] = (         void  *)&ptriDBorden->simcarga;
 sqlstm.sqinds[31] = (         int  )0;
 sqlstm.sqharm[31] = (unsigned int  )0;
 sqlstm.sqadto[31] = (unsigned short )0;
 sqlstm.sqtdso[31] = (unsigned short )0;
 sqlstm.sqhstv[32] = (         void  *)ptrDBorden->ordret;
 sqlstm.sqhstl[32] = (unsigned int  )2;
 sqlstm.sqhsts[32] = (         int  )0;
 sqlstm.sqindv[32] = (         void  *)&ptriDBorden->ordret;
 sqlstm.sqinds[32] = (         int  )0;
 sqlstm.sqharm[32] = (unsigned int  )0;
 sqlstm.sqadto[32] = (unsigned short )0;
 sqlstm.sqtdso[32] = (unsigned short )0;
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
 if (sqlca.sqlcode == 1403) goto GetOrdenNoHay;
 if (sqlca.sqlcode < 0) plsql_error("GetOrden: ",sqlca,oraca);
}



	// Michael 27.11.2001: Solo actualizar si es una orden nueva. Si no, es la
	// recuperación de órdenes que ya estaban asignadas

	if (NOTHING == estado)
	{
		dbEstado = NEWORDER;
		/* EXEC SQL UPDATE ORDTRAN
			set estado = :dbEstado
			where current of CURORDTRANNEW; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 33;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "update ORDTRAN  set estado=:b0 where rowid = :x";
  sqlstm.iters = (unsigned int  )13;
  sqlstm.offset = (unsigned int  )563;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&dbEstado;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )sizeof(int);
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
  if (sqlca.sqlcode == 1403) goto GetOrdenNoHay;
  if (sqlca.sqlcode < 0) plsql_error("GetOrden: ",sqlca,oraca);
}


		/* EXEC SQL COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 33;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )582;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
  if (sqlca.sqlcode < 0) plsql_error("GetOrden: ",sqlca,oraca);
}


	}

	/* EXEC SQL CLOSE CURORDTRANNEW; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 33;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )597;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetOrden: ",sqlca,oraca);
}



	// Michael 29.04.2015 wasn't initializing the order state
	ptrOrden->SetEstadoWithoutDB ((TEstadoOrden)dbEstado);

	TYP_AT_DAT at;
	
	memset (&at,0,sizeof(at));
	
	ptrOrden->m_at.herkunft     = ptrDBorden->tipord [0];	// tipo orden

	CAgv::SetZiel (&ptrOrden->m_at.hz,
		ptrDBorden->ptocomori,
		ptrDBorden->posori,
		ptrDBorden->ladoori [0],
		0);

	CAgv::SetZiel (&ptrOrden->m_at.bz,
		ptrDBorden->ptocomdes,
		ptrDBorden->posdes,
		ptrDBorden->ladodes [0],
		0);
	sprintf ((char *)ptrOrden->m_at.id_nr, "%*d", KMAXID_AT - 1, ptrDBorden->codord);



	// Revise all this initialization
	ptrOrden->m_at.af_status = 0;
	ptrOrden->m_at.akt_org = 0;	// Original Action
	ptrOrden->m_at.akt_z.nLayer = 0;
	ptrOrden->m_at.akt_z.ucSide = 0;
	ptrOrden->m_at.akt_z.nDest = 0;
	ptrOrden->m_at.akt_z.nPos = 0;
	ptrOrden->m_at.aktion = 1;
	ptrOrden->m_at.art = 2; // Not used
	ptrOrden->m_at.at_lauf_nr = 1;
	ptrOrden->m_at.bnr = 1;

	if (ptrDBorden->ordret [0] == 'S')
		ptrOrden->m_at.gesp_at = 1;	// Orden suspendida
	else
		ptrOrden->m_at.gesp_at = 0;	// Orden no suspendida
	ptrOrden->m_at.gesp_hz = 0;	// Collection suspended
	ptrOrden->m_at.gesp_bz = 0;	// Delivery suspended

	ptrOrden->m_at.fznr = ptrDBorden->agv;
	
	ptrOrden->m_at.fMesa1 = ptrDBorden->mesa1 [0] == 'S' ? true : false;
	ptrOrden->m_at.fMesa2 = ptrDBorden->mesa2 [0] == 'S' ? true : false;
	ptrOrden->m_at.fSimCarga = ptrDBorden->simcarga [0] == 'S' ? true : false;


	ptrOrden->m_at.prio_org = ptrDBorden->priini;
	ptrOrden->m_at.prio_akt = ptrDBorden->priact;

	// Get station information
	ptrOrden->m_ptrEstOri = new CDBEstacion;
	ptrOrden->m_ptrEstDes = new CDBEstacion;

	ptrOrden->m_ptrEstOri->GetEstAgv (ptrOrden->m_at.hz.nDest,
		ptrOrden->m_at.hz.nPos,
		ptrOrden->m_at.hz.ucSide);

	ptrOrden->m_ptrEstDes->GetEstAgv (ptrOrden->m_at.bz.nDest,
		ptrOrden->m_at.bz.nPos,
		ptrOrden->m_at.bz.ucSide);

	// Michael 2015 TODO Move to PC...
	strcpy((char *)ptrOrden->m_at.HzString, g_pCourseData->GetAIDStation(1, 
			ptrOrden->m_at.hz.nDest,
			ptrOrden->m_at.hz.ucSide,
			ptrOrden->m_at.hz.nPos,
			ptrOrden->m_at.hz.nLayer + 1).GetPSName().GetBuffer());

	strcpy((char *)ptrOrden->m_at.BzString, g_pCourseData->GetAIDStation(1, 
			ptrOrden->m_at.bz.nDest,
			ptrOrden->m_at.bz.ucSide,
			ptrOrden->m_at.bz.nPos,
			ptrOrden->m_at.bz.nLayer + 1).GetPSName().GetBuffer());
	



	delete ptrDBorden;
	delete ptriDBorden;

	return (ptrOrden);

		
GetOrdenNoHay:

		// No hay órdenes

	delete ptrDBorden;
	delete ptrOrden;

	return (NULL);
}


// Modificar una orden en la base de datos
void CDBGestorOrden::ModificarOrden(COrden *ptrOrden)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
		/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	T_sqlca sqlca;	// For this is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;

    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("ModificarOrden: ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 

	
	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbCodord;
	int dbPtocomdes;
	int dbPosdes;
	char dbLadodes [2];
	int dbPriact;

	/* EXEC SQL END DECLARE SECTION; */ 



	dbCodord = atoi ((char *)ptrOrden->m_at.id_nr); 
	dbPtocomdes = ptrOrden->m_at.bz.nDest;
	dbPosdes = ptrOrden->m_at.bz.nPos;
	sprintf (dbLadodes, "%c", ptrOrden->m_at.bz.ucSide);
	dbPriact = ptrOrden->m_at.prio_akt;

	if (dbPriact <= 0 || dbPriact > 5)
		dbPriact = 1;

	
	/* EXEC SQL EXECUTE
		declare bRetVal boolean;
		begin
		bRetVal := map.CanviarDestOrdTran (:dbCodord,
					:dbPtocomdes,
					:dbPosdes,
					:dbLadodes,
					:dbPriact);
		commit;
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 33;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "declare bRetVal boolean ; begin bRetVal := map . CanviarDest\
OrdTran ( :dbCodord , :dbPtocomdes , :dbPosdes , :dbLadodes , :dbPriact ) ; co\
mmit ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )612;
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
 sqlstm.sqhstv[1] = (         void  *)&dbPtocomdes;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&dbPosdes;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)dbLadodes;
 sqlstm.sqhstl[3] = (unsigned int  )2;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&dbPriact;
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
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
 if (sqlca.sqlcode < 0) plsql_error("ModificarOrden: ",sqlca,oraca);
}



}

// Resetear las ordenes que no han sido recogidas
void CDBGestorOrden::IniciarSistema()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	T_sqlca sqlca;	// For this is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;


    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("IniciarSistema (): ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL EXECUTE
		begin
			map.IniciarSistema ();
			commit;
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 33;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin map . IniciarSistema ( ) ; commit ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )647;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("IniciarSistema (): ",sqlca,oraca);
}




}


// Ver si se puede forzar ordenes. Llamado periodicamente
void CDBGestorOrden::ForzarOrden()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
		/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

	
	T_sqlca sqlca;	// For this is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;

	CSingleLock lockOrden (&g_Container.m_csForzarOrden);
	MSGTRACE ("ForzarOrden () Esperando lock...");
	lockOrden.Lock ();
	MSGTRACE ("ForzarOrden () Got lock...");

// Michael 19.06.2003    EXEC SQL WHENEVER SQLERROR DO plsql_error("ForzarOrden (): ", sqlca, oraca);
	/* EXEC SQL WHENEVER SQLERROR GOTO ForzarOrdenOraError; */ 


	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL EXECUTE
		begin
			map.ForzarOrden ();
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 33;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin map . ForzarOrden ( ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )662;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) goto ForzarOrdenOraError;
}



	lockOrden.Unlock ();

	return;

// Michael 19.06.2003
ForzarOrdenOraError:
 lockOrden.Unlock ();
 switch(sqlca.sqlcode) 
	{
	case -1400:	// Cannot insert null...
		MSGERROR ("UpdateEstadoOrden (): ForzarOrden insert null..");
		/* EXEC SQL WHENEVER SQLERROR CONTINUE; */ 

		/* EXEC SQL ROLLBACK; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 33;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )677;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
}


		break;
	default:	// otro error - se debe tratar
		MSGERROR1 ("ForzarOrden (): Oracle salca.sqlcode is: %d", sqlca.sqlcode);
		plsql_error("ForzarOrden: ",sqlca, oraca);
	}
}


// Devolver la prioridad actual de una orden
UCHAR CDBGestorOrden::GetPrioridadOrden(int codord)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
		/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

	T_sqlca sqlca;	// For this is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;

    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("GetPrioridadOrden: ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 

	
	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbCodord;
	int dbPrio;

	/* EXEC SQL END DECLARE SECTION; */ 


	dbCodord = codord;

	/* EXEC SQL EXECUTE
		begin
			:dbPrio := map.GetPrioridad (:dbCodord);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 33;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin :dbPrio := map . GetPrioridad ( :dbCodord ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )692;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&dbPrio;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbCodord;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
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
 if (sqlca.sqlcode < 0) plsql_error("GetPrioridadOrden: ",sqlca,oraca);
}



	return (dbPrio);

}


