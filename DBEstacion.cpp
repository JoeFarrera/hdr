
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
           char  filnam[30];
};
static const struct sqlcxp sqlfpn =
{
    29,
    "f:\\projects\\tpc\\dbestacion.pc"
};


static unsigned int sqlctx = 569056933;


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
            void  *sqhstv[26];
   unsigned int   sqhstl[26];
            int   sqhsts[26];
            void  *sqindv[26];
            int   sqinds[26];
   unsigned int   sqharm[26];
   unsigned int   *sqharc[26];
   unsigned short  sqadto[26];
   unsigned short  sqtdso[26];
} sqlstm = {12,26};

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

 static const char *sq0002 = 
"select *  from ESTAGV where CODESTPUL=(select CODESTPUL  from ESTAGV where (\
(PTOCOM=:b0 and POSICION=:b1) and LADO=:b2)) order by POSICION asc            \
 ";

 static const char *sq0003 = 
"select *  from VCONEST where CODESTPUL=:b0           ";

 static const char *sq0004 = 
"select CODPULVAC  from ASIGPULVAC where CODEST=:b0           ";

 static const char *sq0011 = 
"select *  from ESTAGV where CODESTPUL=:b0 order by posicion asc             ";

 static const char *sq0012 = 
"select *  from VCONEST where CODESTPUL=:b0           ";

 static const char *sq0015 = 
"select ptocom  from CARAGV where ((((agv=0 and bit0=1) and bit1=0) and bit2=\
1) and ptocom=:b0)           ";

 static const char *sq0016 = 
"select ptocom  from CARAGV where (((agv=0 and bit0=1) and bit1=0) and bit2=1\
) order by PRIORIDAD asc             ";

 static const char *sq0017 = 
"select bit2 ,bit3  from CARAGV where ptocom=:b0           ";

 static const char *sq0022 = 
"select ptocom ,agv ,wegstr  from caragv where agv<>0           ";

 static const char *sq0024 = 
"select codestpul ,ptocom ,posicion ,lado ,tipmesa ,senagv ,numagvdes ,trunc(\
(espacio* 10))  from vpuntodescanso            ";

 static const char *sq0025 = 
"select nvl(sum(nvl(descartest.numagvdes,estacion.numagvdes)),0)  from tren ,\
estacion ,descartest where ((((tren.codtren=estacion.codtren and estacion.code\
stpul=descartest.codestpul(+)) and estacion.codart=descartest.codart(+)) and e\
stacion.estest<>'P') and tren.codtren=(select codtren  from estacion where cod\
estpul=:b0))           ";

 static const char *sq0026 = 
"select nvl(sum(nvl(descartest.numagvdes,estacion.numagvdes)),0)  from tren ,\
estacion ,descartest where ((((tren.codtren=estacion.codtren and estacion.code\
stpul=descartest.codestpul(+)) and estacion.codart=descartest.codart(+)) and t\
ren.estado='H') and tren.codtren in (select distinct codtren  from estacion wh\
ere estest='P'))           ";

 static const char *sq0029 = 
"select ptocom  from CARAGV  order by PRIORIDAD asc             ";

 static const char *sq0030 = 
"select estagv.ptocom ,estagv.posicion ,estagv.lado ,estacion.codestdes ,esta\
cion.numagvdes  from ESTAGV ,ESTACION where ESTAGV.CODESTPUL=ESTACION.CODESTPU\
L           ";

typedef struct { unsigned short len; unsigned char arr[1]; } VARCHAR;
typedef struct { unsigned short len; unsigned char arr[1]; } varchar;

/* cud (compilation unit data) array */
static const short sqlcud0[] =
{12,4130,178,0,0,
5,0,0,1,468,0,6,288,0,0,5,5,0,1,0,3,9,0,0,1,3,0,0,1,3,0,0,1,1,0,0,1,9,0,0,
40,0,0,2,155,0,9,404,0,0,3,3,0,1,0,1,3,0,0,1,3,0,0,1,1,0,0,
67,0,0,2,0,0,13,406,0,0,11,0,0,1,0,2,5,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,5,
0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
126,0,0,3,53,0,9,412,0,0,1,1,0,1,0,1,5,0,0,
145,0,0,3,0,0,13,414,0,0,26,0,0,1,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,
5,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,
2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,5,0,
0,2,3,0,0,
264,0,0,3,0,0,15,432,0,0,0,0,0,1,0,
279,0,0,2,0,0,13,442,0,0,11,0,0,1,0,2,5,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,
5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
338,0,0,4,61,0,9,455,0,0,1,1,0,1,0,1,5,0,0,
357,0,0,4,0,0,13,457,0,0,1,0,0,1,0,2,5,0,0,
376,0,0,4,0,0,15,463,0,0,0,0,0,1,0,
391,0,0,2,0,0,15,465,0,0,0,0,0,1,0,
406,0,0,5,72,0,5,573,0,0,4,4,0,1,0,1,5,0,0,1,3,0,0,1,5,0,0,1,5,0,0,
437,0,0,6,0,0,29,580,0,0,0,0,0,1,0,
452,0,0,7,51,0,5,586,0,0,2,2,0,1,0,1,3,0,0,1,5,0,0,
475,0,0,8,0,0,29,590,0,0,0,0,0,1,0,
490,0,0,9,83,0,5,668,0,0,5,5,0,1,0,1,5,0,0,1,5,0,0,1,3,0,0,1,5,0,0,1,5,0,0,
525,0,0,10,0,0,29,677,0,0,0,0,0,1,0,
540,0,0,11,76,0,9,767,0,0,1,1,0,1,0,1,5,0,0,
559,0,0,11,0,0,13,769,0,0,11,0,0,1,0,2,5,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,
5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
618,0,0,12,53,0,9,775,0,0,1,1,0,1,0,1,5,0,0,
637,0,0,12,0,0,13,777,0,0,26,0,0,1,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,
5,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,
2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,3,0,0,2,5,0,
0,2,3,0,0,
756,0,0,12,0,0,15,788,0,0,0,0,0,1,0,
771,0,0,11,0,0,13,797,0,0,11,0,0,1,0,2,5,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,3,0,0,2,
5,0,0,2,5,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,5,0,0,
830,0,0,11,0,0,15,802,0,0,0,0,0,1,0,
845,0,0,13,98,0,5,851,0,0,6,5,0,1,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,1,3,0,0,2,
5,0,0,
884,0,0,14,0,0,29,860,0,0,0,0,0,1,0,
899,0,0,15,105,0,9,918,0,0,1,1,0,1,0,1,3,0,0,
918,0,0,15,0,0,13,920,0,0,1,0,0,1,0,2,3,0,0,
937,0,0,16,113,0,9,932,0,0,0,0,0,1,0,
952,0,0,16,0,0,13,936,0,0,1,0,0,1,0,2,3,0,0,
971,0,0,16,0,0,15,947,0,0,0,0,0,1,0,
986,0,0,15,0,0,15,952,0,0,0,0,0,1,0,
1001,0,0,15,0,0,15,958,0,0,0,0,0,1,0,
1016,0,0,16,0,0,15,959,0,0,0,0,0,1,0,
1031,0,0,17,58,0,9,995,0,0,1,1,0,1,0,1,3,0,0,
1050,0,0,17,0,0,13,997,0,0,2,0,0,1,0,2,3,0,0,2,3,0,0,
1073,0,0,17,0,0,15,999,0,0,0,0,0,1,0,
1088,0,0,17,0,0,15,1008,0,0,0,0,0,1,0,
1103,0,0,18,74,0,6,1049,0,0,2,2,0,1,0,2,3,0,0,1,5,0,0,
1126,0,0,19,76,0,6,1057,0,0,2,2,0,1,0,2,3,0,0,1,5,0,0,
1149,0,0,20,74,0,6,1098,0,0,2,2,0,1,0,2,3,0,0,1,5,0,0,
1172,0,0,21,76,0,6,1106,0,0,2,2,0,1,0,2,3,0,0,1,5,0,0,
1195,0,0,22,63,0,9,1150,0,0,0,0,0,1,0,
1210,0,0,22,0,0,13,1156,0,0,3,0,0,1,0,2,3,0,0,2,3,0,0,2,3,0,0,
1237,0,0,22,0,0,15,1172,0,0,0,0,0,1,0,
1252,0,0,23,78,0,6,1196,0,0,3,3,0,1,0,2,3,0,0,1,5,0,0,1,5,0,0,
1279,0,0,24,123,0,9,1291,0,0,0,0,0,1,0,
1294,0,0,24,0,0,13,1295,0,0,8,0,0,1,0,2,5,0,0,2,3,0,0,2,3,0,0,2,5,0,0,2,5,0,0,
2,5,0,0,2,3,0,0,2,3,0,0,
1341,0,0,25,333,0,9,1305,0,0,1,1,0,1,0,1,5,0,0,
1360,0,0,25,0,0,13,1308,0,0,1,0,0,1,0,2,3,0,0,
1379,0,0,25,0,0,15,1311,0,0,0,0,0,1,0,
1394,0,0,26,337,0,9,1333,0,0,0,0,0,1,0,
1409,0,0,26,0,0,13,1334,0,0,1,0,0,1,0,2,3,0,0,
1428,0,0,26,0,0,15,1335,0,0,0,0,0,1,0,
1443,0,0,24,0,0,15,1339,0,0,0,0,0,1,0,
1458,0,0,27,73,0,6,1369,0,0,3,3,0,1,0,2,3,0,0,1,5,0,0,1,5,0,0,
1485,0,0,28,70,0,6,1399,0,0,3,3,0,1,0,2,3,0,0,1,5,0,0,1,5,0,0,
1512,0,0,29,63,0,9,1435,0,0,0,0,0,1,0,
1527,0,0,29,0,0,13,1440,0,0,1,0,0,1,0,2,3,0,0,
1546,0,0,29,0,0,15,1446,0,0,0,0,0,1,0,
1561,0,0,30,166,0,9,1482,0,0,0,0,0,1,0,
1576,0,0,30,0,0,13,1486,0,0,5,0,0,1,0,2,3,0,0,2,3,0,0,2,5,0,0,2,5,0,0,2,3,0,0,
1611,0,0,30,0,0,15,1506,0,0,0,0,0,1,0,
1626,0,0,31,385,0,3,1530,0,0,1,1,0,1,0,1,5,0,0,
1645,0,0,32,0,0,29,1570,0,0,0,0,0,1,0,
};


// DBEstacion.cpp: implementation of the CDBEstacion class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "tpc.h"
#include "DBEstacion.h"
#include "Agv.h"
#include "GestorOrden.h"
#include "GestorAgv.h"
#include "PuntoDescanso.h"
#include "DBEstadistica.h"


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



// Cambiar si TEstAgv cambia
struct dbEstAgv
{
 char codclind [8];
 int ptocom;       
 int posicion;     
 char lado [2];    
 int levelest;     
 char prepal [2];  
 char mesaop [2];  
 int errmesa;      
 char finprod [2]; 
 char tipest [2];  
 char codestpul [8];
};

// Cambiar si TEstAgv cambia
struct idbEstAgv
{
 short codclind;
 short ptocom;       
 short posicion;     
 short lado;    
 short levelest;     
 short prepal;  
 short mesaop;  
 short errmesa;      
 short finprod; 
 short tipest;  
 short codestpul;
};



// Cambiar si TEstacion cambia
struct dbEstacion
{
			char codestpul [7 + 1];
            char descrip [40 + 1];
            char tipestpul [1 + 1];
            char senper [1 + 1];
            char tipmesa [1 + 1];
            char tippal [1 + 1];
            char tippalenv [1 + 1];
            int cappul;
            int puncriped;
            char codtren [3 + 1];
            char estest [1 + 1];
            char estpul [1 + 1];
            char codart [10 + 1];
            int palped; 
            char finsum [1 + 1];
            char sumcon [1 + 1];
            char codpullno [7 + 1];
            char evacuar [1 + 1];
		    char automan [1 + 1];
            char intcamb [1 + 1];
            int ocupul;
            char senact [1 + 1];
            int palsum;
            int resagv;
            char senagv [1 + 1];
            int palpedrad;

};
struct idbEstacion
{
		short codestpul;
        short descrip;
        short tipestpul;
        short senper;
        short tipmesa;
        short tippal;
        short tippalenv;
        short cappul;
        short puncriped;
        short codtren;
        short estest;
        short estpul;
        short codart;
        short palped; 
        short finsum;
        short sumcon;
        short codpullno;
        short evacuar;
		short automan;
        short shortcamb;
        short ocupul;
        short senact;
        short palsum;
        short resagv;
        short senagv;
        short palpedrad;
};

struct dbAsigPulVac
{
 char codpulvac [8];
};
struct idbAsigPulVac
{
		short codpulvac;
};


// struct dbEstAgv * ptrDBEstAgv;
// struct dbEstacion * ptrDBEstacion;
// struct idbEstacion * ptriDBEstacion;
/* EXEC SQL END DECLARE SECTION; */ 



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBEstacion::CDBEstacion()
{
	m_EstadoMesa2 = 0;
	m_EstadoMesa1 = 0;
	m_EstadoEstacion = 0;
	m_CapPulmon = 0;
	m_OcuPulmon = 0;
	// Afegit xavi, 09/06/05: Nous camps que apareixeran en el missatge del bub

	// Fi afegit xavi, 09/06/05: Nous camps que apareixeran en el missatge del bub
	m_pEstAgv1 = NULL;
	m_pEstAgv2 = NULL;
	m_pEstacion = NULL;
	m_fGotDatos = false;
	oraca.orastxtf = ORASTFERR;

}

CDBEstacion::~CDBEstacion()
{
	if (m_pEstAgv1)
		delete m_pEstAgv1;
	if (m_pEstAgv2)
		delete m_pEstAgv2;
	if (m_pEstacion)
		delete m_pEstacion;
}

/*
void CDBEstacion::InsertarEstacion(UINT index)
{
	if (! g_Container.m_dbutil.IsConnected ())
		{
		MSGWARNING ("InsertarEstacion ()-> No hay conexión a Oracle");
		return;
		}
		
	int pos;
	CString strCodest;
	unsigned char tipEstIndumat;
	char tipEstacion;	// E: Entrada S: Salida A: Entrada o Salida

	CCourseAGVType* pCourse = g_pCourseData->GetPtrCourseAGVType(1);
	CAGVTDestination* pDestination = pCourse->GetPtrDestinationByIndex(index);

		// hay que descifrar la estación
		//for (pos = 0; pos < statdat [index].pos_anz_l; pos++)
		for (pos = 0; pos < pDestination->GetLeftPositions(); pos++)
		{
			//strCodest = (LPTSTR)&kn_tab [statdat [index].ziel_index_l + pos];
			strCodest = (LPTSTR)&kn_tab[pDestination->GetFirstDestPosIndex('L') + pos];
			//tipEstIndumat = e_typ_tab[z_typ_tab[zt_ver_tab[statdat [index].zieltyp_l]].index].ziel_art;
			//tipEstIndumat = e_typ_tab[z_typ_tab[zt_ver_tab[pDestination->GetDestTypeLeft()]].index].ziel_art;
			tipEstIndumat = g_pCourseData->GetLayerTypeProperties(pDestination->GetDestTypeLeft(), pos);

			if (tipEstIndumat & (HOL + BRI))
				tipEstacion = 'A';
			else if (tipEstIndumat & HOL)
				tipEstacion = 'S';
			else if (tipEstIndumat & BRI)
				tipEstacion = 'E';
			else tipEstacion  = 'N';	// No es una estación de interface ...

			//if (statdat [index].pos_anz_l > 1)
			if (pDestination->GetLeftPositions() > 1)
			{
				int ind = strCodest.ReverseFind ('.');
				if (ind != -1)
					strCodest = strCodest.Left (ind);
			}

			if (tipEstacion != 'N')
				InsertOrUpdateEstacionAgv(index,
				//statdat [index].mp,
				pDestination->GetNumber(),
				pos + 1,
				'L',
				(LPCTSTR)strCodest,
				//(LPTSTR)&kn_tab [statdat [index].ziel_index_l + pos]);
				(LPTSTR)&kn_tab[pDestination->GetFirstDestPosIndex('L') + pos]);
		}

		//for (pos = 0; pos < statdat [index].pos_anz_r; pos++)
		for (pos = 0; pos < pDestination->GetRightPositions(); pos++)
		{
			//strCodest = (LPTSTR)&kn_tab [statdat [index].ziel_index_r + pos];
			strCodest = (LPTSTR)&kn_tab[pDestination->GetFirstDestPosIndex('R') + pos];
			//tipEstIndumat = e_typ_tab[z_typ_tab[zt_ver_tab[statdat[index].zieltyp_r]].index].ziel_art;
			//tipEstIndumat = e_typ_tab[z_typ_tab[zt_ver_tab[pDestination->GetDestTypeRight()]].index].ziel_art;
			tipEstIndumat = g_pCourseData->GetLayerTypeProperties(pDestination->GetDestTypeRight(), pos);

			if (tipEstIndumat & HOL + BRI)
				tipEstacion = 'A';
			else if (tipEstIndumat & HOL)
				tipEstacion = 'S';
			else if (tipEstIndumat & BRI)
				tipEstacion = 'E';
			else tipEstacion  = 'N';	// No es una estación de interface ...

			//if (statdat [index].pos_anz_r > 1)
			if (pDestination->GetRightPositions() > 1)
			{
				int ind = strCodest.ReverseFind ('.');
				if (ind != -1)
					strCodest = strCodest.Left (ind);
			}

			if (tipEstacion != 'N')
				InsertOrUpdateEstacionAgv(index,
				//statdat [index].mp,
				pDestination->GetNumber(),
				pos + 1,
				'R',
				(LPCTSTR)strCodest,
				//(LPTSTR)&kn_tab [statdat [index].ziel_index_r + pos]);
				(LPTSTR)&kn_tab[pDestination->GetFirstDestPosIndex('R') + pos]);
		}

}
*/

void CDBEstacion::InsertOrUpdateEstacionAgv(int index,
											int ptocom,
											int posicion,
											char lado,
											LPCTSTR ptrCodest,
											LPCTSTR ptrCodestInd)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	/* VARCHAR dbCodclind [8]; */ 
struct { unsigned short len; unsigned char arr[8]; } dbCodclind;

	int dbPtocom;
	int dbPosicion;
	char dbLado;
	/* VARCHAR dbCodestpul [8]; */ 
struct { unsigned short len; unsigned char arr[8]; } dbCodestpul;



	/* EXEC SQL END DECLARE SECTION; */ 

  
    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("InsertOrUpdateEstacionAgv: ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 

	

	CString strTemp = ptrCodest;
	strTemp.TrimRight ();

	strcpy ((char *)dbCodestpul.arr, (LPCTSTR)strTemp); 
	dbCodestpul.len = strlen ((char *)dbCodestpul.arr);
	dbPtocom = ptocom;
	dbPosicion = posicion;
	dbLado = lado;

	strTemp = ptrCodestInd;
	strTemp.TrimRight ();
	strcpy ((char *)dbCodclind.arr, (LPCTSTR)strTemp); 
	dbCodclind.len = strlen ((char *)dbCodclind.arr);



	/* EXEC SQL EXECUTE
	begin
		declare
		cursor curestagv is
		select codclind
		from estagv
		where codclind = :dbCodclind;
		begin
			open curestagv;
			fetch curestagv into :dbCodclind;
		if (curestagv%notfound) then
	       INSERT INTO ESTAGV
            (CODCLIND
            ,PTOCOM
            ,POSICION
            ,LADO
            ,LEVELEST
            ,PREPAL
            ,MESAOP
            ,ERRMESA
            ,FINPROD
            ,TIPEST
            ,CODESTPUL)
			values
			(:dbCodclind,
			:dbPtocom,
			:dbPosicion,
			:dbLado,
			0,	-- level (no usado en HdR)
			'N',	-- prepal
			'S',	-- MesaOp
			0,	-- ErrMesa
			'N',	-- FinProd
			'E',	-- TipEst
			:dbCodestpul);

		COMMIT;
		end if;
		close curestagv;
		end;
		end;
		END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 5;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin declare cursor curestagv is select codclind from estag\
v where codclind = :dbCodclind ; begin open curestagv ; fetch curestagv into :\
dbCodclind ; if ( curestagv % notfound ) then INSERT INTO ESTAGV ( CODCLIND , \
PTOCOM , POSICION , LADO , LEVELEST , PREPAL , MESAOP , ERRMESA , FINPROD , TI\
PEST , CODESTPUL ) values ( :dbCodclind , :dbPtocom , :dbPosicion , :dbLado , \
0 , 'N' , 'S' , 0 , 'N' , 'E' , :dbCodestpul ) ; COMMIT ; end if ; close cures\
tagv ; end ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )5;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)&dbCodclind;
 sqlstm.sqhstl[0] = (unsigned int  )10;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbPtocom;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&dbPosicion;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&dbLado;
 sqlstm.sqhstl[3] = (unsigned int  )1;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&dbCodestpul;
 sqlstm.sqhstl[4] = (unsigned int  )10;
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
 if (sqlca.sqlcode < 0) plsql_error("InsertOrUpdateEstacionAgv: ",sqlca,oraca);
}



}


// Get datos de la estación AGV identificado por su posicion Indumat
bool CDBEstacion::GetEstAgv(int ptocom, int posicion, int lado)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	if (! g_Container.m_dbutil.IsConnected ())
		{
		MSGWARNING ("GetEstAgv ()-> No hay conexión a Oracle");
		return (false);
		}
		

	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	struct idbEstacion * ptriDBEstacion;
	struct dbEstAgv * ptrDBEstAgv;
	struct idbEstAgv * ptriDBEstAgv;
	struct dbEstacion * ptrDBEstacion;
	struct idbAsigPulVac * ptriDBAsigPulVac;
	struct dbAsigPulVac * ptrDBAsigPulVac;
	int dbPtocom;
	int dbPosicion;
	char dbLado;


	/* EXEC SQL END DECLARE SECTION; */ 


	ptrDBEstAgv = new struct dbEstAgv; 
	ptrDBEstacion = new struct dbEstacion;
	ptrDBAsigPulVac = new struct dbAsigPulVac;
	

    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("GetEstAgv: ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO GetEstacionNotFound; */ 


	// Interesan las dos mesas....
	/*
	switch (posicion)
	{
		case 2:
		case 4:
			posicion = posicion - 1; // First !!!
	}
	*/
	dbLado = lado;	// Michael Was: == 1 ? 'L' : 'R';
	dbPtocom = ptocom;
	dbPosicion = posicion;


	// Si es doble, interesan las dos mesas
	/* EXEC SQL DECLARE CURESTAGV CURSOR FOR
		SELECT * FROM ESTAGV
		WHERE CODESTPUL = 
			(SELECT CODESTPUL
			  FROM ESTAGV WHERE 
			  PTOCOM = :dbPtocom
			  AND POSICION = :dbPosicion
			  AND LADO = :dbLado)
		ORDER BY POSICION ASC; */ 

	
	/* EXEC SQL DECLARE CURESTACION CURSOR FOR
		SELECT * FROM VCONEST
		WHERE CODESTPUL = :ptrDBEstAgv->codestpul; */ 
	  

	/* EXEC SQL DECLARE CURPULVAC CURSOR FOR
		SELECT CODPULVAC
		FROM ASIGPULVAC
		WHERE CODEST = :ptrDBEstAgv->codestpul; */ 


	/* EXEC SQL OPEN CURESTAGV; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 5;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0002;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )40;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (         void  *)&dbPtocom;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbPosicion;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&dbLado;
 sqlstm.sqhstl[2] = (unsigned int  )1;
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
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv: ",sqlca,oraca);
}



	/* EXEC SQL FETCH CURESTAGV INTO :ptrDBEstAgv; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 11;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )67;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqfoff = (           int )0;
 sqlstm.sqfmod = (unsigned int )2;
 sqlstm.sqhstv[0] = (         void  *)ptrDBEstAgv->codclind;
 sqlstm.sqhstl[0] = (unsigned int  )8;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&ptrDBEstAgv->ptocom;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&ptrDBEstAgv->posicion;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)ptrDBEstAgv->lado;
 sqlstm.sqhstl[3] = (unsigned int  )2;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&ptrDBEstAgv->levelest;
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)ptrDBEstAgv->prepal;
 sqlstm.sqhstl[5] = (unsigned int  )2;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)ptrDBEstAgv->mesaop;
 sqlstm.sqhstl[6] = (unsigned int  )2;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)&ptrDBEstAgv->errmesa;
 sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (         void  *)ptrDBEstAgv->finprod;
 sqlstm.sqhstl[8] = (unsigned int  )2;
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         void  *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned int  )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (         void  *)ptrDBEstAgv->tipest;
 sqlstm.sqhstl[9] = (unsigned int  )2;
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         void  *)0;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned int  )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (         void  *)ptrDBEstAgv->codestpul;
 sqlstm.sqhstl[10] = (unsigned int  )8;
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         void  *)0;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned int  )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
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
 if (sqlca.sqlcode == 1403) goto GetEstacionNotFound;
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv: ",sqlca,oraca);
}



	m_pEstAgv1 = (TEstAgv *)ptrDBEstAgv;

	ptriDBEstacion = new struct idbEstacion;
	// Estacion
	/* EXEC SQL OPEN CURESTACION; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 11;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0003;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )126;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (         void  *)(ptrDBEstAgv->codestpul);
 sqlstm.sqhstl[0] = (unsigned int  )8;
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
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv: ",sqlca,oraca);
}



	/* EXEC SQL FETCH CURESTACION INTO :ptrDBEstacion:ptriDBEstacion; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )145;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqfoff = (           int )0;
 sqlstm.sqfmod = (unsigned int )2;
 sqlstm.sqhstv[0] = (         void  *)ptrDBEstacion->codestpul;
 sqlstm.sqhstl[0] = (unsigned int  )8;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)&ptriDBEstacion->codestpul;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)ptrDBEstacion->descrip;
 sqlstm.sqhstl[1] = (unsigned int  )41;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)&ptriDBEstacion->descrip;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)ptrDBEstacion->tipestpul;
 sqlstm.sqhstl[2] = (unsigned int  )2;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)&ptriDBEstacion->tipestpul;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)ptrDBEstacion->senper;
 sqlstm.sqhstl[3] = (unsigned int  )2;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)&ptriDBEstacion->senper;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)ptrDBEstacion->tipmesa;
 sqlstm.sqhstl[4] = (unsigned int  )2;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)&ptriDBEstacion->tipmesa;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)ptrDBEstacion->tippal;
 sqlstm.sqhstl[5] = (unsigned int  )2;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)&ptriDBEstacion->tippal;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)ptrDBEstacion->tippalenv;
 sqlstm.sqhstl[6] = (unsigned int  )2;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)&ptriDBEstacion->tippalenv;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)&ptrDBEstacion->cappul;
 sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)&ptriDBEstacion->cappul;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (         void  *)&ptrDBEstacion->puncriped;
 sqlstm.sqhstl[8] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         void  *)&ptriDBEstacion->puncriped;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned int  )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (         void  *)ptrDBEstacion->codtren;
 sqlstm.sqhstl[9] = (unsigned int  )4;
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         void  *)&ptriDBEstacion->codtren;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned int  )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (         void  *)ptrDBEstacion->estest;
 sqlstm.sqhstl[10] = (unsigned int  )2;
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         void  *)&ptriDBEstacion->estest;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned int  )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
 sqlstm.sqhstv[11] = (         void  *)ptrDBEstacion->estpul;
 sqlstm.sqhstl[11] = (unsigned int  )2;
 sqlstm.sqhsts[11] = (         int  )0;
 sqlstm.sqindv[11] = (         void  *)&ptriDBEstacion->estpul;
 sqlstm.sqinds[11] = (         int  )0;
 sqlstm.sqharm[11] = (unsigned int  )0;
 sqlstm.sqadto[11] = (unsigned short )0;
 sqlstm.sqtdso[11] = (unsigned short )0;
 sqlstm.sqhstv[12] = (         void  *)ptrDBEstacion->codart;
 sqlstm.sqhstl[12] = (unsigned int  )11;
 sqlstm.sqhsts[12] = (         int  )0;
 sqlstm.sqindv[12] = (         void  *)&ptriDBEstacion->codart;
 sqlstm.sqinds[12] = (         int  )0;
 sqlstm.sqharm[12] = (unsigned int  )0;
 sqlstm.sqadto[12] = (unsigned short )0;
 sqlstm.sqtdso[12] = (unsigned short )0;
 sqlstm.sqhstv[13] = (         void  *)&ptrDBEstacion->palped;
 sqlstm.sqhstl[13] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[13] = (         int  )0;
 sqlstm.sqindv[13] = (         void  *)&ptriDBEstacion->palped;
 sqlstm.sqinds[13] = (         int  )0;
 sqlstm.sqharm[13] = (unsigned int  )0;
 sqlstm.sqadto[13] = (unsigned short )0;
 sqlstm.sqtdso[13] = (unsigned short )0;
 sqlstm.sqhstv[14] = (         void  *)ptrDBEstacion->finsum;
 sqlstm.sqhstl[14] = (unsigned int  )2;
 sqlstm.sqhsts[14] = (         int  )0;
 sqlstm.sqindv[14] = (         void  *)&ptriDBEstacion->finsum;
 sqlstm.sqinds[14] = (         int  )0;
 sqlstm.sqharm[14] = (unsigned int  )0;
 sqlstm.sqadto[14] = (unsigned short )0;
 sqlstm.sqtdso[14] = (unsigned short )0;
 sqlstm.sqhstv[15] = (         void  *)ptrDBEstacion->sumcon;
 sqlstm.sqhstl[15] = (unsigned int  )2;
 sqlstm.sqhsts[15] = (         int  )0;
 sqlstm.sqindv[15] = (         void  *)&ptriDBEstacion->sumcon;
 sqlstm.sqinds[15] = (         int  )0;
 sqlstm.sqharm[15] = (unsigned int  )0;
 sqlstm.sqadto[15] = (unsigned short )0;
 sqlstm.sqtdso[15] = (unsigned short )0;
 sqlstm.sqhstv[16] = (         void  *)ptrDBEstacion->codpullno;
 sqlstm.sqhstl[16] = (unsigned int  )8;
 sqlstm.sqhsts[16] = (         int  )0;
 sqlstm.sqindv[16] = (         void  *)&ptriDBEstacion->codpullno;
 sqlstm.sqinds[16] = (         int  )0;
 sqlstm.sqharm[16] = (unsigned int  )0;
 sqlstm.sqadto[16] = (unsigned short )0;
 sqlstm.sqtdso[16] = (unsigned short )0;
 sqlstm.sqhstv[17] = (         void  *)ptrDBEstacion->evacuar;
 sqlstm.sqhstl[17] = (unsigned int  )2;
 sqlstm.sqhsts[17] = (         int  )0;
 sqlstm.sqindv[17] = (         void  *)&ptriDBEstacion->evacuar;
 sqlstm.sqinds[17] = (         int  )0;
 sqlstm.sqharm[17] = (unsigned int  )0;
 sqlstm.sqadto[17] = (unsigned short )0;
 sqlstm.sqtdso[17] = (unsigned short )0;
 sqlstm.sqhstv[18] = (         void  *)ptrDBEstacion->automan;
 sqlstm.sqhstl[18] = (unsigned int  )2;
 sqlstm.sqhsts[18] = (         int  )0;
 sqlstm.sqindv[18] = (         void  *)&ptriDBEstacion->automan;
 sqlstm.sqinds[18] = (         int  )0;
 sqlstm.sqharm[18] = (unsigned int  )0;
 sqlstm.sqadto[18] = (unsigned short )0;
 sqlstm.sqtdso[18] = (unsigned short )0;
 sqlstm.sqhstv[19] = (         void  *)ptrDBEstacion->intcamb;
 sqlstm.sqhstl[19] = (unsigned int  )2;
 sqlstm.sqhsts[19] = (         int  )0;
 sqlstm.sqindv[19] = (         void  *)&ptriDBEstacion->shortcamb;
 sqlstm.sqinds[19] = (         int  )0;
 sqlstm.sqharm[19] = (unsigned int  )0;
 sqlstm.sqadto[19] = (unsigned short )0;
 sqlstm.sqtdso[19] = (unsigned short )0;
 sqlstm.sqhstv[20] = (         void  *)&ptrDBEstacion->ocupul;
 sqlstm.sqhstl[20] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[20] = (         int  )0;
 sqlstm.sqindv[20] = (         void  *)&ptriDBEstacion->ocupul;
 sqlstm.sqinds[20] = (         int  )0;
 sqlstm.sqharm[20] = (unsigned int  )0;
 sqlstm.sqadto[20] = (unsigned short )0;
 sqlstm.sqtdso[20] = (unsigned short )0;
 sqlstm.sqhstv[21] = (         void  *)ptrDBEstacion->senact;
 sqlstm.sqhstl[21] = (unsigned int  )2;
 sqlstm.sqhsts[21] = (         int  )0;
 sqlstm.sqindv[21] = (         void  *)&ptriDBEstacion->senact;
 sqlstm.sqinds[21] = (         int  )0;
 sqlstm.sqharm[21] = (unsigned int  )0;
 sqlstm.sqadto[21] = (unsigned short )0;
 sqlstm.sqtdso[21] = (unsigned short )0;
 sqlstm.sqhstv[22] = (         void  *)&ptrDBEstacion->palsum;
 sqlstm.sqhstl[22] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[22] = (         int  )0;
 sqlstm.sqindv[22] = (         void  *)&ptriDBEstacion->palsum;
 sqlstm.sqinds[22] = (         int  )0;
 sqlstm.sqharm[22] = (unsigned int  )0;
 sqlstm.sqadto[22] = (unsigned short )0;
 sqlstm.sqtdso[22] = (unsigned short )0;
 sqlstm.sqhstv[23] = (         void  *)&ptrDBEstacion->resagv;
 sqlstm.sqhstl[23] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[23] = (         int  )0;
 sqlstm.sqindv[23] = (         void  *)&ptriDBEstacion->resagv;
 sqlstm.sqinds[23] = (         int  )0;
 sqlstm.sqharm[23] = (unsigned int  )0;
 sqlstm.sqadto[23] = (unsigned short )0;
 sqlstm.sqtdso[23] = (unsigned short )0;
 sqlstm.sqhstv[24] = (         void  *)ptrDBEstacion->senagv;
 sqlstm.sqhstl[24] = (unsigned int  )2;
 sqlstm.sqhsts[24] = (         int  )0;
 sqlstm.sqindv[24] = (         void  *)&ptriDBEstacion->senagv;
 sqlstm.sqinds[24] = (         int  )0;
 sqlstm.sqharm[24] = (unsigned int  )0;
 sqlstm.sqadto[24] = (unsigned short )0;
 sqlstm.sqtdso[24] = (unsigned short )0;
 sqlstm.sqhstv[25] = (         void  *)&ptrDBEstacion->palpedrad;
 sqlstm.sqhstl[25] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[25] = (         int  )0;
 sqlstm.sqindv[25] = (         void  *)&ptriDBEstacion->palpedrad;
 sqlstm.sqinds[25] = (         int  )0;
 sqlstm.sqharm[25] = (unsigned int  )0;
 sqlstm.sqadto[25] = (unsigned short )0;
 sqlstm.sqtdso[25] = (unsigned short )0;
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
 if (sqlca.sqlcode == 1403) goto GetEstacionNotFound;
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv: ",sqlca,oraca);
}


	m_tipMesa = 'D' == ptrDBEstacion->tipmesa [0] ? 2 : 1;

	//Xavi,09/06/05: actualitzat desde cpp i comentat fins que no es demostri el contrari (agafat del PLC)
	// Michael 14.12.2004 Actualiza cappul y ocupul TODO - ver que pasa cuando no se simula
	m_OcuPulmon = ptrDBEstacion->ocupul;
	m_CapPulmon = ptrDBEstacion->cappul;
	// Michael 14.12.2004 fin
	//Fi xavi, 09/06/05
	
	// Afegit xavi, 09/06/05: resta de camps a mostrar en l'estacio del bub
	m_Automan = ptrDBEstacion->automan[0];
	m_Senact = ptrDBEstacion->senact[0];
	m_Resagv = ptrDBEstacion->resagv;
	// Fi afegit xavi, 09/06/05

	delete ptriDBEstacion;

	/* EXEC SQL CLOSE CURESTACION; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )264;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv: ",sqlca,oraca);
}



	m_pEstacion = (TEstacion *)ptrDBEstacion;

	if (2 == m_tipMesa)
		{
		// Hay dos mesas
		ptrDBEstAgv = new struct dbEstAgv; 
		ptriDBEstAgv = new struct idbEstAgv;

		/* EXEC SQL FETCH CURESTAGV INTO :ptrDBEstAgv:ptriDBEstAgv; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )279;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (           int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (         void  *)ptrDBEstAgv->codclind;
  sqlstm.sqhstl[0] = (unsigned int  )8;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)&ptriDBEstAgv->codclind;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&ptrDBEstAgv->ptocom;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)&ptriDBEstAgv->ptocom;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&ptrDBEstAgv->posicion;
  sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)&ptriDBEstAgv->posicion;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)ptrDBEstAgv->lado;
  sqlstm.sqhstl[3] = (unsigned int  )2;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)&ptriDBEstAgv->lado;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)&ptrDBEstAgv->levelest;
  sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         void  *)&ptriDBEstAgv->levelest;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned int  )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (         void  *)ptrDBEstAgv->prepal;
  sqlstm.sqhstl[5] = (unsigned int  )2;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         void  *)&ptriDBEstAgv->prepal;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned int  )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (         void  *)ptrDBEstAgv->mesaop;
  sqlstm.sqhstl[6] = (unsigned int  )2;
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         void  *)&ptriDBEstAgv->mesaop;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned int  )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (         void  *)&ptrDBEstAgv->errmesa;
  sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         void  *)&ptriDBEstAgv->errmesa;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned int  )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (         void  *)ptrDBEstAgv->finprod;
  sqlstm.sqhstl[8] = (unsigned int  )2;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         void  *)&ptriDBEstAgv->finprod;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned int  )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (         void  *)ptrDBEstAgv->tipest;
  sqlstm.sqhstl[9] = (unsigned int  )2;
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         void  *)&ptriDBEstAgv->tipest;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned int  )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
  sqlstm.sqhstv[10] = (         void  *)ptrDBEstAgv->codestpul;
  sqlstm.sqhstl[10] = (unsigned int  )8;
  sqlstm.sqhsts[10] = (         int  )0;
  sqlstm.sqindv[10] = (         void  *)&ptriDBEstAgv->codestpul;
  sqlstm.sqinds[10] = (         int  )0;
  sqlstm.sqharm[10] = (unsigned int  )0;
  sqlstm.sqadto[10] = (unsigned short )0;
  sqlstm.sqtdso[10] = (unsigned short )0;
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
  if (sqlca.sqlcode == 1403) goto GetEstacionNotFound;
  if (sqlca.sqlcode < 0) plsql_error("GetEstAgv: ",sqlca,oraca);
}


		m_pEstAgv2 = (TEstAgv *)ptrDBEstAgv;
		delete ptriDBEstAgv;

		}
	ptriDBAsigPulVac = new struct idbAsigPulVac;

	m_fGotDatos = true;
	
	/* EXEC SQL WHENEVER NOTFOUND GOTO GetCodPulVacNotFound; */ 


	m_CodPulVac = "-";

	/* EXEC SQL OPEN CURPULVAC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0004;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )338;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (         void  *)(ptrDBEstAgv->codestpul);
 sqlstm.sqhstl[0] = (unsigned int  )8;
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
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv: ",sqlca,oraca);
}


	
	/* EXEC SQL FETCH CURPULVAC INTO :ptrDBAsigPulVac:ptriDBAsigPulVac; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )357;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqfoff = (           int )0;
 sqlstm.sqfmod = (unsigned int )2;
 sqlstm.sqhstv[0] = (         void  *)ptrDBAsigPulVac->codpulvac;
 sqlstm.sqhstl[0] = (unsigned int  )8;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)&ptriDBAsigPulVac->codpulvac;
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
 if (sqlca.sqlcode == 1403) goto GetCodPulVacNotFound;
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv: ",sqlca,oraca);
}


	if (ptrDBAsigPulVac->codpulvac != NULL)
		m_CodPulVac = ptrDBAsigPulVac->codpulvac;

	delete ptriDBAsigPulVac;

	/* EXEC SQL CLOSE CURPULVAC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )376;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv: ",sqlca,oraca);
}



	/* EXEC SQL CLOSE CURESTAGV; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )391;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv: ",sqlca,oraca);
}



	return (true);

GetEstacionNotFound:
	// M2015 TODO WAS error
	MSGTRACE3 ("GetEstAgv ()-> ptocom: %d posicion: %d lado %d Not Found",
					ptocom,
					posicion,
					lado);



	return (false);

GetCodPulVacNotFound:

	return (true);


}



// Actualizar los datos de la estación avisado por el PLC
void CDBEstacion::UpdateEstacionPLC(BYTE *bMensaje)
{

	int ptocom = bMensaje [1];
	int posicion = (bMensaje [2] & 0xFE) >> 1;
	int lado = bMensaje [2] & 0x01;

	lado = lado == 0 ? 'R' : 'L';

	m_EstadoMesa1 = bMensaje [3];
	m_EstadoMesa2 = bMensaje [4];
	m_EstadoEstacion = bMensaje [5];

// Michael 08.07.2013 
	if (ptocom != 1)
{

	if (GetEstAgv (ptocom, posicion, lado))
	{
		m_CapPulmon = bMensaje [6];
		m_OcuPulmon = bMensaje [7];
		
		UpdateEstacion ();

		UpdateEstAgv (m_pEstAgv1);
		if (2 == m_tipMesa)
			UpdateEstAgv (m_pEstAgv2);

		// Michael 21.01.2002 Realizado para cada estación / estAgv EXEC SQL COMMIT;

		SetOrdenEstacion ();

		// Michael 20.03.2002 Generar estadisticas de ocupación estación

		InsertarHistOcuEst ();


	}
	// XXX Else not found the station !!!
}	// Michael 08.07.2013
}

// Actualizar estacion del tren
void CDBEstacion::UpdateEstacion()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	if (! g_Container.m_dbutil.IsConnected ())
		{
		MSGWARNING ("GetEstacion ()-> No hay conexión a Oracle");
		return;
		}

	/* EXEC SQL BEGIN DECLARE SECTION; */ 
		

	struct dbEstacion * ptrDBEstacion;

	/* EXEC SQL END DECLARE SECTION; */ 



    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("UpdateEstacionTren: ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO GetEstacionTrenNotFound; */ 


	bool fUpdateEst = false;
	if (m_pEstacion->cappul != m_CapPulmon)
		fUpdateEst = true;

	//...Etc...

	m_pEstacion->cappul = m_CapPulmon;	// No se usa ?
	// Michael: 21.11.2001 Puede ser ni entrada ni salida
	if (ESTACIONENTRADA)
		m_pEstacion->senact [0] = 'E';
	else if (ESTACIONSALIDA)
		m_pEstacion->senact [0] = 'S';
	else
		m_pEstacion->senact [0] = 'N';

	m_pEstacion->ocupul = m_OcuPulmon;
	// Michael 18.04.2002
	if (m_pEstacion->ocupul > 99)
		m_pEstacion->ocupul = 99;

	m_pEstacion->automan [0] = ESTACIONAUTOMATICO ? 'A' : 'M';

	ptrDBEstacion = (struct dbEstacion *)m_pEstacion;

	/* EXEC SQL UPDATE CONEST
		SET SENACT = :ptrDBEstacion->senact,
			OCUPUL = :ptrDBEstacion->ocupul,
			AUTOMAN = :ptrDBEstacion->automan
		WHERE CODESTPUL = :ptrDBEstacion->codestpul; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update CONEST  set SENACT=:b0,OCUPUL=:b1,AUTOMAN=:b2 where C\
ODESTPUL=:b3";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )406;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)(ptrDBEstacion->senact);
 sqlstm.sqhstl[0] = (unsigned int  )2;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&(ptrDBEstacion->ocupul);
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)(ptrDBEstacion->automan);
 sqlstm.sqhstl[2] = (unsigned int  )2;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)(ptrDBEstacion->codestpul);
 sqlstm.sqhstl[3] = (unsigned int  )8;
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
 if (sqlca.sqlcode == 1403) goto GetEstacionTrenNotFound;
 if (sqlca.sqlcode < 0) plsql_error("UpdateEstacionTren: ",sqlca,oraca);
}



	// Michael 21.01.2002 Realizar el commit para evitar deadlocks...
	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )437;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("UpdateEstacionTren: ",sqlca,oraca);
}




	if (g_Container.IsSincronizacion ())
	{
		// Solo al arranque ya que complica los forms de estaciones
		/* EXEC SQL UPDATE ESTACION
		SET CAPPUL = :ptrDBEstacion->cappul
			WHERE CODESTPUL = :ptrDBEstacion->codestpul; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = "update ESTACION  set CAPPUL=:b0 where CODESTPUL=:b1";
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )452;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqhstv[0] = (         void  *)&(ptrDBEstacion->cappul);
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)(ptrDBEstacion->codestpul);
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
  if (sqlca.sqlcode == 1403) goto GetEstacionTrenNotFound;
  if (sqlca.sqlcode < 0) plsql_error("UpdateEstacionTren: ",sqlca,oraca);
}


		// Michael 21.01.2002 Realizar el commit para evitar deadlocks...
		/* EXEC SQL COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )475;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
  if (sqlca.sqlcode < 0) plsql_error("UpdateEstacionTren: ",sqlca,oraca);
}


	}


	return;
GetEstacionTrenNotFound:

	MSGWARNING ("UpdateEstacion () -> Not Found");
}


// Devolver número de mesas que tiene la estación
UINT CDBEstacion::GetNumeroMesas()
{
	if (m_fGotDatos)
	{
		return (m_tipMesa);
	}
	else
		return (0);

}


// Devolver el cliente Indumat de la estación AGV
LPCTSTR CDBEstacion::GetClIND()
{
	if (m_fGotDatos)
	{
		return (m_pEstAgv1->codclind);
	}
	else
		return ("????");

}


void CDBEstacion::UpdateEstAgv(TEstAgv * ptrEstAgv)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	if (! g_Container.m_dbutil.IsConnected ())
		{
		MSGWARNING ("UpdateEstAgv ()-> No hay conexión a Oracle");
		return;
		}
	
	/* EXEC SQL BEGIN DECLARE SECTION; */ 
	
		struct dbEstAgv * ptrDBEstAgv;
	/* EXEC SQL END DECLARE SECTION; */ 




    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("UpdateEstAgv: ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 
	// Update doesn't do not found

	//...Etc...

	BYTE mesa;
	if (ptrEstAgv == m_pEstAgv1)
		mesa = m_EstadoMesa1;
	else
		mesa = m_EstadoMesa2;

	ptrEstAgv->prepal [0] = PRESENCIAPALET (mesa) ?
		'S' : 'N';
	ptrEstAgv->mesaop [0] = MESAOPERATIVA (mesa) ?
		'S' : 'N';
	ptrEstAgv->errmesa = ERRORMESA (mesa);
	if (ptrEstAgv->ptocom != 41)
		// Para 41 finproducción es de pantalla
		ptrEstAgv->finprod [0] = FINPRODUCCION (mesa) ?
			'S' : 'N';

			
	ptrDBEstAgv = (struct dbEstAgv *)ptrEstAgv;

	/* EXEC SQL UPDATE ESTAGV
		SET PREPAL = :ptrDBEstAgv->prepal,
			MESAOP = :ptrDBEstAgv->mesaop,
			ERRMESA = :ptrDBEstAgv->errmesa,
			FINPROD = :ptrDBEstAgv->finprod
		WHERE CODCLIND = :ptrDBEstAgv->codclind; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update ESTAGV  set PREPAL=:b0,MESAOP=:b1,ERRMESA=:b2,FINPROD\
=:b3 where CODCLIND=:b4";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )490;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)(ptrDBEstAgv->prepal);
 sqlstm.sqhstl[0] = (unsigned int  )2;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)(ptrDBEstAgv->mesaop);
 sqlstm.sqhstl[1] = (unsigned int  )2;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&(ptrDBEstAgv->errmesa);
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)(ptrDBEstAgv->finprod);
 sqlstm.sqhstl[3] = (unsigned int  )2;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)(ptrDBEstAgv->codclind);
 sqlstm.sqhstl[4] = (unsigned int  )8;
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
 if (sqlca.sqlcode < 0) plsql_error("UpdateEstAgv: ",sqlca,oraca);
}



	// Michael 21.01.2002: Realizar el commit para evitar deadlocks...
	
	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )525;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("UpdateEstAgv: ",sqlca,oraca);
}




	// Actualizar la representacion de la estacion para el BuB
	g_Container.m_GestorEstBuB.SetEstacionBuB (
		ptrEstAgv->ptocom,
		ptrEstAgv->posicion,
		ptrEstAgv->lado [0],
		ERRORMESA (mesa) ? true : false,
		PRESENCIAPALET (mesa) ? true : false);

	return;


}


// Devolver el estado de la estacion
bool CDBEstacion::SentidoDelante()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	if (m_fGotDatos)
	{

		bool fSentido = true;
		fSentido = 'F' == m_pEstacion->senagv [0] ? true : false;
		return (fSentido);
	}
	else
	{
		MSGERROR ("No hay datos de la estación");
		return (true);
	}

}

// Devolver código del pulmón
LPCTSTR CDBEstacion::GetCodEstPul()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	if (m_fGotDatos)
	{
		return (m_pEstacion->codestpul);
	}
	else
		return ("????");

}

// Get datos de la estación AGV identificado por su código de estación
bool CDBEstacion::GetEstAgv(LPCTSTR codestpul)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 

	
	if (! g_Container.m_dbutil.IsConnected ())
		{
		MSGWARNING ("GetEstAgv (codestpul)-> No hay conexión a Oracle");
		return (false);
		}

	/* EXEC SQL BEGIN DECLARE SECTION; */ 
	
		struct dbEstAgv * ptrDBEstAgv;
		struct dbEstacion * ptrDBEstacion;
		struct idbEstacion * ptriDBEstacion;
	/* EXEC SQL END DECLARE SECTION; */ 

		
	ptrDBEstAgv = new struct dbEstAgv; 
	ptrDBEstacion = new struct dbEstacion;

    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("GetEstAgv (codestpul): ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO GetEstAgvCodEstNotFound; */ 


	strcpy (ptrDBEstacion->codestpul, codestpul);


	/* EXEC SQL DECLARE CURESTAGVCODEST CURSOR FOR
		SELECT * FROM ESTAGV
		WHERE CODESTPUL = :ptrDBEstacion->codestpul
		ORDER BY posicion asc; */ 


	/* EXEC SQL DECLARE CURESTACIONCODEST CURSOR FOR
		SELECT * FROM VCONEST
		WHERE CODESTPUL = :ptrDBEstacion->codestpul; */ 



	/* EXEC SQL OPEN CURESTAGVCODEST; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0011;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )540;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (         void  *)(ptrDBEstacion->codestpul);
 sqlstm.sqhstl[0] = (unsigned int  )8;
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
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv (codestpul): ",sqlca,oraca);
}



	/* EXEC SQL FETCH CURESTAGVCODEST INTO :ptrDBEstAgv; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )559;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqfoff = (           int )0;
 sqlstm.sqfmod = (unsigned int )2;
 sqlstm.sqhstv[0] = (         void  *)ptrDBEstAgv->codclind;
 sqlstm.sqhstl[0] = (unsigned int  )8;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&ptrDBEstAgv->ptocom;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&ptrDBEstAgv->posicion;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)ptrDBEstAgv->lado;
 sqlstm.sqhstl[3] = (unsigned int  )2;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&ptrDBEstAgv->levelest;
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)ptrDBEstAgv->prepal;
 sqlstm.sqhstl[5] = (unsigned int  )2;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)ptrDBEstAgv->mesaop;
 sqlstm.sqhstl[6] = (unsigned int  )2;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)0;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)&ptrDBEstAgv->errmesa;
 sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)0;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (         void  *)ptrDBEstAgv->finprod;
 sqlstm.sqhstl[8] = (unsigned int  )2;
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         void  *)0;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned int  )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (         void  *)ptrDBEstAgv->tipest;
 sqlstm.sqhstl[9] = (unsigned int  )2;
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         void  *)0;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned int  )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (         void  *)ptrDBEstAgv->codestpul;
 sqlstm.sqhstl[10] = (unsigned int  )8;
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         void  *)0;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned int  )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
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
 if (sqlca.sqlcode == 1403) goto GetEstAgvCodEstNotFound;
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv (codestpul): ",sqlca,oraca);
}



	m_pEstAgv1 = (TEstAgv *)ptrDBEstAgv;

	ptriDBEstacion = new struct idbEstacion;
	// Estacion
	/* EXEC SQL OPEN CURESTACIONCODEST; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0012;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )618;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (         void  *)(ptrDBEstacion->codestpul);
 sqlstm.sqhstl[0] = (unsigned int  )8;
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
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv (codestpul): ",sqlca,oraca);
}



	/* EXEC SQL FETCH CURESTACIONCODEST INTO :ptrDBEstacion:ptriDBEstacion; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )637;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqfoff = (           int )0;
 sqlstm.sqfmod = (unsigned int )2;
 sqlstm.sqhstv[0] = (         void  *)ptrDBEstacion->codestpul;
 sqlstm.sqhstl[0] = (unsigned int  )8;
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)&ptriDBEstacion->codestpul;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)ptrDBEstacion->descrip;
 sqlstm.sqhstl[1] = (unsigned int  )41;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)&ptriDBEstacion->descrip;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)ptrDBEstacion->tipestpul;
 sqlstm.sqhstl[2] = (unsigned int  )2;
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)&ptriDBEstacion->tipestpul;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)ptrDBEstacion->senper;
 sqlstm.sqhstl[3] = (unsigned int  )2;
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)&ptriDBEstacion->senper;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)ptrDBEstacion->tipmesa;
 sqlstm.sqhstl[4] = (unsigned int  )2;
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)&ptriDBEstacion->tipmesa;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)ptrDBEstacion->tippal;
 sqlstm.sqhstl[5] = (unsigned int  )2;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)&ptriDBEstacion->tippal;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
 sqlstm.sqhstv[6] = (         void  *)ptrDBEstacion->tippalenv;
 sqlstm.sqhstl[6] = (unsigned int  )2;
 sqlstm.sqhsts[6] = (         int  )0;
 sqlstm.sqindv[6] = (         void  *)&ptriDBEstacion->tippalenv;
 sqlstm.sqinds[6] = (         int  )0;
 sqlstm.sqharm[6] = (unsigned int  )0;
 sqlstm.sqadto[6] = (unsigned short )0;
 sqlstm.sqtdso[6] = (unsigned short )0;
 sqlstm.sqhstv[7] = (         void  *)&ptrDBEstacion->cappul;
 sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[7] = (         int  )0;
 sqlstm.sqindv[7] = (         void  *)&ptriDBEstacion->cappul;
 sqlstm.sqinds[7] = (         int  )0;
 sqlstm.sqharm[7] = (unsigned int  )0;
 sqlstm.sqadto[7] = (unsigned short )0;
 sqlstm.sqtdso[7] = (unsigned short )0;
 sqlstm.sqhstv[8] = (         void  *)&ptrDBEstacion->puncriped;
 sqlstm.sqhstl[8] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[8] = (         int  )0;
 sqlstm.sqindv[8] = (         void  *)&ptriDBEstacion->puncriped;
 sqlstm.sqinds[8] = (         int  )0;
 sqlstm.sqharm[8] = (unsigned int  )0;
 sqlstm.sqadto[8] = (unsigned short )0;
 sqlstm.sqtdso[8] = (unsigned short )0;
 sqlstm.sqhstv[9] = (         void  *)ptrDBEstacion->codtren;
 sqlstm.sqhstl[9] = (unsigned int  )4;
 sqlstm.sqhsts[9] = (         int  )0;
 sqlstm.sqindv[9] = (         void  *)&ptriDBEstacion->codtren;
 sqlstm.sqinds[9] = (         int  )0;
 sqlstm.sqharm[9] = (unsigned int  )0;
 sqlstm.sqadto[9] = (unsigned short )0;
 sqlstm.sqtdso[9] = (unsigned short )0;
 sqlstm.sqhstv[10] = (         void  *)ptrDBEstacion->estest;
 sqlstm.sqhstl[10] = (unsigned int  )2;
 sqlstm.sqhsts[10] = (         int  )0;
 sqlstm.sqindv[10] = (         void  *)&ptriDBEstacion->estest;
 sqlstm.sqinds[10] = (         int  )0;
 sqlstm.sqharm[10] = (unsigned int  )0;
 sqlstm.sqadto[10] = (unsigned short )0;
 sqlstm.sqtdso[10] = (unsigned short )0;
 sqlstm.sqhstv[11] = (         void  *)ptrDBEstacion->estpul;
 sqlstm.sqhstl[11] = (unsigned int  )2;
 sqlstm.sqhsts[11] = (         int  )0;
 sqlstm.sqindv[11] = (         void  *)&ptriDBEstacion->estpul;
 sqlstm.sqinds[11] = (         int  )0;
 sqlstm.sqharm[11] = (unsigned int  )0;
 sqlstm.sqadto[11] = (unsigned short )0;
 sqlstm.sqtdso[11] = (unsigned short )0;
 sqlstm.sqhstv[12] = (         void  *)ptrDBEstacion->codart;
 sqlstm.sqhstl[12] = (unsigned int  )11;
 sqlstm.sqhsts[12] = (         int  )0;
 sqlstm.sqindv[12] = (         void  *)&ptriDBEstacion->codart;
 sqlstm.sqinds[12] = (         int  )0;
 sqlstm.sqharm[12] = (unsigned int  )0;
 sqlstm.sqadto[12] = (unsigned short )0;
 sqlstm.sqtdso[12] = (unsigned short )0;
 sqlstm.sqhstv[13] = (         void  *)&ptrDBEstacion->palped;
 sqlstm.sqhstl[13] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[13] = (         int  )0;
 sqlstm.sqindv[13] = (         void  *)&ptriDBEstacion->palped;
 sqlstm.sqinds[13] = (         int  )0;
 sqlstm.sqharm[13] = (unsigned int  )0;
 sqlstm.sqadto[13] = (unsigned short )0;
 sqlstm.sqtdso[13] = (unsigned short )0;
 sqlstm.sqhstv[14] = (         void  *)ptrDBEstacion->finsum;
 sqlstm.sqhstl[14] = (unsigned int  )2;
 sqlstm.sqhsts[14] = (         int  )0;
 sqlstm.sqindv[14] = (         void  *)&ptriDBEstacion->finsum;
 sqlstm.sqinds[14] = (         int  )0;
 sqlstm.sqharm[14] = (unsigned int  )0;
 sqlstm.sqadto[14] = (unsigned short )0;
 sqlstm.sqtdso[14] = (unsigned short )0;
 sqlstm.sqhstv[15] = (         void  *)ptrDBEstacion->sumcon;
 sqlstm.sqhstl[15] = (unsigned int  )2;
 sqlstm.sqhsts[15] = (         int  )0;
 sqlstm.sqindv[15] = (         void  *)&ptriDBEstacion->sumcon;
 sqlstm.sqinds[15] = (         int  )0;
 sqlstm.sqharm[15] = (unsigned int  )0;
 sqlstm.sqadto[15] = (unsigned short )0;
 sqlstm.sqtdso[15] = (unsigned short )0;
 sqlstm.sqhstv[16] = (         void  *)ptrDBEstacion->codpullno;
 sqlstm.sqhstl[16] = (unsigned int  )8;
 sqlstm.sqhsts[16] = (         int  )0;
 sqlstm.sqindv[16] = (         void  *)&ptriDBEstacion->codpullno;
 sqlstm.sqinds[16] = (         int  )0;
 sqlstm.sqharm[16] = (unsigned int  )0;
 sqlstm.sqadto[16] = (unsigned short )0;
 sqlstm.sqtdso[16] = (unsigned short )0;
 sqlstm.sqhstv[17] = (         void  *)ptrDBEstacion->evacuar;
 sqlstm.sqhstl[17] = (unsigned int  )2;
 sqlstm.sqhsts[17] = (         int  )0;
 sqlstm.sqindv[17] = (         void  *)&ptriDBEstacion->evacuar;
 sqlstm.sqinds[17] = (         int  )0;
 sqlstm.sqharm[17] = (unsigned int  )0;
 sqlstm.sqadto[17] = (unsigned short )0;
 sqlstm.sqtdso[17] = (unsigned short )0;
 sqlstm.sqhstv[18] = (         void  *)ptrDBEstacion->automan;
 sqlstm.sqhstl[18] = (unsigned int  )2;
 sqlstm.sqhsts[18] = (         int  )0;
 sqlstm.sqindv[18] = (         void  *)&ptriDBEstacion->automan;
 sqlstm.sqinds[18] = (         int  )0;
 sqlstm.sqharm[18] = (unsigned int  )0;
 sqlstm.sqadto[18] = (unsigned short )0;
 sqlstm.sqtdso[18] = (unsigned short )0;
 sqlstm.sqhstv[19] = (         void  *)ptrDBEstacion->intcamb;
 sqlstm.sqhstl[19] = (unsigned int  )2;
 sqlstm.sqhsts[19] = (         int  )0;
 sqlstm.sqindv[19] = (         void  *)&ptriDBEstacion->shortcamb;
 sqlstm.sqinds[19] = (         int  )0;
 sqlstm.sqharm[19] = (unsigned int  )0;
 sqlstm.sqadto[19] = (unsigned short )0;
 sqlstm.sqtdso[19] = (unsigned short )0;
 sqlstm.sqhstv[20] = (         void  *)&ptrDBEstacion->ocupul;
 sqlstm.sqhstl[20] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[20] = (         int  )0;
 sqlstm.sqindv[20] = (         void  *)&ptriDBEstacion->ocupul;
 sqlstm.sqinds[20] = (         int  )0;
 sqlstm.sqharm[20] = (unsigned int  )0;
 sqlstm.sqadto[20] = (unsigned short )0;
 sqlstm.sqtdso[20] = (unsigned short )0;
 sqlstm.sqhstv[21] = (         void  *)ptrDBEstacion->senact;
 sqlstm.sqhstl[21] = (unsigned int  )2;
 sqlstm.sqhsts[21] = (         int  )0;
 sqlstm.sqindv[21] = (         void  *)&ptriDBEstacion->senact;
 sqlstm.sqinds[21] = (         int  )0;
 sqlstm.sqharm[21] = (unsigned int  )0;
 sqlstm.sqadto[21] = (unsigned short )0;
 sqlstm.sqtdso[21] = (unsigned short )0;
 sqlstm.sqhstv[22] = (         void  *)&ptrDBEstacion->palsum;
 sqlstm.sqhstl[22] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[22] = (         int  )0;
 sqlstm.sqindv[22] = (         void  *)&ptriDBEstacion->palsum;
 sqlstm.sqinds[22] = (         int  )0;
 sqlstm.sqharm[22] = (unsigned int  )0;
 sqlstm.sqadto[22] = (unsigned short )0;
 sqlstm.sqtdso[22] = (unsigned short )0;
 sqlstm.sqhstv[23] = (         void  *)&ptrDBEstacion->resagv;
 sqlstm.sqhstl[23] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[23] = (         int  )0;
 sqlstm.sqindv[23] = (         void  *)&ptriDBEstacion->resagv;
 sqlstm.sqinds[23] = (         int  )0;
 sqlstm.sqharm[23] = (unsigned int  )0;
 sqlstm.sqadto[23] = (unsigned short )0;
 sqlstm.sqtdso[23] = (unsigned short )0;
 sqlstm.sqhstv[24] = (         void  *)ptrDBEstacion->senagv;
 sqlstm.sqhstl[24] = (unsigned int  )2;
 sqlstm.sqhsts[24] = (         int  )0;
 sqlstm.sqindv[24] = (         void  *)&ptriDBEstacion->senagv;
 sqlstm.sqinds[24] = (         int  )0;
 sqlstm.sqharm[24] = (unsigned int  )0;
 sqlstm.sqadto[24] = (unsigned short )0;
 sqlstm.sqtdso[24] = (unsigned short )0;
 sqlstm.sqhstv[25] = (         void  *)&ptrDBEstacion->palpedrad;
 sqlstm.sqhstl[25] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[25] = (         int  )0;
 sqlstm.sqindv[25] = (         void  *)&ptriDBEstacion->palpedrad;
 sqlstm.sqinds[25] = (         int  )0;
 sqlstm.sqharm[25] = (unsigned int  )0;
 sqlstm.sqadto[25] = (unsigned short )0;
 sqlstm.sqtdso[25] = (unsigned short )0;
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
 if (sqlca.sqlcode == 1403) goto GetEstAgvCodEstNotFound;
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv (codestpul): ",sqlca,oraca);
}


	m_tipMesa = 'D' == ptrDBEstacion->tipmesa [0] ? 2 : 1;
	//Xavi, 09/06/05: actualitzat de cpp i comentat fins que no es demostri el contrari (agafat del plc)
	// Michael 14.12.2004 Actualiza cappul y ocupul TODO - ver que pasa cuando no se simula
	m_OcuPulmon = ptrDBEstacion->ocupul;
	m_CapPulmon = ptrDBEstacion->cappul;
	// Michael 14.12.2004 fin
	// Fi xavi, 09/06/05: actualitzat de cpp

	delete ptriDBEstacion;

	/* EXEC SQL CLOSE CURESTACIONCODEST; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )756;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv (codestpul): ",sqlca,oraca);
}



	m_pEstacion = (TEstacion *)ptrDBEstacion;

	if (2 == m_tipMesa)
		{
		// Hay dos mesas
		ptrDBEstAgv = new struct dbEstAgv; 

		/* EXEC SQL FETCH CURESTAGVCODEST INTO :ptrDBEstAgv; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )771;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (           int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (         void  *)ptrDBEstAgv->codclind;
  sqlstm.sqhstl[0] = (unsigned int  )8;
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&ptrDBEstAgv->ptocom;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)&ptrDBEstAgv->posicion;
  sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)ptrDBEstAgv->lado;
  sqlstm.sqhstl[3] = (unsigned int  )2;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)&ptrDBEstAgv->levelest;
  sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         void  *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned int  )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (         void  *)ptrDBEstAgv->prepal;
  sqlstm.sqhstl[5] = (unsigned int  )2;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         void  *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned int  )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (         void  *)ptrDBEstAgv->mesaop;
  sqlstm.sqhstl[6] = (unsigned int  )2;
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         void  *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned int  )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (         void  *)&ptrDBEstAgv->errmesa;
  sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         void  *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned int  )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
  sqlstm.sqhstv[8] = (         void  *)ptrDBEstAgv->finprod;
  sqlstm.sqhstl[8] = (unsigned int  )2;
  sqlstm.sqhsts[8] = (         int  )0;
  sqlstm.sqindv[8] = (         void  *)0;
  sqlstm.sqinds[8] = (         int  )0;
  sqlstm.sqharm[8] = (unsigned int  )0;
  sqlstm.sqadto[8] = (unsigned short )0;
  sqlstm.sqtdso[8] = (unsigned short )0;
  sqlstm.sqhstv[9] = (         void  *)ptrDBEstAgv->tipest;
  sqlstm.sqhstl[9] = (unsigned int  )2;
  sqlstm.sqhsts[9] = (         int  )0;
  sqlstm.sqindv[9] = (         void  *)0;
  sqlstm.sqinds[9] = (         int  )0;
  sqlstm.sqharm[9] = (unsigned int  )0;
  sqlstm.sqadto[9] = (unsigned short )0;
  sqlstm.sqtdso[9] = (unsigned short )0;
  sqlstm.sqhstv[10] = (         void  *)ptrDBEstAgv->codestpul;
  sqlstm.sqhstl[10] = (unsigned int  )8;
  sqlstm.sqhsts[10] = (         int  )0;
  sqlstm.sqindv[10] = (         void  *)0;
  sqlstm.sqinds[10] = (         int  )0;
  sqlstm.sqharm[10] = (unsigned int  )0;
  sqlstm.sqadto[10] = (unsigned short )0;
  sqlstm.sqtdso[10] = (unsigned short )0;
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
  if (sqlca.sqlcode == 1403) goto GetEstAgvCodEstNotFound;
  if (sqlca.sqlcode < 0) plsql_error("GetEstAgv (codestpul): ",sqlca,oraca);
}


		m_pEstAgv2 = (TEstAgv *)ptrDBEstAgv;

		}

	/* EXEC SQL CLOSE CURESTAGVCODEST; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )830;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetEstAgv (codestpul): ",sqlca,oraca);
}



	m_fGotDatos = true;

	return (true);

GetEstAgvCodEstNotFound:

	MSGERROR1 ("GetEstAgv ()-> %s Not Found",
		codestpul);

	return (false);



}



void CDBEstacion::UpdateCargadorPLC (BYTE * bMensaje)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	UINT nCargador;

    /* EXEC SQL WHENEVER SQLERROR DO plsql_error("UpdateCargadorPLC () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO UpdateCargadorPLCNotFound; */ 


		
	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbBit0;
	int dbBit1;
	int dbBit2;
	int dbBit3;
	char dbCodCar [2 + 1];


	int dbPtoCom;

	/* EXEC SQL END DECLARE SECTION; */ 


	dbPtoCom = bMensaje [1];
	dbBit0 = (bMensaje [3] & BIT1) > 0 ? 1 : 0;
	dbBit1 = (bMensaje [3] & BIT2) > 0 ? 1 : 0;
	dbBit2 = (bMensaje [3] & BIT3) > 0 ? 1 : 0;
	dbBit3 = (bMensaje [3] & BIT4) > 0 ? 1 : 0;

	/* EXEC SQL UPDATE CARAGV
	 SET bit0 = :dbBit0,
		bit1 = :dbBit1,
		bit2 = :dbBit2,
		bit3 = :dbBit3
		where ptocom = :dbPtoCom
		returning codcar into :dbCodCar; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "update CARAGV  set bit0=:b0,bit1=:b1,bit2=:b2,bit3=:b3 where\
 ptocom=:b4 returning codcar into :b5 ";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )845;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )1;
 sqlstm.sqhstv[0] = (         void  *)&dbBit0;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbBit1;
 sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)&dbBit2;
 sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[2] = (         int  )0;
 sqlstm.sqindv[2] = (         void  *)0;
 sqlstm.sqinds[2] = (         int  )0;
 sqlstm.sqharm[2] = (unsigned int  )0;
 sqlstm.sqadto[2] = (unsigned short )0;
 sqlstm.sqtdso[2] = (unsigned short )0;
 sqlstm.sqhstv[3] = (         void  *)&dbBit3;
 sqlstm.sqhstl[3] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[3] = (         int  )0;
 sqlstm.sqindv[3] = (         void  *)0;
 sqlstm.sqinds[3] = (         int  )0;
 sqlstm.sqharm[3] = (unsigned int  )0;
 sqlstm.sqadto[3] = (unsigned short )0;
 sqlstm.sqtdso[3] = (unsigned short )0;
 sqlstm.sqhstv[4] = (         void  *)&dbPtoCom;
 sqlstm.sqhstl[4] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[4] = (         int  )0;
 sqlstm.sqindv[4] = (         void  *)0;
 sqlstm.sqinds[4] = (         int  )0;
 sqlstm.sqharm[4] = (unsigned int  )0;
 sqlstm.sqadto[4] = (unsigned short )0;
 sqlstm.sqtdso[4] = (unsigned short )0;
 sqlstm.sqhstv[5] = (         void  *)dbCodCar;
 sqlstm.sqhstl[5] = (unsigned int  )3;
 sqlstm.sqhsts[5] = (         int  )0;
 sqlstm.sqindv[5] = (         void  *)0;
 sqlstm.sqinds[5] = (         int  )0;
 sqlstm.sqharm[5] = (unsigned int  )0;
 sqlstm.sqadto[5] = (unsigned short )0;
 sqlstm.sqtdso[5] = (unsigned short )0;
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
 if (sqlca.sqlcode == 1403) goto UpdateCargadorPLCNotFound;
 if (sqlca.sqlcode < 0) plsql_error("UpdateCargadorPLC () : ",sqlca,oraca);
}




	/* EXEC SQL COMMIT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )884;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("UpdateCargadorPLC () : ",sqlca,oraca);
}



	// Indicar que tenemos el estado del cargador
	nCargador = atoi (dbCodCar);
	g_Container.SetEstadoCargador (nCargador);

	return;

UpdateCargadorPLCNotFound:
	MSGERROR1 ("Cargador de bateria %d no encontrado",
		dbPtoCom);
}


/* Buscar el siguiente cargador de bateria libre.
	Si el Agv ya está posicionado sobre la posicion (mpAgv), ya le vale
 */
bool CDBEstacion::GetCargadorLibre(UINT mpAgv, UINT * ptoCom)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	T_sqlca sqlca;	// This is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;

	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("GetCargadorLibre () : ", sqlca, oraca); */ 

	
	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbPtoCom;

	/* EXEC SQL END DECLARE SECTION; */ 


	dbPtoCom = mpAgv;

	/* EXEC SQL DECLARE CURCARAGVACT	// Posición Actual del Agv
		CURSOR FOR
		SELECT ptocom 
		FROM CARAGV
		WHERE agv = 0
			AND bit0 = 1	// en funcionamiento
			AND bit1 = 0	// Sin error
			AND bit2 = 1	// No cargando
			AND ptocom = :dbPtoCom; */ 


	/* EXEC SQL DECLARE CURCARAGV 
		CURSOR FOR
		SELECT ptocom 
		FROM CARAGV
		WHERE agv = 0
			AND bit0 = 1	// en funcionamiento
			AND bit1 = 0	// Sin error
			AND bit2 = 1	// No cargando
		ORDER BY PRIORIDAD ASC; */ 


	/* EXEC SQL WHENEVER NOTFOUND GOTO GetCargadorLibreNotPosAct; */ 


	/* EXEC SQL OPEN CURCARAGVACT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0015;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )899;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (         void  *)&dbPtoCom;
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
 if (sqlca.sqlcode < 0) plsql_error("GetCargadorLibre () : ",sqlca,oraca);
}



	/* EXEC SQL FETCH CURCARAGVACT INTO :dbPtoCom; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )918;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqfoff = (           int )0;
 sqlstm.sqfmod = (unsigned int )2;
 sqlstm.sqhstv[0] = (         void  *)&dbPtoCom;
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
 if (sqlca.sqlcode == 1403) goto GetCargadorLibreNotPosAct;
 if (sqlca.sqlcode < 0) plsql_error("GetCargadorLibre () : ",sqlca,oraca);
}




	*ptoCom = dbPtoCom;

	if (false)
	{
	
	GetCargadorLibreNotPosAct:
	
		// Solo si no se ha encontrado en su posicion actual
		/* EXEC SQL WHENEVER NOTFOUND GOTO GetCargadorLibreNotFound; */ 

		/* EXEC SQL OPEN CURCARAGV; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = sq0016;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )937;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqcmod = (unsigned int )0;
  sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
  if (sqlca.sqlcode < 0) plsql_error("GetCargadorLibre () : ",sqlca,oraca);
}



		do
		{
			/* EXEC SQL FETCH CURCARAGV INTO :dbPtoCom; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 26;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )952;
   sqlstm.selerr = (unsigned short)1;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqfoff = (           int )0;
   sqlstm.sqfmod = (unsigned int )2;
   sqlstm.sqhstv[0] = (         void  *)&dbPtoCom;
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
   if (sqlca.sqlcode == 1403) goto GetCargadorLibreNotFound;
   if (sqlca.sqlcode < 0) plsql_error("GetCargadorLibre () : ",sqlca,oraca);
}


			// Si el Agv está ya en esta posicion, ya le vale. 
			// Si está ocupada la posicion, try again
			if (dbPtoCom != (int)mpAgv && (CAgv::IsOccupiedCP (dbPtoCom) || CAgv::IsOccupiedCP (dbPtoCom + 100)))
				continue;
			else
				break;
		} while (true);



		/* EXEC SQL CLOSE CURCARAGV; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )971;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
  if (sqlca.sqlcode < 0) plsql_error("GetCargadorLibre () : ",sqlca,oraca);
}



		*ptoCom = dbPtoCom;
	}

	/* EXEC SQL CLOSE CURCARAGVACT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )986;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetCargadorLibre () : ",sqlca,oraca);
}



	return (true);

GetCargadorLibreNotFound:

	/* EXEC SQL CLOSE CURCARAGVACT; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1001;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetCargadorLibre () : ",sqlca,oraca);
}


	/* EXEC SQL CLOSE CURCARAGV; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1016;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetCargadorLibre () : ",sqlca,oraca);
}


	return (false);

}


// Ver si el PLC está indicando que el cargador ya está
bool CDBEstacion::IsCargado(UINT ptoCom, bool* todaviaCargando)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	T_sqlca sqlca;	// This is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;

	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("IsCargado () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO IsCargadoNotFound; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbPtoCom;
	int dbBit2;
	int dbBit3;

	/* EXEC SQL END DECLARE SECTION; */ 

	
	dbPtoCom = ptoCom;

	/* EXEC SQL DECLARE CURCARGADO
		CURSOR FOR
			SELECT bit2,	// 0: Cargando 1: No cargando
					bit3	// 0: Batería Llena
			FROM CARAGV
			WHERE ptocom = :dbPtoCom; */ 


	/* EXEC SQL OPEN CURCARGADO; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0017;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1031;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlstm.sqhstv[0] = (         void  *)&dbPtoCom;
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
 if (sqlca.sqlcode < 0) plsql_error("IsCargado () : ",sqlca,oraca);
}



	/* EXEC SQL FETCH CURCARGADO INTO :dbBit2, :dbBit3; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1050;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqfoff = (           int )0;
 sqlstm.sqfmod = (unsigned int )2;
 sqlstm.sqhstv[0] = (         void  *)&dbBit2;
 sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
 sqlstm.sqhsts[0] = (         int  )0;
 sqlstm.sqindv[0] = (         void  *)0;
 sqlstm.sqinds[0] = (         int  )0;
 sqlstm.sqharm[0] = (unsigned int  )0;
 sqlstm.sqadto[0] = (unsigned short )0;
 sqlstm.sqtdso[0] = (unsigned short )0;
 sqlstm.sqhstv[1] = (         void  *)&dbBit3;
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
 if (sqlca.sqlcode == 1403) goto IsCargadoNotFound;
 if (sqlca.sqlcode < 0) plsql_error("IsCargado () : ",sqlca,oraca);
}



	/* EXEC SQL CLOSE CURCARGADO; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1073;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("IsCargado () : ",sqlca,oraca);
}



	*todaviaCargando = 0 == dbBit2 ? true : false;

	return (0 == dbBit3 ? true : false);


IsCargadoNotFound:

	/* EXEC SQL CLOSE CURCARGADO; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1088;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("IsCargado () : ",sqlca,oraca);
}



	MSGERROR1 ("Cargador de bateria del punto %d no encontrado", ptoCom);
	return (false);
}


// Ver si se puede generar orden basado en la ocupación
void CDBEstacion::SetOrdenEstacion()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("SetOrdenEstacion () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		int dbResult;
		char * ptrCodEstPul;

	/* EXEC SQL END DECLARE SECTION; */ 


	ptrCodEstPul = m_pEstacion->codestpul;

	dbResult = 0;


	// Michael 29.01.2002. No generar órdenes si no está en automático
	if (!g_Container.IsEnFuncionamiento ())
		return;

	CSingleLock lockOrden (&g_Container.m_csForzarOrden);
	MSGTRACE ("SetOrdenEstacion () Esperando lock...");
	lockOrden.Lock ();
	MSGTRACE ("SetOrdenEstacion () Got lock...");


	switch (m_pEstacion->tipestpul [0])
	{
		case 'P':

			/* EXEC SQL EXECUTE
				begin
					:dbResult := map.SetOrdenPulVac (:ptrCodEstPul);
					commit;
				end;
			END-EXEC; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 26;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "begin :dbResult := map . SetOrdenPulVac ( :ptrCodEstPul ) \
; commit ; end ;";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1103;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&dbResult;
   sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)ptrCodEstPul;
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
   if (sqlca.sqlcode < 0) plsql_error("SetOrdenEstacion () : ",sqlca,oraca);
}


			break;
		case 'T':
			/* EXEC SQL EXECUTE
				begin
					:dbResult := map.SetOrdenEstacion (:ptrCodEstPul);
					commit;
				end;
			END-EXEC; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 26;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "begin :dbResult := map . SetOrdenEstacion ( :ptrCodEstPul \
) ; commit ; end ;";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1126;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&dbResult;
   sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)ptrCodEstPul;
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
   if (sqlca.sqlcode < 0) plsql_error("SetOrdenEstacion () : ",sqlca,oraca);
}


			break;
	}

	lockOrden.Unlock ();

	if (dbResult != 0)
	{
		g_GestorOrden.GetAutomaticOrden ();
	}

}		

// Solo para pruebas
void CDBEstacion::TestGeneracionOrden(LPCTSTR ptrCodest, LPCTSTR tipEst)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("TestGeneracionOrden () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		int dbResult;
		char * ptrCodEstPul;

	/* EXEC SQL END DECLARE SECTION; */ 


	ptrCodEstPul = (char *)ptrCodest;

	dbResult = 0;
	
	switch (tipEst [0])
	{
		case 'P':

			/* EXEC SQL EXECUTE
				begin
					:dbResult := map.SetOrdenPulVac (:ptrCodEstPul);
					commit;
				end;
			END-EXEC; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 26;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "begin :dbResult := map . SetOrdenPulVac ( :ptrCodEstPul ) \
; commit ; end ;";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1149;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&dbResult;
   sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)ptrCodEstPul;
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
   if (sqlca.sqlcode < 0) plsql_error("TestGeneracionOrden () : ",sqlca,oraca);
}


			break;
		case 'T':
			/* EXEC SQL EXECUTE
				begin
					:dbResult := map.SetOrdenEstacion (:ptrCodEstPul);
					commit;
				end;
			END-EXEC; */ 

{
   struct sqlexd sqlstm;
   sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
   sqlstm.sqlvsn = 12;
   sqlstm.arrsiz = 26;
   sqlstm.sqladtp = &sqladt;
   sqlstm.sqltdsp = &sqltds;
   sqlstm.stmt = "begin :dbResult := map . SetOrdenEstacion ( :ptrCodEstPul \
) ; commit ; end ;";
   sqlstm.iters = (unsigned int  )1;
   sqlstm.offset = (unsigned int  )1172;
   sqlstm.cud = sqlcud0;
   sqlstm.sqlest = (unsigned char  *)&sqlca;
   sqlstm.sqlety = (unsigned short)256;
   sqlstm.occurs = (unsigned int  )0;
   sqlstm.sqhstv[0] = (         void  *)&dbResult;
   sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
   sqlstm.sqhsts[0] = (         int  )0;
   sqlstm.sqindv[0] = (         void  *)0;
   sqlstm.sqinds[0] = (         int  )0;
   sqlstm.sqharm[0] = (unsigned int  )0;
   sqlstm.sqadto[0] = (unsigned short )0;
   sqlstm.sqtdso[0] = (unsigned short )0;
   sqlstm.sqhstv[1] = (         void  *)ptrCodEstPul;
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
   if (sqlca.sqlcode < 0) plsql_error("TestGeneracionOrden () : ",sqlca,oraca);
}


			break;
	}
	if (dbResult != 0)
	{
		g_GestorOrden.GetAutomaticOrden ();
	}


}


// Actualizar los Agvs que están en carga de bateria
void CDBEstacion::GetAgvEnCargador()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	T_sqlca sqlca;	// This is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;

	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("GetAgvEnCargador () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbPtoCom;
	int dbAgv;
	int dbWegstr;

	/* EXEC SQL END DECLARE SECTION; */ 



	/* EXEC SQL DECLARE CURAGVENCARGA CURSOR FOR
	SELECT ptocom, agv, wegstr
	FROM caragv
	WHERE agv != 0; */ 


	/* EXEC SQL OPEN CURAGVENCARGA; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0022;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1195;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetAgvEnCargador () : ",sqlca,oraca);
}



	/* EXEC SQL WHENEVER NOTFOUND GOTO GetAgvEnCargadorNoHayMas; */ 


	for (;;)
	{
		/* EXEC SQL FETCH CURAGVENCARGA INTO :dbPtoCom, :dbAgv, :dbWegstr; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1210;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (           int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (         void  *)&dbPtoCom;
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
  sqlstm.sqhstv[2] = (         void  *)&dbWegstr;
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
  if (sqlca.sqlcode == 1403) goto GetAgvEnCargadorNoHayMas;
  if (sqlca.sqlcode < 0) plsql_error("GetAgvEnCargador () : ",sqlca,oraca);
}


		CAgv * ptrAgv = g_pGestor->GetAgvPtr (dbAgv);
		if (ptrAgv)
		{
			ptrAgv->InsertarAGV (dbPtoCom, 1);
			ptrAgv->SetEstado (CARGARBAT);
			ptrAgv->m_pFzdat->wegstr = dbWegstr;
		}
		else
			MSGERROR1 ("GetAgvEnCargador: No encontrado el Agv: %d", dbAgv);
	}


	
GetAgvEnCargadorNoHayMas:

	/* EXEC SQL CLOSE CURAGVENCARGA; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1237;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("GetAgvEnCargador () : ",sqlca,oraca);
}

		

}

// Ver si se puede descargar en un pulmón de lleno
UINT CDBEstacion::GetDescargaLleno(LPCTSTR codord)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("GetDescargaLleno () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	const char * dbCodEstPul;
	const char * dbCodord;
	int dbRetVal;

	/* EXEC SQL END DECLARE SECTION; */ 


	dbCodord = codord;
	// TODO 19.03.2014
	if (!m_pEstacion)
		int i = 0;

	dbCodEstPul = m_pEstacion->codestpul;

	/* EXEC SQL EXECUTE
	begin
		:dbRetVal := map.GetDescargaLleno (:dbCodEstPul, :dbCodord);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin :dbRetVal := map . GetDescargaLleno ( :dbCodEstPul , :\
dbCodord ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1252;
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
 sqlstm.sqhstv[1] = (         void  *)dbCodEstPul;
 sqlstm.sqhstl[1] = (unsigned int  )0;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)dbCodord;
 sqlstm.sqhstl[2] = (unsigned int  )0;
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
 if (sqlca.sqlcode < 0) plsql_error("GetDescargaLleno () : ",sqlca,oraca);
}



	return (dbRetVal);
}


// Llenar array global de los puntos de espera
void CDBEstacion::SetPuntosEspera()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	T_sqlca sqlca;	// This is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;

	UINT numAgvs = 0;
	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("SetPuntosEspera () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO SetPuntosEsperaNotFound; */ 

	
	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	int dbPtoCom;
	int dbPos;
	char dbLado [2];
	char dbTipMesa [2];
	char dbSenAgv [2];
	char dbCodEstPul [8];
	int dbNumAgvDes;
	int dbNumTotalTren;
	int dbNumTotal;
	int dbUrgency;

	/* EXEC SQL END DECLARE SECTION; */ 


	CPuntoDescanso * ptrPunto;


	/* EXEC SQL DECLARE curvpdes CURSOR FOR
	select 
		codestpul,
		ptocom, 
		posicion, 
		lado, 
		tipmesa,
		senagv,
		numagvdes,
		trunc (espacio*10)
	from vpuntodescanso; */ 

	
	/* EXEC SQL DECLARE CURTOTALTREN
	CURSOR FOR			// Xavi, 18/10/02: Afegim descartest al cursor
		select nvl (sum (nvl(descartest.numagvdes,estacion.numagvdes)), 0)
			from tren, estacion, descartest 
			where tren.codtren = estacion.codtren
			and estacion.codestpul = descartest.codestpul(+)
			and estacion.codart = descartest.codart(+)
			and estacion.estest != 'P'
			and tren.codtren = 
				(select codtren from estacion where codestpul = :dbCodEstPul); */ 


	/* EXEC SQL DECLARE CURTOTALPERMITIDO
	CURSOR FOR        // Xavi, 18/10/02: Afegim descartest al cursor
		select nvl (sum (nvl(descartest.numagvdes,estacion.numagvdes)), 0)
			from tren, estacion, descartest
			where tren.codtren = estacion.codtren
			and estacion.codestpul = descartest.codestpul(+)
			and estacion.codart = descartest.codart(+)
			and tren.estado = 'H'
			and tren.codtren in
				(select distinct (codtren) from estacion where estest = 'P'); */ 

 
	
	CSingleLock lockPuntos (&g_Container.m_csPuntoDescanso);
	lockPuntos.Lock ();

	// Borrar configuración actual

	g_Container.SetTotalAgvsAllowed (0);

	UINT numPuntos = g_Container.m_PuntoDescansoArray.GetSize ();
	for (UINT i = 0; i < numPuntos; i++)
	{
		ptrPunto = g_Container.m_PuntoDescansoArray.GetAt (0);
		delete ptrPunto;
		g_Container.m_PuntoDescansoArray.RemoveAt (0);
	}

	if (g_Container.IsEnFuncionamiento ())
	{
	
	/* EXEC SQL OPEN curvpdes; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0024;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1279;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("SetPuntosEspera () : ",sqlca,oraca);
}



	for (numAgvs = 0;;)
	{
		/* EXEC SQL FETCH curvpdes INTO 
			:dbCodEstPul,
			:dbPtoCom,
			:dbPos,
			:dbLado,
			:dbTipMesa,
			:dbSenAgv,
			:dbNumAgvDes,
			:dbUrgency; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1294;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (           int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (         void  *)dbCodEstPul;
  sqlstm.sqhstl[0] = (unsigned int  )8;
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
  sqlstm.sqhstv[2] = (         void  *)&dbPos;
  sqlstm.sqhstl[2] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)dbLado;
  sqlstm.sqhstl[3] = (unsigned int  )2;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)dbTipMesa;
  sqlstm.sqhstl[4] = (unsigned int  )2;
  sqlstm.sqhsts[4] = (         int  )0;
  sqlstm.sqindv[4] = (         void  *)0;
  sqlstm.sqinds[4] = (         int  )0;
  sqlstm.sqharm[4] = (unsigned int  )0;
  sqlstm.sqadto[4] = (unsigned short )0;
  sqlstm.sqtdso[4] = (unsigned short )0;
  sqlstm.sqhstv[5] = (         void  *)dbSenAgv;
  sqlstm.sqhstl[5] = (unsigned int  )2;
  sqlstm.sqhsts[5] = (         int  )0;
  sqlstm.sqindv[5] = (         void  *)0;
  sqlstm.sqinds[5] = (         int  )0;
  sqlstm.sqharm[5] = (unsigned int  )0;
  sqlstm.sqadto[5] = (unsigned short )0;
  sqlstm.sqtdso[5] = (unsigned short )0;
  sqlstm.sqhstv[6] = (         void  *)&dbNumAgvDes;
  sqlstm.sqhstl[6] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[6] = (         int  )0;
  sqlstm.sqindv[6] = (         void  *)0;
  sqlstm.sqinds[6] = (         int  )0;
  sqlstm.sqharm[6] = (unsigned int  )0;
  sqlstm.sqadto[6] = (unsigned short )0;
  sqlstm.sqtdso[6] = (unsigned short )0;
  sqlstm.sqhstv[7] = (         void  *)&dbUrgency;
  sqlstm.sqhstl[7] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[7] = (         int  )0;
  sqlstm.sqindv[7] = (         void  *)0;
  sqlstm.sqinds[7] = (         int  )0;
  sqlstm.sqharm[7] = (unsigned int  )0;
  sqlstm.sqadto[7] = (unsigned short )0;
  sqlstm.sqtdso[7] = (unsigned short )0;
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
  if (sqlca.sqlcode == 1403) goto SetPuntosEsperaNotFound;
  if (sqlca.sqlcode < 0) plsql_error("SetPuntosEspera () : ",sqlca,oraca);
}

 

		/* EXEC SQL OPEN CURTOTALTREN; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.stmt = sq0025;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1341;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqcmod = (unsigned int )0;
  sqlstm.sqhstv[0] = (         void  *)dbCodEstPul;
  sqlstm.sqhstl[0] = (unsigned int  )8;
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
  if (sqlca.sqlcode < 0) plsql_error("SetPuntosEspera () : ",sqlca,oraca);
}


		

		/* EXEC SQL FETCH CURTOTALTREN INTO
			:dbNumTotalTren; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1360;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (           int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (         void  *)&dbNumTotalTren;
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
  if (sqlca.sqlcode == 1403) goto SetPuntosEsperaNotFound;
  if (sqlca.sqlcode < 0) plsql_error("SetPuntosEspera () : ",sqlca,oraca);
}



		/* EXEC SQL CLOSE CURTOTALTREN; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1379;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
  if (sqlca.sqlcode < 0) plsql_error("SetPuntosEspera () : ",sqlca,oraca);
}



		dbNumAgvDes += dbNumTotalTren;
	
		// Para compensar por el posicionamiento en mesas simples cuando entra de culo
		if (dbTipMesa [0] == 'S' && dbSenAgv [0] == 'B' && dbPos > 1)
			dbPos--;
	


		CPuntoDescanso * ptrPunto = new CPuntoDescanso (dbPtoCom, 
			dbPos, 
			dbLado [0],
			dbNumAgvDes,
			dbUrgency);

		g_Container.m_PuntoDescansoArray.Add (ptrPunto);
	}


SetPuntosEsperaNotFound:

	/* EXEC SQL OPEN CURTOTALPERMITIDO; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0026;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1394;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("SetPuntosEspera () : ",sqlca,oraca);
}


	/* EXEC SQL FETCH CURTOTALPERMITIDO INTO :dbNumTotal; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1409;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqfoff = (           int )0;
 sqlstm.sqfmod = (unsigned int )2;
 sqlstm.sqhstv[0] = (         void  *)&dbNumTotal;
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
 if (sqlca.sqlcode == 1403) goto SetPuntosEsperaNotFound;
 if (sqlca.sqlcode < 0) plsql_error("SetPuntosEspera () : ",sqlca,oraca);
}


	/* EXEC SQL CLOSE CURTOTALPERMITIDO; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1428;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("SetPuntosEspera () : ",sqlca,oraca);
}



	g_Container.SetTotalAgvsAllowed (dbNumTotal);

	/* EXEC SQL CLOSE curvpdes; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1443;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("SetPuntosEspera () : ",sqlca,oraca);
}



	} // Si está en funcionamiento
	lockPuntos.Unlock ();




}


UINT CDBEstacion::GetDescarga(LPCTSTR codord)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("GetDescarga () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	const char * dbCodEstPul;
	const char * dbCodord;
	int dbRetVal;

	/* EXEC SQL END DECLARE SECTION; */ 


	dbCodord = codord;
	dbCodEstPul = m_pEstacion->codestpul;

	/* EXEC SQL EXECUTE
	begin
		:dbRetVal := map.GetDescarga (:dbCodEstPul, :dbCodord);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin :dbRetVal := map . GetDescarga ( :dbCodEstPul , :dbCod\
ord ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1458;
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
 sqlstm.sqhstv[1] = (         void  *)dbCodEstPul;
 sqlstm.sqhstl[1] = (unsigned int  )0;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)dbCodord;
 sqlstm.sqhstl[2] = (unsigned int  )0;
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
 if (sqlca.sqlcode < 0) plsql_error("GetDescarga () : ",sqlca,oraca);
}



	return (dbRetVal);
}


// Ver si se puede realizar una operación de carga en la estación
UINT CDBEstacion::GetCarga(LPCTSTR codord)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("GetCarga () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	const char * dbCodEstPul;
	const char * dbCodord;
	int dbRetVal;

	/* EXEC SQL END DECLARE SECTION; */ 


	dbCodord = codord;
	dbCodEstPul = m_pEstacion->codestpul;

	/* EXEC SQL EXECUTE
	begin
		:dbRetVal := map.GetCarga (:dbCodEstPul, :dbCodord);
		end;
	END-EXEC; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "begin :dbRetVal := map . GetCarga ( :dbCodEstPul , :dbCodord\
 ) ; end ;";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1485;
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
 sqlstm.sqhstv[1] = (         void  *)dbCodEstPul;
 sqlstm.sqhstl[1] = (unsigned int  )0;
 sqlstm.sqhsts[1] = (         int  )0;
 sqlstm.sqindv[1] = (         void  *)0;
 sqlstm.sqinds[1] = (         int  )0;
 sqlstm.sqharm[1] = (unsigned int  )0;
 sqlstm.sqadto[1] = (unsigned short )0;
 sqlstm.sqtdso[1] = (unsigned short )0;
 sqlstm.sqhstv[2] = (         void  *)dbCodord;
 sqlstm.sqhstl[2] = (unsigned int  )0;
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
 if (sqlca.sqlcode < 0) plsql_error("GetCarga () : ",sqlca,oraca);
}



	return (dbRetVal);

}

// Inicializar array de los cargadores de bareria
void CDBEstacion::InitArrayCargadores(UINT *ptrArray)
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	T_sqlca sqlca;	// This is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;

	/* EXEC SQL DECLARE CURCARAGVARRAY 
		CURSOR FOR
		SELECT ptocom 
		FROM CARAGV
		ORDER BY PRIORIDAD ASC; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		
		int dbPtoCom;

	/* EXEC SQL END DECLARE SECTION; */ 


	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("InitArrayCargadores () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO InitArrayCargadoresNotFound; */ 



	/* EXEC SQL OPEN CURCARAGVARRAY; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0029;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1512;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("InitArrayCargadores () : ",sqlca,oraca);
}



	// Michael 24.05.2007 for (int i = 0; i < 8; i ++)
	int i;
	for (i = 0; i < MAX_CARGADORES; i ++)
	{
		/* EXEC SQL FETCH CURCARAGVARRAY INTO :dbPtoCom; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1527;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (           int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (         void  *)&dbPtoCom;
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
  if (sqlca.sqlcode == 1403) goto InitArrayCargadoresNotFound;
  if (sqlca.sqlcode < 0) plsql_error("InitArrayCargadores () : ",sqlca,oraca);
}


		ptrArray [i] = dbPtoCom;
	}

InitArrayCargadoresNotFound:

	/* EXEC SQL CLOSE CURCARAGVARRAY; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1546;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("InitArrayCargadores () : ",sqlca,oraca);
}


	ASSERT (i == MAX_CARGADORES);

}

// Actualizar la configuración de los descansos para el BuB
void CDBEstacion::UpdateBuBDescansos()
{
	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	T_sqlca sqlca;	// This is static
	T_oraca oraca;
	oraca.orastxtf = ORASTFERR;

	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("UpdateBuBDescansos () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND GOTO UpdateBuBDescansosNotFound; */ 

	
	/* EXEC SQL DECLARE CURDESCANSOS 
		CURSOR FOR
		SELECT estagv.ptocom, estagv.posicion, estagv.lado, 
			estacion.codestdes, estacion.numagvdes
		FROM ESTAGV, ESTACION
		WHERE ESTAGV.CODESTPUL = ESTACION.CODESTPUL; */ 



	/* EXEC SQL BEGIN DECLARE SECTION; */ 

		int dbPtoCom;
		int dbPos;
		char dbLado [2];
		char dbCodEstDes [8];
		int dbNumAgvDes;

	/* EXEC SQL END DECLARE SECTION; */ 



	/* EXEC SQL OPEN CURDESCANSOS; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = sq0030;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1561;
 sqlstm.selerr = (unsigned short)1;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqcmod = (unsigned int )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("UpdateBuBDescansos () : ",sqlca,oraca);
}



	for (;;)
	{
		/* EXEC SQL FETCH CURDESCANSOS INTO
			:dbPtoCom,
			:dbPos,
			:dbLado,
			:dbCodEstDes,
			:dbNumAgvDes; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1576;
  sqlstm.selerr = (unsigned short)1;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlstm.sqfoff = (           int )0;
  sqlstm.sqfmod = (unsigned int )2;
  sqlstm.sqhstv[0] = (         void  *)&dbPtoCom;
  sqlstm.sqhstl[0] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[0] = (         int  )0;
  sqlstm.sqindv[0] = (         void  *)0;
  sqlstm.sqinds[0] = (         int  )0;
  sqlstm.sqharm[0] = (unsigned int  )0;
  sqlstm.sqadto[0] = (unsigned short )0;
  sqlstm.sqtdso[0] = (unsigned short )0;
  sqlstm.sqhstv[1] = (         void  *)&dbPos;
  sqlstm.sqhstl[1] = (unsigned int  )sizeof(int);
  sqlstm.sqhsts[1] = (         int  )0;
  sqlstm.sqindv[1] = (         void  *)0;
  sqlstm.sqinds[1] = (         int  )0;
  sqlstm.sqharm[1] = (unsigned int  )0;
  sqlstm.sqadto[1] = (unsigned short )0;
  sqlstm.sqtdso[1] = (unsigned short )0;
  sqlstm.sqhstv[2] = (         void  *)dbLado;
  sqlstm.sqhstl[2] = (unsigned int  )2;
  sqlstm.sqhsts[2] = (         int  )0;
  sqlstm.sqindv[2] = (         void  *)0;
  sqlstm.sqinds[2] = (         int  )0;
  sqlstm.sqharm[2] = (unsigned int  )0;
  sqlstm.sqadto[2] = (unsigned short )0;
  sqlstm.sqtdso[2] = (unsigned short )0;
  sqlstm.sqhstv[3] = (         void  *)dbCodEstDes;
  sqlstm.sqhstl[3] = (unsigned int  )8;
  sqlstm.sqhsts[3] = (         int  )0;
  sqlstm.sqindv[3] = (         void  *)0;
  sqlstm.sqinds[3] = (         int  )0;
  sqlstm.sqharm[3] = (unsigned int  )0;
  sqlstm.sqadto[3] = (unsigned short )0;
  sqlstm.sqtdso[3] = (unsigned short )0;
  sqlstm.sqhstv[4] = (         void  *)&dbNumAgvDes;
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
  if (sqlca.sqlcode == 1403) goto UpdateBuBDescansosNotFound;
  if (sqlca.sqlcode < 0) plsql_error("UpdateBuBDescansos () : ",sqlca,oraca);
}


			


		g_Container.m_GestorEstBuB.SetEstacionBuB (
			dbPtoCom,
			dbPos,
			dbLado [0],
			dbCodEstDes,
			dbNumAgvDes);

	}


UpdateBuBDescansosNotFound:
	/* EXEC SQL CLOSE CURDESCANSOS; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1611;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("UpdateBuBDescansos () : ",sqlca,oraca);
}



}



// Insertar en histórico de ocupaciones
void CDBEstacion::InsertarHistOcuEst()
{

	sql_context * ptrCtx = (sql_context *)g_Container.m_dbutil.GetOraContext ();
	/* EXEC SQL CONTEXT USE :*ptrCtx; */ 


	/* EXEC SQL BEGIN DECLARE SECTION; */ 


	const char * dbCodEstPul;

	/* EXEC SQL END DECLARE SECTION; */ 


	dbCodEstPul = m_pEstacion->codestpul;
	
	/* EXEC SQL WHENEVER SQLERROR DO plsql_error("InsertarHistOcuEst () : ", sqlca, oraca); */ 

	/* EXEC SQL WHENEVER NOTFOUND CONTINUE; */ 


	/* EXEC SQL INSERT INTO HISTOCUEST 
           (REGID
            ,CODESTPUL
            ,OCUPUL
            ,FECREG
            ,AUTOMAN
            ,SENACT
            ,PALSUM
            ,RESAGV
            ,TIPESTPUL
            ,FINSUM
            ,SUMCON
            ,PREPAL1
            ,PREPAL2
            ,MESAOP1
            ,MESAOP2
            ,ERRMESA1
            ,ERRMESA2
            ,CODPULVAC)
			SELECT HISTOCUEST_SEQ.NEXTVAL,
			 CODESTPUL
            ,OCUPUL
            ,FECREG
            ,AUTOMAN
            ,SENACT
            ,PALSUM
            ,RESAGV
            ,TIPESTPUL
            ,FINSUM
            ,SUMCON
            ,PREPAL1
            ,PREPAL2
            ,MESAOP1
            ,MESAOP2
            ,ERRMESA1
            ,ERRMESA2
            ,CODPULVAC
			FROM VOCUEST
			WHERE CODESTPUL = :dbCodEstPul; */ 

{
 struct sqlexd sqlstm;
 sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
 sqlstm.sqlvsn = 12;
 sqlstm.arrsiz = 26;
 sqlstm.sqladtp = &sqladt;
 sqlstm.sqltdsp = &sqltds;
 sqlstm.stmt = "insert into HISTOCUEST (REGID,CODESTPUL,OCUPUL,FECREG,AUTOMA\
N,SENACT,PALSUM,RESAGV,TIPESTPUL,FINSUM,SUMCON,PREPAL1,PREPAL2,MESAOP1,MESAOP2\
,ERRMESA1,ERRMESA2,CODPULVAC)select HISTOCUEST_SEQ.nextval  ,CODESTPUL ,OCUPUL\
 ,FECREG ,AUTOMAN ,SENACT ,PALSUM ,RESAGV ,TIPESTPUL ,FINSUM ,SUMCON ,PREPAL1 \
,PREPAL2 ,MESAOP1 ,MESAOP2 ,ERRMESA1 ,ERRMESA2 ,CODPULVAC  from VOCUEST where \
CODESTPUL=:b0";
 sqlstm.iters = (unsigned int  )1;
 sqlstm.offset = (unsigned int  )1626;
 sqlstm.cud = sqlcud0;
 sqlstm.sqlest = (unsigned char  *)&sqlca;
 sqlstm.sqlety = (unsigned short)256;
 sqlstm.occurs = (unsigned int  )0;
 sqlstm.sqhstv[0] = (         void  *)dbCodEstPul;
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
 sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
 if (sqlca.sqlcode < 0) plsql_error("InsertarHistOcuEst () : ",sqlca,oraca);
}



		/* EXEC SQL COMMIT; */ 

{
  struct sqlexd sqlstm;
  sqlorat(&(*(ptrCtx)), &sqlctx, &oraca);
  sqlstm.sqlvsn = 12;
  sqlstm.arrsiz = 26;
  sqlstm.sqladtp = &sqladt;
  sqlstm.sqltdsp = &sqltds;
  sqlstm.iters = (unsigned int  )1;
  sqlstm.offset = (unsigned int  )1645;
  sqlstm.cud = sqlcud0;
  sqlstm.sqlest = (unsigned char  *)&sqlca;
  sqlstm.sqlety = (unsigned short)256;
  sqlstm.occurs = (unsigned int  )0;
  sqlcxt(&(*(ptrCtx)), &sqlctx, &sqlstm, &sqlfpn);
  if (sqlca.sqlcode < 0) plsql_error("InsertarHistOcuEst () : ",sqlca,oraca);
}


}



void CDBEstacion::UpdateDefectoMesa(BYTE *bMensaje)
{

	int zona = bMensaje [1];
	int indMesa = bMensaje [2];
	SHORT defecto = bMensaje [3];
	
	#pragma message ("Don't forget to update procedure INICIARSISTEMA in package")

	if (zona > 0 && zona <= 3 && indMesa >= 0 && indMesa < 100)
	{
		SHORT oldDefecto = g_Container.m_defArray [zona - 1] [indMesa];
		if (oldDefecto != defecto)
		{
			// Mirar cada bit
			for (int i = 0; i < 5; i++)
			{
				if ((0x01 & (defecto >> i)) != (0x01 & (oldDefecto >> i)))
					if (0x01 & (defecto >> i))
					{
						// El defecto se produce
						CDBEstadistica::LogDefecto (zona, indMesa, i, false);
						/* Michael 13.08.2002
							GetStringDefecto occasionally crashes.
							According to Oracle Documentation Character pointer
							variables call strlen () to get length of buffer.
							So must be initialized */

						LPTSTR strCodMes = new CHAR [10]; 
						strcpy (strCodMes, "         ");	// Inicializar
						LPTSTR strCodClInd = new CHAR [10];
						strcpy (strCodClInd, "         ");	// Inicializar
						LPTSTR strDefecto = new CHAR [31];
						memset (strDefecto, ' ', 30);
						strDefecto [30] = '\0';
						CDBEstadistica::GetStringsDefecto (zona, 
							indMesa, 
							i, 
							strCodMes,
							strCodClInd,
							strDefecto);

						CAlarma * ptrAlarm = new CAlarma;
						ptrAlarm->AvisarAlarma (80, 1 + i, 3, zona, 0, 0, 0, 0, 0,
							strCodMes, strCodClInd);

						delete [] strCodMes;
						delete [] strCodClInd;
						delete [] strDefecto;

					}
					else
					{
						CDBEstadistica::LogDefecto (zona, indMesa, i, true);
					}

			}
			g_Container.m_defArray [zona - 1] [indMesa] = defecto;
		}
	}
	else
		MSGERROR2 ("Defecto zona: %d index Mesa: %d no encontrado",
			zona, indMesa);
}
