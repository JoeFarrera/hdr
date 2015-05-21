/* Modulkopf Autoblockung: TabSetFV */
/*
 Copyright INDUMAT GmbH & Co. KG, 2000,2001,...2009

 Bearbeiter: J.Lachmund    Projekt : HDR_ES

 File      : TabSetFV.Cia  Pfad :\luk_arol\anlag_sw\steuer\blockung

 ƒnderungen:
 14.12.2009	Lachmund: theAutoTrafficControl neu
 28.11.2001 Lachmund: AEBGrund 147 gibt wieder FV !
					  n‰chsten Strecke des AFz zu bilden !
 17.10.2001 Lachmund: GetExtWegStrMenge() erweitert den Weg nur bis Verzweiger,
                      nicht zus‰tzlich noch desses Nachfolger ! 
 16.10.2001 Lachmund: GetEABFahrverbot() pr¸ft G¸ltigkeit von Mp, NMp und
                      UEMp des AFZ
 30.04.2001 Lachmund: von Ballina3 ¸bernommen und GetStrIndexFzT() neu !
 27.04.2001 Lachmund: Deadlock-History erweitert und Aufruf SetFVAusBlTab
                      mit bTestUmleiten !
 25.09.2000 Lachmund: AB-Gr¸nde eindeutig !
 19.07.2000 Lachmund: VFzSetFahrverbot(): nÑchste VFz-Strecke betrachten,
                      auch wenn aktuelle VFz-Strecke keinen Crash hat !
 11.05.2000 Lachmund: MITRANFAHREN
 28.04.2000 Lachmund: GetEintauchstrecke geÑndert,
                      CheckAmZielFreiZumBereich() neu
 10.04.2000 Lachmund: GetBlFzKette() nur fÅr Fahrzeuge ungleich VFz
                      aufrufen !
 02.02.2000 Lachmund: Abgeleitet von Autoblk.cia !
*/
/* Definitionen */

/* Compiler controls */
//#pragma title (" Auto-Blockung RMOS 3 ")

/* Includes */

#include "stdafx.h"
#include "tpc.h"

//#include        <string.h>
//#include        <stdio.h>
//#include        <malloc.h>
//#include        <direct.h>
//#include		"a1_konst.c86"
#include        "konstant.h"
#include        "komdat.h"
#include        "macro.h"
#include        "typedef.h"

// #include        "../include/euroskon.h"
#include        "sontab.h"
// #include        "../allgem/utility.h"
// #include        "../system/init_zs.h"
// #include        "blkglb.h"
// #include        "ublock.h"
#include "SituationSet.h"

//#include        "v_block.h"
//#include        "TabSetFV.h"

#include "AutomaticTrafficControl.h"



#include "agv.h"


CSituationSet tFVSituation;
CSituationSet tFESituation;

/* Deklarationen */

unsigned short gauM[401];

long           glCrash;
unsigned short gVIndex,
               gAIndex;



TYP_FZDAT tAFzDat; // Actual vehicle data global used in functions


unsigned short  merke_blk_fz;   // Blocking vehicle /* blockierendes Fz (wird im fzdat eingetr.)*/
unsigned short  blk_fznr;       // Actual vehicle numero under analisis /* Aktuelle Fz-Nummer    */
unsigned short  blk_error;      // Internal error number /* Interne Fehlerkennung */
unsigned short blk_mp;          // Actual mp /* aktueller MP          */
unsigned short blk_nmp;         // Next mp /* nÑchster MP           */
unsigned short blk_uemp;        // Over next MP /* ÅbernÑchster MP       */
unsigned char  fz_fe = 0;       // Marker for stop / drive del AGV /* Merker fuer FE des Fz.          */
unsigned char  abs_fz_fe;       // Marker for absolute stop / drive ! /* Merker fuer absolute FE des Fz. */
unsigned char  tst_anz;         // Help marker is active ?? /* Hilfsanzeige ist aktiv          */
unsigned short *pMpFolge;		// Vector for each communication point to destination, for all vehicles
unsigned short **pMpFolgeVerz;	// Pointer vector to pMpFolge pointer for each vehicle


/* NOTE:Implement function to write deadlock history to file
    See WriteFzHistory function for example
   History is generated to memory - must be written out to file */
#ifdef MIT_DEADLOCK_HISTORY
#ifdef ORIGINAL_STRUKTUR_BIS_27042001
            typedef struct{unsigned char  ucVFz,
                                          bFv,
                                          bVFzFv,
                                          bLast,
                                          ucStunde,
                                          ucMinute,
                                          ucSekunde;
                           unsigned short uMp,
                                          uNmp,
                                          uUemp,
                                          uZiel,
                                          uWegGefahren,
                                          uGrundAB,
                                          uGrundEAB;
                          } TypVFzFeFv;
#endif
         typedef struct{unsigned char  m_bVFzFv,
                                       m_bLast,
                                       m_ucStunde,
                                       m_ucMinute,
                                       m_ucSekunde;
                        unsigned short m_uMp,
                                       m_uNmp,
                                       m_uUemp,
                                       m_uZiel,
                                       m_uWegGefahren,
                                       m_uGrundAB,
                                       m_uGrundEAB;
                       } TypVFzInfos;
#define DL_HISTORY_LEN       10

         typedef struct{unsigned char  m_ucVFz,
                                       m_bFv,
                                       m_bFe;
                        TypVFzInfos    m_tFvInfos,
                                       m_tFeInfos;
                       } TypVFzFeFv;

         typedef struct{unsigned short uMp,
                                       uNmp,
                                       uUemp,
                                       uZiel,
                                       uErsterWegGefahren,
                                       uAktWegGefahren,
                                       uAnzBehandelt,
                                       m_ucErstStunde,
                                       m_ucErstMinute,
                                       m_ucErstSekunde,
                                       m_ucAktStunde,
                                       m_ucAktMinute,
                                       m_ucAktSekunde;
                        unsigned char  bLast;
                        TypVFzFeFv     *ptVFzFeFv;
                       } TypMpHistory;

         typedef struct{unsigned char  ucFz;
                        TypMpHistory   *ptMpHist;
                       } TypDLHistory;

         TypDLHistory  *ptDLHistoryRoot;
         TypMpHistory  *ptMpHistoryRoot;
         TypVFzFeFv    *ptVFzFeFvRoot;
#endif
#ifdef ALLEFZKOENNENBLOCKIEREN
      unsigned short **ppBlkFz,
                     *pAllBlkFz;
#endif
#ifdef MITRANFAHREN
      unsigned char *pcBitRan,
                    *pcTmpBitRan;
      unsigned short iRanVektLen;
#endif
      TypEABFzStrukt tEABaFzVirt, /* EAB-Struktur akt. Fz mit virt.MpFolge */
                     tEABaFz,     /* EAB-Struktur des aktuellen Fahrzeuges */
                     tEABvFz;     /* EAB-Struktur des Vergleichsfahrzeuges */

      unsigned char  MitAutoBlkStop = 1;/* AutoBlockung erteilt Fahrverbot    
                                        */
      unsigned char  MitAutoBlk     = 1;/* AutoBlockung                       
                                        */
      unsigned char  MitAWL         = 1;
      unsigned char  MitAutoBlkLog  = 0;
      unsigned char  ucBlockungTestAusgabe;

      unsigned char fz_fe_auto;
      unsigned char auto_merke_blk_fz;

      static unsigned short merke_blk_fznr;
      static unsigned short merke_auto_blk_fznr;

      /* Deklaration lokalen Daten */
      unsigned long  *Ver_BlStre_Root;
      //BlStre_Typ     *BlStre_Root;	JLac
      //BlStreExt_Typ  *BlStreExt_Root;	JLac
      static NoCheckTyp     *pNoCheckTab;
      static TYP_SYSLOG     sys;
      unsigned short uVStr,
                     uAStr,
                     uFeFvGrundAB,
                     uFeFvGrundEAB;
      unsigned short *pAMenge,
                     *pNeueAMenge,
                     *pFolgeStrMenge,
                     *pATest,
                     *pVStre,
                     *pgVBegin,
                     *pgVEnd,
                     *pVTest,
                     *pgTFolge,
                     uVerglFz,
                     *pCrMenge,
                     *pCrMengeCopy,     /* Copy-Zeiger fÅr Umleiten */
                     *pCrMengeUmleiten, /* Copy-Vektor fÅr Umleiten */
                     **pCrMengeVerz,
                     *pVCrMenge,
                     *pVirtCrMenge,
                     uigV,
                     uigA,
                     ugStrIndex,
                     ugStrIndex1,
                     ugStrIndex2,
                     *pVirtMpFolge,
                     *pMpFolgeUmleiten,/* Copy-Vektor fÅr Umleiten */
                     *pMpFolgeCopy;    /* Copy-Zeiger fÅr Umleiten */
      unsigned char  bUmdispoGesperrt;	// Michael: Change of order is not allowed
/*~ BLOCK */
      char StrCrashTxt[30],
           acFzFF[30];
/*~ ENDCONTRACTION */
/*~ CONTRACTION */
   /*~
   Prototypen
   ~*/
/*~ CONTRACTION */
      /*~
      FÅr erweiterte Autoblockung !
      ~*/
#ifdef MIT_DEADLOCK_HISTORY
         void NeuesHistEreignis(unsigned short uFz,
                                unsigned short uMp,
                                unsigned short uNmp,
                                unsigned short uUemp,
                                unsigned short uZiel,
                                unsigned short uWegGefahren,
                                unsigned char  bLast);
#endif
         static unsigned char SetDeadLockFahrverbot(unsigned char bTestUmleiten
         );
		 BOOL MpInCrStrMenge(unsigned short nMpSel,
						     unsigned short *pMenge);
 

         //long           StreckenCrash (unsigned short *,unsigned short *);
         short          GetKollWeg(unsigned short);
         short          GetVBlockWeg(unsigned short uIndex);
         short GetStrecke(unsigned short,unsigned short*,unsigned short*);
         unsigned short *ElemInMenge(unsigned short *,unsigned short);
         void  CopyMenge(unsigned short*,unsigned short*);
         void  MoveMenge(unsigned short*,unsigned short*);
         unsigned char  MengeErweitern(unsigned short*,unsigned short);
         unsigned short GetIndexMpFolge(unsigned short *pFolge,unsigned short 
         uMp);
         unsigned short *GetMpFolgeEnd(unsigned short*);
         unsigned short ErweiternCrStreMenge(unsigned short *,unsigned short *);
		 unsigned short ErweiternCrStreMengeIdx(unsigned short *,unsigned short *);
         unsigned short GetFreeStrecke(unsigned short *pVWeg,
                                       unsigned short *pBWeg,
                                       unsigned short uBStrIndex,
                                       unsigned short *pBlkWeg);
         unsigned char GetEABFahrverbot(TypEABFzStrukt tAFz,
                                         TypEABFzStrukt tVFz);
		void GetMpFolgeMp(unsigned short uMp,
						  unsigned short *pMpFolge,
						  unsigned short uZiel);

      /* Fuer automatische Blockung */
		 void ErweiternMpFolgeIdx(unsigned short *pWeg,unsigned short *pMpFolge);
         //long           GetCrashIndex        (unsigned short uAStrIndex,
         //                                     unsigned short uVStrIndex);
         unsigned char SetCrashFahrverbot   (TYP_FZDAT *ptAFz,
                                              TYP_FZDAT *ptVFz);
         unsigned short *GetBlMenge          (unsigned short uStrIndex);
         unsigned short GetEintauchStrecke(unsigned short *pWeg,
                                           unsigned short *pMenge,
                                           unsigned short *pWStrMenge,
                                           unsigned short uFzWeg,
                                           unsigned char  bFzAmZiel,
                                           unsigned char  *pFzBlocktVorher);
         unsigned char   VFzIstKollisionsfrei(TypEABFzStrukt tVFz,
                                              TypEABFzStrukt tAFz,
                                              unsigned short *pMenge);
         void            ZeigeMenge         (unsigned short *pMenge);
         void            FreeMenge          (unsigned short *puMenge);
         unsigned short *GetNextDurchschnitt( unsigned short *pWeg,
                                              unsigned short *pCrMenge);
         static unsigned char BlkWegFrei (unsigned short uVFzWeg,
                                          TYP_FZDAT      *ptAFz,
                                          unsigned short uNAStrIndex,
                                          CCrashDistance *ptCrash);
         static unsigned char VFzIstFrei (TYP_FZDAT     *ptFz,
                                          unsigned short uFzWeg,
										  CCrashDistance    *ptCrash,
                                          unsigned short uAStrIndex,
                                          unsigned short uAFzWeg);
		 static CCrashDistance *GetGegenCrash(CCrashDistance     *ptCrash,
                                                unsigned short uStrIndex);
         //static BlStreExt_Typ *GetGegenCrashExt(BlStre_Typ     *ptCrash,
         //                                       unsigned short uStrIndex);
         static unsigned char  FzAmZiel        (TYP_FZDAT *ptAFz);
         static unsigned char  VFzIstFreiAmZiel(unsigned short uWegVFz,
												CCrashDistance  *pBlStreExt);
         static unsigned short GetStrIndex     (unsigned short  mp1,
                                                unsigned short  mp2);
         static unsigned short GetStrIndexFzT (unsigned short  mp1,
                                               unsigned short  mp2);
         static unsigned short GetStrLaenge    (unsigned short  mp1,
                                                unsigned short  mp2);

		 static unsigned short KollisionswegOK(unsigned char bFz, CCrashDistance* pBlStre,unsigned short StrIndexBlkFz);

         static short VorausFzOK(unsigned char bFz, unsigned short BlkStreIndex
         );
         static short StreckeNichtPruefen(unsigned short AktStreindex,unsigned 
         short BfzStreIndex);
         static unsigned short GetFzWeg (TYP_FZDAT *ptAFz);
         static unsigned short GetNewSegment(FILE *fp);
         static unsigned short ReadNoCheckSegments(unsigned short nStreIndex, 
         FILE *fp);
         //static unsigned char  VFzSetFahrverbot  (TYP_FZDAT      *ptAFz,
         //                                       unsigned short uAFzWeg,
         //                                         unsigned short uAnzBlkStr,
         //                                       unsigned short uNAnzBlkStr,
         //                                       unsigned short uAStreIndex,
         //                                         unsigned short uNAStreIndex,
		 //									  CCrashDistance     *pBlStre,
		 //										  CCrashDistance     *pNBlStre,
         //                                         TYP_FZDAT      *ptVFz);
/*~ ENDCONTRACTION */
//      static unsigned short FzAufBlStre       (TYP_FZDAT  *ptVFz,
//											   CCrashDistance *pBlStre,
//                                               unsigned short uAnzBlkStr);
      static unsigned char StreckenPaarPruefen(unsigned short uAStreIndex,
                                               unsigned short uVStreIndex);
      unsigned short *NewMenge                (unsigned short uAnzElemente);
      unsigned short *MengenDifferenz         (unsigned short *puM1,
                                               unsigned short *puM2);
      unsigned short *MengenDurchschnitt      (unsigned short *puM1,
                                               unsigned short *puM2);
      unsigned char MengeVereinen             (unsigned short *psource,
                                               unsigned short *pdest);
      //long          StreckenCrash             (unsigned short *pA,
      //                                         unsigned short *pV);
      unsigned char GetBlFzKette              (unsigned short uFz,
                                               unsigned short uVFz);
      unsigned short GetFzInCrashMenge        (unsigned short *pCrMenge,
                                               unsigned short *pCrFz,
                                               unsigned short uAFz);
      unsigned short *GetWegStrMenge          (unsigned short *pWeg);
	  unsigned short *GetWegStrMengePlus      (unsigned short *pWeg);
	  unsigned short *GetExtWegStrMenge		  (unsigned short *pWeg);
      unsigned char  VorbeiFahren             (TypEABFzStrukt *ptAFz,
                                               unsigned short *pCrMenge,
                                               unsigned short uVStrIndex,
                                               unsigned short uVFzWeg);
      unsigned char  FzIstVoraus              (TypEABFzStrukt *ptAFz,
                                               unsigned short *pCrMenge,
                                               unsigned short uVStrIndex,
                                               unsigned short uVFzWeg,
                                               unsigned short uBlkWegVFz);
      unsigned short GetAnzMengenElem         (unsigned short *pMenge);
      unsigned char WegGehtAusMenge           (unsigned short *pWeg,
                                               unsigned short *pMenge);
unsigned short *GetKollStrMenge     (unsigned short uStrIndex,
                                     unsigned short *pDMenge);
unsigned short *GetFolgeMenge       (unsigned short uMp);
unsigned char AlleStrWerdenFrei     (unsigned short *pMenge  , unsigned short uIndex);
unsigned char FzWegIstFreiZurStrecke(unsigned short uStrIndex, unsigned short *pWeg);
unsigned short *GetTeilMpFolge(unsigned short *pMpF,
                               unsigned short *pVMpF,
                               unsigned char  *pbEnde);
unsigned char CheckBlkWeg           (CCrashDistance* pCrash,
									 unsigned short nMp,
									 unsigned short nStreIndex);

/*~ STRUCT */
#ifdef MITRANFAHREN
      void           LoescheRanVektorBit      (unsigned short uVektFz,
                                               unsigned short uBitFz);
#endif

	  
/* Call the DeadLock function. May be more here in the future	 */ 
unsigned char SetFVAusBlTab(TYP_FZDAT *ptAFz,unsigned char bTestUmleiten)
{
	unsigned char  ucRet;
	
	/* Erweiterte Autoblockung, um Deadlocks vermeiden!
	   Extended autoblock version */

	if(ptAFz->fzziel.nDest)
	{
		// Auto-Blockung nur aufrufen, wenn Ziel vorhanden.
		blk_mp   = ptAFz->mp;
		blk_nmp  = ptAFz->nmp;
		blk_uemp = ptAFz->uemp;
		
		blk_fznr = ptAFz->fznr;
		
		memcpy ((void *)&tAFzDat, ptAFz, sizeof (tAFzDat)); // Copy vehicle in global for debugging purposes
		ucRet = SetDeadLockFahrverbot(bTestUmleiten);
		//ucRet = FALSE;
		
		if(ucRet)
		{
			ptAFz->fz_blk = (UCHAR) auto_merke_blk_fz;
		}
		else
		{
			ptAFz->fz_blk = 0;
		}
	}
	else
	{
		// Lac, 09.11.01:
		// Fz mit Ziel 0: Fahrverbot
		// Normalerweise wird die Blockungsfunktion nicht aufgerufen
		// f¸r ein Fz, das am Ziel ist. Aber bei HDR_ES wird die Funktion
		// mit fehlehaften Fahrzeugdaten aufgerufen,
		// wenn gelegentlich ein Fz-Telegramm fehlerhaft empfangen wird
 		ptAFz->fz_blk = 0;
		ucRet         = 1;
	}
	
	return ucRet;
}


int InitNoCheckTab(void)
{
   FILE                    *fp;
   unsigned short  nStreIndex;

   errno_t error = fopen_s(&fp, NOCHECKTAB, "r");
   if (error != 0)
   {
      printf("\nDatei %s nicht gefunden",NOCHECKTAB);
      pNoCheckTab = NULL;
      return 0;
   }

   if (pNoCheckTab == NULL)
   {
      //pNoCheckTab = (NoCheckTyp *)calloc(1,(KKMAX_FAHR_ZEIT+1)*sizeof(NoCheckTyp));
	   pNoCheckTab = (NoCheckTyp *)calloc(1, (g_pCourseData->GetNumberAIDCpDists() + 1)*sizeof(NoCheckTyp));
	  if (pNoCheckTab == NULL)
      {
         printf("Zu wenig Speicher fÅr No-Check-tabelle");
         fclose(fp);
         return 0;
      }
   }
   else
   {
      //memset(pNoCheckTab,0,(KKMAX_FAHR_ZEIT+1)*sizeof(NoCheckTyp));
	   memset(pNoCheckTab, 0, (g_pCourseData->GetNumberAIDCpDists() + 1)*sizeof(NoCheckTyp));
   }
   printf("\nNo-Check-Tabelle %s einlesen.. ",NOCHECKTAB);

   while ( (nStreIndex = GetNewSegment(fp)) != 0 )
   {
      ReadNoCheckSegments(nStreIndex,fp) ;
   }

   fclose(fp);
   printf("OK.");
   return 1;
}
/*
void FreeBlockTab()
{
   if (Ver_BlStre_Root != NULL)
   {
      free(Ver_BlStre_Root);
      Ver_BlStre_Root = NULL;
   }
   if (BlStre_Root != NULL)
   {
      free(BlStre_Root);
      BlStre_Root = NULL;
   }
   if (BlStreExt_Root != NULL)
   {
      free(BlStreExt_Root);
      BlStreExt_Root = NULL;
   }
   return;
}
*/
/*************************************************************************/
/* Pruefung Blocktabelle */

static unsigned int CheckBlockTab(unsigned int nAnzStrecken)
{
  unsigned int //i,
               //j,
               //k,
               //index1,
               //ok,
               uBlTabOk = 1;
  /*
  // Schleife ¸ber alle Strecken der Tabelle 
  for (i = 0,uBlTabOk = 1;
       i < nAnzStrecken;
       i++)
  {
    // Schleife ¸ber alle blockierenden Strecke der i-ten Strecke
    for (j = Ver_BlStre_Root[i];
         j < Ver_BlStre_Root[i+1];
         j++)
    {
      index1 = BlStre_Root[j].index-1;   //XXX old [j-1]
      // Schleife ¸ber alle blockierenden Strecken der j-ten blockierenden
      // Strecke der i-ten Strecke
      for (k = Ver_BlStre_Root[index1], ok = 0;
           (k < Ver_BlStre_Root[index1+1]) && !ok;
           k++)
      {
        if (BlStre_Root[k].index == (i+1))  //XXX old [k-1]
        {
          // die i-te Strecke ist blockierend zur j-ten Strecke, das ist ok !
          ok = 1;
        }
      }
      if (!ok)
      {
        printf("\n%d.Strecke blockt %d.Strecke, aber nicht umgekehrt !", index1+1,i+1);
        uBlTabOk = 0;
      }
    }
  }
  if (uBlTabOk)
  {
    printf("\nBlock-Tab ist ok !");
  }
  else
  {
    printf("\nBlock-Tab ist unvollstaendig !");
  }
  JLac */
  return uBlTabOk;
}

/*
unsigned short GetBlkWeg (unsigned short  mp1,
                          unsigned short  mp2)
{
   // Zeiger auf Streckentabelle des MP
   Meldepunkttabelle *standtab;

   // Zeiger auf NMP-Tabelle 
   NMP_tabelle       *mp_nmp_tab;

   unsigned char   h_nmp;
   long            weg = 0;
   if (mp2 == 0 && vtstat[mp1] != 0xffff)
   {
	   mp2 = CAgv::GetNmpWeg (mp1,mp1,&weg);
   }
   if (mp2 != 0)
   {
      // Zeiger auf MP in der MP-DatenTabelle
      standtab = mptab + vmptab [mp1] - 1;

      // Zeiger auf Tabelle der naechsten Meldepunkte
      mp_nmp_tab = MpNmpTab + standtab->str_idx - 1;
      h_nmp = 0;
      // so lange suchen bis Ende oder nmp gefunden
      while ((mp_nmp_tab->nmp != mp2) &&
             (h_nmp < standtab->anz_nmp))
      {
         mp_nmp_tab++;
         h_nmp++;
      }
      if (mp_nmp_tab->nmp == mp2)
      {
         //Blockweg runden
          return mp_nmp_tab->blk_weg ;
      }
      else
      {
         return 0;
      }
   }
   else
   {
      return 0;
   }
}
*/

unsigned short GetBlkWeg(unsigned short  mp1, unsigned short  mp2)
{
	unsigned short nBlkWay = 0;

	CCourseAGVType* pCourseAGVType = g_pCourseData->GetPtrCourseAGVType(1);
	int indexMp1 = pCourseAGVType->GetCommPointIndex(mp1);
	if ((mp2 == 0) && (indexMp1 > 0))
	{
		long weg = 0;
		mp2 = CAgv::GetNmpWeg(mp1, mp1, &weg);
	}
	if (mp2 != 0)
	{
		unsigned short nIndex = pCourseAGVType->GetDistIndex(mp1, mp2);
		if (nIndex > 0)
		{
			nBlkWay = pCourseAGVType->GetPtrDistance(nIndex)->GetBlockWay();
		}
	}

	return nBlkWay;
}
/*
void LogBlkErgebnis(void)
{
   //Deklarationen
      typedef struct {
      unsigned char fea,feh;
      unsigned short blk_fz,fz_blk;
      unsigned short mp1,mp2;
      unsigned short nmp1,nmp2;
      unsigned short uemp1,uemp2;
      unsigned short weg1, weg2;
       } T_BLKERG;
      FILE *fp;
      unsigned short fz;
      static T_BLKERG BlkErgAlt,BlkErgNeu;
   if (fz_fe == 0)
   {
      fz = merke_blk_fz;
   }
   else
   {
      fz = auto_merke_blk_fz;
   }
   BlkErgNeu.fea    = fz_fe_auto;
   BlkErgNeu.feh    = fz_fe;
   BlkErgNeu.blk_fz = blk_fznr;
   BlkErgNeu.fz_blk = fz;
   BlkErgNeu.mp1    = blk_mp;
   BlkErgNeu.mp2    = fzdat[fz].mp;
   BlkErgNeu.nmp1   = blk_nmp;
   BlkErgNeu.nmp2   = fzdat[fz].nmp;
   BlkErgNeu.uemp1  = blk_uemp;
   BlkErgNeu.uemp2  = fzdat[fz].uemp;
   BlkErgNeu.weg1  = tAFzDat.wegstr;
   BlkErgNeu.weg2  = fzdat[fz].wegstr;

   if ( memcmp (&BlkErgNeu,&BlkErgAlt,sizeof(T_BLKERG) ) != 0)
   {
      memcpy(&BlkErgAlt,&BlkErgNeu,sizeof(T_BLKERG));
	  errno_t error = fopen_s(fp, ".\\blklog.txt", "a");

	  if ( error == 0 )
      {
         //fprintf(fp,"\nBlokungsdifferenzen AutoBlk <-> HandBlk\n");
         fprintf(fp,
         "\n\nAutoBlk: %d, blk_fznr:%2d, Strecke: %4d->%4d->%4d, Weg: %3d"
         , fz_fe_auto,blk_fznr,blk_mp,blk_nmp,blk_uemp,BlkErgNeu.weg1);

         fprintf(fp,
         "\nHandBlk: %d,   fz_blk:%2d, Strecke: %4d->%4d->%4d, Weg: %3d",
         fz_fe,fz,fzdat[fz].mp,fzdat[fz].nmp,fzdat[fz].uemp,BlkErgNeu.weg2
         );

		 fclose (fp);
      }
      else
      {
         //sys.meld_nr = 32;
         //sys.var_anz = 0;
         //syslog(&sys);
      }
   }
}
*/

/*
unsigned short GetPosWegBlk(TYP_FZDAT *ptFzDat)
{
   //lokale Variablen
      unsigned char eb_typ = 0,
                    z_index = 0,
                    z_typ_index = 0,
                    z_typ = 0,
                    pos_anz;

      unsigned short usPosWeg;

      unsigned int ziel;
      TYPE_STATION_DEF *pZiel,
                      tZiel;

	if (ptFzDat->fzziel.nDest)
   {
      pZiel = &(ptFzDat->fzziel);
   }
   else
   {
      tZiel.nDest    = ptFzDat->mp;
      tZiel.nPos   = ptFzDat->mppos;
      tZiel.nLayer = 0;
      tZiel.ucSide  = ptFzDat->mplage;
      pZiel       = &tZiel;
   }
   ziel = vtstat[pZiel->nDest];

   if (ziel > 0 && ziel <= KMAX_STATIONEN)
   {
      //Index in Zieltyptabelle berechnen ( Å¸ber Zieltypverweistabelle )
      if (pZiel->ucSide == 'L')
      {
         // linkes Ziel
         z_index = zt_ver_tab[statdat[ziel].zieltyp_l];
         pos_anz = statdat[ziel].pos_anz_l;
      }
      else
      {
         if (pZiel->ucSide == 'R')
         {
            // rechtes Ziel
            z_index = zt_ver_tab[statdat[ziel].zieltyp_r];
            pos_anz = statdat[ziel].pos_anz_r;
         }
      }
   }
   if ((z_index != 0) && (pZiel->nPos > 0) && (pZiel->nPos <= pos_anz))
   {
      //Posweg ermitteln (z_index + pos - 1)
      usPosWeg = z_typ_tab[z_index+pZiel->nPos - 1].pos_weg;

	  if (usPosWeg == 1)
      {
         usPosWeg = GetBlkWeg(pZiel->nDest, 0);
      }
   }
   else
   {
      usPosWeg = 0xFFFF;
   }
   return usPosWeg;
}
*/

unsigned short GetPosWegBlk(TYP_FZDAT *ptFzDat)
{
	TYPE_STATION_DEF *pZiel,
		tZiel;

	if (ptFzDat->fzziel.nDest)
	{
		pZiel = &(ptFzDat->fzziel);
	}
	else
	{
		tZiel.nDest = ptFzDat->mp;
		tZiel.nPos = ptFzDat->mppos;
		tZiel.nLayer = 0;
		tZiel.ucSide = ptFzDat->mplage;
		pZiel = &tZiel;
	}
	unsigned short usPosWeg = g_pCourseData->GetPtrCourseAGVType(1)->GetPosWay(*pZiel);

	if (usPosWeg == 1)
	{
		usPosWeg = GetBlkWeg(pZiel->nDest, 0);
	}

	return usPosWeg;
}

#ifdef MITRANFAHREN
void LoescheVektorUndRanBits(unsigned short uFz)
{
	unsigned char  *pcFzBitRan;
	//Vektor des uFz nullen
	pcFzBitRan = pcBitRan + (iRanVektLen * uFz);
	memset(pcFzBitRan, 0, iRanVektLen);

	LoescheRanBits(uFz);

	return;
}

void LoescheRanBits(unsigned short uFz)
{
	unsigned short uVFz;
	//RanBit des uFz in allen Vektoren nullen ! 
	for (uVFz = 1;
		//uVFz <= KKFZMAX;
		uVFz <= g_pCourseData->GetNumberAGVs();
		uVFz++)
	{
		LoescheRanVektorBit(uVFz, uFz);
	}

	return;
}

unsigned short RanBitsGesetzt(unsigned short uFz)
{
	unsigned short  uRet;
	unsigned char  *pcFzBitRan,
		ucBitMaske;
	unsigned short uVFz;
	div_t          dtRan;

	dtRan = div(uFz, 8);
	ucBitMaske = 1 << dtRan.rem;
	//Alle RanBits des uFz auf Null testen ! 
	for (uVFz = 1, pcFzBitRan = pcBitRan + iRanVektLen + dtRan.quot, uRet = 0;
		//(uVFz <= KKFZMAX) && !uRet;
		(uVFz <= g_pCourseData->GetNumberAGVs()) && !uRet;
	uVFz++, pcFzBitRan += iRanVektLen)
	{
		if (*pcFzBitRan & ucBitMaske)
		{
			uRet = uVFz;
		}
	}

	return uRet;
}

void LoescheRanVektorBit(unsigned short uVektFz,
	unsigned short uBitFz)
{
	div_t         dtRan;
	unsigned char *pcFzBitRan;

	dtRan = div(uBitFz, 8);
	pcFzBitRan = pcBitRan + (iRanVektLen * uVektFz);
	*(pcFzBitRan + dtRan.quot) &= ~(1 << dtRan.rem);

	return;
}
#endif

/*
static unsigned char VFzSetFahrverbot(TYP_FZDAT      *ptAFz,
                                      unsigned short uAFzWeg,
                                      unsigned short uAnzBlkStr,
                                      unsigned short uNAnzBlkStr,
                                      unsigned short uAStreIndex,
                                      unsigned short uNAStreIndex,
                                      BlStre_Typ     *pBlStre,
                                      BlStreExt_Typ  *pBlStreExt,
                                      BlStre_Typ     *pNBlStre,
                                      BlStreExt_Typ  *pNBlStreExt,
                                      TYP_FZDAT      *ptVFz)
{
   unsigned char  ucRet = 0,
                  bVFzIstFrei,
                  bBlkWegFrei,
                  bVFzFreiZurAStreA,
                  bVFzFe;
   unsigned short uCrash;
   unsigned short uVStreIndex,
                  uNVStreIndex,
                  uVFzWeg,
                  uABlkWegTab,
				  uVBlkWegTab,
                  uPosWeg;
   BlStre_Typ     *ptCrash,
                  *pGCrash;
   BlStreExt_Typ  *ptCrashE,
                  *pGCrashExt;
   TYP_FZDAT      tVFz;
   long           lCrash;

   tVFz   = *ptVFz;
   bVFzFe = !FZ_FAHRVERBOT(tVFz);
   // Aktuelle Strecke betrachten
      if (!tVFz.nmp)
      {
         // VFz hat keine Strecke, es mu· eine her !
		  tVFz.nmp = CAgv::GetPNmp (tVFz.mp,statdat[1].mp)->nmp;
      }
      uVStreIndex = GetStrIndexFzT(tVFz.mp,tVFz.nmp);
      uVFzWeg     = GetFzWeg(&tVFz);

      if (tVFz.uemp)
      {
         uNVStreIndex = GetStrIndexFzT(tVFz.nmp,tVFz.uemp);
      }
      else
      {
         uNVStreIndex = 0;
      }
       
      uCrash = FzAufBlStre(&tVFz,pBlStre,uAnzBlkStr);
      uABlkWegTab = GetBlkWegStrTab(ptAFz->mp,uAStreIndex+1);
      bVFzFreiZurAStreA = 0;

      if (uCrash)
     {
        ptCrash  = pBlStre    + uCrash - 1;
        ptCrashE = pBlStreExt + uCrash - 1;
        // Fahrzeugpaar gefunden !
           if (StreckenPaarPruefen(uAStreIndex+1,ptCrash->index))
           {
              // Streckenpaar soll gepr¸Åft werden, da kein Eintrag in NOCHECK.INI !
                 bVFzIstFrei = VFzIstFrei(&tVFz, uVFzWeg, ptCrash, ptCrashE, uAStreIndex+1, uAFzWeg);
                 if (!bVFzIstFrei)
                 {
                    // VFz blockiert:
                       if ((uAStreIndex+1) == uNVStreIndex)
                       {
                          if (pNBlStre)
                          {
                             //AFz hat eine n‰chste Strecke !
                                lCrash  = GetCrashIndex((uAStreIndex+1
                                ),(uNAStreIndex+1));
                                ptCrash = BlStre_Root + lCrash - 1;

                                if (ptCrash->blockweg || ptCrash->
                                kollisionsweg)
                                {
                                   uFeFvGrundAB = 11;
                                }
                                else
                                {
                                   ucRet = 1;
                                   uFeFvGrundAB = 12;
                                }
                          }
                          else
                          {
                             ucRet = 1;
                             uFeFvGrundAB = 10;
                          }
                       }
                       else
                       {
                          // Feststellen, ob AFz blockungsm‰ssig vor VFz f‰hrt !
                             pGCrash    = GetGegenCrash   (ptCrash, 
                             uAStreIndex+1);
                             pGCrashExt = GetGegenCrashExt(ptCrash, 
                             uAStreIndex+1);
                             if(pGCrashExt == 0)
                             {
                                 uAStreIndex++;
                                 uAStreIndex--;
                             }
                             if (pGCrashExt->uKWegFuerBlWeg)
                             {
                                // AFz kann den Blockweg des VFz freifahren !
                                   if ((uAFzWeg >= pGCrashExt-> uKWegFuerBlWeg) ||
                                       (!ptCrash->blockweg || (ptCrash->blockweg < uABlkWegTab)))
                                   {
                                     if (ptCrash->blockweg)
                                     {
                                       // AFz hat einen Blockweg !
                                       if ((ptCrash->blockweg < uABlkWegTab))
                                       {
                                         // AFz hat einen zu kurzen Blockweg !
                                         if (ptCrashE->uKWegFuerBlWeg)
                                         {
                                           // VFz hat den Blockweg des AFz freigemacht: AFz kann blocken !
                                           ucRet = 1;
                                           uFeFvGrundAB = 26;

                                         }
                                         else
                                         {
                                           uFeFvGrundAB = 14;
                                           bVFzFreiZurAStreA = 1;
                                         }
                                       }
                                       else
                                       {
                                       ucRet = 1;
                                       uFeFvGrundAB = 9;
                                       }
                                     }
                                     else
                                     {
                                     // AFz hat keinen Blockweg !
                                     if (pGCrash->blockweg && !bVFzFe)
                                     {
                                     uFeFvGrundAB = 25;
                                     }
                                     else
                                     {
                                     // AFz f‰hrt blockungsm‰ssig vor VFz
                                     //uFeFvGrundAB = 13;
                                     //bVFzFreiZurAStreA = 1;

									 // 27.04.2000 EE-la: Kollisionswege untersuchen !
                                     if (ptCrash->kollisionsweg && !(pGCrash->kollisionsweg))
                                     {
										 //VFz kann frei werden, AFz nicht !

										 //AFz f‰hrt blockungsm‰ssig hinter 
										 //VFz
										 ucRet = 1;
										 uFeFvGrundAB = 22;
                                     }
                                     else
                                     {
										 if (pGCrash->kollisionsweg &&
											 !(ptCrash->kollisionsweg))
										 {
											 //AFz kann frei werden, VFz nicht !
											 //AFz f‰hrt blockungsm‰ssig vor VFz
											 uFeFvGrundAB = 23;
											 bVFzFreiZurAStreA = 1;
										 }
										 else
										 {
											 if (pGCrash->kollisionsweg &&
												 ptCrash->kollisionsweg)
											 {
												 //Beide kˆnnen frei werden !
												 //Fahren !
												 uFeFvGrundAB = 24;
												 bVFzFreiZurAStreA = 1;
												 }
											 else
											 {
												 //Keiner kann frei werden !
												 //Stopp !
												 ucRet = 1;
												 uFeFvGrundAB = 25;
											 }
										 }
                                     }
                                     }
                                     }
                                   }
                                   else
                                   {
                                     ucRet = 1;
                                     uFeFvGrundAB = 2;
                                   }
                             }
                             else
                             {
                                //AFz kann den Blockweg des VFz nicht 
                                //freifahren !
                                   if (ptCrash->blockweg)
                                   {
                                    //AFz hat einen Blockweg !
                                     if ((ptCrash->blockweg < 
                                     uABlkWegTab))
                                     {
                                     //AFz hat einen zu kurzen Blockweg!
                                     if (ptCrashE->uKWegFuerBlWeg ||
                                         ptAFz->mppos)
                                     {
                                     //VFz hat den Blockweg des AFz freigemacht !
                                     //AFz kann blocken !
                                     ucRet = 1;
                                     uFeFvGrundAB = 27;
                                     }
                                     else
                                     {
                                     uFeFvGrundAB = 28;
                                     bVFzFreiZurAStreA = 1;
                                     }
                                     }
                                     else
                                     {
                                     ucRet = 1;
                                     uFeFvGrundAB = 29;
                                     }
                                   }
                                   else
                                   {
                                     //AFz hat keinen Blockweg !
                                     //AFz f‰hrt blockungsm‰ssig vor VFz:
                                     //uFeFvGrundAB = 13;
                                     //bVFzFreiZurAStreA = 1;
                                     //14.03.2000 EE-la:
                                     //Kollisionswege untersuchen !
                                     if (ptCrash->kollisionsweg &&
                                         !(pGCrash->kollisionsweg))
                                     {
                                     //VFz kann frei werden, AFz nicht !
                                     //AFz fÑhrt blockungsmÑssig hinter VFz
                                     ucRet = 1;
                                     uFeFvGrundAB = 31;
                                     }
                                     else
                                     {
                                     if (pGCrash->kollisionsweg &&
                                         !(ptCrash->kollisionsweg))
                                     {
                                     //AFz kann frei werden, VFz nicht !
                                     //AFz fÑhrt blockungsmÑssig vor VFz
                                     uFeFvGrundAB = 32;
                                     bVFzFreiZurAStreA = 1;
                                     }
                                     else
                                     {
                                     if (pGCrash->kollisionsweg &&
                                         ptCrash->kollisionsweg)
                                     {
                                     //Beide kˆnnen frei werden !
                                     //Fahren !
                                     uFeFvGrundAB = 33;
                                     bVFzFreiZurAStreA = 1;
                                     }
                                     else
                                     {
                                     //Keiner kann frei werden !
                                     if (pGCrash->blockweg)
                                     {
                                     if ((tVFz.nmp == ptAFz->nmp) &&
                                         uNAStreIndex)
                                     {
                                     lCrash = GetCrashIndex
                                     (uVStreIndex,uNAStreIndex+1);

									 glCrash = lCrash;
                                     gVIndex = uVStreIndex;
                                     gAIndex = uNAStreIndex;

									 ptCrash = BlStre_Root + lCrash -1;

									 if (ptCrash->kollisionsweg)
                                     {
                                     ucRet = 0;
                                     uFeFvGrundAB = 34;
                                     }
                                     else
                                     {
                                     ucRet = 1;
                                     uFeFvGrundAB = 37;
                                     //Stopp !
                                     }
                                     }
                                     else
                                     {
                                     ucRet = 1;
                                     uFeFvGrundAB = 38;
                                     //Stopp !
                                     }
                                     }
                                     else
                                     {
                                     ucRet = 1;
                                     uFeFvGrundAB = 39;
                                    //Stopp !
                                     }
                                     }
                                     }
                                     }
                                   }
                             }
                       }
                 }
                 else
                 {
                    //VFz ist frei zur aktuellen Strecke des AFz!
                    bVFzFreiZurAStreA = 1;
                 }

#ifdef EE_la19072000
              //EE_la19072000
              //Diese Struktur wurde eine Ausblendung hîher kopiert, um 
              //sie immer auszufÅhren !
                 if (!ucRet && bVFzFe)
                 {
                    //EE-la: 27.06.2000
                    //Noch kein Fahrverbot und VFz hat FE: nÑchste 
                    //Strecke des VFz zur aktuellen Strecke des AFz 
                       lCrash  = GetCrashIndex((uAStreIndex+1),
                       uNVStreIndex);
                       if (lCrash)
                       {
                          ucRet        = 1;
                          uFeFvGrundAB = 18;
                       }
                 }
#endif
           }
     }
         if (!ucRet && bVFzFe)
         {
            //EE-la: 26.06.2000
            //Noch kein Fahrverbot und VFz hat FE: nÑchste Strecke des VFz 
            //zur aktuellen Strecke des AFz
               lCrash  = GetCrashIndex((uAStreIndex+1),uNVStreIndex);
               if (blk_fznr == 1)
               {
                  ucRet = 0;
               }
               if (lCrash)
               {
                 //EE_la,21.07.2000:
                  //ucRet        = 1;
                  //uFeFvGrundAB = 18;
                  ptCrash = BlStre_Root + lCrash - 1;
                  if (ptCrash->blockweg &&
                      (uAFzWeg <= ptCrash->blockweg) &&
                      (ptCrash->blockweg < uABlkWegTab))
                  {
                     ucRet        = 1;
                     uFeFvGrundAB = 40;
                  }
               }
               if (!ucRet)
               {
                  if (bVFzFe)
                  {
                     //EE-la: 18.10.2000
                     //Noch kein Fahrverbot und VFz hat FE: nÑchste Strecke 
                     //des VFz zur aktuellen Strecke des AFz
                        lCrash  = GetCrashIndex(uNVStreIndex,(uAStreIndex+
                        1));
                        if (lCrash)
                        {
                           if (tVFz.nmp == tVFz.fzziel.nr)
                           {
                              //Wird VFz am Ziel frei sein ?
                                 ptCrash = BlStre_Root + lCrash - 1;
                                 uPosWeg =  GetPosWegBlk(&tVFz);
                                 if (uPosWeg <= ptCrash->blockweg)
                                 {
                                    uFeFvGrundAB = 42;
                                 }
                                 else
                                 {
                                    ucRet = 1;
                                    uFeFvGrundAB = 43;
                                 }
                           }
                        }
                  }
               }
         }
         if (!ucRet)
         {
            if (pNBlStre)
            {
               // AFz hat eine n‰chste Strecke
                  uCrash = FzAufBlStre(&tVFz,pNBlStre,uNAnzBlkStr);
                  if (uCrash)
                  {
                     // Strecke des VFz ist auch blockierend zur n‰chsten 
					 //	Strecke des AFz
                     ptCrash  = pNBlStre    + uCrash - 1;
                     ptCrashE = pNBlStreExt + uCrash - 1;
                     bVFzIstFrei = VFzIstFrei(&tVFz,uVFzWeg,ptCrash,
                     ptCrashE,uNAStreIndex+1,uAFzWeg);
                     if (!bVFzIstFrei)
                     {
                        // Fahrzeugpaar gefunden ! 
						if (ptCrash->blockweg)
                        {
							// AFz hat einen n‰chsten Blockweg !
							uABlkWegTab = GetBlkWegStrTab(ptAFz->nmp,
								uNAStreIndex+1);
							if (uABlkWegTab <= ptCrash->blockweg)
							{
								bBlkWegFrei = BlkWegFrei(uVFzWeg,
									ptAFz,
									uNAStreIndex+1,
									ptCrash,
									ptCrashE);
								if (!bBlkWegFrei)
								{
								// VFz ist nicht frei zum Blockweg/PosWeg der n‰chsten Strecke des AFz:                            :
								//	Fahrverbot !
									ucRet = 1;
									uFeFvGrundAB = 8;
								}
								else
								{
									BOOL bVFzSetFV = false;
									if(uNVStreIndex)
									{
										// Das VFz hat eine n‰chste Strecke:
										// Crash mit n‰chster Strecke des AFZ ?
										lCrash = GetCrashIndex(uNVStreIndex,(uNAStreIndex+1));
										if(lCrash)
										{
											ptCrash = BlStre_Root + lCrash - 1;
											// Blockweg der n‰chsten des VFZ
											// aus Streckentabelle:
											uVBlkWegTab = GetBlkWegStrTab(tVFz.nmp,uNVStreIndex);
											if(ptCrash->blockweg < uVBlkWegTab)
											{
												// freier Weg kleiner als Blockweg ?
												if(bVFzFe)
												{
													bVFzSetFV = true;
												}
											}
										}
									}
									if(bVFzSetFV)
									{
										ucRet = true;
										uFeFvGrundAB = 71;
									}
									else
									{
										uFeFvGrundAB = 7;
									}

								}
							}
							else
							{
								//AFz kann in n‰chster Strecke zum VFz nicht blocken !
								if (ptCrashE->uKWegFuerBlWeg)
								{
									if (uVFzWeg >= ptCrashE->
										uKWegFuerBlWeg)
									{
										//Aber das FVz ist schon soweit 
										//gefahren, dass das AFz in seiner 
										//n‰chsten Strecke den Blockweg 
										//abfahren kann !
										uFeFvGrundAB = 30;
									}
									else
									{
										ucRet = 1;
										uFeFvGrundAB = 6;
									}
								}
								else
								{
									ucRet = 1;
									uFeFvGrundAB = 13;
								}
							}
						}
						else
						{
							//AFz hat keinen nÑchsten Blockweg !
							//ucRet = 1;
							//uFeFvGrundAB = 5;
							if (ptCrashE->uKWegFuerBlWeg)
							{
								if (uVFzWeg >= ptCrashE->
                                    uKWegFuerBlWeg)
								{
									//Aber das FVz ist schon soweit 
									//gefahren, dass das AFz in seiner 
									//n‰chsten Strecke den Blockweg 
									//abfahren kann !
									uFeFvGrundAB = 15;
								}
								else
								{
									if(!bVFzFe &&
										uVFzWeg >= ptCrashE->uZKWeg &&
										uVFzWeg < (ptCrashE->uZKWeg + ptCrashE->uZKFreiWeg)
										)
									{
                                          uFeFvGrundAB = 44;
                                       }
                                       else
                                       {
                                          ucRet = 1;
                                          uFeFvGrundAB = 5;
                                       }
                                    }
                                 }
                                 else
                                 {
                                    //ucRet = 1;
                                    //uFeFvGrundAB = 5;
                                    pGCrash    = GetGegenCrash   (ptCrash,
                                     uNAStreIndex+1);
                                    if (bVFzFreiZurAStreA &&
                                        pGCrash->kollisionsweg)
                                    {
                                       //VFz ist frei zur aktuellen Strecke 
                                       //des AFz und AFz kann sich in der 
                                       //Folgestrecke freifahren ! 
                                       if (bVFzFe)
                                       {
                                         uFeFvGrundAB = 16;
                                       }
                                       else
                                       {
                                         ucRet = 1;
                                         uFeFvGrundAB = 17;
                                       }
                                    }
                                    else
                                    {
                                       ucRet = 1;
                                       uFeFvGrundAB = 41;
                                       if (tVFz.nmp == ptAFz->nmp)
                                       {
                                         if (pGCrash->kollisionsweg)
                                         {
                                         ucRet = 0;
                                         uFeFvGrundAB = 35;
                                         }
                                         else
                                         {
											//Stopp !
                                         }
                                       }
                                       else
                                       {
                                         //Stopp !
                                       }
                                    }
                                 }
                           }
                     }
                     else
                     {
                        uFeFvGrundAB = 4;
                     }
                  }
                  else
                  {
                     if (bVFzFe)
                     {
                        //EE-la: 20.07.2000
                        //Noch kein Fahrverbot und VFz hat FE: n‰chste 
                        //Strecke des VFz zur n‰chsten Strecke des AFz
                           lCrash  = GetCrashIndex(uNVStreIndex,(
                           uNAStreIndex+1));
                           if (lCrash)
                           {
                              //ucRet        =  1;
                              //uFeFvGrundAB = 19;
                              if (tVFz.nmp == tVFz.fzziel.nr)
                              {
                                 //Wird VFz am Ziel frei sein ?
                                    ptCrash = BlStre_Root + lCrash - 1;
                                    uPosWeg =  GetPosWegBlk(&tVFz);
                                    if (uPosWeg <= ptCrash->blockweg)
                                    {
                                       uFeFvGrundAB = 21;
                                    }
                                    else
                                    {
                                       ucRet = 1;
                                       uFeFvGrundAB = 20;
                                    }
                              }
                              else
                              {
                                 //ucRet = 1;
                                 //uFeFvGrundAB = 19;
                                 //EE-la: 22.09.2000:
                                 ptCrash = BlStre_Root + lCrash -1;
                                 lCrash  = GetCrashIndex((uNAStreIndex+1),
                                 uNVStreIndex);
                                 pGCrash = BlStre_Root + lCrash -1;
                                 if (ptCrash->kollisionsweg &&
                                     !pGCrash->blockweg)
                                 {
                                    uFeFvGrundAB = 36;
                                 }
                                 else
                                 {
                                    ucRet = 1;
                                    uFeFvGrundAB = 19;
                                 }
                              }
                           }
                     }
                     else
                     {
                        uFeFvGrundAB = 3;
                     }
                  }
            }
         }
   if (!ucRet)
   {
      //Ran-Bit des VFz im Vektor des AFz lˆschen !
#ifdef MITRANFAHREN
         LoescheRanVektorBit(ptAFz->fznr,ptVFz->fznr);
#endif
   }
   return ucRet;
}
*/

static CCrashDistance *GetGegenCrash(CCrashDistance* pCrashDist,
                                 unsigned short uStrIndex)
{
   CBlockTableAGVTypePair* pBlockTable = theAutoTrafficControl.GetPtrBlockTable();
   CCrashDistance* pGegenCrashDist = pBlockTable->GetGegenCrash(pCrashDist, uStrIndex);

   return pGegenCrashDist;
} 

/*
static BlStreExt_Typ *GetGegenCrashExt(BlStre_Typ     *ptCrash,
                                       unsigned short uStrIndex)
{
   BlStre_Typ     *ptCr;
   BlStreExt_Typ  *ptRet = NULL,
                  *ptCrE;
   unsigned long  *pVerBlStr;
   unsigned short uAnzBlkStr,
                  uI;

   pVerBlStr = Ver_BlStre_Root + ptCrash->index - 1;

   ptCr       = BlStre_Root      + *pVerBlStr;
   ptCrE      = BlStreExt_Root   + *pVerBlStr;
   uAnzBlkStr = (unsigned short )(*(pVerBlStr + 1) - *pVerBlStr);

   for (uI = 0;
        uI < uAnzBlkStr;
        uI++,ptCr++,ptCrE++)
   {
      if (ptCr->index == uStrIndex)
      {
         ptRet = ptCrE;
      }
   }
   return ptRet;
}
*/

/*
unsigned short FzAufBlStre(TYP_FZDAT *ptFz,
                           BlStre_Typ *pBlStre,
                           unsigned short uAnzBlkStr)
{
   unsigned short uStreIndex,
                  uRet = 0,
                  uIndex;

   uStreIndex = GetStrIndexFzT(ptFz->mp,ptFz->nmp);

   for (uIndex = 1;
        uIndex <= uAnzBlkStr && !uRet;
        uIndex++, pBlStre++)
   {
      if (uStreIndex == pBlStre->index)
      {
         uRet = uIndex;
      }
   }

   return uRet;
}
*/

/*
Die Funktion ermittelt, ob uFz an einem Ziel steht !
 */
static unsigned char FzAmZiel(TYP_FZDAT *ptFzDat)
{
  unsigned char ucRet;
  ucRet = !(ptFzDat->nmp) ||
          (ptFzDat->mp == ptFzDat->fzziel.nDest) ||
		  (ptFzDat->mppos && TEST_AGV_STOP(*ptFzDat));
  return ucRet;
}

//----------------------------------------------------
//      Datum       : 08.10.99
//      Beschreibung: Die Funktion ermittelt, ob ein Fahrzeug, das sich an einem
//                    Ziel befindet und den Weg uWegVFz abgefahren hat, sich im
//                    kollisionsfreien Bereich befindet.
//      Returnwert  : 0 - Fz ist nicht kollisionsfrei
//                    1 - Fz ist kollisionsfrei
//----------------------------------------------------
static unsigned char VFzIstFreiAmZiel(unsigned short uWegVFz,
	CCrashDistance* pCrashDist)
{
	unsigned char ucRet = 0;
	if (pCrashDist->HasRelCollWay())
	{
		if (pCrashDist->CrossOverRelCollWay(uWegVFz))
		{
			//VFz hat den Ziel Zielkollisionsweg abgefahren
			if (!pCrashDist->CrossOverRelFreeArea(uWegVFz))
			{
				//VFz steht am Ziel im kollisionsfreien Bereich
				ucRet = 1;
			}
		}
	}

	return ucRet;
}

/*
static unsigned char VFzIstFrei(TYP_FZDAT     *ptFz,
	unsigned short uFzWeg,
	BlStre_Typ    *ptCrash,
	BlStreExt_Typ *ptCrashE,
	unsigned short uAStrIndex,
	unsigned short uAFzWeg)
{
	unsigned char  ucRet = 0;
	unsigned short uVBlockWeg,
		uFzZiel;
	BlStre_Typ     *ptGegenCrash;
	BlStreExt_Typ  *ptGegenCrashE;

	//VFz frei im eigenen Blockweg ?
	ptGegenCrash = GetGegenCrash(ptCrash, uAStrIndex);
	ptGegenCrashE = GetGegenCrashExt(ptCrash, uAStrIndex);

	if (ptGegenCrash)
	{
		//Gegencrash gefunden !
		//Blockweg/Posweg ermitteln
		if (fzdat[ptFz->fznr].mppos)
		{
			uFzZiel = ptFz->fzziel.nr;
			ptFz->fzziel.nr = 0;
			uVBlockWeg = GetPosWegBlk(ptFz);
			ptFz->fzziel.nr = uFzZiel;
			if (uVBlockWeg == 0xFFFF)
			{
				uVBlockWeg = GetBlkWegStrTab(ptFz->mp, ptCrash->index
					);
			}
		}
		else
		{
			uVBlockWeg = GetBlkWegStrTab(ptFz->mp, ptCrash->index);
		}
		if (uFzWeg <= uVBlockWeg)
		{
			if (ptGegenCrash->blockweg)
			{
				if (uVBlockWeg <= ptGegenCrash->blockweg)
				{
					ucRet = 1;
				}
				else
				{
					//Blockweg im CAD zu lang !
					if (ptGegenCrashE->uKWegFuerBlWeg)
					{
						//Afz kann den Blockweg des VFz freimachen !
						if (uAFzWeg >= ptGegenCrashE->uKWegFuerBlWeg)
						{
							//AFz kann von VFz-Strecke nicht blockiert 
							//werden, weil AFz schon frei !
							ucRet = 1;
						}
					}
					else
					{
						//Afz kann den Blockweg des VFz nicht freimachen !
					}
				}
			}
			else
			{
				//VFz hat keinen GBlock-Blockweg
				if ((uFzWeg >= ptCrashE->uZKWeg) && (uFzWeg <
					ptCrashE->uZKFreiWeg))
				{
					//VFz ist in kollisionsfreien Bereich !
					ucRet = 1;
				}
			}
		}
		else
		{
			//VFz ist bereits Å¸ber den Blockweg/PosWeg hinaus gefahren !
		}
	}
	else
	{
		//Keinen Gegencrash gefunden: V_Block.Bin unvollstÑndig !
	}

	if (!ucRet)
	{
		//VFz ist noch nicht frei !
		if (ptCrash->kollisionsweg)
		{
			if (uFzWeg < ptCrash->kollisionsweg)
			{
				if (FzAmZiel(ptFz))
				{
					if (VFzIstFreiAmZiel(uFzWeg, ptCrashE))
					{
						//VFz ist frei am Ziel !
						ucRet = 1;
					}
				}
			}
			else
			{
				//VFz ist absolut frei !
				ucRet = 1;
			}
		}
	}
	if (ucRet)
	{
		ucRet = ucRet && FZ_FAHRVERBOT(*ptFz);
	}
	return ucRet;
}
*/

/*
static unsigned char BlkWegFrei(unsigned short uVFzWeg,
	TYP_FZDAT      *ptAFz,
	unsigned short uNAStrIndex,
	CCrashDistance *ptCrash)
{
	unsigned char  bRet = 0,
		bAFzAmZiel,
		bTestKWegAbs;
	unsigned short uBlkWeg,
		uPosWeg;

	bTestKWegAbs = 0;
	bAFzAmZiel = (ptAFz->fzziel.nr == ptAFz->nmp);

	if (bAFzAmZiel)
	{
		//AFz ist mit dem nÑchsten Mp am Ziel !
		uBlkWeg = GetBlkWegStrTab(ptAFz->nmp, uNAStrIndex);
		uPosWeg = GetPosWegBlk(ptAFz);

		if (uPosWeg > uBlkWeg)
		{
			//PosWeg ist l‰nger als der Blockweg, VFz muss absolut frei sein.
			bTestKWegAbs = 1;
		}
	}
	if (!bTestKWegAbs)
	{
		//Test, ob Blockweg frei !
		if (ptCrashE->uKWegFuerBlWeg)
		{
			if (uVFzWeg >= ptCrashE->uKWegFuerBlWeg)
			{
				bRet = 1;
			}
		}
	}
	else
	{
		//Test, ob absolut frei !
		//AFz ist mit dem NMp am Ziel !
		if (uPosWeg <= ptCrash->blockweg)
		{
			//Der Posweg ist kÅrzer als der mîgliche Blockweg von GBLOCK !
			bRet = 1;
		}
		else
		{
			if (ptCrash->kollisionsweg)
			{
				if (uVFzWeg >= ptCrash->kollisionsweg)
				{
					bRet = 1;
				}
			}
		}
	}
	return bRet;
}
*/

static unsigned char StreckenPaarPruefen(unsigned short uAStreIndex,
	unsigned short uVStreIndex)
{
	unsigned char  ucRet = 1;
	unsigned short i = 0;
	//Feststellen, ob die uAStreIndex. und die uVStreIndex. Strecke ein 
	//Streckenpaar sind, das nicht geprÅft werden soll.
	if (pNoCheckTab != NULL)
	{
		for (i = 0;
			i < KKMAX_NOCHECK_STRE && pNoCheckTab[uAStreIndex].noCheck[i
			];
		i++)
		{
			if (pNoCheckTab[uAStreIndex].noCheck[i] == uVStreIndex)
			{
				//Strecke soll nicht geprÅ¸ft werden
				ucRet = 0;
				break;
			}
		}
	}
	return ucRet;
}

/*
unsigned short GetRestLaengeIndex(unsigned short Index)
{
  return (MpNmpTab[Index].str_len - MpNmpTab[Index].blk_weg );
}
*/

/*
unsigned short GetBlkWegStrTab(unsigned short mp, unsigned short StreIndex)
{
	// Zeiger auf Streckentabelle des MP
	Meldepunkttabelle *standtab;

	// Zeiger auf NMP-Tabelle
	NMP_tabelle   *mp_nmp_tab;

	unsigned char   h_nmp;
	if (mp != 0)
	{
		// Zeiger auf MP in der MP-DatenTabelle
		standtab = mptab + vmptab[mp] - 1;

		// Zeiger auf Tabelle der naechsten Meldepunkte
		mp_nmp_tab = MpNmpTab + standtab->str_idx - 1;
		h_nmp = 0;
		// so lange suchen bis Ende oder nmp gefunden
		while ((mp_nmp_tab->zeit_index != StreIndex) &&
			(h_nmp < standtab->anz_nmp))
		{
			mp_nmp_tab++;
			h_nmp++;
		}
		if (mp_nmp_tab->zeit_index == StreIndex)
		{
			return mp_nmp_tab->blk_weg;
		}
		else
		{
			return 0xffff;
		}
	}
	else
	{
		return 0xffff;
	}
}
*/

unsigned short GetBlkWegStrTab(unsigned short nMp, unsigned short StreIndex)
{
	CCourseAGVType* pCourseAGVType = g_pCourseData->GetPtrCourseAGVType(1);
	CCpDistance* pDist = pCourseAGVType->GetPtrDistance(StreIndex);

	//CCommPoint* pCp = pCourseAGVType->GetPtrCommPoint(nMp);
	//int nIndex = pCp->GetDistanceIndex();
	//int nNCps = pCp->GetNCps();

	return pDist->GetBlockWay();
}

static unsigned short GetFzWeg (TYP_FZDAT *ptFzDat)
{
#ifdef FZ_WEG_KORREKTUR
   return ptFzDat->wegstr;
#else
   return ptFzDat->wegstr*100;
#endif
}

/*
static unsigned short GetStrIndex(unsigned short  mp1,
	unsigned short  mp2)
{
	// Zeiger auf Streckentabelle des MP
	Meldepunkttabelle *standtab;

	// Zeiger auf NMP-Tabelle
	NMP_tabelle       *mp_nmp_tab;

	unsigned char   h_nmp;
	if (mp2 != 0)
	{
		// Zeiger auf MP in der MP-DatenTabelle
		standtab = mptab + vmptab[mp1] - 1;

		// Zeiger auf Tabelle der naechsten Meldepunkte
		mp_nmp_tab = MpNmpTab + standtab->str_idx - 1;
		h_nmp = 0;
		// so lange suchen bis Ende oder nmp gefunden
		while ((mp_nmp_tab->nmp != mp2) &&
			(h_nmp < standtab->anz_nmp))
		{
			mp_nmp_tab++;
			h_nmp++;
		}
		if (mp_nmp_tab->nmp == mp2)
		{
			// wenn NMP ok => Zeitindex auslesen
			return mp_nmp_tab->zeit_index;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}
*/

static unsigned short GetStrIndex(unsigned short  mp1, unsigned short  mp2)
{
	CCourseAGVType* pCourseAGVType = g_pCourseData->GetPtrCourseAGVType(1);
	return	pCourseAGVType->GetDistIndex(mp1, mp2);
}

/*
static unsigned short GetStrIndexFzT (unsigned short  mp1,
                                      unsigned short  mp2)
{
   // Zeiger auf Streckentabelle des MP
   Meldepunkttabelle *standtab;

   // Zeiger auf NMP-Tabelle
   NMP_tabelle       *mp_nmp_tab;

   unsigned char   h_nmp;
   if (mp2 != 0)
   {
      // Zeiger auf MP in der MP-DatenTabelle
      standtab = mptab + vmptab [mp1] - 1;

      // Zeiger auf Tabelle der naechsten Meldepunkte
      mp_nmp_tab = MpNmpTab + standtab->str_idx - 1;
      h_nmp = 0;
      // so lange suchen bis Ende oder nmp gefunden
      while ((mp_nmp_tab->nmp != mp2) &&
             (h_nmp < standtab->anz_nmp))
      {
         mp_nmp_tab++;
         h_nmp++;
      }
      if (mp_nmp_tab->nmp == mp2)
      {
         // wenn NMP ok => Zeitindex auslesen
         return mp_nmp_tab->zeit_index;
      }
      else
      {
         return 0;
      }
   }
   else
   {
      return 0;
   }
}
*/

static unsigned short GetStrIndexFzT(unsigned short  mp1, unsigned short  mp2)
{
	CCourseAGVType* pCourseAGVType = g_pCourseData->GetPtrCourseAGVType(1);
	return	pCourseAGVType->GetDistIndex(mp1, mp2);
}

static unsigned short GetNewSegment(FILE *fp)
{
   char cLine[128];
   char *p1,*p2;
   unsigned short nMp1,nMp2;
   while ( fgets(cLine,sizeof(cLine),fp))
   {
      /*
      z.B. [100-101]
       */
      p1 = strchr(cLine,'[');
      if ( p1 != NULL )
      {
         p2 = strchr(p1+1,'-');
         if ( p2 != NULL)
         {
            *p2 = 0;
            nMp1 = atoi(p1+1);
            p1 = strchr(p2+1,']');
            if ( p1 != NULL)
            {
               *p1 = 0;
               nMp2 = atoi(p2+1);
               return GetStrIndex(nMp1,nMp2);
            }
         }
      }
   }
   return 0;
}

static unsigned short ReadNoCheckSegments(unsigned short nStreIndex, FILE *fp)
{
   int i = 0;
   char cLine[128];
   char *p1;
   unsigned short nMp1,nMp2;
/*~ STRUCT */
   while ( fgets(cLine,sizeof(cLine),fp) )
   {
      p1 = strchr(cLine,'-');
/*~ STRUCT */
      if ( p1 != NULL)
      {
         *p1  = 0;
         nMp1 = atoi(cLine);
         nMp2 = atoi(p1+1);
         pNoCheckTab[nStreIndex].noCheck[i++] = GetStrIndex(nMp1,nMp2);
/*~ STRUCT */
         if ( i >= KKMAX_NOCHECK_STRE )
         {
            printf("\nzu viele Strecken in Strecke %d",nStreIndex);

            return 2;
         }
      }
      else
      {
         return 1;
      }
   }
   return 0;
}

/* UP's f¸r erweiterte Autoblockung */
void EABInit(void)
{
	unsigned short i,
		*puBlkFz;
	
#ifdef MIT_DEADLOCK_HISTORY
	/* Deadlock-History */
	TypDLHistory   *ptDLHist;
	TypMpHistory   *ptMpHist;
	TypVFzFeFv     *ptVFzFeFv;
	unsigned char  ucFz,
		ucVFz;
	unsigned short uMpIndex;
	unsigned int   uiHistLen;
	
	int nAGVs = g_pCourseData->GetNumberAGVs();
	int nCpsPlus1 = g_pCourseData->GetNumberAIDCps() + 1;
	int nCpDistances = g_pCourseData->GetNumberAIDCpDists();

	//uiHistLen       = sizeof(TypDLHistory) * KKFZMAX;
	uiHistLen = sizeof(TypDLHistory)* nAGVs;
	// XXX ptDLHistoryRoot = (TypDLHistory *)calloc(1,uiHistLen);
	ptDLHistoryRoot = (TypDLHistory *)new TypDLHistory [uiHistLen];
	//uiHistLen       = sizeof(TypMpHistory) * KKFZMAX * DL_HISTORY_LEN;
	uiHistLen = sizeof(TypMpHistory)* nAGVs * DL_HISTORY_LEN;
	// XXX ptMpHistoryRoot = (TypMpHistory *)calloc(1,uiHistLen);
	ptMpHistoryRoot = (TypMpHistory *)new TypMpHistory [uiHistLen];
	//uiHistLen       = sizeof(TypVFzFeFv)*KKFZMAX*(KKFZMAX-1)*DL_HISTORY_LEN;
	uiHistLen = sizeof(TypVFzFeFv)*nAGVs*(nAGVs - 1)*DL_HISTORY_LEN;
	// XXX ptVFzFeFvRoot   = (TypVFzFeFv *)calloc(1,uiHistLen);
	ptVFzFeFvRoot   = (TypVFzFeFv *)new TypVFzFeFv [uiHistLen];
	
	ptDLHist  = ptDLHistoryRoot;
	ptMpHist  = ptMpHistoryRoot;
	ptVFzFeFv = ptVFzFeFvRoot;
	
	for (ucFz = 1;
	//ucFz <= KKFZMAX;
	ucFz <= nAGVs;
	ptDLHist++, ucFz++)
	{
		ptDLHist->ucFz      = ucFz;
		ptDLHist->ptMpHist = ptMpHist;
		
		for (uMpIndex = 0;
		uMpIndex < DL_HISTORY_LEN;
		uMpIndex++,ptMpHist++)
		{
            ptMpHist->uMp                = 0;
			ptMpHist->uNmp               = 0;
			ptMpHist->uUemp              = 0;
			ptMpHist->uZiel              = 0;
			ptMpHist->uAnzBehandelt      = 0;
			ptMpHist->uAktWegGefahren    = 0;
			ptMpHist->uErsterWegGefahren = 0;
			ptMpHist->m_ucErstStunde     = 0;
			ptMpHist->m_ucErstSekunde    = 0;
			ptMpHist->m_ucErstMinute     = 0;
			ptMpHist->m_ucAktStunde      = 0;
			ptMpHist->m_ucAktSekunde     = 0;
			ptMpHist->m_ucAktMinute      = 0;
			ptMpHist->bLast              = 0;
            ptMpHist->ptVFzFeFv = ptVFzFeFv;
			
            for (ucVFz = 1;
			//ucVFz <= KKFZMAX;
			ucVFz <= nAGVs;
			ucVFz++)
            {
				if (ucFz != ucVFz)
				{
					ptVFzFeFv->m_ucVFz     = ucVFz;
					ptVFzFeFv->m_bFv       = 0;
					ptVFzFeFv->m_bFe       = 0;
					ptVFzFeFv->m_tFvInfos.m_uGrundAB  = 0;
					ptVFzFeFv->m_tFvInfos.m_uGrundEAB = 0;
					ptVFzFeFv->m_tFvInfos.m_uMp       = 0;
					ptVFzFeFv->m_tFvInfos.m_uNmp      = 0;
					ptVFzFeFv->m_tFvInfos.m_uUemp     = 0;
					ptVFzFeFv->m_tFeInfos.m_uGrundAB  = 0;
					ptVFzFeFv->m_tFeInfos.m_uGrundEAB = 0;
					ptVFzFeFv->m_tFeInfos.m_uMp       = 0;
					ptVFzFeFv->m_tFeInfos.m_uNmp      = 0;
					ptVFzFeFv->m_tFeInfos.m_uUemp     = 0;
					ptVFzFeFv++;
				}
            }
		}
	}
	
#endif
	// XXX pMpFolgeVerz = (unsigned short ** )calloc(1,sizeof(unsigned short*)*(KKFZMAX+1));
	//pMpFolgeVerz = (unsigned short **) new unsigned short * [KKFZMAX + 1];
	pMpFolgeVerz = (unsigned short **) new unsigned short *[nAGVs + 1];
	// XXX pMpFolge = (unsigned short *)calloc(1,sizeof(unsigned short)*KKFZMAX*(KKMAX_STO_P1+1));
	//pMpFolge = (unsigned short *) new unsigned short [KKFZMAX*KKMAX_STO_P1 + 1];
	pMpFolge = (unsigned short *) new unsigned short[nAGVs*nCpsPlus1 + 1];
	pMpFolgeVerz[0] = pMpFolge;
	*(pMpFolgeVerz[0])   = 0;
	
#ifdef MITRANFAHREN
	//iRanVektLen = div(KKFZMAX+1, 8).quot + 1;
	iRanVektLen = div(nAGVs + 1, 8).quot + 1;

	// XXX pcBitRan = (unsigned char *)calloc(1,iRanVektLen*(KKFZMAX+1));
	//pcBitRan = (unsigned char *) new unsigned char [iRanVektLen*(KKFZMAX+1)];
	pcBitRan = (unsigned char *) new unsigned char[iRanVektLen*(nAGVs + 1)];

	// pcTmpBitRan = (unsigned char *)calloc(1,iRanVektLen);
	pcTmpBitRan = (unsigned char *) new unsigned char [iRanVektLen];
#endif
	for (i = 1;
	//i < KKFZMAX;
	i < nAGVs;
	i++)
	{
		//pMpFolgeVerz[i]    = pMpFolgeVerz[i-1] + KKMAX_STO_P1 + 1;
		pMpFolgeVerz[i] = pMpFolgeVerz[i - 1] + nCpsPlus1 + 1;
		*(pMpFolgeVerz[i]) = 0;
	}
	//pAMenge        = (unsigned short *)malloc(sizeof(unsigned short)*KKMAX_FAHR_ZEIT);
	pAMenge = (unsigned short *)malloc(sizeof(unsigned short)*nCpDistances);
	//pNeueAMenge = (unsigned short *)malloc(sizeof(unsigned short)*KKMAX_FAHR_ZEIT);
	pNeueAMenge = (unsigned short *)malloc(sizeof(unsigned short)*nCpDistances);
	pFolgeStrMenge = (unsigned short *)malloc(sizeof(unsigned short)* 256);
	//pVirtCrMenge   = (unsigned short *)malloc(sizeof(unsigned short)*(KKMAX_FAHR_ZEIT+1));
	pVirtCrMenge = (unsigned short *)malloc(sizeof(unsigned short)*(nCpDistances + 1));
	//pVirtMpFolge   = (unsigned short *)malloc(sizeof(unsigned short)*(KKMAX_STO_P1+1));
	pVirtMpFolge = (unsigned short *)malloc(sizeof(unsigned short)*(nCpsPlus1 + 1));
	*pAMenge = 0;
	*pNeueAMenge    = 0;
	*pFolgeStrMenge = 0;
	*pVirtCrMenge   = 0;
	*pVirtMpFolge   = 0;
	
#ifdef ALLEFZKOENNENBLOCKIEREN
	
	//i         = sizeof(unsigned short*)*(KKFZMAX + 1);
	i = sizeof(unsigned short*)*(nAGVs + 1);
	//ppBlkFz = (unsigned short **)malloc(sizeof(unsigned short*)*(KKFZMAX + 1));
	ppBlkFz = (unsigned short **)malloc(sizeof(unsigned short*)*(nAGVs + 1));
	memset(ppBlkFz, 0, i);
	//i         = sizeof(unsigned short) * (KKFZMAX + 1) * KKFZMAX;
	i = sizeof(unsigned short)* (nAGVs + 1) * nAGVs;
	pAllBlkFz = (unsigned short *)malloc(i);
	memset(pAllBlkFz,0,i);
	
	for (i = 1, puBlkFz = pAllBlkFz;
		//i <= KKFZMAX;
		i <= nAGVs;
	//i++, puBlkFz += (KKFZMAX + 1))
	i++, puBlkFz += (nAGVs + 1))
	{
		ppBlkFz[i] = puBlkFz;
	}
	
#endif
#ifdef ZEITSPAREN
	
	// XXX pCrMengeVerz = (unsigned short **)calloc(1,sizeof(unsigned short*)*(KKFZMAX+1));
	//pCrMengeVerz = (unsigned short **) new unsigned short * [KKFZMAX+1];
	pCrMengeVerz = (unsigned short **) new unsigned short *[nAGVs + 1];
	// XXX pCrMenge   = (unsigned short *)calloc(1,sizeof(unsigned short)*KKFZMAX*(KKMAX_FAHR_ZEIT+1));
	//pCrMenge   = (unsigned short *) new unsigned short [KKFZMAX*(KKMAX_FAHR_ZEIT+1)];
	pCrMenge = (unsigned short *) new unsigned short[nAGVs*(nCpDistances + 1)];

	pCrMengeVerz[0] = pCrMenge;
	*(pCrMengeVerz[0]) = 0;
	
	for (i = 1;
	//i < KKFZMAX;
	i < nAGVs;
	i++)
	{
		//pCrMengeVerz[i]    = pCrMengeVerz[i-1] + KKMAX_FAHR_ZEIT + 1;
		pCrMengeVerz[i]    = pCrMengeVerz[i-1] + nCpDistances + 1;
		*(pCrMengeVerz[i]) = 0;
	}
	
#else
	/* HABENZEIT */
	pCrMenge       = malloc(sizeof(unsigned short)*(KKMAX_FAHR_ZEIT+1));
	pVCrMenge      = malloc(sizeof(unsigned short)*(KKMAX_FAHR_ZEIT+1));
	
#endif
	// XXX pCrMengeUmleiten = (unsigned short *)calloc(1,sizeof(unsigned short)*(KKMAX_FAHR_ZEIT+1));
	//pCrMengeUmleiten = (unsigned short *) new unsigned short [KKMAX_FAHR_ZEIT+1];
	pCrMengeUmleiten = (unsigned short *) new unsigned short[nCpDistances + 1];
	// XXX pMpFolgeUmleiten = (unsigned short *)calloc(1,sizeof(unsigned short)*(KKMAX_STO_P1+1));
	//pMpFolgeUmleiten = (unsigned short *) new unsigned short [KKMAX_STO_P1+1];
	pMpFolgeUmleiten = (unsigned short *) new unsigned short[nCpsPlus1 + 1];

	// tFVSituation initialisieren !
	CString strPath;
	strPath.LoadString(IDS_FILEPATH);
	CString strFile;
	
	// Read the sitStop.txt file
	tFVSituation.m_bSetFV = true;
	strFile.LoadString(IDS_FILE_SITUATION_FV);
	tFVSituation.Read(strPath + "\\" + strFile);
	
	// Read the sitGo.txt file
	tFESituation.m_bSetFV = false;
	strFile.LoadString(IDS_FILE_SITUATION_FE);
	tFESituation.Read(strPath + "\\" + strFile);
	
	return;
}

static unsigned char SetDeadLockFahrverbot(unsigned char bTestUmleiten)
{
    // Variable
	unsigned char  ucRet,
		bVFzSetFV = 0,
		bMpFolgeNeu = false;;
	unsigned short *pMpABegin,
		*pMpVBegin,
		uAFz,
		uVFz,
		uIndexMpFolge;
	TypVFzInfos    *pVFzInfos;
#ifdef ALLEFZKOENNENBLOCKIEREN
	unsigned short    *pBlkFz;
#endif
#ifdef MIT_DEADLOCK_HISTORY
	TypVFzFeFv         *ptFeFv;
	TypMpHistory       *ptMpHist;
	unsigned short     nAFzWeg;
#endif
#ifdef MITRANFAHREN
	unsigned char ucBit,
		*pcFzBitRan;
	div_t         dtRan,
		dtARan;
#endif
	/* Bislang kein Fahrverbot: Nun entscheidet TABSETFV !  */
	ucRet            = 0;
	bUmdispoGesperrt = 0;
	uFeFvGrundAB     = 0;
	uFeFvGrundEAB    = 0;
	uAFz = tAFzDat.fznr - 1;
	int nAGVs = g_pCourseData->GetNumberAGVs();
	if (bTestUmleiten)
	{
		/* Daten des AFz sichern */
		pMpFolgeCopy       = pMpFolgeVerz[uAFz];
		pMpFolgeVerz[uAFz] = pMpFolgeUmleiten;
		*pMpFolgeUmleiten  = 0;
		pCrMengeCopy       = pCrMengeVerz[uAFz];
		pCrMengeVerz[uAFz] = pCrMengeUmleiten;
		*pCrMengeUmleiten  = 0;
	}
	
	/* Voraussetzungen f¸r uAFz schaffen */
	tEABaFz.uNr      = uAFz;
	tEABaFz.pMpFolge = pMpFolgeVerz[uAFz];
	
	// Lac : 28,10.2001:
	if(tAFzDat.mp)
	{
		// Fz mit Mp
		tEABaFz.uIndexMpFolge = GetIndexMpFolge(tEABaFz.pMpFolge,
			tAFzDat.mp);
		if(tEABaFz.uIndexMpFolge)
		{
			if(tAFzDat.mp != tAFzDat.fzziel.nDest)
			{
				// Fz ist noch nicht am Ziel
				if(tAFzDat.nmp)
				{
					// Fz mit NMp
					uIndexMpFolge = GetIndexMpFolge(tEABaFz.pMpFolge,
						tAFzDat.nmp);
					if(uIndexMpFolge)
					{
						if(tAFzDat.uemp)
						{
							// Fz mit UEMp
							uIndexMpFolge = GetIndexMpFolge(tEABaFz.pMpFolge,
								tAFzDat.uemp);
							if(!uIndexMpFolge)
							{
								// UEMp nicht in der MpFolge
								if(tAFzDat.nmp != tAFzDat.fzziel.nDest)
								{
									// Der NMp ist nicht das Ziel
									bMpFolgeNeu = true;
								}
							}
						}
					}
					else
					{
						bMpFolgeNeu = true;
					}
				}
			}
		}
		else
		{
			bMpFolgeNeu = true;
		}
		if(bMpFolgeNeu)
		{
			GetMpFolge(&tAFzDat,
				tEABaFz.pMpFolge,
				tAFzDat.fzziel.nDest);
			tEABaFz.uIndexMpFolge = 1;
		}
		pMpABegin = tEABaFz.pMpFolge + tEABaFz.uIndexMpFolge - 1;
		
		tEABaFz.pMpBegin    = pMpABegin;
		tEABaFz.pMpEnd      = GetMpFolgeEnd(tEABaFz.pMpFolge);
		
		// Wenn Fz am Ziel, dann Schutzh¸lle nicht neu berechnen !
		if(tAFzDat.mp != tAFzDat.fzziel.nDest)
		{
			tEABaFz.uAnzCrMenge = GetCrStreMenge(pMpABegin,pCrMengeVerz[uAFz]);
		}
		tEABaFz.pCrMenge    = pCrMengeVerz[uAFz];
		
		if(tAFzDat.mp == 36)
		{
			ZeigeMenge(tEABaFz.pCrMenge);
		}
		
		if(*tEABaFz.pCrMenge == 0)
		{
			CString strMsg;
			strMsg.Format(IDS_AGV_WITH_EMPTY_CRASH_PROTECT_AREA,(int)tAFzDat.fznr);
			// AfxMessageBox(strMsg);
			CAlarma * ptrAlarma = new CAlarma;
			ptrAlarma->AvisarAlarma (3, 1, 1,(int)tAFzDat.fznr,0,0,0,0,0, strMsg);
		}
		
#ifdef MITRANFAHREN
		/* temor‰ren Bit-Vektor lˆschen */
		memset(pcTmpBitRan,0,iRanVektLen);
		dtARan = div(uAFz+1,8);
#endif
		auto_merke_blk_fz = 0;
		
#ifdef MIT_DEADLOCK_HISTORY
		/* ggf. Deadlock-History schreiben*/
		if (!bTestUmleiten)
		{
			ptMpHist = ptDLHistoryRoot[uAFz].ptMpHist;
			if ((tAFzDat.mp != ptMpHist->uMp) ||
				(tAFzDat.nmp != ptMpHist->uNmp) ||
				(tAFzDat.uemp != ptMpHist->uUemp))
			{
				/* NeuesHistEreignis */
				NeuesHistEreignis(uAFz,
					tAFzDat.mp,
					tAFzDat.nmp,
					tAFzDat.uemp,
					tAFzDat.fzziel.nDest,
					GetFzWeg(&tAFzDat),
					FZ_BELADEN(tAFzDat));
			}
			else
			{
				nAFzWeg = GetFzWeg(&tAFzDat);
				if (nAFzWeg != ptMpHist->uAktWegGefahren)
				{
					/* Weg ist ver‰ndert */
					ptMpHist->uAktWegGefahren = nAFzWeg;
					CTime time =CTime::GetCurrentTime ();
					ptMpHist->m_ucAktStunde  = (unsigned char)time.GetHour ();
					ptMpHist->m_ucAktMinute  = (unsigned char)time.GetMinute ();
					ptMpHist->m_ucAktSekunde = (unsigned char)time.GetSecond ();
				}
			}
			ptMpHist->uAnzBehandelt++;
		}
#endif
		/* Schleife ¸ber alle Fahrzeuge ! */
		for (uVFz = 0;
			//(uVFz < KKFZMAX)// && !(ucRet && bUmdispoGesperrt);
			(uVFz < nAGVs);// && !(ucRet && bUmdispoGesperrt);
		uVFz++)
		{
			uFeFvGrundAB  = 0;
			uFeFvGrundEAB = 0;
			if (uVFz != uAFz)
			{
				/* Ein Fahrzeug kann sich nicht selbst blocken ! */
				uVerglFz = uVFz+1;
				if (TEST_FZ_IM_SYSTEM(fzdat[uVFz+1]))
				{
					BOOL bVMpFolgeErweitert = false;		// Lac: 11.11.01
					/* VFz ist im System !*/
					/* Voraussetzungen f¸r uVFz schaffen */
					tEABvFz.uNr      = uVFz;
					tEABvFz.pMpFolge = pMpFolgeVerz[uVFz];
					if (*(tEABvFz.pMpFolge) == 0)
					{
						*(tEABvFz.pMpFolge)   = fzdat[uVFz+1].mp;
						*(tEABvFz.pMpFolge+1) = 0;
					}
					
					tEABvFz.uIndexMpFolge   = GetIndexMpFolge(tEABvFz.pMpFolge,
						fzdat[uVFz+1].mp);
					if (!tEABvFz.uIndexMpFolge)
					{
						if (fzdat[uVFz+1].fzziel.nDest)
						{
							GetMpFolge( &fzdat[uVFz+1], /*fzdat[uVFz+1].fznr, fzdat[uVFz+1].mp */
								tEABvFz.pMpFolge,
								fzdat[uVFz+1].fzziel.nDest);
							
							tEABvFz.uAnzCrMenge = GetCrStreMenge(
								tEABvFz.pMpFolge,pCrMengeVerz[uVFz]);
							tEABvFz.uIndexMpFolge = GetIndexMpFolge(
								tEABvFz.pMpFolge,fzdat[uVFz+1].mp);
						}
						else
						{
							tEABvFz.uIndexMpFolge = 1;
						}
					}
					pMpVBegin = tEABvFz.pMpFolge + tEABvFz.uIndexMpFolge - 1;
					
					
					// Lac: 11.11.01
					if(*(pMpVBegin+1) == 0)
					{
						// kein NMp: 1.Mp als NMp
						//*(pMpVBegin + 1) = CAgv::GetPNmp(*pMpVBegin, statdat[1].mp)->nmp;
						int nNextCp = g_pCourseData->GetPtrCourseAGVType(1)->GetPtrCommPointByIndex(0)->GetCp();
						*(pMpVBegin + 1) = CAgv::GetPNmp(*pMpVBegin, nNextCp)->GetNCp();
						*(pMpVBegin + 2) = 0;
						bVMpFolgeErweitert = true;		// Lac: 11.11.01
					}
					
					tEABvFz.pMpBegin    = pMpVBegin;
					tEABvFz.pMpEnd      = GetMpFolgeEnd(tEABvFz.pMpFolge);
					
					tEABvFz.pCrMenge = pCrMengeVerz[uVFz];
					
					// Lac: 25.10.2001:
					// Wenn noch keine Crash-Menge vorhanden ist, dann
					// wird sie nun berechnet, damit auch das VFz eine 
					// CrashMenge hat !
					if(*tEABvFz.pCrMenge == 0)
					{
						tEABvFz.uAnzCrMenge = GetCrStreMenge(pMpVBegin,
							tEABvFz.pCrMenge);
						
					}
					
					if(*tEABvFz.pCrMenge!= 0)
					{
						bVFzSetFV = GetEABFahrverbot(tEABaFz,tEABvFz);
					}
					else
					{
						if(fzdat[uVFz+1].mp)
						{
							if(MpInCrStrMenge(fzdat[uVFz+1].mp,tEABaFz.pCrMenge))
							{
								ucRet = 1;
								uFeFvGrundEAB = 554;
							}								 
						}
						else
						{
							ucRet = 1;
							uFeFvGrundEAB = 555;
						}
					}
					if(bVMpFolgeErweitert)		// Lac: 11.11.01
					{	
						*(pMpVBegin+1) = 0;
					}
					/* Fahrverbot/Fahrerlaubnis ertragen */
					if (bVFzSetFV && !ucRet)
					{
						auto_merke_blk_fz = uVFz+1;
					}
					ucRet |= bVFzSetFV;
				}
				else
				{
					/* VFz ist nicht im System !*/
					uFeFvGrundEAB = 1;
				}
#ifdef MIT_DEADLOCK_HISTORY
				/* Grund f¸r FeFv eintragen */
				if (!bTestUmleiten)
				{
					ptFeFv = (ptMpHist->ptVFzFeFv) + uVFz - (uVFz > uAFz
						);
					if ((bVFzSetFV != 0))
					{
						/* Fahrverbot vom VFz */
						ptFeFv->m_bFv = 1;
						pVFzInfos = &(ptFeFv->m_tFvInfos);
					}
					else
					{
						/*Kein Fahrverbot vom VFz */
						ptFeFv->m_bFe = 1;
						pVFzInfos = &(ptFeFv->m_tFeInfos);
					}
					pVFzInfos->m_uGrundAB     = uFeFvGrundAB;
					pVFzInfos->m_uGrundEAB    = uFeFvGrundEAB;
					pVFzInfos->m_uMp          = fzdat[uVFz+1].mp;
					pVFzInfos->m_uNmp         = fzdat[uVFz+1].nmp;
					pVFzInfos->m_uUemp        = fzdat[uVFz+1].uemp;
					pVFzInfos->m_uZiel        = fzdat[uVFz+1].fzziel.nDest;
					pVFzInfos->m_uWegGefahren = GetFzWeg(fzdat + uVFz +1
						);
					pVFzInfos->m_bVFzFv       = (TEST_AGV_STOP(fzdat[uVFz
						+1]) != 0);
					
					CTime time = time.GetCurrentTime ();
					
					pVFzInfos->m_ucStunde  = (unsigned char)time.GetHour ();
					pVFzInfos->m_ucMinute  = (unsigned char)time.GetMinute ();
					pVFzInfos->m_ucSekunde = (unsigned char)time.GetSecond ();
					
				}
#else
				uFeFvGrundEAB = uFeFvGrundEAB;
#endif
#ifdef ALLEFZKOENNENBLOCKIEREN
				pBlkFz         = ppBlkFz[uAFz+1];
				pBlkFz[uVFz+1] = bVFzSetFV;
#endif
			}
		}
		
		/* Fahrerlaubnis in tAFzDat eintragen */
		// This variable should be consulted before changing destination of the vehicle
		tAFzDat.cUmdispoGesperrt = bUmdispoGesperrt;
		if (!ucRet)
		{
			tAFzDat.fz_blk = 0;
#ifdef MITRANFAHREN
			/* Ran-Bits eintragen */
			/* Schleife ¸ber alle Fahrzeuge */
			for (uVFz = 1;
			//uVFz <= KKFZMAX;
			uVFz <= nAGVs;
			uVFz++)
			{
				/* Bit aus tempor‰ren Vektor holen */
				dtRan = div(uVFz,8);
				ucBit = *(pcTmpBitRan + dtRan.quot) >> dtRan.rem;
				ucBit &= 1;
				if (ucBit)
				{
					/* Vektor des VFz ermitteln */
					pcFzBitRan = pcBitRan + (iRanVektLen * uVFz);
					/* Bit in der Stelle des AFz im VFz-Vektor eintragen */
					*(pcFzBitRan + dtARan.quot) |= (ucBit << 
						dtARan.rem); 
				}
			}
#endif
			/* Daten des AFz sichern */
			/*
			if (bTestUmleiten)
			{
				pMpFolgeVerz[uAFz] = pMpFolgeCopy;
				pCrMengeVerz[uAFz] = pCrMengeCopy;
			}
			*/
		}	
	}
	else
	{
		// AFz ohne Mp: Fahrverbot !
		ucRet = 1;
	}
	if (bTestUmleiten)
	{
		/* Daten des AFz sichern */
		pMpFolgeVerz[uAFz] = pMpFolgeCopy;
		pCrMengeVerz[uAFz] = pCrMengeCopy;
	}
	
	return ucRet;
}

CCrashDistanceArray* GetPtrArrCrashDistances(int nDistIndex)
{
	return theAutoTrafficControl.GetPtrBlockTable()->GetPtrArrCrashDistances(nDistIndex);
}

CCrashDistance* GetPtrCrashDistance(int nTopIndex, int nCompIndex)
{
	CCrashDistance* pRet = NULL;
	theAutoTrafficControl.GetPtrBlockTable()->GetPCrashDistance(nTopIndex, nCompIndex);
	return pRet;
}

unsigned char GetEABFahrverbot(TypEABFzStrukt tAFz, TypEABFzStrukt tVFz)
{
	unsigned short auCrFz[20],
		uAnzCrFz;
	unsigned char  bBlFzKette;
	unsigned char  bAFzBlocktVorher,
		bVFzBlocktVorher,
		bCrGefunden,
		bAFzDarfRan,
		bVFzKannWarten;
	unsigned short *pMp;

	unsigned char uRet = 0;
	unsigned short uVStrIndex,
		uVStrIndexBegin,
		uNVStrIndex,
		uAStrIndex,
		uNAStrIndex,
		uLMp,
		uLNMp,
		uiMpIndex,
		*pCrElem,
		*pDMenge,
		*pTmpMenge,
		*pADMenge,
		uVFz,
		uAFz,
		*pVMenge,
		*pDiffMenge,
		*pNewDMenge,
		uAFzWeg,
		uVFzWeg,
		*pAWegStrMenge,
		*pVWegStrMenge,
		*pKollMenge,
		uPosWeg,
		uBlkWeg,
		uBlkWegVFz,
		uPosWegVFz,
		//uNMp,
		uFzZiel,
		*pVTeilMpF,
		*pTmpVCrMenge,
		*pExtVWegStrMenge,
		*pTmpVMpFolge;
	unsigned char  bVFzFe,
		bVFzKollisionsfrei,
		bVFzBlockt,
		bAbbruch,
		bAFzKannPosWeg,
		bNStre,
		bAFzAmZiel,
		bVFzAmZiel,
		bAFzDarfVorbei,
		bAFzIstVoraus,
		bFVSit = false;
	CCrashDistance     *pCrash,
		*pGCrash;

	//Meldepunkttabelle *ptMp;
	CCommPoint* pCommPoint;
	unsigned short    uI,
		uBegIndex,
		uEndIndex;

	TYP_FZDAT      *pAFzDat,
		*pVFzDat;
	int nStrIndex;

	CCourseAGVType* pCourseAGVType = g_pCourseData->GetPtrCourseAGVType(1);

#ifdef MITRANFAHREN
	div_t          dtRan;
#endif
	// Mp, NMp und UEMp pr¸fen: 16.10.01
	uVFz = tVFz.uNr + 1;
	uAFz = tAFz.uNr + 1;
	pAFzDat = /*fzdat + uAFz*/ &tAFzDat;
	pVFzDat = fzdat + uVFz;
	if (pAFzDat->mp && pAFzDat->nmp)
	{
		nStrIndex = GetStrIndexFzT(pAFzDat->mp, pAFzDat->nmp);
		if (nStrIndex == 0)
		{
			// AFz mit ung¸ltigen Daten.
			uRet = 1;
			uFeFvGrundEAB = 99;
		}
	}
	if (pAFzDat->nmp && pAFzDat->uemp)
	{
		nStrIndex = GetStrIndexFzT(pAFzDat->nmp, pAFzDat->uemp);
		if (nStrIndex == 0)
		{
			// AFz mit ung¸ltigen Daten.
			uRet = 1;
			uFeFvGrundEAB = 99;

		}
	}
	if (!uRet)
	{
		uRet = tFVSituation.CheckSet(CFzBedingung(*pAFzDat), CFzBedingung(*pVFzDat));
		if (uRet)
		{
			// Grund eintragen:
			uFeFvGrundEAB = 200 + tFVSituation.m_nResultSit;
			bFVSit = true;
		}
		if (!uRet)
		{
			if (theAutoTrafficControl.IsActivated())
			{
				int nFeFvGrundAB(0);
				//uRet = theAutoTrafficControl.GetNoCrashDriveStop(uAFz,uVFz,nFeFvGrundAB);
				uRet = theAutoTrafficControl.GetNoCrashDriveStop(pAFzDat,tAFz, uVFz,tVFz, nFeFvGrundAB);
				uFeFvGrundAB = (USHORT)nFeFvGrundAB;
			}
		}
		if (!uRet)
		{
			// Schutzh¸lle des VFz kopieren

			//pTmpVCrMenge = NewMenge(KKMAX_FAHR_ZEIT + 1);
			pTmpVCrMenge = NewMenge(g_pCourseData->GetNumberAIDCpDists() + 1);
			CopyMenge(tVFz.pCrMenge, pTmpVCrMenge);
			ZeigeMenge(pTmpVCrMenge);

			pExtVWegStrMenge = GetExtWegStrMenge(tVFz.pMpBegin);
			ZeigeMenge(pExtVWegStrMenge);

			ErweiternCrStreMengeIdx(pExtVWegStrMenge, pTmpVCrMenge);
			ZeigeMenge(pTmpVCrMenge);

			//pADMenge = GetNextDurchschnitt(tAFz.pMpBegin,tVFz.pCrMenge);
			pADMenge = GetNextDurchschnitt(tAFz.pMpBegin, pTmpVCrMenge);
			ZeigeMenge(pTmpVCrMenge);
			ZeigeMenge(pADMenge);

			/*
			uVFz       = tVFz.uNr+1;
			uAFz       = tAFz.uNr+1;
			pAFzDat    = fzdat + uAFz;
			pVFzDat    = fzdat + uVFz;
			*/

			if (uAFz == 1 &&
				fzdat[uAFz].mp == 235 &&
				fzdat[uAFz].nmp == 236 &&
				uVFz == 2 &&
				fzdat[uVFz].mp == 60
				)
			{
				uAFz++;
				uAFz--;
			}

			uAFzWeg = GetFzWeg(pAFzDat);
			bAFzAmZiel = FzAmZiel(pAFzDat);

#ifdef MITRANFAHREN
			dtRan = div(uVFz, 8);
#endif
			if (*pADMenge)
			{
				/* AFz und VFz haben Gemeinsamkeiten */
				//pTmpVMpFolge = NewMenge(KKMAX_STO + 1);
				pTmpVMpFolge = NewMenge(g_pCourseData->GetNumberAIDCps() + 1);
				CopyMenge(tVFz.pMpBegin, pTmpVMpFolge);
				ErweiternMpFolgeIdx(pExtVWegStrMenge, pTmpVMpFolge);

				//pVTeilMpF = GetTeilMpFolge(tVFz.pMpBegin,tAFz.pMpBegin,&bVFzKannWarten);
				pVTeilMpF = GetTeilMpFolge(pTmpVMpFolge, tAFz.pMpBegin, &bVFzKannWarten);
				ZeigeMenge(pVTeilMpF);
				if (*(pVTeilMpF + 1))
				{
					pDMenge = GetNextDurchschnitt(pVTeilMpF, pADMenge);
				}
				else
				{
					//pDMenge = NewMenge(KKMAX_FAHR_ZEIT + 1);
					pDMenge = NewMenge(g_pCourseData->GetNumberAIDCpDists() + 1);
					CopyMenge(pADMenge, pDMenge);
				}
				FreeMenge(pVTeilMpF);
				FreeMenge(pTmpVMpFolge);

				ZeigeMenge(pDMenge);

				if (*pDMenge)
				{
					/* VFz untersuchen */
					uVStrIndex = GetStrIndexFzT(*(tVFz.pMpBegin), *(tVFz.pMpBegin + 1));
					uBlkWegVFz = GetBlkWegStrTab(*(tVFz.pMpBegin), uVStrIndex);
					pCrElem = ElemInMenge(pDMenge, uVStrIndex);
					bVFzFe = !TEST_AGV_STOP(fzdat[tVFz.uNr + 1]);
					uVFzWeg = GetFzWeg(pVFzDat);
					bVFzAmZiel = FzAmZiel(pVFzDat);
					if (bVFzAmZiel)
					{
						uPosWegVFz = GetPosWegBlk(pVFzDat);
					}
					else
					{
						uPosWegVFz = 0;
					}

					uVStrIndexBegin = uVStrIndex;

					/*
					EE-la: 28.03.2000
					if (!(*pCrElem) && bVFzFe && *(tVFz.pMpBegin+2))
					*/
					if (!(*pCrElem) && *(tVFz.pMpBegin + 1) && *(tVFz.pMpBegin + 2))
					{
						uNVStrIndex = GetStrIndexFzT(*(tVFz.pMpBegin + 1), *(tVFz.pMpBegin + 2));
						pCrElem = ElemInMenge(pDMenge, uNVStrIndex);
					}
					if (*pCrElem)
					{
						// Das VFz ist auf einer Strecke der Durchschnittsmenge !

						if ((blk_fznr == 2)/* && (blk_mp == 6)*/)
						{
							blk_fznr++;
							blk_fznr--;
						}
						//pVWegStrMenge = GetWegStrMengePlus(tVFz.pMpBegin); 
						pVWegStrMenge = GetWegStrMenge(tVFz.pMpBegin);

						MengeVereinen(pExtVWegStrMenge, pVWegStrMenge);

						uAStrIndex = GetEintauchStrecke(tAFz.pMpBegin,
							pDMenge,
							pVWegStrMenge,
							uAFzWeg,
							bAFzAmZiel,
							&bAFzBlocktVorher);

						if (uAStrIndex)
						{
							// Ran-Bit des VFz im Vektor des AFz lˆschen
#ifdef MITRANFAHREN
							LoescheRanVektorBit(uAFz, uVFz);
#endif
							GetStrecke(uAStrIndex, &uLMp, &uLNMp);

							/* AFz sollte nur bis uLMp fahren nicht bis uLNMp */
							uiMpIndex = GetIndexMpFolge(tAFz.pMpFolge, uLMp);
							if ((uiMpIndex == tAFz.uIndexMpFolge) //||
								//(((uiMpIndex - tAFz.uIndexMpFolge) == 1) && bAFzBlocktVorher)
								)
							{
								// AFZ kommt in Crashbereich
								pAWegStrMenge = GetWegStrMenge(tAFz.pMpBegin);
								uVStrIndex = GetEintauchStrecke(tVFz.pMpBegin,
									pDMenge,
									pAWegStrMenge,
									uVFzWeg,
									bVFzAmZiel,
									&bVFzBlocktVorher);

								if ((bVFzAmZiel && (pVFzDat->mp == pAFzDat->
									fzziel.nDest)))
								{
									// SW-la: 22.02.2001:
									//VFz ist am Ziel und AFz hat das gleiche Ziel
									if (*(tVFz.pMpBegin + 1) == 109)
									{
										blk_fznr++;
										blk_fznr--;
									}
									pTmpMenge = GetFolgeMenge(*(tVFz.pMpBegin + 1));
									pKollMenge = GetKollStrMenge(uAStrIndex, pTmpMenge);

									if (pKollMenge != NULL)
									{
										if (AlleStrWerdenFrei(pKollMenge, uAStrIndex))
										{
											uFeFvGrundEAB = 146;
										}
										else
										{
											//uRet = 1;
											//uFeFvGrundEAB = 145;
											if (!bVFzFe &&
												(abs((int)uPosWegVFz - (int)uBlkWegVFz) < 3) &&
												(abs((int)uVFzWeg - (int)uBlkWegVFz) < 3))
											{
												// Das VFz hat FV und und der PosWeg ist <= Blockweg und der gefahrene Weg <= Blockweg 
												if (FzWegIstFreiZurStrecke(uVStrIndexBegin, pAWegStrMenge))
												{
													// FE, weil AFz frei ist und frei bleibt.
													uFeFvGrundEAB = 145;
												}
												else
												{
													// FV, weil AFz nicht frei ist oder nicht frei wird.
													uRet = 1;
													uFeFvGrundEAB = 148;
												}
											}
											else
											{
												// FV, weil VFz entweder FE hat oder zu weit gefahren ist.
												uRet = 1;
												uFeFvGrundEAB = 149;
											}
										}
									}
									else
									{
										// FV: event.bekannter Weg ab n‰chster Strecke des VFz sagt nichts
										// aus, ob AFz weiter heran fahren kann, da ab der n‰chsten Strecke
										// des VFz kein Crash mit der Strecke uAStrIndex mehr voliegt !
										uRet = 1;
										uFeFvGrundEAB = 147;
									}
									FreeMenge(pTmpMenge);
									FreeMenge(pKollMenge);
								}
								else
								{
									if (uVStrIndex)
									{
										//VFz kommt auch in Crashbereich
										if (bVFzFe)
										{
											//Und hat Fahrerlaubnis 
											//uRet = 1;
											//uFeFvGrundEAB = 14;
											if (*(tVFz.pMpBegin + 1) == pVFzDat
												->fzziel.nDest)
											{
												pCrash = GetPtrCrashDistance(uVStrIndex, uAStrIndex);
												if (pCrash != NULL)
												{
													//Wird VFz am Ziel frei sein ?
													uPosWeg = GetPosWegBlk(pVFzDat);
													if (!pCrash->CrossOverFreeWay(uPosWeg))
													{
														uFeFvGrundEAB = 142;
													}
													else
													{
														uRet = 1;
														uFeFvGrundEAB = 143;
													}
												}
												else
												{
													uFeFvGrundEAB = 141;
													if (*(tAFz.pMpBegin + 2))
													{
														//AFz hat n‰chste Strecke !
														uNAStrIndex = GetStrIndexFzT(*(tAFz.pMpBegin + 1), *(tAFz.pMpBegin + 2));
														pCrash = GetPtrCrashDistance(uNAStrIndex, uVStrIndex);
														if (pCrash)
														{
															uBlkWeg = GetBlkWegStrTab(*(tAFz.pMpBegin + 1), uNAStrIndex);
															//if (pCrash->blockweg < uBlkWeg)
															if (pCrash->CrossOverFreeWay(uBlkWeg))
															{
																//Freiweg in der n‰chsten Strecke ist zu kurz !
																uRet = 1;
																uFeFvGrundEAB = 144;
															}
														}
													}
												}
											}
											else
											{
												uRet = 1;
												uFeFvGrundEAB = 14;
											}
										}
										else
										{
											//Und hat Fahrverbot:
											//uFeFvGrundEAB = 13;

											//Das AFz wird nun das VFz blockieren. Daraus kann ein Deadlock entstehen, wenn ein 
											//anderes Fz sich im n‰heren Crash-Bereich des AFz befindet, das Åber eine Kette bis hin zum VFz FV hat!
											//Wenn das AFz nun f¸Åhre und so von diesem Fz blockiert wÅ¸rde, w‰re dies ein Deadlock !
											pVirtMpFolge[0] = *tAFz.pMpBegin;
											pVirtMpFolge[1] = *(tAFz.pMpBegin + 1);
											pVirtMpFolge[2] = *(tAFz.pMpBegin + 2);
											pVirtMpFolge[3] = 0;
											uI = GetCrStreMenge(pVirtMpFolge, pVirtCrMenge);
											uAnzCrFz = GetFzInCrashMenge(pVirtCrMenge, auCrFz, uAFz);

											for (uI = 0, bBlFzKette = 0;
												(uI < uAnzCrFz) && !bBlFzKette;
												uI++)
											{
												if (auCrFz[uI] != uVFz)
												{
													bBlFzKette = GetBlFzKette(auCrFz[uI], uVFz);
												}
											}
											if (bBlFzKette)
											{
												pTmpMenge = MengenDurchschnitt(pDMenge, pVirtCrMenge);

												uVStrIndex = GetStrIndexFzT(*(tVFz.pMpBegin), *(tVFz.pMpBegin + 1));
												uBlkWegVFz = GetBlkWegStrTab(*(tVFz.pMpBegin), uVStrIndex);
												bAFzDarfVorbei = VorbeiFahren(&tAFz, pTmpMenge, uVStrIndex, uVFzWeg);

												bAFzIstVoraus = FzIstVoraus(&tAFz, pTmpMenge, uVStrIndex, uVFzWeg, uBlkWegVFz);

												if (!bVFzFe && (bAFzDarfVorbei || bAFzIstVoraus))
												{
													bUmdispoGesperrt |= 1;
													uFeFvGrundEAB = 43;
												}
												else
												{
													//Deadlock in Verzug : FV !

													uRet = 1;
													uFeFvGrundEAB = 25;
												}
												FreeMenge(pTmpMenge);
											}
											else
											{
												uFeFvGrundEAB = 13;
											}
										}
									}
									else
									{
										// VFz ist im gemeinsamen Crash-Bereich
										bVFzKollisionsfrei = VFzIstKollisionsfrei(tVFz, tAFz, pDMenge);
										if (bVFzKollisionsfrei && !bVFzFe)
										{
											//VFz hat Fahrverbot und ist kollisionsfrei zum Weg des AFz:
											//AFz f‰hrt !
											if ((ppBlkFz[uVFz])[uAFz] == uAFz)
											{
												//AFz ist selbst blockierend zu VFz

												uRet = 1;
												uFeFvGrundEAB = 12;
											}
											else
											{
												uFeFvGrundEAB = 11;
											}
										}
										else
										{
											//Vorbei oder n‰her ran ?
											uVStrIndex = GetStrIndexFzT(*(tVFz.pMpBegin), *(tVFz.pMpBegin + 1));

											//EE-la,22.05.2000:

											if (bVFzAmZiel)
											{
												//PosWeg ermitteln
												uFzZiel = pVFzDat->fzziel.nDest;
												pVFzDat->fzziel.nDest = 0;
												uBlkWegVFz = GetPosWegBlk(pVFzDat);
												pVFzDat->fzziel.nDest = uFzZiel;
											}
											else
											{
												// BlockWeg ermitteln
												uBlkWegVFz = GetBlkWegStrTab(*(tVFz.pMpBegin), uVStrIndex);
											}
											bAFzDarfVorbei = VorbeiFahren(&tAFz, pDMenge, uVStrIndex, uVFzWeg);
											bAFzIstVoraus = FzIstVoraus(&tAFz, pDMenge, uVStrIndex, uVFzWeg, uBlkWegVFz);
											if (!bVFzFe && (bAFzDarfVorbei || bAFzIstVoraus))
											{
												bUmdispoGesperrt |= 1;
												uFeFvGrundEAB = 42;
											}
											else
											{
												// Untersuchen, ob AFz eventuell n‰her heran fahren kann !
												uFeFvGrundEAB = 10;
												// EE-la, 26.07.2000: Struktur
												bAFzDarfRan = 0;
												if (*(tAFz.pMpBegin + 2))
												{
													/* AFz hat n‰chste Strecke ! */
													uNAStrIndex = GetStrIndexFzT(*(tAFz.pMpBegin + 1), *(tAFz.pMpBegin + 2));
													pGCrash = GetPtrCrashDistance(uNAStrIndex, uVStrIndex);
													if (pGCrash != NULL)
													{
														//if (pGCrashExt->uKWegFuerBlWeg && (pGCrashExt->uKWegFuerBlWeg <= uVFzWeg))
														if (pGCrash->HasCollWayBlkWay() && pGCrash->CrossOverCollWayBlkWay(uVFzWeg))
														{
															// VFz kann den Blockweg der n‰chsten Strecke des AFz freifahren und hat ihn bereits freigefahren !
															//EE-la: 10.11.2000: AFZ hat Ziel des VFz-Mp: FV !
															if (pAFzDat->fzziel.nDest != *(tVFz.pMpBegin))
															{
																// bAFzDarfRan = 1;
																bAFzDarfRan = bVFzKollisionsfrei;
															}
														}
													}
													else
													{
														bAFzDarfRan = bVFzKannWarten;
													}
												}
												if (!bAFzDarfRan)
												{
													// EE-la, 28.06.2000, Das ersetzt nicht ganz die Blockbereiche !
													if (bAFzBlocktVorher)
													{
														uNAStrIndex = GetStrIndexFzT(*(tAFz.pMpBegin + 1), *(tAFz.pMpBegin + 2));
													}
													else
													{
														uNAStrIndex = uAStrIndex;
													}
													for (pMp = tVFz.pMpBegin, bAbbruch = 0, bCrGefunden = 0;
														*(pMp + 1) && !bAbbruch;
														pMp++)
													{
														uVStrIndex = GetStrIndexFzT(*pMp, *(pMp + 1));
														pCrash = GetPtrCrashDistance(uVStrIndex, uNAStrIndex);
														pGCrash = GetPtrCrashDistance(uNAStrIndex, uVStrIndex);
														if ((pCrash != NULL) && (pGCrash != NULL))
														{
															bCrGefunden = 1;
															//if (!pCrash->kollisionsweg || !pGCrash->blockweg)
															if (!pCrash->HasAbsCollWay() || !pGCrash->HasFreeWay())
															{
																//Keine Entscheidung, wenn kein Kollisionsweg und erste Strecke des VFz und AFz blockt vorher !
																//if (!(!pCrash->kollisionsweg && (pMp == tVFz.pMpBegin) && (bAFzBlocktVorher)))
																if (!(!pCrash->HasAbsCollWay() && (pMp == tVFz.pMpBegin) && (bAFzBlocktVorher)))
																{
																	// Lac: 13.11.01
																	//uRet          = 1;
																	//bAbbruch      = 1;
																	//uFeFvGrundEAB = 45;

																	//if((pGCrashExt->uKWegFuerBlWeg > 0) && (uVFzWeg >= pGCrashExt->uKWegFuerBlWeg))
																	if (pGCrash->HasCollWayBlkWay() && pGCrash->CrossOverCollWayBlkWay(uVFzWeg))
																	{
																		// Bislang FE
																	}
																	else
																	{
																		uRet = 1;
																		bAbbruch = 1;
																		uFeFvGrundEAB = 45;
																	}
																}
															}
														}
														else
														{
															bAbbruch = bCrGefunden;
														}
													}
													if (!uRet)
													{
														uVStrIndex = GetStrIndexFzT(*(tVFz.pMpBegin), *(tVFz.pMpBegin + 1));
														pCrash = GetPtrCrashDistance(uAStrIndex, uVStrIndex);

														if (pCrash != NULL)
														{
															//if (!pCrash->kollisionsweg)
															if (!pCrash->HasAbsCollWay())
															{
																uRet = 1;
																uFeFvGrundEAB = 44;
															}
														}
													}

													if (!uRet)
													{
														//AFz kann eventuell noch n‰her heran !
														//pVMenge = NewMenge(KKMAX_FAHR_ZEIT + 1);
														pVMenge = NewMenge(g_pCourseData->GetNumberAIDCpDists() + 1);

														if (*(tVFz.pMpBegin + 2))
														{
															GetCrStreMenge(tVFz.pMpBegin + 1, pVMenge);
														}
														else
														{
															//VFz hat nur eine Strecke
															//GetCrStreMenge(tVFz.pMpBegin, pVMenge);
															//Schleife Å¸ber alle mˆglichen NMp's
															//ptMp = mptab + vmptab[*(tVFz.pMpBegin + 1)] - 1;
															//uBegIndex = ptMp->str_idx - 1;
															pCommPoint = pCourseAGVType->GetPtrCommPoint(*(tVFz.pMpBegin + 1));
															uBegIndex = pCommPoint->GetDistanceIndex();

															//if (*(tVFz.pMpBegin + 1) == KKMAXMP)
															//{
															//	uEndIndex = KKMAX_FAHR_ZEIT + 1;
															//}
															//else
															//{
															//	uEndIndex = (ptMp + 1)->str_idx - 1;
															//}
															uEndIndex = uBegIndex + pCommPoint->GetNCps();

															for (uI = uBegIndex;
																uI < uEndIndex;
																uI++)
															{
																pNewDMenge = GetBlMenge(uI);
																MengeVereinen(pNewDMenge, pVMenge);
																FreeMenge(pNewDMenge);
															}
														}

														//Kann AFz noch n‰her ran ?
														//!!!!
														pCrElem = ElemInMenge(pDMenge,uAStrIndex);

														if (*(tAFz.pMpBegin + 2))
														{
															uNAStrIndex = GetStrIndexFzT(*(tAFz.pMpBegin + 1), *(tAFz.pMpBegin + 2));
														}
														else
														{
															uNAStrIndex = 0;
														}

														if (!(*pCrElem) && *(tAFz.pMpBegin + 2))
														{
															uAStrIndex = uNAStrIndex;
															bNStre = 1;
														}
														else
														{
															bNStre = 0;
														}

														//ptMp = mptab + vmptab[*(tAFz.pMpBegin + 1)] - 1;
														pCommPoint = pCourseAGVType->GetPtrCommPoint(*(tAFz.pMpBegin + 1));

														//if (bNStre && (*(tAFz.pMpBegin + 1) == pAFzDat->fzziel.nDest) && (ptMp->anz_nmp > 1))
														if (bNStre && (*(tAFz.pMpBegin + 1) == pAFzDat->fzziel.nDest) && (pCommPoint->GetNCps() > 1))
														{
															/*~ CONTRACTION */
															/*~
															AFz vor einem Ziel mit mehreren
															Folgestrecken !
															~*/
															FreeMenge(pVMenge);

															uPosWeg = GetPosWegBlk(pAFzDat);

															uAStrIndex = pCommPoint->GetDistanceIndex();

															for (uI = 0, bAbbruch = 0, bAFzKannPosWeg = 1;
																(uI < pCommPoint->GetNCps()) && !bAbbruch && bAFzKannPosWeg;
																uI++)
															{
																//uNMp = (MpNmpTab + (ptMp->str_idx + uI))->nmp;
																//uNMp = pCommPoint->GetDistanceIndex ->nmp;

																//uAStrIndex = GetStrIndexFzT(*(tAFz.pMpBegin + 1), uNMp);
																uAStrIndex += uI;

																uBlkWeg = GetBlkWegStrTab(*(tAFz.pMpBegin + 1), uAStrIndex);

																pCrash = GetPtrCrashDistance(uAStrIndex, uVStrIndex);

																if (pCrash != NULL)
																{
																	if (uPosWeg <= uBlkWeg)
																	{
																		//if (pCrashExt->uKWegFuerBlWeg)
																		if (pCrash->HasCollWayBlkWay())
																		{
																			//if (uVFzWeg >= pCrashExt->uKWegFuerBlWeg)
																			if (pCrash->CrossOverCollWayBlkWay(uVFzWeg))
																			{
																				bAbbruch = 1;
																			}
																		}
																		//if (pCrash->blockweg < uPosWeg)
																		if (pCrash->CrossOverFreeWay(uPosWeg))
																		{
																			//AFz kann den PosWeg in dieser
																			//n‰chsten Strecke nicht fahren !
																			bAFzKannPosWeg = 0;
																		}
																	}
																}
															}
															if (bAbbruch || bAFzKannPosWeg)
															{
																uFeFvGrundEAB = 26;
															}
															else
															{
																uRet = 1;
																uFeFvGrundEAB = 27;
															}
														}
														else
														{
															//AFz nicht vor einem Ziel mit mehreren Folgestrecken !
															pNewDMenge = MengenDurchschnitt(pVMenge, pDMenge);
															FreeMenge(pVMenge);

															pVMenge = GetBlMenge(uVStrIndex);

															//Die Menge pDiffMenge enth‰lt alle Strecken, die nicht mehr blockiert werden, wenn VFz auf
															//seiner n‰chsten Strecke f¸Åhre !

															pDiffMenge = MengenDifferenz(pVMenge, pNewDMenge);
															FreeMenge(pVMenge);
															FreeMenge(pNewDMenge);

															pCrElem = ElemInMenge(pDiffMenge,uAStrIndex);

															if (!(*pCrElem))
															{
																//AFz wird nicht frei, wenn VFz seine n‰chste Strecke befahren wird !
																//Alle n‰chsten Strecken des VFz testen !

																//ptMp = mptab + vmptab[*(tVFz.pMpBegin + 1)] - 1;
																//uBegIndex = ptMp->str_idx - 1;

																pCommPoint = pCourseAGVType->GetPtrCommPoint(*(tVFz.pMpBegin + 1));
																uBegIndex = pCommPoint->GetDistanceIndex();

																//if (*(tVFz.pMpBegin + 1) == KKMAXMP)
																//{
																//	uEndIndex = KKMAX_FAHR_ZEIT + 1;
																//}
																//else
																//{
																//	uEndIndex = (ptMp + 1)->str_idx - 1;
																//}
																uEndIndex = uBegIndex + pCommPoint->GetNCps();

																for (uI = uBegIndex, bVFzBlockt = 0;
																	uI < uEndIndex && bVFzBlockt;
																	uI++)
																{
																	pCrash = GetPtrCrashDistance(uAStrIndex, uI);
																	if (pCrash != NULL)
																	{
																		//Strecken haben einen Crash !
																		//if (!(pCrash->kollisionsweg))
																		if (!(pCrash->HasAbsCollWay()))
																		{
																			bVFzBlockt = 1;
																		}
																	}
																}

																//01.03.2000: EE-la
																//Sicherstellen, dass VFz den Blockweg der n‰chsten Strecke des
																//AFz frei gemacht hat !
																if (!bVFzBlockt)
																{
																	if (uNAStrIndex)
																	{
																		pCrash = GetPtrCrashDistance(uNAStrIndex, uVStrIndex);
																		if (pCrash != NULL)
																		{
																			//if (pCrash->uKWegFuerBlWeg)
																			if (pCrash->HasCollWayBlkWay())
																			{
																				//if (uVFzWeg < pCrashExt->uKWegFuerBlWeg)
																				if (!pCrash->CrossOverCollWayBlkWay(uVFzWeg))
																				{
																					bVFzBlockt = 1;
																				}
																			}
																			else
																			{
																				bVFzBlockt = 1;
																			}
																		}
																	}
																}

																if (!bVFzBlockt)
																{
																	//Alle n‰chsten Strecken des VFz sind oder werden frei und VFz ist!
																	uFeFvGrundEAB = 24;
																}
																else
																{
																	uRet = 1;
																	uFeFvGrundEAB = 15;
																}
															}
															else
															{
																//AStre/NStre AFz zu AStre VFz wird in CAutomaticTrafficControl
																//behandelt
																if (!theAutoTrafficControl.IsActivated())
																{
																	//AFz wird frei, wenn VFz seine n‰chste Strecke befahren wird !
																	pCrash =GetPtrCrashDistance(uAStrIndex, uVStrIndex);
																	if (pCrash != NULL)
																	{
																		//Strecken haben einen Crash !
																		//if (pCrashExt->uKWegFuerBlWeg)
																		if (pCrash->HasCollWayBlkWay())
																		{
																			//if (uVFzWeg >= pCrashExt->uKWegFuerBlWeg)
																			if (pCrash->CrossOverCollWayBlkWay(uVFzWeg))
																			{
																				if (*(tAFz.pMpBegin + 2))
																				{
																					uAStrIndex = GetStrIndexFzT(*(tAFz.pMpBegin + 1), *(tAFz.pMpBegin + 2));
																					pCrash = GetPtrCrashDistance(uAStrIndex, uVStrIndex);
																					if (pCrash != NULL)
																					{
																						//if (pCrash->blockweg)
																						if (pCrash->HasFreeWay())
																						{
																							//if (uVFzWeg >= pCrashExt->uKWegFuerBlWeg)
																							if (pCrash->CrossOverCollWayBlkWay(uVFzWeg))
																							{
																								uFeFvGrundEAB = 20;
																							}
																							else
																							{
																								uRet = 1;
																								uFeFvGrundEAB = 21;
																							}
																						}
																						else
																						{
																							uRet = 1;
																							uFeFvGrundEAB = 22;
																						}
																					}
																					else
																					{
																						uFeFvGrundEAB = 23;
																					}
																				}
																				else
																				{
																					uFeFvGrundEAB = 16;
																				}
																			}
																			else
																			{
																				uRet = 1;
																				uFeFvGrundEAB = 17;
																			}
																		}
																		else
																		{
																			uRet = 1;
																			uFeFvGrundEAB = 18;
																		}
																	}
																	else
																	{
																		uFeFvGrundEAB = 19;
																	}
																}
															}
															FreeMenge(pDiffMenge);
														}
													}
												}
#ifdef MITRANFAHREN
												if (!uRet)
												{
													// AFz darf n‰her ran
													*(pcTmpBitRan + dtRan.quot) |= (1 << dtRan.rem);
												}
#endif
											}
										}
									}
								}
								FreeMenge(pAWegStrMenge);
								if (!uRet)
								{
									// bUmdispoGesperrt |= 1;
									bUmdispoGesperrt |= 0;
								}
							}
							else
							{
								// AFz hat noch Land
								uFeFvGrundEAB = 9;

								LoescheRanVektorBit(uVFz, uAFz);
							}
						}
						else
						{
							// AFz ist schon in der Durchschnittsmenge !
							if (bVFzAmZiel)
							{
								// PosWeg ermitteln
								uFzZiel = pVFzDat->fzziel.nDest;
								pVFzDat->fzziel.nDest = 0;
								uBlkWegVFz = GetPosWegBlk(pVFzDat);
								pVFzDat->fzziel.nDest = uFzZiel;
							}
							else
							{
								//BlockWeg ermitteln
								uBlkWegVFz = GetBlkWegStrTab(*(tVFz.pMpBegin), uVStrIndex);
							}
							bAFzDarfVorbei = VorbeiFahren(&tAFz, pDMenge,uVStrIndex, uVFzWeg);
							bAFzIstVoraus = FzIstVoraus(&tAFz, pDMenge,uVStrIndex, uVFzWeg, uBlkWegVFz);
							if (!bVFzFe && (bAFzDarfVorbei || bAFzIstVoraus))
							{
								//AFz kann vorbei fahren !
								bUmdispoGesperrt |= 1;
								uFeFvGrundEAB = 28;
#ifdef MITRANFAHREN
								LoescheRanVektorBit(uAFz, uVFz);
#endif
							}
							else
							{
								pAFzDat = &tAFzDat;
								if (!theAutoTrafficControl.IsActivated())
								{
									//uRet = SetCrashFahrverbot(pAFzDat,pVFzDat);
								}
								else
								{
									uFeFvGrundEAB = 98;
								}
								bUmdispoGesperrt |= 0;
							}
						}
						FreeMenge(pVWegStrMenge);
					}
					else
					{
						//AFz kann Fahren !
						uFeFvGrundEAB = 4;

#ifdef MITRANFAHREN
						//Ran-Bit des VFz im Vektor des AFz lˆschen
						//Ran-Bit des AFz im Vektor des VFz lˆschen
						LoescheRanVektorBit(uAFz, uVFz);
						LoescheRanVektorBit(uVFz, uAFz);
#endif
					}
				}
				else
				{
					uFeFvGrundEAB = 3;

#ifdef MITRANFAHREN
					//Ran-Bit des VFz im Vektor des AFz lîschen
					//Ran-Bit des AFz im Vektor des VFz lîschen
					LoescheRanVektorBit(uAFz, uVFz);
					LoescheRanVektorBit(uVFz, uAFz);
#endif
				}
				FreeMenge(pDMenge);
			}
			else
			{
				// AFz und VFz sehen sich derzeit nicht
				uFeFvGrundEAB = 2;

#ifdef MITRANFAHREN
				// Ran-Bit des VFz im Vektor des AFz lîschen
				// Ran-Bit des AFz im Vektor des VFz lîschen*/
				LoescheRanVektorBit(uAFz, uVFz);
				LoescheRanVektorBit(uVFz, uAFz);
#endif
			}
			FreeMenge(pADMenge);
			FreeMenge(pTmpVCrMenge);
			FreeMenge(pExtVWegStrMenge);
		}
	}

	if (uRet) // Michael 21.12.2001 XXX && !bFVSit )
	{
		// Fahrverbot nicht aus Behandlung der Situation-Liste
		uRet = tFESituation.CheckSet(CFzBedingung(*pAFzDat),
			CFzBedingung(*pVFzDat));
		if (!uRet) uFeFvGrundEAB = 200;
	}



	return uRet;
}

/*
unsigned char SetCrashFahrverbot(TYP_FZDAT *ptAFz,TYP_FZDAT *ptVFz)
{
   unsigned char uRet;
	unsigned short
                  uAFzWeg,
                  uAnzBlkStr,
                  uNAnzBlkStr,
                  uAStreIndex,
                  uNAStreIndex;
   BlStre_Typ    *pBlStre,
                 *pNBlStre;
   BlStreExt_Typ *pBlStreExt,
                 *pNBlStreExt;
   unsigned long  *pVerBlStr;
   //Zeiger auf Tabelle der blockierenden Strecken und Anzahl der blockierenden Strecken !
      uAStreIndex = GetStrIndexFzT(ptAFz->mp,ptAFz->nmp) - 1;
      pVerBlStr   = Ver_BlStre_Root + uAStreIndex;
      pBlStre    = BlStre_Root      + *pVerBlStr;
      pBlStreExt = BlStreExt_Root   + *pVerBlStr;
      uAnzBlkStr = (unsigned short)(*(pVerBlStr + 1) - *pVerBlStr);
   if (ptAFz->uemp)
   {
      //AFz hat eine nÑchste Strecke !
         //Zeiger auf Tabelle der blockierenden Strecken und Anzahl der 
         //blockierenden Strecken !
         uNAStreIndex = GetStrIndexFzT(ptAFz->nmp,ptAFz->uemp) - 1;
         pVerBlStr    = Ver_BlStre_Root + uNAStreIndex;

		 pNBlStre    = BlStre_Root      + *pVerBlStr;
         pNBlStreExt = BlStreExt_Root   + *pVerBlStr;
         uNAnzBlkStr = (unsigned short)(*(pVerBlStr + 1) - *pVerBlStr);
   }
   else
   {
      pNBlStre   = NULL;
      uNAnzBlkStr = 0;
   }
   uAFzWeg = GetFzWeg ( ptAFz );

   uRet = VFzSetFahrverbot(ptAFz,
                           uAFzWeg,
                           uAnzBlkStr,
                           uNAnzBlkStr,
                           uAStreIndex,
                           uNAStreIndex,
                           pBlStre,
                           pBlStreExt,
                           pNBlStre,
                           pNBlStreExt,
                           ptVFz);

   return uRet;
}
*/

/* Mengen-Funktionen */
void FreeMenge(unsigned short *puMenge)
{
  if (puMenge)
  {
     free(puMenge);
     puMenge = NULL;
  }
}

unsigned short *NewMenge(unsigned short uAnzElemente)
{
  unsigned short *puRet;
/*~ BLOCK */
  puRet = (unsigned short *)malloc(uAnzElemente*sizeof(uAnzElemente));
/*~ STRUCT */
  if (puRet)
  {
     *puRet = 0;
  }
  return puRet;
}

#ifdef ZEITSPAREN
unsigned short *MengenDurchschnitt(unsigned short *puM1,
                                  unsigned short *puM2)
{
  unsigned short *puRet,
                 *puE,
                 uAnzM1,
                 uAnzM2;
/*~ CONTRACTION */
  /*~
  ZÑhlen und Speicher allocieren
  ~*/
     uAnzM1= GetAnzMengenElem(puM1);
     uAnzM2= GetAnzMengenElem(puM2);
/*~ STRUCT */
     if (uAnzM1 < uAnzM2)
     {
        uAnzM1 = uAnzM2;
     }
     puRet = NewMenge(uAnzM1 + 1);
/*~ ENDCONTRACTION */
/*~ CONTRACTION */
  /*~
  Speicher fÅllen;
  ~*/
     if (puRet)
     {
        *puRet = 0;
/*~ STRUCT */
        for (puE = puM2;
             *puE;
             puE++)
        {
           if (*ElemInMenge(puM1,*puE))
           {
              MengeErweitern(puRet,*puE);
           }
        }
     }
/*~ ENDCONTRACTION */
  return puRet;
}

unsigned short *MengenDifferenz(unsigned short *puM1,
                               unsigned short *puM2)
{
  unsigned short *puRet,
                 *puE,
                 uAnzM1,
                 uAnzM2;
/*~ CONTRACTION */
  /*~
  ZÑhlen und Speicher allocieren
  ~*/
     uAnzM1= GetAnzMengenElem(puM1);
     uAnzM2= GetAnzMengenElem(puM2);
/*~ STRUCT */
     if (uAnzM1 < uAnzM2)
     {
        uAnzM1 = uAnzM2;
     }
     puRet = NewMenge(uAnzM1 + 1);
/*~ ENDCONTRACTION */
/*~ CONTRACTION */
  /*~
  Speicher fÅllen;
  ~*/
     if (puRet)
     {
        *puRet = 0;
/*~ STRUCT */
        for (puE = puM1;
             *puE;
             puE++)
        {
           if (!*ElemInMenge(puM2,*puE))
           {
              MengeErweitern(puRet,*puE);
           }
        }
     }
/*~ ENDCONTRACTION */
  return puRet;
}
/*~ ENDCONTRACTION */
#else
/*~ CONTRACTION */
      /*~
      unsigned short *MengenDurchschnitt(...)
      ~*/
unsigned short *MengenDurchschnitt(unsigned short *puM1,
                                  unsigned short *puM2)
{
  unsigned short *puRet,
                 *puE,
                 uAnzElemente;
  /* Z‰hlen und Speicher allocieren */
     for (puE = puM2;
          *puE;
          puE++)
     {
        if (*ElemInMenge(puM1,*puE))
        {
           uAnzElemente++;
        }
     }
     puRet = NewMenge(uAnzElemente + 1);
  /* Speicher fÅllen */
     if (puRet)
     {
        *puRet = 0;
        for (puE = puM2;
             *puE;
             puE++)
        {
           if (*ElemInMenge(puM1,*puE))
           {
              MengeErweitern(puRet,*puE);
           }
        }
     }
  return puRet;
}

unsigned short *MengenDifferenz(unsigned short *puM1,
                               unsigned short *puM2)
{
  unsigned short *puRet,
                 *puE,
                 uAnzElemente;
  /* Z‰hlen und Speicher allocieren */
     for (puE = puM1;
          *puE;
          puE++)
     {
        if (!*ElemInMenge(puM2,*puE))
        {
           uAnzElemente++;
        }
     }
     puRet = NewMenge(uAnzElemente + 1);
  /* Speicher fÅllen */
     if (puRet)
     {
        *puRet = 0;
        for (puE = puM1;
             *puE;
             puE++)
        {
           if (!*ElemInMenge(puM2,*puE))
           {
              MengeErweitern(puRet,*puE);
           }
        }
     }
  return puRet;
}
#endif

/* ZeigeMenge() kopiert die via pMenge ¸bergebene Menge in die global definierte
   Menge gauM, die im Debugger sichtbar ist.
   An Stellen, wo man eine allocierte Menge in Debugger ¸bersichtlich sehen
   mˆchte, sollte diese Funktion aufgerufen werden. */
void ZeigeMenge(unsigned short *pMenge)
{
  unsigned short i;
  for (i = 0;
       i<400 && *pMenge;
       i++,pMenge++)
  {
     gauM[i] = *pMenge;
  }
  gauM[i] = 0;
  return;
}

unsigned short GetAnzMengenElem(unsigned short *pMenge)
{
  unsigned short uRet;
  for (uRet = 0;
       *pMenge;
       pMenge++, uRet++)
  {
  }
  return uRet;
}

unsigned char CheckMengeAlsWeg(unsigned short *pMenge,
                               unsigned short *pWeg)
{
   unsigned char  bRet = 0;
   unsigned short *pW,
                  *pM;
   for (pW = pWeg;
        *pW;
        pW++)
   {
      pM = ElemInMenge(pMenge,*pW);
      if (*pM)
      {
         break;
      }
   }
   if (*pW)
   {
      for (;
           *pMenge && !bRet;
           pMenge++,pW++)
      {
         if (*pMenge != *pW)
         {
            *pMenge = 0;
            bRet    = 1;
         }
      }
   }
   return bRet;
}

/*
unsigned short *GetBlMenge(unsigned short uStrIndex)
{
   unsigned short *pRet,
                  *pM,
                  uAnzElem;
   BlStre_Typ    *pAStr,
                 *pEnd;
   if (!uStrIndex || (uStrIndex > KKMAX_FAHR_ZEIT))
   {
      //!!! Breakpoint !!!
      uStrIndex = 1;
   }
   pAStr = BlStre_Root+Ver_BlStre_Root[uStrIndex-1];
   pEnd  = BlStre_Root+Ver_BlStre_Root[uStrIndex];
   uAnzElem = (pEnd - pAStr) + 2;
   pRet = (unsigned short *)malloc(uAnzElem*sizeof(unsigned short)); 
   for (pM = pRet;
        (pAStr < pEnd);
        pAStr++,pM++)
   {
      *pM = pAStr->index;
   }
   *pM = 0;
   return pRet;
}
*/

unsigned short *GetBlMenge(unsigned short uStrIndex)
{
	CCrashDistanceArray* pArrCrashDistances = GetPtrArrCrashDistances(uStrIndex - 1);
	unsigned short *pRet,
		*pMenge,
		uAnzElem = pArrCrashDistances->GetSize() + 1;

	pRet = (unsigned short *)malloc(uAnzElem*sizeof(unsigned short));
	pMenge = pRet;
	for (int count = 0;
		count < pArrCrashDistances->GetSize();
		count++, pMenge++)
	{
		*pMenge = (*pArrCrashDistances)[count].GetIndex();
	}

	*pMenge = 0;
	return pRet;
}

/*
unsigned char AlleStrWerdenFrei(unsigned short *pMenge,unsigned short uIndex)
{
   unsigned char  bRet = 1;
   long           lCrash;
   unsigned short *pM;
   BlStre_Typ     *pCrash;
   for (pM = pMenge;
        *pM && bRet;
        pM++)
   {
      lCrash = GetCrashIndex(*pM,uIndex);
      if (lCrash)
      {
         pCrash = BlStre_Root + lCrash -1;
         if (pCrash->kollisionsweg == 0)
         {
            bRet = 0;
         }
      }
      else
      {
         bRet = 0;
      }
   }
   return bRet;
}
*/

unsigned char AlleStrWerdenFrei(unsigned short *pMenge, unsigned short uIndex)
{
	unsigned char  bRet = 1;
	unsigned short *pM;
	CCrashDistance* pCrash;
	for (pM = pMenge;
		*pM && bRet;
		pM++)
	{
		pCrash = GetPtrCrashDistance(*pM, uIndex);
		if (pCrash != NULL)
		{
			if (!pCrash->HasAbsCollWay())
			{
				bRet = 0;
			}
		}
		else
		{
			bRet = 0;
		}
	}
	return bRet;
}

/*
unsigned char BlkWegZuKurz(unsigned short uBlkWeg,
                           unsigned short uStr1,
                           unsigned short uStr2)
{
   unsigned char ucRet;
   BlStre_Typ    *pAStr,
                 *pEnd;
   pAStr = BlStre_Root+Ver_BlStre_Root[uStr1-1];
   pEnd  = BlStre_Root+Ver_BlStre_Root[uStr1];
   for (ucRet = 0;
        (pAStr < pEnd);
        pAStr++)
   {
      if (pAStr->index == uStr2)
      {
         ucRet = (pAStr->blockweg < uBlkWeg);
      }
   }
   return ucRet;
}
*/

unsigned char BlkWegZuKurz(unsigned short uBlkWeg,
	unsigned short uStr1,
	unsigned short uStr2)
{
	unsigned char ucRet = 0;
	bool bFound = false;
	CCrashDistanceArray* pArrCrashDistances = GetPtrArrCrashDistances(uStr1 - 1);

	for (int count = 0;
		(count < pArrCrashDistances->GetSize()) && !bFound;
		count++)
	{
		if ((*pArrCrashDistances)[count].GetIndex() == uStr2)
		{
			ucRet = (*pArrCrashDistances)[count].CrossOverFreeWay(uBlkWeg);
		}
	}

	return ucRet;
}

/*
unsigned char Str1BlocktZuStr2(unsigned short uStr1,unsigned short uStr2)
{
   unsigned char ucRet;
   BlStre_Typ    *pAStr,
                 *pEnd;
   pAStr = BlStre_Root+Ver_BlStre_Root[uStr1-1];
   pEnd  = BlStre_Root+Ver_BlStre_Root[uStr1];
   for (;
        (pAStr < pEnd);
        pAStr++)
   {
      if (pAStr->index == uStr2)
      {
         ucRet = (pAStr->blockweg != 0);
      }
   }
   return ucRet;
}
*/

unsigned char CheckEinfahrstrecke(unsigned short uStrIndex,
                                  unsigned short uNStrIndex,
                                  unsigned short *pMenge)
{
   unsigned char  ucRet = 1;
   unsigned short *pStrMenge,
                  *pDiffMenge;
   pStrMenge  = GetBlMenge(uStrIndex);
   pDiffMenge = MengenDifferenz(pStrMenge,pMenge);
   if (*pDiffMenge)
   {
      
      /* Die Differenz-Menge ist nicht leer:
         Es ist eine Ein-oder Ausfahrstrecke ! */ 
      
      if (*ElemInMenge(pDiffMenge,uNStrIndex))
      {
         /* Es ist eine Ausfahrstrecke */
         ucRet = 0;
      }
      else
      {
         /* Es ist eine Einfahrstrecke */
      }
   }
   else
   {
      /* Es ist eine Einfahrstrecke */
   }
   free(pStrMenge);
   free(pDiffMenge);

   return ucRet;
}

// XXXX-Lac
// Da brauche ich noch eine ‰hnliche Funktion, die noch zus‰tzlich die mˆglichen n‰chsten
// Strecken am Wegende in die Menge aufnimmt!
unsigned short *GetWegStrMengePlus(unsigned short *pWeg)
{
   unsigned short *pRet = NULL,
                  *pMenge,
                  nMp,
				  nNMp,
				  nIndex,
				  uAnzMp;
   //pRet = NewMenge(KKMAX_FAHR_ZEIT +1);
   pRet = NewMenge(g_pCourseData->GetNumberAIDCpDists() + 1);

   uAnzMp = GetAnzMengenElem(pWeg);

   pMenge = GetWegStrMenge(pWeg);
   CopyMenge(pMenge,pRet);
   FreeMenge(pMenge);

   pMenge  = GetFolgeMenge(*(pWeg+uAnzMp-1));
   MengeVereinen(pMenge,pRet);

   while(*(pMenge+1) == 0)
   {
	   nIndex = *pMenge;
	   GetStrecke(nIndex,&nMp,&nNMp);
	   FreeMenge(pMenge);
	   pMenge  = GetFolgeMenge(nNMp);
	   MengeVereinen(pMenge,pRet);
   }
   FreeMenge(pMenge);


   return pRet;
}
/*
unsigned short *GetExtWegStrMenge(unsigned short *pWeg)
{
   unsigned short *pRet = NULL,
                  *pMenge,
                  nMp,
				  nNMp,
				  nIndex,
				  uAnzMp;
   pRet = NewMenge(KKMAX_FAHR_ZEIT +1);

   uAnzMp = GetAnzMengenElem(pWeg);

   pMenge  = GetFolgeMenge(*(pWeg+uAnzMp-1));
   MengeVereinen(pMenge,pRet);

   while(*(pMenge+1) == 0)
   {
	   nIndex = *pMenge;
	   GetStrecke(nIndex,&nMp,&nNMp);
	   FreeMenge(pMenge);
	   pMenge  = GetFolgeMenge(nNMp);
	   MengeVereinen(pMenge,pRet);
   }
   FreeMenge(pMenge);

   return pRet;
}
*/

unsigned short *GetExtWegStrMenge(unsigned short *pWeg)
{
   unsigned short *pRet = NULL,
                  *pMenge,
                  nMp,
				  nNMp,
				  nIndex,
				  uAnzMp;
   //pRet = NewMenge(KKMAX_FAHR_ZEIT +1);
   pRet = NewMenge(g_pCourseData->GetNumberAIDCpDists() + 1);

   uAnzMp = GetAnzMengenElem(pWeg);

   pMenge  = GetFolgeMenge(*(pWeg+uAnzMp-1));

   while(*(pMenge+1) == 0)
   {
	MengeVereinen(pMenge,pRet);
	   nIndex = *pMenge;
	   GetStrecke(nIndex,&nMp,&nNMp);
	   FreeMenge(pMenge);
	   pMenge  = GetFolgeMenge(nNMp);
   }
   FreeMenge(pMenge);

   return pRet;
}

/* GetWegStrMenge bildet die Menge der Streckenindizes der Strecken,
   die sich aus der Meldepunktfolge pWeg ergibt. */
unsigned short *GetWegStrMenge(unsigned short *pWeg)
{
   unsigned short *pRet = NULL,
                  *pMp,
                  uAnzElem;
   //uAnzElem = GetAnzMengenElem(pWeg);
   //pRet = NewMenge(uAnzElem);
   //pRet = NewMenge(KKMAX_FAHR_ZEIT + 1);
   pRet = NewMenge(g_pCourseData->GetNumberAIDCpDists() + 1);
   if (pRet)
   {
      for (pMp = pWeg, uAnzElem = 0;
           *(pMp+1);
           pMp++, uAnzElem++)
      {
         *(pRet + uAnzElem)  = GetStrIndexFzT(*pMp,*(pMp+1));
      }
      *(pRet + uAnzElem) = 0;
   }
   return pRet;
}

/*
long GetCrashIndex(unsigned short uAStrIndex,unsigned short uVStrIndex)
{
   long        lRet;
   BlStre_Typ  *pAStr,
               *pEnd;
   pAStr = BlStre_Root+Ver_BlStre_Root[uAStrIndex-1];
   pEnd  = BlStre_Root+Ver_BlStre_Root[uAStrIndex];
   for (lRet = 0;
        (pAStr < pEnd) && !lRet;
        pAStr++)
   {
      if (pAStr->index == uVStrIndex)
      {
         lRet = pAStr - BlStre_Root + 1;
      }
   }
   return lRet;
}
*/

/*
unsigned char VFzIstFreiImBlockweg(unsigned short uWegVFz,
                                   unsigned short uBlkWeg,
                                   BlStre_Typ     *pGCrash)
{
   unsigned char uRet = 0;
   if (pGCrash->blockweg)
   {
      // VFz hat einen Blockweg im Crash
      if (uBlkWeg <= pGCrash->blockweg)
      {
         // Blockweg ist nicht keiner als CAD-Blockweg
         if (uWegVFz <= uBlkWeg)
         {
            // VFz ist noch im Blockweg !
            uRet = 1;
         }
      }
   }

   return uRet;
}
*/

unsigned char VFzIstFreiImBlockweg(unsigned short uWegVFz,
	unsigned short uBlkWeg,
	CCrashDistance* pCrashDist)
{
	unsigned char uRet = 0;
	if (pCrashDist->HasFreeWay())
	{
		// VFz hat einen Freiweg im Crash
		if (!pCrashDist->CrossOverFreeWay(uBlkWeg))
		{
			// Freiweg ist nicht keiner als Blockweg
			if (uWegVFz <= uBlkWeg)
			{
				// VFz ist noch im Blockweg !
				uRet = 1;
			}
		}
	}

	return uRet;
}

/*
unsigned char VFzIstKollisionsfrei(TypEABFzStrukt tVFz,
	TypEABFzStrukt tAFz,
	unsigned short *pMenge)
{
	// Variable
	unsigned char  ucRet = 0,
		bBlkWegZuKurz = 0,
		bKollisionsfrei,
		bVFzAmZiel;
	unsigned short *pStrMenge,
		*pDMenge,
		*pDM,
		uBlkWeg,
		uStrIndex,
		uWegVFz,
		uPosWeg,
		uFzZiel;
	long           lCrash;
	BlStre_Typ     *pBlStre,
		*pGCrash;
	BlStreExt_Typ  *pBlStreExt;
	TYP_FZDAT      *ptFzDat;

	// Menge der Strecken des AFz auf seinem Weg zum Ziel
	pStrMenge = GetWegStrMenge(tAFz.pMpBegin);

	pDMenge = MengenDurchschnitt(pStrMenge, pMenge);

	// StreckenIndex, CAD-Blockweg und gefahrenen Weg des VFz
	uStrIndex = GetStrIndexFzT(*(tVFz.pMpBegin), *(tVFz.pMpBegin + 1));
	uBlkWeg = GetBlkWegStrTab(*(tVFz.pMpBegin), uStrIndex);
	uWegVFz = GetFzWeg(fzdat + (tVFz.uNr + 1));

	// Feststellen, ob VFz am Ziel ist !
	ptFzDat = fzdat + (tVFz.uNr + 1);
	bVFzAmZiel = FzAmZiel(ptFzDat);
	if (bVFzAmZiel)
	{
		// Wenn ein Fz am Ziel ist, dann f‰hrt es nicht den Blockweg sondern den Positionierweg !
		uFzZiel = ptFzDat->fzziel.nDest;
		ptFzDat->fzziel.nDest = 0;

		uPosWeg = GetPosWegBlk(ptFzDat);

		ptFzDat->fzziel.nDest = uFzZiel;

		if (uPosWeg != 0xffff)
		{
			uBlkWeg = uPosWeg;
		}
	}
	// Schleife Å¸ber Streckenmenge und Mengen freigeben
	for (pDM = pDMenge, bKollisionsfrei = 1;
		*pDM && (!bBlkWegZuKurz && bKollisionsfrei);
		pDM++)
	{
		bKollisionsfrei = 0;
		// Feststellen, ob Blockweg/PosWeg zu kurz ist !
		lCrash = GetCrashIndex(uStrIndex, *pDM);
		if (lCrash)
		{
			pBlStre = BlStre_Root + lCrash - 1;
			if (pBlStre->blockweg)
			{
				bBlkWegZuKurz |= (pBlStre->blockweg < uBlkWeg);
			}
		}
		lCrash = GetCrashIndex(*pDM, uStrIndex);
		if (lCrash)
		{
			// Beide Strecken kollidieren !
			pBlStre = BlStre_Root + lCrash - 1;
			pBlStreExt = BlStreExt_Root + lCrash - 1;
			pGCrash = GetGegenCrash(pBlStre, *pDM);
			if (bVFzAmZiel)
			{
				// VFz ist am Ziel !
				bKollisionsfrei = VFzIstFreiAmZiel(uWegVFz, pBlStreExt);
				bKollisionsfrei |= VFzIstFreiImBlockweg(uWegVFz, uBlkWeg, pGCrash);
				if (pBlStre->kollisionsweg)
				{
					bKollisionsfrei |= (uWegVFz >= pBlStre->kollisionsweg);
				}
			}
			else
			{
				// VFz ist nicht am Ziel !
				if (pBlStre->kollisionsweg)
				{
					bKollisionsfrei = (uWegVFz >= pBlStre->
						kollisionsweg);
				}
			}
		}
		else
		{
			// Kein Crash der Strecken *pDM und uStrIndex !
#ifdef Lac27042000
			if (*pDM != uStrIndex)
			{
				bKollisionsfrei = 1;
			}
#endif
			bKollisionsfrei = 1;
		}
	} // Ende For-Schleife 
	FreeMenge(pStrMenge);
	FreeMenge(pDMenge);

	return (!bBlkWegZuKurz && bKollisionsfrei);
}
*/

unsigned char VFzIstKollisionsfrei(TypEABFzStrukt tVFz,
	TypEABFzStrukt tAFz,
	unsigned short *pMenge)
{
	// Variable
	unsigned char  ucRet = 0,
		bBlkWegZuKurz = 0,
		bKollisionsfrei,
		bVFzAmZiel;
	unsigned short *pStrMenge,
		*pDMenge,
		*pDM,
		uBlkWeg,
		uStrIndex,
		uWegVFz,
		uPosWeg,
		uFzZiel;
	CCrashDistance *pBlStre,
				   *pGCrash;
	TYP_FZDAT      *ptFzDat;

	// Menge der Strecken des AFz auf seinem Weg zum Ziel
	pStrMenge = GetWegStrMenge(tAFz.pMpBegin);

	pDMenge = MengenDurchschnitt(pStrMenge, pMenge);

	// StreckenIndex, CAD-Blockweg und gefahrenen Weg des VFz
	uStrIndex = GetStrIndexFzT(*(tVFz.pMpBegin), *(tVFz.pMpBegin + 1));
	uBlkWeg = GetBlkWegStrTab(*(tVFz.pMpBegin), uStrIndex);
	uWegVFz = GetFzWeg(fzdat + (tVFz.uNr + 1));

	// Feststellen, ob VFz am Ziel ist !
	ptFzDat = fzdat + (tVFz.uNr + 1);
	bVFzAmZiel = FzAmZiel(ptFzDat);
	if (bVFzAmZiel)
	{
		// Wenn ein Fz am Ziel ist, dann f‰hrt es nicht den Blockweg sondern den Positionierweg !
		uFzZiel = ptFzDat->fzziel.nDest;
		ptFzDat->fzziel.nDest = 0;

		uPosWeg = GetPosWegBlk(ptFzDat);

		ptFzDat->fzziel.nDest = uFzZiel;

		if (uPosWeg != 0xffff)
		{
			uBlkWeg = uPosWeg;
		}
	}
	// Schleife Å¸ber Streckenmenge und Mengen freigeben
	for (pDM = pDMenge, bKollisionsfrei = 1;
		*pDM && (!bBlkWegZuKurz && bKollisionsfrei);
		pDM++)
	{
		bKollisionsfrei = 0;
		// Feststellen, ob Blockweg/PosWeg zu kurz ist !
		pBlStre = GetPtrCrashDistance(uStrIndex, *pDM);
		if (pBlStre != NULL)
		{
			//if (pBlStre->blockweg)
			if (pBlStre->HasFreeWay())
			{
				//bBlkWegZuKurz |= (pBlStre->blockweg < uBlkWeg);
				bBlkWegZuKurz |= pBlStre->CrossOverFreeWay(uBlkWeg) ? 1 : 0;
			}
		}

		pBlStre = GetPtrCrashDistance(*pDM, uStrIndex);
		if (pBlStre != NULL)
		{
			// Beide Strecken kollidieren !
			pGCrash = GetGegenCrash(pBlStre, *pDM);
			if (bVFzAmZiel)
			{
				// VFz ist am Ziel !
				bKollisionsfrei = VFzIstFreiAmZiel(uWegVFz, pBlStre);
				bKollisionsfrei |= VFzIstFreiImBlockweg(uWegVFz, uBlkWeg, pGCrash);
				//if (pBlStre->kollisionsweg)
				if (pBlStre->HasAbsCollWay())
				{
					//bKollisionsfrei |= (uWegVFz >= pBlStre->kollisionsweg);
					bKollisionsfrei |= (pBlStre->CrossOverAbsCollWay(uWegVFz)) ? 1 : 0;
				}
			}
			else
			{
				// VFz ist nicht am Ziel !
				//if (pBlStre->kollisionsweg)
				if (pBlStre->HasAbsCollWay())
				{
					//bKollisionsfrei = (uWegVFz >= pBlStre->kollisionsweg);
					bKollisionsfrei = (pBlStre->CrossOverAbsCollWay(uWegVFz));
				}
			}
		}
		else
		{
			// Kein Crash der Strecken *pDM und uStrIndex !
#ifdef Lac27042000
			if (*pDM != uStrIndex)
			{
				bKollisionsfrei = 1;
			}
#endif
			bKollisionsfrei = 1;
		}
	} // Ende For-Schleife 
	FreeMenge(pStrMenge);
	FreeMenge(pDMenge);

	return (!bBlkWegZuKurz && bKollisionsfrei);
}

unsigned char CheckBlkWegZumBereich(unsigned short uMp,
                                    unsigned short uStrIndex,
                                    unsigned short *pMenge,
                                    unsigned short *pWStrMenge)
{
   //unsigned short *pStrMenge;
   unsigned char  ucRet = 0;
   unsigned short *pDMenge,
                  *pDM,
                  uBlkWeg;

   //pStrMenge = GetBlMenge(uStrIndex);
   //pDMenge   = MengenDurchschnitt(pStrMenge,pMenge);

   pDMenge   = MengenDurchschnitt(pWStrMenge,pMenge);
   uBlkWeg   = GetBlkWegStrTab(uMp,uStrIndex);

   for (pDM = pDMenge;
        *pDM && !ucRet;
        pDM++)
   {
      ucRet = BlkWegZuKurz(uBlkWeg,uStrIndex,*pDM);
   }

   //FreeMenge(pStrMenge);
   FreeMenge(pDMenge);

   return ucRet;
}

/*
unsigned char CheckAmZielFreiZumBereich(unsigned short uFzWeg,
                                        unsigned short uStrIndex,
                                        unsigned short *pMenge,
                                        unsigned short *pWStrMenge)
{
   //unsigned short *pStrMenge;
   unsigned char  ucRet;
   unsigned short *pDMenge,
                  *pDM;
   long           lCrash;
   BlStreExt_Typ  *pCrashExt;

   pDMenge   = MengenDurchschnitt(pWStrMenge,pMenge);

   for (pDM = pDMenge,ucRet = 1;
        *pDM && ucRet;
        pDM++)
   {
      lCrash = GetCrashIndex(*pDM,uStrIndex);
      if (lCrash)
      {
         pCrashExt = BlStreExt_Root + lCrash - 1;
         if (!VFzIstFreiAmZiel(uFzWeg,pCrashExt))
         {
            //Fz ist nicht frei Ziel
            ucRet = 0;
         }
      }
   }

   FreeMenge(pDMenge);

   return ucRet;
}
*/

unsigned char CheckAmZielFreiZumBereich(unsigned short uFzWeg,
	unsigned short uStrIndex,
	unsigned short *pMenge,
	unsigned short *pWStrMenge)
{
	//unsigned short *pStrMenge;
	unsigned char  ucRet;
	unsigned short *pDMenge,
		*pDM;
	CCrashDistance  *pCrash;

	pDMenge = MengenDurchschnitt(pWStrMenge, pMenge);

	for (pDM = pDMenge, ucRet = 1;
		*pDM && ucRet;
		pDM++)
	{
		pCrash = GetPtrCrashDistance(*pDM, uStrIndex);
		if (pCrash != NULL)
		{
			if (!VFzIstFreiAmZiel(uFzWeg, pCrash))
			{
				//Fz ist nicht frei Ziel
				ucRet = 0;
			}
		}
	}

	FreeMenge(pDMenge);

	return ucRet;
}

unsigned short GetEintauchStrecke(unsigned short *pWeg,
                                  unsigned short *pMenge,
                                  unsigned short *pWStrMenge,
                                  unsigned short uFzWeg,
                                  unsigned char  bFzAmZiel,
                                  unsigned char  *pFzBlocktVorher)
{
	unsigned short uRet = 0,
		*pMp,
		uStrIndex,
		*pCrElem;
	unsigned char  bSofort,
		bEinfahrstrecke,
		bVorherBlocken,
		bFreiAmZiel;
	
	*pFzBlocktVorher = 0;
	
	for (pMp = pWeg, bSofort = 1;
	*(pMp+1) && !uRet;
	pMp++)
	{
		uStrIndex = GetStrIndexFzT(*pMp,*(pMp + 1));
		pCrElem   = ElemInMenge(pMenge,uStrIndex);
		
		if (*pCrElem)
		{
			uRet = uStrIndex;
		}
		else
		{
			bSofort = 0;
		}
	}
	/* Der Weg taucht nicht in die gemeinsame Crashmenge ein ! */
	if (!uRet)
	{
		uRet = uStrIndex;
	}
	/* Wenn vorhanden den Index der nÑchsten Strecke ermitteln ! */
	if (*(pMp+1))
	{
		uStrIndex = GetStrIndexFzT(*pMp,*(pMp + 1));
	}
	else
	{
		uStrIndex = 0;
	}
	if (bSofort && uStrIndex)
	{
	/* Fz ist sofort in der Durchschnittsmenge und hat eine n‰chste Strecke:
	   Feststellen, ob es eine Einfahrstrecke in die Menge ist. */
		bEinfahrstrecke = CheckEinfahrstrecke(uRet,uStrIndex,pMenge);

		if (!bEinfahrstrecke)
		{
			uRet = 0;
		}
		else
		{
			/* Blockweg zum CrBereich pr¸fen */
			bVorherBlocken = CheckBlkWegZumBereich(*(pMp - 1),uRet,pMenge,pWStrMenge);

			if (bVorherBlocken)
			{
				if (bFzAmZiel)
				{
					/* Mp ist ein Ziel */
					bFreiAmZiel = CheckAmZielFreiZumBereich(uFzWeg,uRet,pMenge,pWStrMenge);

					if (!bFreiAmZiel)
					{
						uRet = 0;
					}
				}
				else
				{
					uRet = 0;
				}
			}
		}
	}
	else
	{
		if (!bSofort)
		{
			/* Blockweg zum CrBereich pr¸fen */
			bVorherBlocken = CheckBlkWegZumBereich(*(pMp - 1),uRet,pMenge,pWStrMenge);

			if (bVorherBlocken)
			{
				uRet = GetStrIndexFzT(*(pMp - 2),*(pMp - 1));

				*pFzBlocktVorher = 1;
			}
		}
		else
		{
			uRet = 0;
		}
	}
	return uRet;
}

#ifdef ZEITSPAREN
unsigned short *GetNextDurchschnitt(unsigned short *pWeg,
	unsigned short *pCrMenge)
{
	unsigned short *pDMenge,
		*pStrMenge,
		*pTmpMenge,
		*pMp,
		uStrIndex;
	unsigned char  bAbbruch,
		bEingetaucht,
		bAufgetaucht;

	//pTmpMenge      = NewMenge(KKMAX_FAHR_ZEIT + 1);
	pTmpMenge = NewMenge(g_pCourseData->GetNumberAIDCpDists() + 1);
	bEingetaucht = 0;
	bAufgetaucht = 0;
	//Schleife Å¸ber alle Strecken des Weges solange bis eingetaucht und aufgetaucht ist ! 
	for (pMp = pWeg, bAbbruch = 0;
		*(pMp + 1) && !bAbbruch;
		pMp++)
	{
		uStrIndex = GetStrIndexFzT(*pMp, *(pMp + 1));

		if (*ElemInMenge(pCrMenge, uStrIndex))
		{
			bEingetaucht = 1;
			bAufgetaucht = 0;
			pStrMenge = GetBlMenge(uStrIndex);
			MengeErweitern(pStrMenge, uStrIndex);
			MengeVereinen(pStrMenge, pTmpMenge);

			FreeMenge(pStrMenge);
		}
		else
		{
			bAufgetaucht = 1;
		}
		//¸ber Abbruch entscheiden !
		bAbbruch = (bEingetaucht && bAufgetaucht);
	}
	pDMenge = MengenDurchschnitt(pTmpMenge,pCrMenge);

	FreeMenge(pTmpMenge);

	return pDMenge;
}
#else
/* HABENZEIT */
unsigned short *GetNextDurchschnitt( unsigned short *pWeg,
                                    unsigned short *pCrMenge)
{
  unsigned short *pDMenge,
                 *pStrMenge,
                 *pTmpDMenge,
                 *pMp,
                 uStrIndex,
                 uAnzElem;
  unsigned char  bAbbruch;
/*~ BLOCK */
  uAnzElem   = GetAnzMengenElem(pCrMenge) + 1;
  pDMenge    = NewMenge(uAnzElem);
  pTmpDMenge = NewMenge(1);
/*~ BLOCK */
/*~ COMMENT */
  /*
  Schleife Å¸ber alle Strecken des Weges solange wie die 
  GesamtDurchschnittsmenge leer ist oder
   */
/*~ STRUCT */
  for (pMp = pWeg, bAbbruch = 0;
       *(pMp+1) && !bAbbruch;
       pMp++)
  {
     FreeMenge(pTmpDMenge);
/*~ BLOCK */
     uStrIndex  = GetStrIndexFzT(*pMp,*(pMp+1));
     pStrMenge  = GetBlMenge(uStrIndex);
     MengeErweitern(pStrMenge,uStrIndex);
     pTmpDMenge = MengenDurchschnitt(pStrMenge,pCrMenge);
     MengeVereinen(pTmpDMenge,pDMenge);
/*~ BLOCK */
/*~ COMMENT */
     /*
     öber Abbruch entscheiden !
      */
/*~ BLOCK */
     bAbbruch = (!*pTmpDMenge && *pDMenge);
/*~ BLOCK */
     FreeMenge(pStrMenge);
  }
  FreeMenge(pTmpDMenge);
/*~ BLOCK */
  return pDMenge;
}
#endif

/*
unsigned short ErweiternCrStreMengeIdx(unsigned short *pWeg,
	unsigned short *pMenge)
{
	unsigned short uRet = 0,
		*pIdx,
		uStrIndex;
	BlStre_Typ     *pCr,
		*pCrEnd;

	for (pIdx = pWeg;
		*pIdx;
		pIdx++)
	{
		uStrIndex = *pIdx;
		pCr = BlStre_Root + Ver_BlStre_Root[uStrIndex - 1];
		pCrEnd = BlStre_Root + Ver_BlStre_Root[uStrIndex];
		for (;
			pCr < pCrEnd;
			pCr++)
		{
			uRet += MengeErweitern(pMenge, pCr->index);
		}
		uRet += MengeErweitern(pMenge, uStrIndex);
	}
	return uRet;
}
*/

/*
unsigned short ErweiternCrStreMengeIdx(unsigned short *pWeg,
	unsigned short *pMenge)
{
	unsigned short uRet = 0,
		*pIdx,
		uStrIndex;
	BlStre_Typ     *pCr,
		*pCrEnd;

	for (pIdx = pWeg;
		*pIdx;
		pIdx++)
	{
		uStrIndex = *pIdx;
		pCr = BlStre_Root + Ver_BlStre_Root[uStrIndex - 1];
		pCrEnd = BlStre_Root + Ver_BlStre_Root[uStrIndex];
		for (;
			pCr < pCrEnd;
			pCr++)
		{
			uRet += MengeErweitern(pMenge, pCr->index);
		}
		uRet += MengeErweitern(pMenge, uStrIndex);
	}
	return uRet;
}
*/

unsigned short ErweiternCrStreMengeIdx(unsigned short *pWeg, unsigned short *pMenge)
{
	unsigned short uRet = 0,
		*pIdx;
	CCrashDistanceArray* pArrCrashDistances;

	for (pIdx = pWeg;
		*pIdx;
		pIdx++)
	{
		pArrCrashDistances = GetPtrArrCrashDistances((*pIdx) - 1);
		for (int count = 0;
			count <  pArrCrashDistances->GetSize();
			count++)
		{
			uRet += MengeErweitern(pMenge, (*pArrCrashDistances)[count].GetIndex());
		}
		uRet += MengeErweitern(pMenge, *pIdx);
	}
	return uRet;
}

/*
unsigned short ErweiternCrStreMenge( unsigned short *pWeg,
                                     unsigned short *pMenge)
{
   unsigned short uRet = 0,
                  *pMp,
                  uStrIndex;
   BlStre_Typ     *pCr,
                  *pCrEnd;

   for (pMp = pWeg;
        *(pMp+1);
        pMp++)
   {
      uStrIndex = GetStrIndexFzT(*pMp,*(pMp+1));
      ugStrIndex = uStrIndex;

	  if (uStrIndex)
      {
         pCr    = BlStre_Root+Ver_BlStre_Root[uStrIndex-1];
         pCrEnd = BlStre_Root+Ver_BlStre_Root[uStrIndex];

		 for (;
              pCr < pCrEnd;
              pCr++)
         {
            uRet += MengeErweitern(pMenge,pCr->index);
         }
      }
      else
      {
         break;
      }

      uRet += MengeErweitern(pMenge,uStrIndex);
   }

   return uRet;
}
*/

unsigned short ErweiternCrStreMenge(unsigned short *pWeg,
	unsigned short *pMenge)
{
	unsigned short uRet = 0,
		*pMp,
		uStrIndex;
	CCrashDistanceArray     *pArrCrashDistances;

	for (pMp = pWeg;
		*(pMp + 1);
		pMp++)
	{
		uStrIndex = GetStrIndexFzT(*pMp, *(pMp + 1));
		ugStrIndex = uStrIndex;

		if (uStrIndex)
		{
			pArrCrashDistances = GetPtrArrCrashDistances(uStrIndex - 1);

			for (int count = 0;
				count < pArrCrashDistances->GetSize();
				count++)
			{
				uRet += MengeErweitern(pMenge, (*pArrCrashDistances)[count].GetIndex());
			}
		}
		else
		{
			break;
		}

		uRet += MengeErweitern(pMenge, uStrIndex);
	}

	return uRet;
}

unsigned short GetCrStreMenge(unsigned short *pWeg,
                              unsigned short *pMenge)
{
   unsigned short uRet = 0;
   *pMenge = 0;
   uRet = ErweiternCrStreMenge(pWeg,pMenge);
   return uRet;
}



unsigned short *ElemInMenge(unsigned short *pMenge,unsigned short uElem)
{
   for (;
        *pMenge;
        pMenge++)
   {
      if (*pMenge == uElem)
      {
         break;
      }
   }
   return pMenge;
}

void CopyMenge(unsigned short *psource, unsigned short *pdest)
{
   for (;
        *psource;
        psource++,pdest++)
   {
      *pdest = *psource;
   }
   *pdest = 0;
   return;
}

void MoveMenge(unsigned short *psource, unsigned short *pdest)
{
   CopyMenge(psource, pdest);
   *psource = 0;
   return;
}

/****************************************************************
Datum        : 10.11.97
Beschreibung : Die Funktion prueft, ob iElem in der Menge pmenge vorhanden
               ist. Wenn nicht, dann wird iElem in die Menge aufgenommen,
               indem es ans Ende der Menge geschrieben wird.
****************************************************************/
unsigned char MengeErweitern(unsigned short *pmenge, unsigned short iElem)
{
   unsigned char rein;
   unsigned short *pNextElem;

   pNextElem = ElemInMenge(pmenge,iElem);

   if (!(*pNextElem))
   {
      *pNextElem = iElem;
      pNextElem++;
      *pNextElem = 0;
      rein = 1;
   }
   else
   {
      rein = 0;
   }
   return rein;
}

unsigned char MengeVereinen(unsigned short *psource, unsigned short *pdest)
{
   unsigned short *pUS;
   unsigned char ucRet = 0;
/*~ STRUCT */
   for (pUS = psource;
        *pUS;
        pUS++)
   {
      ucRet |= MengeErweitern(pdest,*pUS);
   }
   return ucRet;
}

/*
// Get distance (Returns:0 and MP and NMP or 1, failure)
short GetStrecke(unsigned short uIndex,
                 unsigned short *pMp,
                 unsigned short *pNMp)
{
   short             ret;
   int * pIndex;
   Meldepunkttabelle * p_mptab;
   unsigned short    uBeginIndex,
                     uEndIndex,
                     i;
   if (uIndex <= KKMAX_FAHR_ZEIT)
   {
	   ret = 0;
	   bool bFound;
	   int nMaxMp = KKMAXMP;
	   for (bFound = false, pIndex = vmptab, i = 0;
	   i <= nMaxMp && (!bFound) ;
	   i++,pIndex++)
	   {
		   if (*pIndex != 0)
		   {
			   // XXX-Lac01:
			   //p_mptab = mptab + vmptab [*pIndex] - 1;
			   p_mptab = mptab + *pIndex - 1;
			   uBeginIndex = p_mptab->str_idx; //- 1;
			   if (i < KKMAXMP)
			   {
				   uEndIndex = (p_mptab+1)->str_idx; // - 1;
			   }
			   else
			   {
				   uEndIndex = uIndex+1;
			   }
			   if ((uIndex >= uBeginIndex) && (uIndex < uEndIndex))
			   {
				   *pMp  = i;
				   *pNMp = (MpNmpTab + uIndex - 1)->nmp;
				   bFound = true;
			   }
		   }
	   }
   }
   else
   {
	   // Index grˆsser als Streckenanzahl !
	   ret = 1;
   }
   return ret;
}
*/

// Get distance (Returns:0 and MP and NMP or 1, failure)
short GetStrecke(unsigned short uIndex,
	unsigned short *pMp,
	unsigned short *pNMp)
{
	short             ret;
	unsigned short    uBeginIndex,
		uEndIndex;
	CCpDistance* pCpDistance;
	CCommPoint*  pCommPoint;
	CCourseAGVType* pCourseAGVtype = g_pCourseData->GetPtrCourseAGVType(1);

	if (uIndex <= pCourseAGVtype->GetNumberDistances())
	{
		ret = 0;
		bool bFound = false;
		for (int count = 0;
			count <= (int)pCourseAGVtype->GetNumberCps() && (!bFound);
			count++)
		{
			// XXX-Lac01:
			//p_mptab = mptab + vmptab [*pIndex] - 1;
			pCommPoint = pCourseAGVtype->GetPtrCommPointByIndex(count);
			uBeginIndex = pCommPoint->GetDistanceIndex();
			uEndIndex = uBeginIndex + pCommPoint->GetNCps();

			if ((uIndex >= uBeginIndex) && (uIndex < uEndIndex))
			{
				//Mp gefunden
				*pMp = pCommPoint->GetCp();
				pCpDistance = pCourseAGVtype->GetPtrDistance(uIndex);
				*pNMp = pCpDistance->GetNCp();
				bFound = true;
			}
		}
	}
	else
	{
		// Index grˆsser als Streckenanzahl !
		ret = 1;
	}
	return ret;
}

/*
long StreckenCrash(unsigned short *pA, unsigned short *pV)
{
	BlStre_Typ    *pAStr,
		*pEnd;
	long           ret;
	pATest = pA;
	pVTest = pV;
	uAStr = GetStrIndexFzT(*pA,*(pA+1));
   uVStr = GetStrIndexFzT(*pV,*(pV+1));
   pAStr = BlStre_Root+Ver_BlStre_Root[uAStr-1];
   pEnd  = BlStre_Root+Ver_BlStre_Root[uAStr];
   for (ret = 0;
        (pAStr < pEnd) && !ret;
        pAStr++)
   {
      if (pAStr->index == uVStr)
      {
         ret = pAStr - BlStre_Root + 1;
      }
   }
   return ret;
}
*/

/*
Datum       : 13.07.98

Beschreibung: Die Funktion liefert den Index des uMp in der Mp-
              Folge pFolge.
              Wird uMp nicht gefunden, liefert die Funktion den Wert 
Null.
*/
unsigned short GetIndexMpFolge(unsigned short *pFolge,unsigned short uMp)
{
   unsigned short ret = 0,
                  index,
                  *pMp;

   for (pMp = pFolge,index = 1;
        *pMp;
        pMp++,index++)
   {
      if (*pMp == uMp)
      {
         ret = index;
         break;
      }
   }
   return ret;
}

unsigned short *GetMpFolgeEnd(unsigned short *pMpFolge)
{
   for (;
        *(pMpFolge+2);
        pMpFolge++)
   {
   }
   return pMpFolge;
}

unsigned short GetFzInCrashMenge(unsigned short *pCrMenge,
                                 unsigned short *pCrFz,
                                 unsigned short uAFz)
{
   unsigned short uRet = 0,
                  uFz,
                  uStrIndex,
                  *pElem;
   for (uFz = 1;
        //uFz <= KKFZMAX;
		uFz <= g_pCourseData->GetNumberAGVs();
   uFz++)
   {
	   if (TEST_FZ_IM_SYSTEM(fzdat[uFz]))
      {
         // Fz ist im System
         if ((uFz != uAFz) &&
             fzdat[uFz].nmp)
         {
            uStrIndex = GetStrIndexFzT(fzdat[uFz].mp,fzdat[uFz].nmp);
            if (uStrIndex)
            {
               pElem = ElemInMenge(pCrMenge,uStrIndex);
               if (*pElem)
               {
                  *(pCrFz+uRet) = uFz;
                  uRet++;
               }
            }
            else
            {
               // unbekannte Strecke !
            }
         }
      }
   }
   return uRet;
}

/*
unsigned char VorbeiFahren(TypEABFzStrukt *ptAFz,
                           unsigned short *pCrMenge,
                           unsigned short uVStrIndex,
                           unsigned short uVFzWeg)
{
   unsigned char  ucRet = 1,
                  bAFzHatKWeg = 0;	// Michael 09.06.2005
   unsigned short *pDMenge,
                  *pAFzMenge,
                  *pM,
                  uZKFreiWegAbs,
                  uAnzElem;
   BlStreExt_Typ  *pCrashExt;
   BlStre_Typ     *pCrash;
   long           lCrash;

   pAFzMenge = GetWegStrMenge(ptAFz->pMpBegin);
   uAnzElem  = GetAnzMengenElem(pAFzMenge);

   if (uAnzElem > 1)
   {
      if (*(pAFzMenge + uAnzElem -1) == uVStrIndex)
      {
         *(pAFzMenge + uAnzElem -1) = 0;
      }
   }
   pDMenge   = MengenDurchschnitt(pCrMenge,pAFzMenge);

   for (pM = pDMenge;
        *pM && ucRet;
        pM++)
   {
      lCrash = GetCrashIndex(*pM,uVStrIndex);
      if (lCrash)
      {
         pCrashExt = BlStreExt_Root + lCrash - 1;
         uZKFreiWegAbs =  pCrashExt->uZKWeg;
         uZKFreiWegAbs += pCrashExt->uZKFreiWeg;
         if (pCrashExt->uZKFreiWeg &&
             (uVFzWeg >= pCrashExt->uZKWeg) &&
             (uVFzWeg <  uZKFreiWegAbs))
         {
            // Ist ok !
         }
         else
         {
            ucRet = 0;
         }
      }
      else
      {
         if (*pM != uVStrIndex)
         {
            // Ist ok !
         }
         else
         {
            ucRet = 0;
         }
      }

      if (ucRet)
      {
         lCrash = GetCrashIndex(uVStrIndex,*pM);
         if (lCrash)
         {
            pCrash = BlStre_Root + lCrash - 1;
            if (pCrash->kollisionsweg)
            {
               bAFzHatKWeg = 1;
            }
            else
            {
               bAFzHatKWeg = 0;
            }
         }
         else
         {
            bAFzHatKWeg = 1;
         }
      }
   }

   if (!bAFzHatKWeg)
   {
      ucRet = 0;
   }

   FreeMenge(pDMenge);
   FreeMenge(pAFzMenge);
   return ucRet;
}
*/

unsigned char VorbeiFahren(TypEABFzStrukt *ptAFz,
	unsigned short *pCrMenge,
	unsigned short uVStrIndex,
	unsigned short uVFzWeg)
{
	unsigned char  ucRet = 1,
		bAFzHatKWeg = 0;	// Michael 09.06.2005
	unsigned short *pDMenge,
		*pAFzMenge,
		*pM,
		uAnzElem;
	CCrashDistance *pCrash;

	pAFzMenge = GetWegStrMenge(ptAFz->pMpBegin);
	uAnzElem = GetAnzMengenElem(pAFzMenge);

	if (uAnzElem > 1)
	{
		if (*(pAFzMenge + uAnzElem - 1) == uVStrIndex)
		{
			*(pAFzMenge + uAnzElem - 1) = 0;
		}
	}
	pDMenge = MengenDurchschnitt(pCrMenge, pAFzMenge);

	for (pM = pDMenge;
		*pM && ucRet;
		pM++)
	{
		pCrash = GetPtrCrashDistance(*pM, uVStrIndex);
		if (pCrash != NULL)
		{
			//uZKFreiWegAbs = pCrashExt->uZKWeg;
			//uZKFreiWegAbs += pCrashExt->uZKFreiWeg;
			//if (pCrashExt->uZKFreiWeg &&
			//	(uVFzWeg >= pCrashExt->uZKWeg) &&
			//	(uVFzWeg <  uZKFreiWegAbs))
			if (pCrash->HasRelCollWay() &&
				pCrash->CrossOverRelCollWay(uVFzWeg) &&
				!pCrash->CrossOverRelFreeArea(uVFzWeg))
			{
				// Ist ok !
			}
			else
			{
				ucRet = 0;
			}
		}
		else
		{
			if (*pM != uVStrIndex)
			{
				// Ist ok !
			}
			else
			{
				ucRet = 0;
			}
		}

		if (ucRet)
		{
			pCrash = GetPtrCrashDistance(uVStrIndex, *pM);
			if (pCrash != NULL)
			{
				//if (pCrash->kollisionsweg)
				if (pCrash->HasAbsCollWay())
				{
					bAFzHatKWeg = 1;
				}
				else
				{
					bAFzHatKWeg = 0;
				}
			}
			else
			{
				bAFzHatKWeg = 1;
			}
		}
	}

	if (!bAFzHatKWeg)
	{
		ucRet = 0;
	}

	FreeMenge(pDMenge);
	FreeMenge(pAFzMenge);
	return ucRet;
}

/*
unsigned char FzIstVoraus(TypEABFzStrukt *ptAFz,
                          unsigned short *pCrMenge,
                          unsigned short uVStrIndex,
                          unsigned short uVFzWeg,
                          unsigned short uBlkWegVFz)
{
   unsigned char  ucRet = 1,
                  bMengeGeaendert;
   unsigned short *pDMenge,
                  *pM;
   BlStre_Typ     *pCrash;
   long           lCrash;
   unsigned short *pAFzMenge;

   pM         = GetNextDurchschnitt( ptAFz->pMpBegin, pCrMenge );
   pAFzMenge  = GetWegStrMenge     ( ptAFz->pMpBegin );
   pDMenge    = MengenDurchschnitt ( pM, pAFzMenge );

   bMengeGeaendert = CheckMengeAlsWeg( pDMenge, pAFzMenge );

   FreeMenge(pM);

   for (pM = pDMenge;
        *pM && ucRet;
        pM++)
   {
      lCrash = GetCrashIndex(uVStrIndex,*pM);

	  if (lCrash)
      {
         pCrash = BlStre_Root + lCrash - 1;

		 if (pCrash->blockweg &&
             (uBlkWegVFz <= pCrash->blockweg) &&
             (uVFzWeg <= pCrash->blockweg))
         {
            //Ist ok !
         }
         else
         {
            if (tAFzDat.mp == fzdat[uVerglFz].nmp)
            {
               //Ist ok !
               //EE-la, 27.06.2000: Dieses 'if' eingef¸Ågt ! 
               if (pCrash->kollisionsweg)
               {
                  //Ist ok !
               }
               else
               {
                  ucRet = 0;
               }
            }
            else
            {
               ucRet = 0;
            }
#ifdef BAL3_EH_AUSFAHRT
            //Original: ucRet = 0;
               if (pCrash->blockweg)
               {
                  //VFz ist Å¸ber den berechneten Blockweg hinaus
                  lCrash = GetCrashIndex(*pM,uVStrIndex);

				  if (lCrash)
                  {
                     pCrash = BlStre_Root + lCrash - 1;

					 if (!pCrash->blockweg)
                     {
                        //AFz hat keinen Blockweg: Ist ok !
                     }
                     else
                     {
                        ucRet = 0;
                     }
                  }
                  else
                  {
                     ucRet = 0;
                  }
               }
               else
               {
                  //VFz hat keinen Blockweg
                  ucRet = 0;
               }
#endif
         }
      }
      else
      {
         if (*pM != uVStrIndex)
         {
            //Ist ok !
         }
         else
         {
            ucRet = 0;
         }
      }
   }

   FreeMenge(pDMenge);
   FreeMenge(pAFzMenge);

   return ucRet;
}
*/

unsigned char FzIstVoraus(TypEABFzStrukt *ptAFz,
	unsigned short *pCrMenge,
	unsigned short uVStrIndex,
	unsigned short uVFzWeg,
	unsigned short uBlkWegVFz)
{
	unsigned char  ucRet = 1,
		bMengeGeaendert;
	unsigned short *pDMenge,
		*pM;
	CCrashDistance     *pCrash;
	unsigned short *pAFzMenge;

	pM = GetNextDurchschnitt(ptAFz->pMpBegin, pCrMenge);
	pAFzMenge = GetWegStrMenge(ptAFz->pMpBegin);
	pDMenge = MengenDurchschnitt(pM, pAFzMenge);

	bMengeGeaendert = CheckMengeAlsWeg(pDMenge, pAFzMenge);

	FreeMenge(pM);

	for (pM = pDMenge;
		*pM && ucRet;
		pM++)
	{
		pCrash = GetPtrCrashDistance(uVStrIndex, *pM);

		if (pCrash != NULL)
		{
			//if (pCrash->blockweg &&
			//	(uBlkWegVFz <= pCrash->blockweg) &&
			//	(uVFzWeg <= pCrash->blockweg))
			if (pCrash->HasFreeWay() &&
				!pCrash->CrossOverFreeWay(uBlkWegVFz) &&
				!pCrash->CrossOverFreeWay(uVFzWeg))
			{
				//Ist ok !
			}
			else
			{
				if (tAFzDat.mp == fzdat[uVerglFz].nmp)
				{
					//Ist ok !
					//EE-la, 27.06.2000: Dieses 'if' eingef¸Ågt ! 
					//if (pCrash->kollisionsweg)
					if (pCrash->HasAbsCollWay())
					{
						//Ist ok !
					}
					else
					{
						ucRet = 0;
					}
				}
				else
				{
					ucRet = 0;
				}
			}
		}
		else
		{
			if (*pM != uVStrIndex)
			{
				//Ist ok !
			}
			else
			{
				ucRet = 0;
			}
		}
	}

	FreeMenge(pDMenge);
	FreeMenge(pAFzMenge);

	return ucRet;
}

unsigned char GetBlFzKette(unsigned short uFz,unsigned short uVFz)
{
   unsigned char  ucRet = 0;
   unsigned short uBlFz,
                  uI = 0;      /* vermeidet Endlos-Schleife bei Deadlock
                                */
   do
   {
	   if (TEST_AGV_STOP(fzdat[uFz]))
      {
         uBlFz = fzdat[uFz].fz_blk;
         if (uBlFz)
         {
            if (uBlFz == uVFz)
            {
               ucRet = 1;
            }
            else
            {
               uFz = uBlFz;
               uI++;
            }
         }
         else
         {
            uFz = 0;
         }
      }
      else
      {
         uFz = 0;
      }
   }
   //while (!ucRet && uFz && (uI < KKFZMAX));
   while (!ucRet && uFz && (uI < g_pCourseData->GetNumberAGVs()));
   return ucRet;
}

unsigned short *GetKollStrMenge(unsigned short uStrIndex,
                                unsigned short *pDMenge)
{
   unsigned short *pM,
                  *pKollMenge = NULL;
 
   for (pM = pDMenge;
        *pM;
        pM++)
   {
      //if (GetCrashIndex(uStrIndex,*pM))
	   if (GetPtrCrashDistance(uStrIndex, *pM) != NULL)
	  {
         if (pKollMenge == NULL)
         {
            pKollMenge = NewMenge(GetAnzMengenElem(pDMenge)+1);
         }
         MengeErweitern(pKollMenge,*pM);
      }
   }
   return pKollMenge;
}

/*
// Get "Set" of possible crash distances around this communication point
unsigned short *GetFolgeMenge(unsigned short uMp)
{
   unsigned short *pMenge = NULL,
                  i,
                  uIndex;
   Meldepunkttabelle *pMp;
   NMP_tabelle       *pStre;
   
   // Zeiger auf MP in der MP-DatenTabelle
   pMp = mptab + vmptab [uMp] - 1;

   // Zeiger auf Tabelle der naechsten Meldepunkte
   pStre = MpNmpTab + pMp->str_idx - 1;

   pMenge = NewMenge(pMp->anz_nmp+1);

   for (i = 0;
        i < pMp->anz_nmp;
        i++)
   {
      uIndex = GetStrIndexFzT(uMp,(pStre+i)->nmp);
      MengeErweitern(pMenge,uIndex);
   }
   return pMenge;
}
*/

// Get "Set" of possible crash distances around this communication point
unsigned short *GetFolgeMenge(unsigned short uMp)
{
	unsigned short *pMenge = NULL,
		nDistances,
		uIndex;

	// Zeiger auf MP in der MP-DatenTabelle
	CCommPoint *pCommPoint = g_pCourseData->GetPtrCourseAGVType(1)->GetPtrCommPoint(uMp);

	// Zeiger auf Tabelle der naechsten Meldepunkte
	uIndex = pCommPoint->GetDistanceIndex();
	nDistances = pCommPoint->GetNCps();

	pMenge = NewMenge(nDistances + 1);

	for (int count = 0;
		count < nDistances;
		count++)
	{
		MengeErweitern(pMenge, uIndex++);
	}

	return pMenge;
}

#ifdef MIT_DEADLOCK_HISTORY 
void NeuesHistEreignis(unsigned short uFz,
                       unsigned short uMp,
                       unsigned short uNmp,
                       unsigned short uUemp,
                       unsigned short uZiel,
                       unsigned short uWegGefahren,
                       unsigned char  bLast)
{
   TypDLHistory   *ptDLHist;
   TypMpHistory   *ptMpHist;
   unsigned short uMpIndex,
                  uVFz;

   ptDLHist = ptDLHistoryRoot + uFz;
   ptMpHist = ptDLHist->ptMpHist + DL_HISTORY_LEN - 1;

   for (uMpIndex = DL_HISTORY_LEN-1;
        uMpIndex;
        uMpIndex--,ptMpHist--)     
   {
      ptMpHist->uMp                = (ptMpHist-1)->uMp;
      ptMpHist->uNmp               = (ptMpHist-1)->uNmp;
      ptMpHist->uUemp              = (ptMpHist-1)->uUemp;
      ptMpHist->uZiel              = (ptMpHist-1)->uZiel;
      ptMpHist->uErsterWegGefahren = (ptMpHist-1)->uErsterWegGefahren;
      ptMpHist->uAktWegGefahren    = (ptMpHist-1)->uAktWegGefahren;
      ptMpHist->bLast              = (ptMpHist-1)->bLast;
      ptMpHist->uAnzBehandelt      = (ptMpHist-1)->uAnzBehandelt;
      ptMpHist->m_ucErstStunde     = (ptMpHist-1)->m_ucErstStunde;
      ptMpHist->m_ucErstMinute     = (ptMpHist-1)->m_ucErstMinute;
      ptMpHist->m_ucErstSekunde    = (ptMpHist-1)->m_ucErstSekunde;
      ptMpHist->m_ucAktStunde      = (ptMpHist-1)->m_ucAktStunde;
      ptMpHist->m_ucAktMinute      = (ptMpHist-1)->m_ucAktMinute;
      ptMpHist->m_ucAktSekunde     = (ptMpHist-1)->m_ucAktSekunde;

      for (uVFz = 0;
           //(uVFz+1) < KKFZMAX;
		   (uVFz + 1) < g_pCourseData->GetNumberAGVs();
	  uVFz++)
      {
         ptMpHist->ptVFzFeFv[uVFz] = (ptMpHist-1)->ptVFzFeFv[uVFz];
      }
   }
   ptDLHist->ptMpHist->uMp                 = uMp;
   ptDLHist->ptMpHist->uNmp                = uNmp;
   ptDLHist->ptMpHist->uUemp               = uUemp;
   ptDLHist->ptMpHist->uZiel               = uZiel;
   ptDLHist->ptMpHist->uErsterWegGefahren  = uWegGefahren;
   ptDLHist->ptMpHist->uAktWegGefahren     = uWegGefahren;
   ptDLHist->ptMpHist->bLast               = bLast;
   ptDLHist->ptMpHist->uAnzBehandelt       = 0;

   CTime time = CTime::GetCurrentTime ();

   ptDLHist->ptMpHist->m_ucErstStunde  = (unsigned char)time.GetHour ();
   ptDLHist->ptMpHist->m_ucErstMinute  = (unsigned char)time.GetMinute ();
   ptDLHist->ptMpHist->m_ucErstSekunde = (unsigned char)time.GetSecond ();
   ptDLHist->ptMpHist->m_ucAktStunde   = (unsigned char)time.GetHour ();
   ptDLHist->ptMpHist->m_ucAktMinute   = (unsigned char)time.GetMinute ();
   ptDLHist->ptMpHist->m_ucAktSekunde  = (unsigned char)time.GetSecond ();

   for (uVFz = 0;
        //(uVFz+1) < KKFZMAX;
		(uVFz + 1) < g_pCourseData->GetNumberAGVs();
   uVFz++)
   {
      ptDLHist->ptMpHist->ptVFzFeFv[uVFz].m_bFv = 0;
      ptDLHist->ptMpHist->ptVFzFeFv[uVFz].m_bFe = 0;
   }
   return;
}

void WriteFzHistory(CString strFz,CStdioFile *pFile)
{
	CString     strWrite,
				strFormat,
				strFeFv,
				strLast[2] = { "L",
		"B" },
		strVFzFeFv[2] = { "FE",
		"FV" };
	TypDLHistory   *ptDLHist;
	TypMpHistory   *ptMpHist;
	unsigned short uMpIndex,
		uFz,
		uVFz,
		uLen = DL_HISTORY_LEN;
	TypVFzInfos    *pVFzInfos;
	
	uFz      = atoi(strFz);
	ptDLHist = ptDLHistoryRoot + uFz - 1;
	ptMpHist = ptDLHist->ptMpHist;
	strWrite.Format("\nGeschichte des Fz %d vom aMp bis %d Mp's vorher:",uFz,uLen);
	TRY
	{
		pFile->WriteString(strWrite);
		for (uMpIndex = 0;
		(uMpIndex) < DL_HISTORY_LEN;
		uMpIndex++,ptMpHist++)
		{
			strWrite.Format(
			"\n%2d.Mp%4d, NMP%4d, UEMP%4d (Ziel %d, EWeg %d, AWeg %d, %s), %d mal behandelt\
			\n Erste Zeit:%2d:%02d:%02d, aktuelle Zeit:%2d:%02d:%02d",
			uMpIndex+1,
			ptMpHist->uMp,
			ptMpHist->uNmp,
			ptMpHist->uUemp,
			ptMpHist->uZiel,
			ptMpHist->uErsterWegGefahren,
			ptMpHist->uAktWegGefahren,
			strLast[ptMpHist->bLast != 0],
			ptMpHist->uAnzBehandelt,
			ptMpHist->m_ucErstStunde,
			ptMpHist->m_ucErstMinute,
			ptMpHist->m_ucErstSekunde,
			ptMpHist->m_ucAktStunde,
			ptMpHist->m_ucAktMinute,
			ptMpHist->m_ucAktSekunde);
			pFile->WriteString(strWrite);
			for (uVFz = 0;
			//(uVFz+1) < KKFZMAX;
			(uVFz + 1) < g_pCourseData->GetNumberAGVs();
			uVFz++)
			{
				strFormat = 
				"\n  VFz%d Mp%4d, NMP%4d, UEMP%4d (Ziel %d, Weg %d, %s, %s) %s, AB/EAB: %d, %d \
(Zeit: %02d:%02d:%02d)";
				uFz = ptMpHist->ptVFzFeFv[uVFz].m_ucVFz;
				if (ptMpHist->ptVFzFeFv[uVFz].m_bFe)
				{
					pVFzInfos = &(ptMpHist->ptVFzFeFv[uVFz].m_tFeInfos);
					if(pVFzInfos->m_uGrundEAB != 1)
					{
						/* VFz war im System */
						/* Fahrerlaubnis-Infos ausgeben */
						strFeFv = "Fahren";
						
						strWrite.Format(strFormat,
							uFz,
							pVFzInfos->m_uMp,
							pVFzInfos->m_uNmp,
							pVFzInfos->m_uUemp,
							pVFzInfos->m_uZiel,
							pVFzInfos->m_uWegGefahren,
							strLast[pVFzInfos->m_bLast != 0],
							strVFzFeFv[pVFzInfos->m_bVFzFv],
							strFeFv,
							pVFzInfos->m_uGrundAB,
							pVFzInfos->m_uGrundEAB,
							pVFzInfos->m_ucStunde,
							pVFzInfos->m_ucMinute,
							pVFzInfos->m_ucSekunde);
						
						pFile->WriteString(strWrite);
					}
				}
				if (ptMpHist->ptVFzFeFv[uVFz].m_bFv)
				{
					pVFzInfos = &(ptMpHist->ptVFzFeFv[uVFz].m_tFvInfos);
					if(pVFzInfos->m_uGrundEAB != 1)
					{
						/* VFz war im System */
						/* Fahrverbots-Infos ausgeben */
						strFeFv = "Stop  ";
						
						strWrite.Format(strFormat,
							uFz,
							pVFzInfos->m_uMp,
							pVFzInfos->m_uNmp,
							pVFzInfos->m_uUemp,
							pVFzInfos->m_uZiel,
							pVFzInfos->m_uWegGefahren,
							strLast[pVFzInfos->m_bLast != 0],
							strVFzFeFv[pVFzInfos->m_bVFzFv],
							strFeFv,
							pVFzInfos->m_uGrundAB,
							pVFzInfos->m_uGrundEAB,
							pVFzInfos->m_ucStunde,
							pVFzInfos->m_ucMinute,
							pVFzInfos->m_ucSekunde);
						
						pFile->WriteString(strWrite);
					}
				}
			}
		}
		pFile->WriteString("\n");
	}
	   
	CATCH(CFileException, e)
	{
		CString strTemp;
		if (strTemp.LoadString(IDS_ERROR_AGVHIST))
			AfxMessageBox (strTemp);
	}
	END_CATCH
	return;
}
#endif
   
/*
unsigned char FzWegIstFreiZurStrecke(unsigned short uStrIndex,unsigned short *pWeg)
{
    unsigned char uRet = 1;
	unsigned short bTotalFrei = 0,
                   *pW;
    long           lCrash;
   BlStreExt_Typ   *pCrashExt;

    for(pW = pWeg;
        *pW && uRet && !bTotalFrei;
        pW++)
    {
        lCrash = GetCrashIndex(uStrIndex,*pW);
        if(lCrash)
        {
          // Crash
          pCrashExt =  BlStreExt_Root + lCrash - 1;;
          if(pCrashExt->uKWegFuerBlWeg != 1)
          {
            uRet = 0;
          }
        }
        else
        {
            bTotalFrei = 1;
        }
    }
    return uRet;
}
*/

unsigned char FzWegIstFreiZurStrecke(unsigned short uStrIndex, unsigned short *pWeg)
{
	unsigned char uRet = 1;
	unsigned short bTotalFrei = 0,
		*pW;
	CCrashDistance   *pCrash;

	for (pW = pWeg;
		*pW && uRet && !bTotalFrei;
		pW++)
	{
		pCrash = GetPtrCrashDistance(uStrIndex, *pW);
		if (pCrash != NULL)
		{
			// Crash
			//if (pCrashExt->uKWegFuerBlWeg != 1)
			if (!pCrash->BlockWayWithoutCollision())
			{
				uRet = 0;
			}
		}
		else
		{
			bTotalFrei = 1;
		}
	}
	return uRet;
}


void GetMpFolge(TYP_FZDAT      *pFzDat,
                unsigned short *pMpFolge,
                unsigned short uZiel)
{
	long            weg = 0;
	unsigned short  *pMpF = pMpFolge;
	BOOL            bAmZiel = false;

	// mp, nmp und uemp (wenn ungleich Null) in MpFolge eintragen
	if (pFzDat->mp != 0)
	{
		*pMpF = pFzDat->mp;
		bAmZiel = (uZiel == *pMpF);
		if(pFzDat->nmp != 0)
		{
			pMpF++;
			*pMpF = pFzDat->nmp;
			bAmZiel = (uZiel == *pMpF);
			if(pFzDat->uemp != 0)
			{
				pMpF++;
				*pMpF = pFzDat->uemp;
				bAmZiel = (uZiel == *pMpF);
			}
		}
		
		if((uZiel != 0) && !bAmZiel)
		{
			// das ist noch nicht erreicht !
			for (;
			*pMpF != uZiel;
			pMpF++)
			{
				*(pMpF+1) = CAgv::GetNmpWeg (*pMpF, uZiel, &weg);
			}
		}
	}
	else
	{
		// Fz im System ohne Mp !
		CString strMsg;
		strMsg.Format(IDS_AGV_WITHOUT_MP,(int)pFzDat->fznr);
		AfxMessageBox(strMsg);
	}

	*(pMpF+1) = 0;

	return;
}
/******************************************
 * void GetMpFolge(unsigned short uFz,
 *                 unsigned short *pMpFolge,
 *         		   unsigned short uZiel)
 ******************************************/
void GetMpFolgeBisher(unsigned short uFz,
                unsigned short *pMpFolge,
                unsigned short uZiel)
{
	unsigned short  i,
					mp;
	long            weg;
	
	
	/*
	 * pMpFolgeVerz[uFz]
	 * fzdat[uFz+1].auftziel.nr
	 * uIndexMpFolge[uFz]   = 1;
	 */
	
	uFz--;
	*pMpFolge = fzdat[uFz+1].mp;
	*(pMpFolge+1) = 0;
	
	for (i = 1, mp = *(pMpFolge);
	mp != uZiel;
	i++)
	{
		*(pMpFolge+i+1) = 0;
		mp = CAgv::GetNmpWeg (mp, uZiel, &weg);
		*(pMpFolge+i) = mp;
	}
	return;
}

void GetMpFolgeMp(unsigned short uMp,
                unsigned short *pMpFolge,
                unsigned short uZiel)
{
	unsigned short  i,
					mp;
	long            weg;
	
	
	*pMpFolge = uMp;
	*(pMpFolge+1) = 0;
	
	for (i = 1, mp = *(pMpFolge);
	mp != uZiel;
	i++)
	{
		*(pMpFolge+i+1) = 0;
		mp = CAgv::GetNmpWeg (mp, uZiel, &weg);
		*(pMpFolge+i) = mp;
	}
	return;
}

unsigned short *GetTeilMpFolge(unsigned short *pMpF,
                               unsigned short *pVMpF,
                               unsigned char  *pbEnde)
{
  unsigned short *pRet;
  unsigned short *pMp,
                 *pVMp,
                 *pNewMp;
  unsigned short nStreIndex,
                 nVStreIndex;
  CCrashDistance* pCrash;
  unsigned char  bEingetaucht,
                 bVEingetaucht,
                 bBlock,
                 bEnde,
                 bBlkWegOk;

  //pRet = NewMenge(KKMAX_STO_P1+1);
  pRet = NewMenge(g_pCourseData->GetNumberAIDCps() + 2);

  // Schleife ¸ber pMpF
  bEingetaucht = 0; 
  bEnde        = 0;
  pNewMp       = pRet;
  *pNewMp      = *pMpF;
  pNewMp++;
  *pNewMp = 0;
      
  for(pMp = pMpF;
      *(pMp+1) && !bEnde;
      pMp++)
  {
    nStreIndex = GetStrIndexFzT(*pMp,*(pMp+1));
    bBlock     = 1;       // Wir nehmen an, dass alle V-Strecken blockbar sind !
    bVEingetaucht = 0;
    // Schleife ¸ber pVMpF
    for(pVMp = pVMpF;
        *(pVMp+1) && !bEnde && bBlock;
        pVMp++)
    {
      nVStreIndex = GetStrIndexFzT(*pVMp,*(pVMp+1));
      // Crash ?
	  pCrash = GetPtrCrashDistance(nStreIndex, nVStreIndex);
      if(pCrash != NULL)
      {
        bVEingetaucht = 1;
        // Kann geblockt werden ?
        bBlkWegOk = CheckBlkWeg(pCrash,*pMp,nStreIndex);
        if(!bBlkWegOk)
        {
          bBlock = 0;  
        }
      }
      else
      {
        // Kein Crash
        if(bVEingetaucht)
        {
          bEnde = 1;
        }
      }
    }
    if(bBlock && bEingetaucht)
    {
      bEnde = 1;
    }

    bEingetaucht = bVEingetaucht;

    *pNewMp = *(pMp+1);
    pNewMp++;
    *pNewMp = 0;

	if(bBlock && bEnde)
	{
		// Da alle V-Strecken blockbar waren, nehmen wir, falls vorhanden, noch die 
		// n‰chste A-Strecke in die Teilfolge auf.
		if(*(pMp+2))
		{
			*pNewMp = *(pMp+2);
			pNewMp++;
			*pNewMp = 0;
		}
	}
  }

  *pbEnde = bEnde;

  return pRet;
}

/*
unsigned char CheckBlkWeg(long lCrash,unsigned short nMp,unsigned short nStreIndex)
{
  unsigned char bRet = 0;
  unsigned short nBlockweg;
  BlStre_Typ *pCrash;

  pCrash = BlStre_Root + lCrash -1;
  if(pCrash->blockweg > 0)
  {
    nBlockweg = GetBlkWegStrTab(nMp,nStreIndex);
    if(nBlockweg <= pCrash->blockweg)
    {
      bRet = 1;
    }
  }

  return bRet;
}
*/

unsigned char CheckBlkWeg(CCrashDistance* pCrash, unsigned short nMp, unsigned short nStreIndex)
{
	unsigned char bRet = 0;
	unsigned short nBlockweg;

	//if (pCrash->blockweg > 0)
	if (pCrash->HasFreeWay())
	{
		nBlockweg = GetBlkWegStrTab(nMp, nStreIndex);
		//if (nBlockweg <= pCrash->blockweg)
		if (!pCrash->CrossOverFreeWay(nBlockweg))
		{
			bRet = 1;
		}
	}

	return bRet;
}

void EABDeInit()
{
	if(ptDLHistoryRoot)
	{
		delete [] ptDLHistoryRoot;
		ptDLHistoryRoot = NULL;
	}
	if(ptMpHistoryRoot)
	{
		delete [] ptMpHistoryRoot;
		ptMpHistoryRoot = NULL;
	}
	if(ptVFzFeFvRoot)
	{
		delete [] ptVFzFeFvRoot;
		ptVFzFeFvRoot = NULL;
	}
	if(pMpFolgeVerz)
	{
		delete [] pMpFolgeVerz;
		pMpFolgeVerz = NULL;
	}
	if(pMpFolge)
	{
		delete [] pMpFolge;
		pMpFolge = NULL;
	}
	if(pcBitRan)
	{
		delete [] pcBitRan;
		pcBitRan = NULL;
	}
	if(pAMenge)
	{
		free(pAMenge);
		pAMenge = NULL;
	}
	if(pNeueAMenge)
	{
		free(pNeueAMenge);
		pNeueAMenge = NULL;
	}
	if(pFolgeStrMenge)
	{
		free(pFolgeStrMenge);
		pFolgeStrMenge = NULL;
	}
	if(pVirtCrMenge)
	{
		free(pVirtCrMenge);
		pVirtCrMenge = NULL;
	}
	if(pVirtMpFolge)
	{
		free(pVirtMpFolge);
		pVirtMpFolge = NULL;
	}
	if(ppBlkFz)
	{
		free(ppBlkFz);
		ppBlkFz = NULL;
	}
	if(pAllBlkFz)
	{
		free(pAllBlkFz);
		pAllBlkFz = NULL;
	}
	if(pCrMengeVerz)
	{
		delete [] pCrMengeVerz;
		pCrMengeVerz = NULL;
	}
	if(pCrMenge)
	{
		delete [] pCrMenge;
		pCrMenge = NULL;
	}
	if(pCrMengeUmleiten)
	{
		delete [] pCrMengeUmleiten;
		pCrMengeUmleiten = NULL;
	}
	if(pMpFolgeUmleiten)
	{
		delete [] pMpFolgeUmleiten;
		pMpFolgeUmleiten = NULL;
	}
}

void ErweiternMpFolgeIdx(unsigned short *pWeg,unsigned short *pMpFolge)
{
	unsigned short *pW,
		           *pMpF,
				   nMp,
				   nNMp;
	unsigned short uAnzElem;
	BOOL bAbbruch;

	uAnzElem = GetAnzMengenElem(pMpFolge);
	pMpF = pMpFolge + uAnzElem;

	for(pW = pWeg, bAbbruch = FALSE;
	    *pW && (bAbbruch == FALSE);
		pW++)
	{
		GetStrecke(*pW,&nMp,&nNMp);
		if(pW == pWeg)
		{
			if(nMp != *(pMpF-1))
			{
				bAbbruch = TRUE;
			}
		}
		if(bAbbruch == FALSE)
		{
			// Alles ok!
			*pMpF = nNMp;
			pMpF++;
			*pMpF = 0;
		}
	}
}

BOOL MpInCrStrMenge(unsigned short nMpSel,
					unsigned short *pMenge)
{
	BOOL           bRet = false;
	unsigned short *pM,
		           nMp,
				   nNMp;

	for(pM = pMenge;
	*pM && !bRet;
	pM++)
	{
		GetStrecke(*pM,&nMp,&nNMp);
		if((nMpSel == nMp) || (nMpSel == nNMp))
		{
			bRet = true;
		}
	}
	return bRet;
}