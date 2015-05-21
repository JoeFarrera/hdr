
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
           char  filnam[31];
};
static const struct sqlcxp sqlfpn =
{
    30,
    "f:\\projects\\tpc\\dbestadoplc.pc"
};


static unsigned int sqlctx = 1138113229;


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
            void  *sqhstv[22];
   unsigned int   sqhstl[22];
            int   sqhsts[22];
            void  *sqindv[22];
            int   sqinds[22];
   unsigned int   sqharm[22];
   unsigned int   *sqharc[22];
   unsigned short  sqadto[22];
   unsigned short  sqtdso[22];
} sqlstm = {12,22};

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
5,0,0,1,246,0,4,109,0,0,22,0,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,
0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,
3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,2,3,0,0,
108,0,0,2,52,0,4,219,0,0,2,1,0,1,0,2,5,0,0,1,3,0,0,
131,0,0,3,0,0,24,230,0,0,1,1,0,1,0,1,5,0,0,
150,0,0,4,54,0,5,234,0,0,3,3,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,
177,0,0,5,0,0,29,241,0,0,0,0,0,1,0,
192,0,0,6,100,0,5,267,0,0,0,0,0,1,0,
207,0,0,7,0,0,29,285,0,0,0,0,0,1,0,
222,0,0,8,60,0,4,311,0,0,2,1,0,1,0,2,3,0,0,1,5,0,0,
245,0,0,9,35,0,5,332,0,0,2,2,0,1,0,1,5,0,0,1,3,0,0,
268,0,0,10,0,0,29,337,0,0,0,0,0,1,0,
283,0,0,11,40,0,4,362,0,0,2,0,0,1,0,2,3,0,0,2,3,0,0,
306,0,0,12,32,0,5,391,0,0,2,2,0,1,0,1,3,0,0,1,3,0,0,
329,0,0,13,0,0,29,396,0,0,0,0,0,1,0,
344,0,0,14,48,0,5,413,0,0,0,0,0,1,0,
359,0,0,15,0,0,29,417,0,0,0,0,0,1,0,
};


// DBEstadoPLC.cpp: implementation of the CDBEstadoPLC class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "tpc.h"
#include "DBEstadoPLC.h"
#include "SiemensSR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The ORACA=YES option must be specified to enable use of the ORACA
/* EXEC ORACLE OPTION (ORACA=YES); */ 


typedef char asciz[80];
/* EXEC SQL TYPE asciz IS STRING(80) REFERENCE; */ 



/* EXEC SQL BEGIN DECLARE SECTION; */ 

struct dbEstado
{
 int idestgen;
 int p1;
 int p2;
 int p31;
 int p32;
 int p5; 
 int p6; 
 int p8; 
 int p9;
 int c1; 
 int c2; 
 int c3; 
 int c4; 
 int c5; 
 int c6; 
 int c7; 
 int c8;
 // Michael Agv9 23.05.2007
 int c9; 
 // Michael Agv9 23.05.2007
 int a2;
 int desa2;
 // Michael 10.06.2002 Semáforos
 int s1;	
 int s2;
 // Michael 10.06.2002 Semáforos Fin
};
/* EXEC SQL END DECLARE SECTION; */ 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBEstadoPLC::CDBEstadoPLC()
{
	
    // Save text of SQL current statement in the ORACA if an error occurs.
    oraca.orastxtf = ORASTFERR;

}

CDBEstadoPLC::~CDBEstadoPLC()
{

}

// Enviar el bloque de estado al PLC
void CDBEstadoPLC::SendEstadoPLC()
{
	if (g_Container.IsSimulateEscribirPLC())
	{
		MSGWARNING ("Simulando escribir estado PLC");
		return;
	}


	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 
    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("SendEstadoPLC: ", sqlca, oraca); */ 


	/* EXEC SQL WHENEVER NOTFOUND GOTO SendEstadoPLCNotFound; */ 


	// Si no han sido leido los cargadores, no enviar estado
	if (! g_Container.IsActualizadoTodosCargadores ())
	{
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (6, 3, 0); 
		return;
	}

	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	
	struct dbEstado * ptrDBEstado;

	/* EXEC SQL END DECLARE SECTION; */ 


	BYTE * bMensaje = new BYTE [MAXESTADOSENDPLC];

	ptrDBEstado = new struct dbEstado;

	// Solo hay una fila en la tabla
	/* EXEC SQL SELECT IDESTGEN,
			 P1, 
			 P2,   
			 P31,  
			 P32,  
			 P5,   
			 P6,   
			 P8,   
			 P9,   
			 C1,   
			 C2,   
			 C3,   
			 C4,   
			 C5,   
			 C6,   
			 C7,   
			 C8, 
			 -- Michael Agv9 23.05.2007
			 C9,
			 // Michael Agv9 23.05.2007
			 A2,   
			 DESA2,
			 S1,
			 S2
		INTO :ptrDBEstado
		FROM ESTGEN; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select IDESTGEN ,P1 ,P2 ,P31 ,P32 ,P5 ,P6 ,P8 ,P9 ,C1 ,C2 ,C\
3 ,C4 ,C5 ,C6 ,C7 ,C8 ,C9 ,A2 ,DESA2 ,S1 ,S2 into :s1 ,:s2 ,:s3 ,:s4 ,:s5 ,:s6\
 ,:s7 ,:s8 ,:s9 ,:s10 ,:s11 ,:s12 ,:s13 ,:s14 ,:s15 ,:s16 ,:s17 ,:s18 ,:s19 ,:\
s20 ,:s21 ,:s22   from ESTGEN ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&ptrDBEstado->idestgen;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&ptrDBEstado->p1;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&ptrDBEstado->p2;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&ptrDBEstado->p31;
 sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&ptrDBEstado->p32;
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)&ptrDBEstado->p5;
 sqlstm.sqhstl[5] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)&ptrDBEstado->p6;
 sqlstm.sqhstl[6] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)&ptrDBEstado->p8;
 sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (         void  *)&ptrDBEstado->p9;
 sqlstm.sqhstl[8] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         void  *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned int  )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (         void  *)&ptrDBEstado->c1;
 sqlstm.sqhstl[9] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         void  *)0;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned int  )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (         void  *)&ptrDBEstado->c2;
 sqlstm.sqhstl[10] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         void  *)0;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned int  )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
 sqlstm.sqhstv[11] = (         void  *)&ptrDBEstado->c3;
 sqlstm.sqhstl[11] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[11] = (         int  )0;
 sqlstm.sqindv[11] = (         void  *)0;
 sqlstm.sqinds[11] = (         int  )0;
 sqlstm.sqharm[11] = (unsigned int  )0;
 sqlstm.sqadto[11] = (unsigned short )0;
 sqlstm.sqtdso[11] = (unsigned short )0;
 sqlstm.sqhstv[12] = (         void  *)&ptrDBEstado->c4;
 sqlstm.sqhstl[12] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[12] = (         int  )0;
 sqlstm.sqindv[12] = (         void  *)0;
 sqlstm.sqinds[12] = (         int  )0;
 sqlstm.sqharm[12] = (unsigned int  )0;
 sqlstm.sqadto[12] = (unsigned short )0;
 sqlstm.sqtdso[12] = (unsigned short )0;
 sqlstm.sqhstv[13] = (         void  *)&ptrDBEstado->c5;
 sqlstm.sqhstl[13] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[13] = (         int  )0;
 sqlstm.sqindv[13] = (         void  *)0;
 sqlstm.sqinds[13] = (         int  )0;
 sqlstm.sqharm[13] = (unsigned int  )0;
 sqlstm.sqadto[13] = (unsigned short )0;
 sqlstm.sqtdso[13] = (unsigned short )0;
 sqlstm.sqhstv[14] = (         void  *)&ptrDBEstado->c6;
 sqlstm.sqhstl[14] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[14] = (         int  )0;
 sqlstm.sqindv[14] = (         void  *)0;
 sqlstm.sqinds[14] = (         int  )0;
 sqlstm.sqharm[14] = (unsigned int  )0;
 sqlstm.sqadto[14] = (unsigned short )0;
 sqlstm.sqtdso[14] = (unsigned short )0;
 sqlstm.sqhstv[15] = (         void  *)&ptrDBEstado->c7;
 sqlstm.sqhstl[15] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[15] = (         int  )0;
 sqlstm.sqindv[15] = (         void  *)0;
 sqlstm.sqinds[15] = (         int  )0;
 sqlstm.sqharm[15] = (unsigned int  )0;
 sqlstm.sqadto[15] = (unsigned short )0;
 sqlstm.sqtdso[15] = (unsigned short )0;
 sqlstm.sqhstv[16] = (         void  *)&ptrDBEstado->c8;
 sqlstm.sqhstl[16] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[16] = (         int  )0;
 sqlstm.sqindv[16] = (         void  *)0;
 sqlstm.sqinds[16] = (         int  )0;
 sqlstm.sqharm[16] = (unsigned int  )0;
 sqlstm.sqadto[16] = (unsigned short )0;
 sqlstm.sqtdso[16] = (unsigned short )0;
 sqlstm.sqhstv[17] = (         void  *)&ptrDBEstado->c9;
 sqlstm.sqhstl[17] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[17] = (         int  )0;
 sqlstm.sqindv[17] = (         void  *)0;
 sqlstm.sqinds[17] = (         int  )0;
 sqlstm.sqharm[17] = (unsigned int  )0;
 sqlstm.sqadto[17] = (unsigned short )0;
 sqlstm.sqtdso[17] = (unsigned short )0;
 sqlstm.sqhstv[18] = (         void  *)&ptrDBEstado->a2;
 sqlstm.sqhstl[18] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[18] = (         int  )0;
 sqlstm.sqindv[18] = (         void  *)0;
 sqlstm.sqinds[18] = (         int  )0;
 sqlstm.sqharm[18] = (unsigned int  )0;
 sqlstm.sqadto[18] = (unsigned short )0;
 sqlstm.sqtdso[18] = (unsigned short )0;
 sqlstm.sqhstv[19] = (         void  *)&ptrDBEstado->desa2;
 sqlstm.sqhstl[19] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[19] = (         int  )0;
 sqlstm.sqindv[19] = (         void  *)0;
 sqlstm.sqinds[19] = (         int  )0;
 sqlstm.sqharm[19] = (unsigned int  )0;
 sqlstm.sqadto[19] = (unsigned short )0;
 sqlstm.sqtdso[19] = (unsigned short )0;
 sqlstm.sqhstv[20] = (         void  *)&ptrDBEstado->s1;
 sqlstm.sqhstl[20] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[20] = (         int  )0;
 sqlstm.sqindv[20] = (         void  *)0;
 sqlstm.sqinds[20] = (         int  )0;
 sqlstm.sqharm[20] = (unsigned int  )0;
 sqlstm.sqadto[20] = (unsigned short )0;
 sqlstm.sqtdso[20] = (unsigned short )0;
 sqlstm.sqhstv[21] = (         void  *)&ptrDBEstado->s2;
 sqlstm.sqhstl[21] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[21] = (         int  )0;
 sqlstm.sqindv[21] = (         void  *)0;
 sqlstm.sqinds[21] = (         int  )0;
 sqlstm.sqharm[21] = (unsigned int  )0;
 sqlstm.sqadto[21] = (unsigned short )0;
 sqlstm.sqtdso[21] = (unsigned short )0;
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
 if (sqlca.sqlcode == 1403) goto SendEstadoPLCNotFound;
 if (sqlca.sqlcode < 0) plsql_error("SendEstadoPLC: ",sqlca,oraca);
}

	


	bMensaje [0] = 'B';	// tipo telegrama
	bMensaje [1] = ptrDBEstado->p1;
	bMensaje [2] = ptrDBEstado->p2;
	bMensaje [3] = ptrDBEstado->p31;
	bMensaje [4] = ptrDBEstado->p32;
	bMensaje [5] = ptrDBEstado->p5;
	bMensaje [6] = ptrDBEstado->p6;
	bMensaje [7] = ptrDBEstado->p8;
	bMensaje [8] = ptrDBEstado->p9;
	bMensaje [9] = ptrDBEstado->c1;
	bMensaje [10] = ptrDBEstado->c2;
	bMensaje [11] = ptrDBEstado->c3;
	bMensaje [12] = ptrDBEstado->c4;
	bMensaje [13] = ptrDBEstado->c5;
	bMensaje [14] = ptrDBEstado->c6;
	bMensaje [15] = ptrDBEstado->c7;
	bMensaje [16] = ptrDBEstado->c8;
	bMensaje [17] = ptrDBEstado->a2 & 0xFF0000;
	bMensaje [18] = ptrDBEstado->a2 & 0xFF00;
	bMensaje [19] = ptrDBEstado->a2 & 0x00FF;
	bMensaje [20] = ptrDBEstado->desa2;
	// Michael 10.06.2002
	bMensaje [21] = ptrDBEstado->s1;
	bMensaje [22] = ptrDBEstado->s2;
	// Michael 10.06.2002 fin
	// Michael 23.05.2007 AGV9
	bMensaje [23] = ptrDBEstado->c9;
	// Michael 23.05.2007 AGV9


	

    g_Container.m_pMainWnd->PostMessage (WM_USER_SIEMENS_SEND, 0, (long)bMensaje);
	
	return;

SendEstadoPLCNotFound:

	MSGERROR ("SendEstadoPLC (): Estado no encontrado");



}



// Activar o desactivar el cargador de bateria
void CDBEstadoPLC::SwitchCargador(UINT nAgv, UINT ptoCom, UINT wegstr, bool setOn)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 
	LPTSTR ptrActPos = "This is the start allocating space";

	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("SwitchOnCargador () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO SwitchOnCargadorNotFound; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbPtoCom;
	int dbAgv;
	asciz dbUpdateStmt;
	char dbCodCar [2 + 1];
	int dbWegstr;

	/* EXEC SQL END DECLARE SECTION; */ 


	dbPtoCom = ptoCom;
	if (setOn)
	{
		dbAgv = nAgv;
		dbWegstr = wegstr;
	}
	else
	{
		dbAgv = 0;	
		dbWegstr = 0;
	}

	ptrActPos = "Select codcar";


	/* EXEC SQL SELECT 
		codcar INTO :dbCodCar
		FROM CARAGV
		WHERE ptocom = :dbPtoCom; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select codcar into :b0  from CARAGV where ptocom=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )108;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)dbCodCar;
 sqlstm.sqhstl[0] = (unsigned int  )3;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbPtoCom;
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
 if (sqlca.sqlcode == 1403) goto SwitchOnCargadorNotFound;
 if (sqlca.sqlcode < 0) plsql_error("SwitchOnCargador () : ",sqlca,oraca);
}




	ptrActPos = "Update Estgen...";

	sprintf ((char *)dbUpdateStmt, 
		"UPDATE ESTGEN SET C%s = %d", dbCodCar, setOn ? 1 : 0);

	/* EXEC SQL EXECUTE IMMEDIATE :dbUpdateStmt; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )131;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)dbUpdateStmt;
 sqlstm.sqhstl[0] = (unsigned int  )80;
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
 if (sqlca.sqlcode == 1403) goto SwitchOnCargadorNotFound;
 if (sqlca.sqlcode < 0) plsql_error("SwitchOnCargador () : ",sqlca,oraca);
}



	// Actualiza el Agv como en el cargador o no
	ptrActPos = "Update Caragv...";
	/* EXEC SQL UPDATE CARAGV
		SET AGV = :dbAgv,
			WEGSTR = :dbWegstr
		WHERE PTOCOM = :dbPtoCom; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update CARAGV  set AGV=:b0,WEGSTR=:b1 where PTOCOM=:b2";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )150;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&dbAgv;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbWegstr;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&dbPtoCom;
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
 if (sqlca.sqlcode == 1403) goto SwitchOnCargadorNotFound;
 if (sqlca.sqlcode < 0) plsql_error("SwitchOnCargador () : ",sqlca,oraca);
}



	ptrActPos = "Update Caragv finished...";

	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )177;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("SwitchOnCargador () : ",sqlca,oraca);
}



	SendEstadoPLC ();

	return;

SwitchOnCargadorNotFound:

	CString strTemp;
	strTemp.Format ("Cargador del punto %d no encontrado en statement: %s",
		ptoCom, 
		ptrActPos);
	AfxMessageBox (strTemp);

}



// Resetear sentidos de pulmones y cargadores al arrancar
void CDBEstadoPLC::LimpiarEstadoPLC()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("LimpiarEstadoPLC () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL UPDATE ESTGEN 
		 SET P1 = 0,      
		 P2 = 0,      
		 P31 = 0,     
		 P32 = 0,     
		 P5 = 0,     
		 P6 = 0,     
		 P8 = 0,     
		 P9 = 0,     
		 C1 = 0,     
		 C2 = 0,     
		 C3 = 0,     
		 C4 = 0,     
		 C5 = 0,     
		 C6 = 0,     
		 C7 = 0,     
		 C8 = 0; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update ESTGEN  set P1=0,P2=0,P31=0,P32=0,P5=0,P6=0,P8=0,P9=0\
,C1=0,C2=0,C3=0,C4=0,C5=0,C6=0,C7=0,C8=0";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )192;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("LimpiarEstadoPLC () : ",sqlca,oraca);
}



	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )207;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("LimpiarEstadoPLC () : ",sqlca,oraca);
}



	return;


}



void CDBEstadoPLC::SetDestinoA2(LPCTSTR codord)
{
	
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("SetDestinoA2 () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO SetDestinoA2NotFound; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	const char * dbCodord;
	int dbPtoCom;

	/* EXEC SQL END DECLARE SECTION; */ 


	dbCodord = codord;

	/* EXEC SQL SELECT PTOCOMDESORIG
		INTO :dbPtoCom
		FROM ORDTRAN
		WHERE CODORD = :dbCodord; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select PTOCOMDESORIG into :b0  from ORDTRAN where CODORD=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )222;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&dbPtoCom;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)dbCodord;
 sqlstm.sqhstl[1] = (unsigned int  )0;
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
 if (sqlca.sqlcode == 1403) goto SetDestinoA2NotFound;
 if (sqlca.sqlcode < 0) plsql_error("SetDestinoA2 () : ",sqlca,oraca);
}



	if (false)
	{
	SetDestinoA2NotFound:
		dbPtoCom = 1;	// Enviamos a 1 si no existe
	}
		

	if (dbPtoCom != 3 && dbPtoCom != 1)
	{
		MSGERROR1 ("Destino en A2 es: %d", dbPtoCom);
		dbPtoCom = 1;
	}

	// 0: A1 1: A3 
	dbPtoCom = dbPtoCom == 1 ? 0 : 1;

		// Michael 8.07.2013 pulmon 1 - not possible
	dbPtoCom = 1; // Following above logic

	
	/* EXEC SQL UPDATE ESTGEN
		SET A2 = :dbCodord,
		DESA2 = :dbPtoCom; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update ESTGEN  set A2=:b0,DESA2=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )245;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)dbCodord;
 sqlstm.sqhstl[0] = (unsigned int  )0;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbPtoCom;
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
 if (sqlca.sqlcode == 1403) goto SetDestinoA2NotFound;
 if (sqlca.sqlcode < 0) plsql_error("SetDestinoA2 () : ",sqlca,oraca);
}




	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )268;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("SetDestinoA2 () : ",sqlca,oraca);
}



	SendEstadoPLC ();

}


// Devolver el estado actual de los semáforos
void CDBEstadoPLC::GetEstadoSemaforo(int * s1, int * s2)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("GetEstadoSemaforo () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbs1;
	int dbs2;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	dbs1 = 0;
	dbs2 = 0;

	/* EXEC SQL 
		SELECT s1, s2 
		INTO :dbs1, :dbs2
		FROM ESTGEN; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "select s1 ,s2 into :b0,:b1  from ESTGEN ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )283;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&dbs1;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbs2;
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
 if (sqlca.sqlcode < 0) plsql_error("GetEstadoSemaforo () : ",sqlca,oraca);
}



	*s1 = dbs1;
	*s2 = dbs2;

}

void CDBEstadoPLC::SetEstadoSemaforo(int s1, int s2)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("SetEstadoSemaforo () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbs1;
	int dbs2;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	dbs1 = s1;
	dbs2 = s2;

	MSGAVISO2 ("Actualizando semaforo S1: %d S2: %d", s1, s2);

	/* EXEC SQL 
	UPDATE ESTGEN
	SET S1 = :dbs1,
		S2 = :dbs2; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update ESTGEN  set S1=:b0,S2=:b1";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )306;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&dbs1;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbs2;
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
 if (sqlca.sqlcode < 0) plsql_error("SetEstadoSemaforo () : ",sqlca,oraca);
}



	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )329;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("SetEstadoSemaforo () : ",sqlca,oraca);
}



	SendEstadoPLC ();

}



void CDBEstadoPLC::UpdateInstanceTimestamp()
{
	// Michael Mods 2009 - no precompilado
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

 	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("UpdateInstanceTimestamp () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 



	/* EXEC SQL 
	UPDATE InstanceTimeStamp
		set lastUpdate = sysdate; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update InstanceTimeStamp  set lastUpdate=sysdate";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )344;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("UpdateInstanceTimestamp () : ",sqlca,oraca);
}



	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 22;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )359;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("UpdateInstanceTimestamp () : ",sqlca,oraca);
}




}
