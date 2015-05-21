/* Copyright INDUMAT GmbH & Co. KG, 2000,2001
   Headerfile der erweiterten automatischen Blockung

Dokumentation der énderungen:
27.01.2001 Lachmund: Prototyp SetFVAusBlTab() mit bTestUmleiten
18.01.2000 Lachmund: Erstanlage, öbernahme von autoblk.h !
22.05.2000 Lachmund: define fÅr Zeitmessung in ublock
*/

#include "SituationSet.h"		//JLac
#include "CCourseData.h"
#include "BlockTableAGVTypePair.h"

#define MIT_DEADLOCK_HISTORY
#define ALLEFZKOENNENBLOCKIEREN
#define MITRANFAHREN
#define UBLOCK_ZEITMESSEN
#define ZEITSPAREN

#define	HIST_CRASH	1
#define	HIST_DEADLOCK	2
#define	HIST_ALLGEMEIN	3
// Michael 10.04.2015 Enum replaced by above as won't compile!!
//enum   T_Hist {
//	HIST_CRASH,
//	HIST_DEADLOCK,
//	HIST_ALLGEMEIN };

typedef struct{unsigned short uNr,
                            *pMpFolge,
                            *pMpBegin,
                            *pMpEnd,
                            uIndexMpFolge,
                            uAnzCrMenge,
                            *pCrMenge;
            } TypEABFzStrukt;


/* Prototypen */

#ifdef MIT_DEADLOCK_HISTORY

void WriteFzHistory(CString strFz,CStdioFile *pFile);

#endif /* MIT_DEADLOCK_HISTORY */

#ifdef MITRANFAHREN
void          LoescheVektorUndRanBits (unsigned short uFz);
void          LoescheRanBits          (unsigned short uFz);
unsigned short RanBitsGesetzt          (unsigned short uFz);
#endif /* MITRANFAHREN */

void           EABInit         (void);
void		   EABDeInit	   ();
unsigned char  SetFVAusBlTab   (TYP_FZDAT *ptAFz,unsigned char bTestUmleiten);
int            InitNoCheckTab  (void);
unsigned int   ReadBlockTab    (void);
unsigned short GetRestLaenge   (unsigned short  mp1,
                                unsigned short  mp2,
                                unsigned char   fznr);
unsigned short GetBlkWeg       (unsigned short  mp1,
                                unsigned short  mp2);
unsigned short GetBlkWegStrTab (unsigned short mp,
                                unsigned short  StreIndex);
//void           LogBlkErgebnis  (void);
unsigned short GetCrStreMenge(unsigned short *,unsigned short *);
unsigned short GetPosWegBlk    (TYP_FZDAT *ptFzDat);
void		   GetMpFolgeBisher(unsigned short uFz,
						  unsigned short *pMpFolge,
						  unsigned short uZiel);
void		   GetMpFolge(TYP_FZDAT *pFzDat,
						  unsigned short *pMpFolge,
						  unsigned short uZiel);


/* Variable */

extern CSituationSet tFVSituation;
extern CSituationSet tFESituation;

#ifdef ALLEFZKOENNENBLOCKIEREN

extern unsigned short **ppBlkFz,
                      *pALLBlkFz;
#endif

#ifdef ZEITSPAREN
extern unsigned short **pCrMengeVerz;
#endif

extern unsigned char  MitAutoBlkStop;      /* AutoBlockung erteilt Fahrverbot    */
extern unsigned char  MitAutoBlk;       /* AutoBlockung                 */
extern unsigned char  MitAWL;
extern unsigned char  MitAutoBlkLog;

extern unsigned char fz_fe_auto;
extern unsigned char auto_merke_blk_fz;

/* Defines */
#define NOCHECKTAB  ".\\NOCHECK.INI"
#define KKMAX_NOCHECK_STRE  15

/* Typedefs */
typedef struct  {
        unsigned short noCheck[KKMAX_NOCHECK_STRE];
                } NoCheckTyp;


extern unsigned short  merke_blk_fz;   // Blocking vehicle /* blockierendes Fz (wird im fzdat eingetr.)*/
extern unsigned short  blk_fznr;       // Actual vehicle numero under analisis /* Aktuelle Fz-Nummer    */
extern unsigned short  blk_error;      // Internal error number /* Interne Fehlerkennung */
extern unsigned short blk_mp;          // Actual mp /* aktueller MP          */
extern unsigned short blk_nmp;         // Next mp /* nÑchster MP           */
extern unsigned short blk_uemp;        // Over next MP /* ÅbernÑchster MP       */
extern unsigned char  fz_fe;           // Marker for stop / drive del AGV /* Merker fuer FE des Fz.          */
extern unsigned char  abs_fz_fe;       // Marker for absolute stop / drive ! /* Merker fuer absolute FE des Fz. */
extern unsigned char  tst_anz;         // Help marker is active ?? /* Hilfsanzeige ist aktiv          */


//extern unsigned long *Ver_BlStre_Root;
//extern BlStre_Typ    *BlStre_Root;
//extern BlStreExt_Typ *BlStreExt_Root;

extern CBlockTableAGVTypePair* pBlockTable;
/********************** Ende AUTOBLK.H **********************/
