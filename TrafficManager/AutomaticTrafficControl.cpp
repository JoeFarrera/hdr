// AutomaticTrafficControl.cpp: implementation of the CAutomaticTrafficControl class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "..\tpc.h"
#include "AutomaticTrafficControl.h"
#include "macro.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAutomaticTrafficControl theAutoTrafficControl;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutomaticTrafficControl::CAutomaticTrafficControl()
{
	m_nDriveStopReasonNCB	= 0;
	m_bActivated			= true;
}

CAutomaticTrafficControl::~CAutomaticTrafficControl()
{

}

/*
bool CAutomaticTrafficControl::ReadBlockTable()
{
	//bool bRet = m_BlockTable.ReadBinary();
	//CCrashDistance *pMyTestCD = m_BlockTable.GetPCrashDistance(251,175);
	//pMyTestCD = m_BlockTable.GetPCrashDistance(251,251);
	return bRet;
}
*/

//bool CAutomaticTrafficControl::GetNoCrashDriveStop(int nTopAGV, int nCompAGV, int &nDrivestopReason)
bool CAutomaticTrafficControl::GetNoCrashDriveStop(TYP_FZDAT *pTopData, TypEABFzStrukt &tAFz,
												   int nCompAGV, TypEABFzStrukt& tVFz,
												   int &nDrivestopReason)
{
	this->m_nDriveStopReasonNCB = 0;
	USHORT nNMpV = (fzdat[nCompAGV].nmp == 0) ? *(tVFz.pMpBegin+1): fzdat[nCompAGV].nmp;
	//bool bRet = GetNoCrashDriveStop(&fzdat[nTopAGV],fzdat[nTopAGV].nmp,nCompAGV-1,fzdat[nCompAGV].nmp);
	//bool bRet = GetNoCrashDriveStop(pTopData,pTopData->nmp,nCompAGV-1,fzdat[nCompAGV].nmp);
	bool bRet = GetNoCrashDriveStop(pTopData,pTopData->nmp,nCompAGV-1,nNMpV);
	nDrivestopReason = this->m_nDriveStopReasonNCB;
	return bRet;
}

bool CAutomaticTrafficControl::GetNoCrashDriveStop(TYP_FZDAT *pTopData,
												   int nNCpTopAGV, int nCompAGV, int nNCpCompAGV)
{
	bool bDriveStop			(false);
	bool bRequestWayCompAGV	(false);

	m_nDriveStopReasonNCB = 0;

	TYP_FZDAT      *pCompData = fzdat + nCompAGV + 1;

	int nDistIndexTop		= GetAGVTypeDistanceIndex(pTopData->mp,nNCpTopAGV);
	int nNextDistIndexTop	= !(pTopData->nmp && pTopData->uemp) ? 0 : GetAGVTypeDistanceIndex(pTopData->nmp,pTopData->uemp);

	int nDistIndexComp		= GetAGVTypeDistanceIndex(pCompData->mp,nNCpCompAGV);
	int nNextDistIndexComp	= !(pCompData->nmp && pCompData->uemp) ? 0 : GetAGVTypeDistanceIndex(pCompData->nmp,pCompData->uemp);

	UINT nNextBlkPosWayTop	= GetBlkPosWay(pTopData ,nNextDistIndexTop	,true );
	UINT nBlkPosWayComp		= GetBlkPosWay(pCompData,nDistIndexComp		,false);
	UINT nNextBlkPosWayComp	= GetBlkPosWay(pCompData,nNextDistIndexComp ,true );

	bool bDrivePermCompAGV	= TEST_AGV_DRIVE(*pCompData);
	bool bStopedCompAGV		= (!bDrivePermCompAGV && (pCompData->mppos != 0));

     // Ermitteln, ob in der nächsten strecke des AFz ein PosWeg gefahren wird, der länger ist, als der Blockweg !
    bool bNTopPosWayLongerBlkWay = PosWayLongerBlkWay(pTopData ,nNextDistIndexTop,true ); 
     // Ermitteln, ob in der Strecke des VFz ein PosWeg gefahren wird, der länger ist, als der Blockweg !
    bool bCompPosWayLongerBlkWay = PosWayLongerBlkWay(pCompData,nDistIndexComp   ,false); 

	bDriveStop = CompAGVOrdersNoCrashDriveStop(	pTopData ->fztyp,
												pCompData->fztyp,
												nDistIndexTop,
												nDistIndexComp,
												nNextDistIndexTop,
												nNextDistIndexComp,
												pCompData->wegstr,
												nBlkPosWayComp,
												bCompPosWayLongerBlkWay,
												nNextBlkPosWayComp,
												bDrivePermCompAGV,
												pTopData->wegstr,
												nNextBlkPosWayTop,
												bNTopPosWayLongerBlkWay,
												bRequestWayCompAGV,
												bStopedCompAGV);
	if(bRequestWayCompAGV)
	{
		TRACE ("CAutomaticTrafficControl::GetNoCrashDriveStop: Should request way for AGV%d on M%04dM%04d\n",
			    pCompData->fznr,
				pCompData->mp,
				pCompData->nmp);
	}

	return bDriveStop;
}

int CAutomaticTrafficControl::GetAGVTypeDistanceIndex(int nCp, int nNCp)
{
	int nDistanceIndex = 0;
	CCourseAGVType* pCourseAGVType = g_pCourseData->GetPtrCourseAGVType(1);
	nDistanceIndex = pCourseAGVType->GetDistIndex(nCp, nNCp);

	/* JLac
	// Pointer to CommPoint nCp
	Meldepunkttabelle *pCommPoint;
	
	// Pointer to NCp-Table (table of distances)
	NMP_tabelle       *pDistance;
	
	unsigned char   nCountNCp;
	if (nNCp == 0 && vtstat[nCp] != 0xffff)
	{
		long lLength = 0;
		nNCp = GetConnectionLength (nCp,nCp,lLength);
	}
	if (nNCp != 0)
	{
		//Set Pointer to CommPoint nCp:
		pCommPoint = mptab + vmptab [nCp] - 1;
		
		//Set Pointer to Distances of nCp:
		pDistance = MpNmpTab + pCommPoint->str_idx - 1;
		nCountNCp = 0;
		// seek to end or nNCp found:
		while ((pDistance->nmp != nNCp) &&
			(nCountNCp < pCommPoint->anz_nmp))
		{
			pDistance++;
			nCountNCp++;
		}
		if (pDistance->nmp == nNCp)
		{
			// if nNCp ok => copy DistanceIndex
			nDistanceIndex = pDistance->zeit_index;
		}
	}
	*/
	return nDistanceIndex;
}

/* JLac
UINT CAutomaticTrafficControl::GetConnectionLength(int nCp, int nDestination, long &lLength)
{
	UINT nNCp  = 0;
	NMP_tabelle *pDistance = GetDistancePointer(nCp,nDestination);

	if (pDistance)
	{
		lLength += pDistance->str_len;
		nNCp	 = pDistance->nmp;
	}
	return nNCp;
}
*/

/* JLac
NMP_tabelle* CAutomaticTrafficControl::GetDistancePointer(int nCp, int nDestination)
{
	unsigned int   nDpIndex;	// Decision point index
	unsigned char  zber;	// Destination area
	int		nmp;          // nächster Meldepunkt
	int		nmp_index;    // Index in EP_MP_Tabelle
	unsigned char  i;
	unsigned char  anz_nmp;	// Number of next coms points
	Meldepunkttabelle  *pCommPoint;
	
	NMP_tabelle	*pDistance = NULL,
				*pDist;
	
	// Nur bearbeiten, wenn mp und ziel _bergeben wurden.
	
	if (nCp && nDestination)
	{
		// Adresse in MP-Tabelle(mpstre) ermitteln
		// XXX Michael 11.09.2001 strtab  = mptab + vmptab [mp]; // Get mp data
		pCommPoint  = mptab + (vmptab [nCp] - 1); // Get mp data
		if (vtstat[nDestination] != 0xFFFF)
		{
			// The destination really exists
			nDpIndex = pCommPoint->ep_nr;	// Decision point index
			zber = statdat[vtstat[nDestination]].f_bereich;	// Area index
			
			if (zber)
			{
				pDistance  = MpNmpTab + pCommPoint->str_idx - 1;	// First next point
				anz_nmp    = pCommPoint->anz_nmp;					// Number of next points
				
				if (nDpIndex)
				{
					// The MP is a decision point
					
					
					// NMP aus EP-FB-Tabelle !
					
					// Entscheidungspunkt- und Zielbereichsnummer erniedrigen, da
					// es keinen Entscheidungspunkt und Zielbereich mit der Nummer
					// "0" gibt!
					nDpIndex--;
					zber--;
					nmp_index = ep_fb_tab [nDpIndex] [zber];
					
					if (nmp_index)
					{
						nmp = ep_mp_tab [nDpIndex] [nmp_index - 1];
						// Get pointer to MpNmpTab for this point from the first next point
						for (;
						nmp != pDistance->nmp;
						pDistance++)
						{
						}
					}
					else
					{
						// Fehler im Fahrkurs !
						//MSGERROR ("Next point not found for decision point");
						AfxMessageBox ("Next point not found for decision point");
						pDistance = 0;
					}
				}
				else
				{
					// Mp mit einem NMp oder Zielsuchpunkt !
					// It is a destination seek point or a normal point
					// which isn't a decision point
					
					// Ermitteln des NMP aus MP-Streckentabelle
					anz_nmp    = pCommPoint->anz_nmp; // Number of next points
					nmp        = pDistance->nmp;		// Number of the first nmp
					
					// Zielsuchpunkt: Ermitteln, ob nächster NMP Ziel ist.
					for (i = 0, pDist = pDistance;
					i < anz_nmp;
					i++)
					{
						if (nmp != nDestination)
						{
							// The next point is different from the destination
							pCommPoint = mptab + vmptab [nmp] - 1; // mptab is 0 based
							// Get the MP data for the next point
							if (pCommPoint->typ == 1)
							{
								// Vorzugsstrecke gefunden
								// It is not a destination. This is the point
								pDist = pDistance;
							}
							else
							{
								// The next point cannot be a destination if it is
								// not the mission destination ???
								pDistance++;
								nmp = pDistance->nmp;
							}
						}
						else
						{
							// Ziel gefunden !
							// The next point is the destination
							pDist = pDistance;
							break;
						}
					}
					pDistance = pDist;
				}
			}
			else
			{
				// ungueltiger Feinbereich !
				// The destination doesn't have an area
			}
		}
		else
		{
			// ziel ist kein Ziel !
			// the mp or destination is null
		}
	}
	return pDistance;
}
*/

UINT CAutomaticTrafficControl::GetBlkPosWay(TYP_FZDAT *pAGVData, int nDistIndex, bool bNCp)
{
	UINT nBlkPosWay = 0;
	if(bNCp)
	{
		if(nDistIndex)
		{
			// BlkPosWay of next pdistance
			nBlkPosWay = (pAGVData->nmp == pAGVData->fzziel.nDest) ? GetPosWay(pAGVData->fzziel) : GetBlockWay(nDistIndex);  
		}
	}
	else
	{
		// BlkPosWay of top distance
		if(IsAGVOnStation(pAGVData))
		{
			/* PosWeg */
			TYPE_STATION_DEF tZiel;
			if(pAGVData->fzziel.nDest != pAGVData->mp)
			{
				tZiel.nDest		= pAGVData->mp;
				tZiel.nPos		= pAGVData->mppos;
				tZiel.nLayer	= 1;
                if(pAGVData->mplage)
                {
					tZiel.ucSide  = pAGVData->mplage;        
					nBlkPosWay = GetPosWay(tZiel);
					if(GetPosWay(tZiel) == 0)
					{
						tZiel.ucSide = (tZiel.ucSide == 'L') ? 'R' : 'L';
					}
                }
               else
                {
					GetAnyStation(tZiel);
                }
			}
			else
			{
				tZiel		= pAGVData->fzziel;
				tZiel.nPos	= tZiel.nPos   & 0x0F;
				tZiel.nLayer	= tZiel.nLayer & 0x0F;
			}
			nBlkPosWay = GetPosWay(tZiel);
            if(nBlkPosWay == 0)
            {
				nBlkPosWay = GetPosWay(tZiel);
				// Syslog-Meldung: Kein Posweg gefunden!
				CString strMsg;
				strMsg.Format("PosWay not found Destination %d, Position %d",(int)tZiel.nDest,(int)tZiel.nPos);
				//SyslogKeinPosWeg(pFzDat, &tZiel);
 				MSGERROR2 ("Next point not found for decision point. Destination: %d, Position: %d",(int)tZiel.nDest,(int)tZiel.nPos );
				// TODO Michael 01.06.2011 AfxMessageBox (strMsg,MB_ICONQUESTION);
           }
		}
		else
		{
			nBlkPosWay = GetBlockWay(nDistIndex);
		}
	}
	return nBlkPosWay;
}

/*
UINT CAutomaticTrafficControl::GetPosWay(TYPE_STATION_DEF &myStation)
{
	UINT nPosWay(0);
	unsigned int nDestIndex = vtstat[myStation.nDest];


	int nMaxStation = KMAX_STATIONEN;
	if ((nDestIndex >				0) &&
	    (nDestIndex <= KMAX_STATIONEN))
	{
		if(nDestIndex != 0)
		{
			int nPosLimit = 0;
			int nIndex = 0;
			if (myStation.ucSide == 'L')
			{
				nIndex = zt_ver_tab[statdat[nDestIndex].zieltyp_l];
				nPosLimit = statdat[nDestIndex].pos_anz_l;
			}
			else
			{
				if (myStation.ucSide == 'R')
				{
					nIndex = zt_ver_tab[statdat[nDestIndex].zieltyp_r];
					nPosLimit = statdat[nDestIndex].pos_anz_r;
				}
			}
			if ((nIndex != 0) && (myStation.nPos > 0) && (myStation.nPos <= nPosLimit))
			{
				//Posweg ermitteln (z_index + pos - 1)
				nPosWay = z_typ_tab[nIndex + myStation.nPos - 1].pos_weg;
				if (nPosWay == 1)
				{
					nPosWay = GetBlockWay(myStation.nDest, 0);
				}
			}
		}
	}
	return nPosWay;
}
*/

UINT CAutomaticTrafficControl::GetPosWay(TYPE_STATION_DEF &myStation)
{
	return g_pCourseData->GetPtrCourseAGVType(1)->GetPosWay(myStation);
}

int CAutomaticTrafficControl::GetBlockWay(int nDistIndex)
{
	//return (MpNmpTab + nDistIndex - 1)->blk_weg; JLac
	return g_pCourseData->GetPtrCourseAGVType(1)->GetBlockWay(nDistIndex);
}

int CAutomaticTrafficControl::GetBlockWay(int nCp, int nNCp)
{
	int nDistIndex = GetAGVTypeDistanceIndex(nCp, nNCp);
	return (nDistIndex > 0) ? 0 : GetBlockWay(nDistIndex);
}

/*
bool CAutomaticTrafficControl::GetAnyStation(TYPE_STATION_DEF &myStation)
{
	bool bRet = false;
	UINT nDestIndex = vtstat[myStation.nDest];
	if(nDestIndex != 0xFFFF)
	{
		myStation.ucSide = (statdat[nDestIndex-1].zieltyp_l != 0) ? 'L':'R';
	}
	return bRet;
}
*/

bool CAutomaticTrafficControl::GetAnyStation(TYPE_STATION_DEF &myStation)
{
	return g_pCourseData->GetPtrCourseAGVType(1)->GetAnyStation(myStation);
}

bool CAutomaticTrafficControl::IsAGVOnStation(TYP_FZDAT *pAGVData)
{
  bool bAGVOnStation = !(pAGVData->nmp) ||
						(pAGVData->mp == pAGVData->fzziel.nDest) ||
						(pAGVData->mppos && TEST_AGV_STOP(*pAGVData));
  return bAGVOnStation;
}

bool CAutomaticTrafficControl::PosWayLongerBlkWay(TYP_FZDAT *pAGVData, int nDistIndex, bool bNCp)
{
  bool bRet(false);
  unsigned short nBlkWay,
                 nBlkPosWay;

  if(nDistIndex)
  {
    nBlkPosWay = GetBlkPosWay(pAGVData,nDistIndex,bNCp);
    nBlkWay    = GetBlockWay(nDistIndex);
    if(nBlkPosWay > nBlkWay)
    {
      bRet = 1;
    }
  }
  return bRet;
}

bool CAutomaticTrafficControl::CompAGVOrdersNoCrashDriveStop(USHORT	nTopAGVType,
															 USHORT	nCompAGVType,
															 int	nDistIndexTop,
															 int	nDistIndexComp,
															 int	nNextDistIndexTop,
															 int	nNextDistIndexComp,
															 USHORT	nCompWay,
															 int	nBlkPosWayComp,
															 bool	bCompPosWayLongerBlkWay,
															 int	nNextBlkPosWayComp,
															 bool	bDrivePermCompAGV,
															 USHORT	nTopWay,
															 int	nNextBlkPosWayTop,
															 bool	bNTopPosWayLongerBlkWay,
															 bool	&bRequestWayCompAGV,
															 bool	bStopedCompAGV)
{
	bool bDriveStop(false);
	CCrashDistance* pCrT_NC		= NULL;
	CCrashDistance* pCrNC_T		= NULL;

	CCrashDistance* pCrNT_C		= NULL;
	CCrashDistance* pCrC_NT		= NULL;

	CCrashDistance* pCrNT_NC	= NULL;
	CCrashDistance* pCrNC_NT	= NULL;

	CCrashDistance* pCrT_C		= GetPCrashDistances(&m_BlockTable,nDistIndexTop ,nDistIndexComp);
	CCrashDistance* pCrC_T		= GetPCrashDistances(&m_BlockTable,nDistIndexComp,nDistIndexTop );

	if(nNextDistIndexComp) 
	{
		pCrT_NC = GetPCrashDistances(&m_BlockTable,nDistIndexTop     ,nNextDistIndexComp);
		pCrNC_T = GetPCrashDistances(&m_BlockTable,nNextDistIndexComp,nDistIndexTop     );
	}
	if(nNextDistIndexTop)
	{
		pCrNT_C = GetPCrashDistances(&m_BlockTable,nNextDistIndexTop,nDistIndexComp   );
		pCrC_NT = GetPCrashDistances(&m_BlockTable,nDistIndexComp   ,nNextDistIndexTop);

		if(nNextDistIndexComp)
		{
			pCrNT_NC = GetPCrashDistances(&m_BlockTable,nNextDistIndexTop ,nNextDistIndexComp);
			pCrNC_NT = GetPCrashDistances(&m_BlockTable,nNextDistIndexComp,nNextDistIndexTop );
		}
	}
	if(pCrT_C)
	{
		//TopDistance has Crash with CompDistance:
		bDriveStop = ProcessTop_Comp(	pCrT_C,
										pCrC_T,
										pCrT_NC,
										pCrNT_C,
										pCrC_NT,
										pCrNT_NC,
										nCompWay,
										nBlkPosWayComp,
										bCompPosWayLongerBlkWay,
										bDrivePermCompAGV,
										nTopWay,
										bRequestWayCompAGV,
										bStopedCompAGV);
	}
	if(!bDriveStop && pCrT_NC)
	{
		// TopDistance has Crash with next CompDistance:
		bDriveStop = ProsessTop_NextComp(	pCrNC_T,
											pCrNC_NT,
											pCrC_NT,
											pCrNT_C,
											nCompWay,
											nBlkPosWayComp,
											nNextBlkPosWayComp,
											nNextBlkPosWayTop,
											bDrivePermCompAGV,
											nTopWay,
											bStopedCompAGV);
	}
	if(!bDriveStop && pCrNT_C)
	{
		//NextTopDistance has Crash with CompDistance:
		bDriveStop = ProcessNextTop_Comp(	pCrNT_C,
											pCrC_NT,
											pCrNT_NC,
											pCrNC_NT,
											nCompWay,
											nBlkPosWayComp,
											nNextBlkPosWayComp,
											bDrivePermCompAGV,
											nNextBlkPosWayTop,
											bNTopPosWayLongerBlkWay,
											bRequestWayCompAGV,
											bStopedCompAGV);
	}
	if(!bDriveStop && pCrNT_NC)
	{
		// Nächste AStrecke hat Crash mit nächster VStrecke
		bDriveStop = ProcessNextTop_NextComp(	pCrNC_NT,
												nNextBlkPosWayComp,
												bDrivePermCompAGV,
												pCrNT_NC,
												nNextBlkPosWayTop);
	}

	return bDriveStop;
}

CCrashDistance* CAutomaticTrafficControl::GetPCrashDistances(CBlockTableAGVTypePair *pBlkTable, int nTDistIndex, int nCDistIndex)
{
	return pBlkTable->GetPCrashDistance(nTDistIndex,nCDistIndex);
}

bool CAutomaticTrafficControl::ProcessTop_Comp(CCrashDistance*	pCrT_C,
											   CCrashDistance*	pCrC_T,
											   CCrashDistance*	pCrT_NC,
											   CCrashDistance*	pCrNT_C,
											   CCrashDistance*	pCrC_NT,
											   CCrashDistance*	pCrNT_NC,
											   USHORT			nCompWay,
											   int				nBlkPosWayComp,
											   bool				bCompPosWayLongerBlkWay,
											   bool				bDrivePermCompAGV,
											   USHORT			nTopWay,
											   bool				&bRequestWayCompAGV,
											   bool				bStopedCompAGV)
{
	bool bDriveStop = false;

	if(pCrT_C->HasAbsCollWay())
	{
		// CompAgv has CollisionWay to actual distance of TopAGV
		if(pCrT_C->CrossOverAbsCollWay(nCompWay))
		{
			// Kollisionsweg zur Strecke des AFz ist gefahren
			if(pCrNT_C)
			{
				if(pCrNT_C->HasCollWayBlkWay())
				{
					// VFz hat KollWeg zum Blockweg der nächsten Strecke des AFz
					if(pCrNT_C->CrossOverCollWayBlkWay(nCompWay))
					{
						// VFz hat diesen KollWeg gefahren:
						// Kein FV !
					}
					else
					{
						// KollWeg zum Blockweg noch nicht gefahren
						bDriveStop = true;
						// Weg des VFz anfordern
						bRequestWayCompAGV = true;	//bDrivePermCompAGV;
					}
				}
				else
				{
					// Kein KollWeg zum Blockweg
					bDriveStop = true;
					// Weg des VFz anfordern
					bRequestWayCompAGV = true;	//bDrivePermCompAGV;
				}
			}
			else
			{
				// Kein Crash mit der nächsten Strecke des VFz
				// Kein FV !
			}
		}
		else
		{
			// KollWeg noch nicht gefahren
			bDriveStop = true;
		}
	}
	else
	{
		// Kein KollWeg
		bDriveStop = true;
	}
	
	if(bDriveStop)
	{
		// Bislang FV aus Kollisionswegen
		// Aber das VFz kann kollisionsfrei sein:
		if(!bDrivePermCompAGV)
		{
			// VFz hat FV
			if (pCrC_T)
			{
				if(CheckBlkPosWay(pCrC_T,pCrT_C,nBlkPosWayComp,nCompWay,bStopedCompAGV) ||
					BlockWayAlreadyFree(pCrC_T,nTopWay,bCompPosWayLongerBlkWay))
				{
					// VFz ist kollisionsfrei im Blockweg oder in kollisionsfreiem Bereich
					// zur aktuellen Strecke des AFz !
					if(pCrNT_C)
					{
						if(pCrC_NT)
						{
							if(CheckBlkPosWay(pCrC_NT,pCrNT_C,nBlkPosWayComp,nCompWay,bStopedCompAGV))
							{
								// VFz ist kollisionsfrei im Blockweg oder in kollisionsfreiem Bereich
								// zur nächsten Strecke des AFz !
								bDriveStop = false;
							}
							else
							{
								// VFz nicht kollisionsfrei zur nächsten Strecke des AFZ						}
								// FV bleibt !
							}
						}
						else
						{
							// Keinen nächsten Gegencrash: FV bleibt !
						}
					}
					else
					{
						// Kein nächster Crash, Kollisionfreiheit zu aktuellen Strecke des AFz
						// ist ausreichend: Kein FV !
						bDriveStop = false;
					}
				}
				else
				{
					// VFz ist nicht kollissionsfrei zur aktuellen Strecke des AFz
					// Aber wenn das AFz die gesamte Strecke des VFz freifahren kann oder
					// in der nächsten Strecke den Blockweg des VFz sofort frei macht und : Kein FV!
					if(pCrT_C->HasFreeWay())
					{
						// FV bleibt
					}
					else
					{
						//AFZ hat keinen Blockweg :
						bDriveStop = false;
						if(pCrT_C->HasCollWayBlkWay())
						{
							if(pCrT_C->CrossOverCollWayBlkWay(nCompWay))
							{
								//VFz hat nur Blockweg des AFz freigefahren, ansonsten blockiert es:
								bDriveStop = true;
							}
						}
						//27.02.2004: Lachmund :
						// Diese Abfrage wurden eingefügt, weil sich ein Fz, das umgeleitet oder umdisponiert wurde,
						// sich plötzlich in einer Situation befindet, in die es die Blockung nicht hätte fahren lassen.
						if(pCrC_T->CrossOverFreeWay(nCompWay	  ) ||		//VFz ist bereits zu weit oder
						   pCrC_T->CrossOverFreeWay(nBlkPosWayComp))			//wird trotz Fahrverbot zu weit Fahren
						{
							//VFz hat keinen freien Weg oder der gefahrene Weg des VFz ist größer als der freie Weg:
							if(pCrNT_C)
							{
								if(pCrC_NT)
								{
									if(pCrC_NT->CrossOverFreeWay(nCompWay      ) ||
									   pCrC_NT->CrossOverFreeWay(nBlkPosWayComp))
									{
										//VFz zur nächsten Sterecke des AFz:
										//VFz hat keinen freien Weg oder der gefahrene Weg des VFz ist größer als der freie Weg:
										bDriveStop = true;
									}
								}
								else
								{
									//Blockungstabelle an dieser Stelle unvollständig: FV
									bDriveStop = true;
								}
							}
						}
						//Ende, 27.02.2004: Lachmund
					}
				}
			}
			else
			{
				// Kein Gegencrash: FV bleibt !
			}
		}
		else
		{
			// VFz hat FE : FV bleibt
		}
		if(bDriveStop)   // Lac: 03.07.02
		{
			// Prüfen, ob AFz möglicherweise schon weit genug !
			if(pCrC_T->HasAbsCollWay())
			{
				if(pCrC_T->CrossOverAbsCollWay(nTopWay))
				{
					// AFz hat absuluten Kollisionsweg abgefahren!
					if(pCrNT_C)
					{
						// nächste AFz-Strecke hat erneut Crash mit aktueller VFz-Strecke: FV bleibt !
					}
					else
					{
						// Kein Crash nächste AFz-Strecke mit aktueller VFz-Strecke
						if(pCrNT_NC)
						{
							// Crash nächste AFZ-Strecke mit nächster VFz-Strecke: FV bleibt ! 
						}
						else
						{
							// Kein Crash nächste AFZ-Strecke mit nächster VFz-Strecke: AFz kann fahren ! 
							bDriveStop = false;
						}
					}
					
				}
				else
				{
					// AFz hat Kollisionsweg noch nicht abgefahren: FV bleibt !
				}
			}
		}
	}
	if(bDriveStop)
	{	
		m_nDriveStopReasonNCB = 80;
	}
	return bDriveStop;
}

bool CAutomaticTrafficControl::CheckBlkPosWay(CCrashDistance *pCD, CCrashDistance *pReversedCD, int nBlkPosWay, int nDrivenWay, bool bStopedAGV)
{
	bool bRet = 0;

	if( (!pCD->CrossOverFreeWay(nBlkPosWay)) &&
		(!pCD->CrossOverFreeWay(nDrivenWay)))
	{
		// Without collision in blaockway!
		bRet = 1;
	}
	else
	{
		// Not without collision in blockway!
		if(pReversedCD->HasRelCollWay())
		{
			// It exists a collision free area!
			if( (pReversedCD->CrossOverRelCollWay(nBlkPosWay) &&
				 pReversedCD->CrossOverRelCollWay(nDrivenWay)) ||
				(bStopedAGV && pReversedCD->CrossOverRelCollWay(nDrivenWay)))
			{
				// BlkPosWeg und der FzWeg liegen hinter dem Beginn des kollisionsfreien Bereiches
				if( (!pReversedCD->CrossOverRelFreeArea(nBlkPosWay) &&
					 !pReversedCD->CrossOverRelFreeArea(nDrivenWay)) ||
				    (bStopedAGV && !pReversedCD->CrossOverRelFreeArea(nDrivenWay)))
				{
					// BlkPosWeg und der FzWeg liegen vor dem Ende des kollisionsfreien Bereiches
					bRet = 1;
				}
			}
		}
	}
	if(!bRet)
	{
		if(pReversedCD->HasAbsCollWay())
		{
			if( pReversedCD->CrossOverAbsCollWay(nBlkPosWay) &&
				pReversedCD->CrossOverAbsCollWay(nDrivenWay))
			{
				//Kollisionsfreiheit nach absolutem Kollisionsweg festgestellt:
				bRet = 1;
			}
		}
	}
	return bRet;
}

bool CAutomaticTrafficControl::BlockWayAlreadyFree(CCrashDistance*	pCD,
												   int				nAGVWay,
												   bool				bPosWayLaengerBlkWay)
{
	return	(!bPosWayLaengerBlkWay &&
			(pCD->HasCollWayBlkWay()) &&
			(pCD->CrossOverCollWayBlkWay(nAGVWay)));
}

bool CAutomaticTrafficControl::ProsessTop_NextComp(CCrashDistance*	pCrNC_T,
												   CCrashDistance*	pCrNC_NT,
												   CCrashDistance*	pCrC_NT,
												   CCrashDistance*	pCrNT_C,
												   USHORT			nCompWay,
												   int				nBlkPosWayComp,
												   int				nNextBlkPosWayComp,
												   int				nNextBlkPosWayTop,
												   bool				bDrivePermCompAGV,
												   USHORT			nTopWay,
												   bool				bStopedCompAGV)
{
	bool bDriveStop = false;
	if (!bDrivePermCompAGV)
	{
		// VFz hat FV
		if(pCrC_NT)
		{
			if(pCrNT_C)
			{			  
				if(CheckBlkPosWay(pCrC_NT,pCrNT_C,nBlkPosWayComp,nCompWay,bStopedCompAGV))
				{
					// Das VFz steht kollisionsfrei im Blockweg oder im kollisionsfreien Bereich
					// zur nächsten Strecke des AFz
					// AFz ist ok !
					// Kein FV !
				}
				else
				{
					// Das VFz steht nicht kollisionsfrei zur nächsten Strecke des AFz: FV !
					//18.06.2004, Lachmund: Diese Abfrage eingefügt:
					if(CheckBlkPosWay(pCrNT_C,pCrC_NT,nNextBlkPosWayTop,nNextBlkPosWayTop,bStopedCompAGV))
					{
						//Das AFz kann seinen Blockweg in der nächsten Strecke noch	fahren:
						//Kein FV
					}
					else
					{
						//Das AFz kann seinen Blockweg in der nächsten Strecke nicht mehr fahren:
						bDriveStop = true;
					}
				}
			}
			else
			{
				// Blocktabelle unvollständig: FV
				bDriveStop = true;  
			}
		}
		else
		{
			// Kein Crash des VFz mit naächster Strecke des AFz!
			// Kein FV !
		}
	}
	else
	{
		// VFz hat FE: FV !
		//	bFV = 1;
		
		// Wenn das VFz FE hat, dann darf das AFz ggf.fahren,
		// wenn das VFz mit seiner nächsten Strecke einen
		// ausreichenden Blockweg hat zur aktuellen Strecke
		// des AFz hat und keinen Crash mit der nächsten Strecke
		// des AFz oder auch einen ausreichenden Blockweg zu
		// nächsten Strecke des AFz !
		// 16.02.2004: Allerdings muss VFz den Blockweg der nächsten Strecke des AFZ
		// freigefahren haben, wenn es einen Crasheintrag lCrashNA_V gibt! 
		if(pCrNT_C)
		{
			if(pCrNT_C->HasCollWayBlkWay() && pCrNT_C->CrossOverCollWayBlkWay(nCompWay))
			{
				if(pCrNC_T)
				{
					if(!pCrNC_T->CrossOverFreeWay(nNextBlkPosWayComp))
					{
						// BlkPosWeg zur aktuellen AFz-Strecke ausreichend
						if(pCrNC_NT)
						{
							// Crash mit nächster AFz-Strecke
							if(pCrNC_NT->CrossOverFreeWay(nNextBlkPosWayComp))
							{
								// BlkPosWeg ist ausreichend: Kein FV !
							}
							else
							{
								// BlkPosWeg nicht ausreichend: FV !
								bDriveStop = true;
							}
						}
						else
						{
							// kein Crash mit nächster AFz-Strecke: Kein FV !
						}
					}
					else
					{
						// Kein ausreichender Blockweg: FV
						bDriveStop = true;
					}
					if(bDriveStop)
					{
						// Prüfen, ob AFz ggf. den Blockweg der nächsten Strecke des VFz freigefahren hat !
						if(pCrNC_T->HasAbsCollWay())
						{
							if(pCrNC_T->RelCollWayIsAbsCollWay())
							{
								if(pCrNC_T->CrossOverCollWayBlkWay(nTopWay))
								{
									// Das AFz hat den Blockweg der nächsten Strecke des VFz freigefahren und kann die
									// ganze Strecke freifahren !
									bDriveStop = false; 
								}
							}
						}
					}
				}
				else
				{
					// Kein Gegencrash (Tabelle unvollständig): FV !
					bDriveStop = true;
				}
			}
			else
			{
				// Kein Kollisionsweg zum Blockweg der nächsten Strecke des AFz oder VFz ist nicht weit genug gefahren : FV !
				bDriveStop = true;
			}
		}
		else
		{
			// 13.04.2004 Lachmund: Dieser else-Zweig ist neu.
			// Kein Crash zwischen nächster Strecke AFz und aktueller Strecke VFz
			if(pCrNC_T)
			{
				if((!pCrNC_T->CrossOverFreeWay(nNextBlkPosWayComp)) ||
					pCrNC_T->CrossOverCollWayBlkWay(nTopWay))			//26.09.2008 Lachmund: diese Zeile zugefügt!
				{
					// BlkPosWeg zur aktuellen AFz-Strecke ausreichend
					if(pCrNC_NT)
					{
						// Crash mit nächster AFz-Strecke
						if(!pCrNC_NT->CrossOverFreeWay(nNextBlkPosWayComp))
						{
							// BlkPosWeg ist ausreichend: Kein FV !
						}
						else
						{
							// BlkPosWeg nicht ausreichend: FV !
							bDriveStop = true;
						}
					}
					else
					{
						// kein Crash mit nächster AFz-Strecke: Kein FV !
					}
				}
				else
				{
					// Kein ausreichender Blockweg: FV
					bDriveStop = true;
				}
			}
			else
			{
				// Kein Gegencrash (Tabelle unvollständig): FV !
				bDriveStop = true;
			}
		}
	}
	if(bDriveStop)
	{	
		m_nDriveStopReasonNCB = 90;
	}
	return bDriveStop;
}

bool CAutomaticTrafficControl::ProcessNextTop_Comp(CCrashDistance*	pCrNT_C,
												   CCrashDistance*	pCrC_NT,
												   CCrashDistance*	pCrNT_NC,
												   CCrashDistance*	pCrNC_NT,
												   USHORT			nCompWay,
												   int				nBlkPosWayComp,
												   int				nNextBlkPosWayComp,
												   bool				bDrivePermCompAGV,
												   int				nNextBlkPosWayTop,
												   bool				bNTopPosWayLongerBlkWay,
												   bool&			bRequestWayCompAGV,
												   bool				bStopedCompAGV)
{
	bool bDriveStop = false;
	if(pCrNT_C->HasAbsCollWay())
	{
		// VFz hat KollWeg zur aktuellen Strecke des AFz
		if(pCrNT_C->CrossOverAbsCollWay(nCompWay))
		{
			// Kollisionsweg zur Strecke des AFz ist gefahren
			if(bDrivePermCompAGV)
			{
				if(pCrNT_NC)
				{
#ifndef WHY_COLLWAYBLKWAY
					if(pCrNC_NT)
					{
						if(!pCrNC_NT->CrossOverFreeWay(nNextBlkPosWayComp))
						{
							//VFz kann in seiner naechsten Strecke noch kollisionsfrei
							//blocken/positionieren: kein FV!
						}
						else
						{
							//VFz kann in seiner naechsten Strecke nicht kollisionsfrei
							//blocken/positionieren: FV!
							bDriveStop = true;
						}
					}
					else
					{
						//Blockungstabelle unvollstaendig:
						bDriveStop = true;
					}
#else
					if(pCrNT_NC->HasCollWayBlkWay())
					{
						// VFz hat KollWeg zum Blockweg der nächsten Strecke des AFz
						if(pCrNT_NC->CrossOverCollWayBlkWay(nCompWay))
						{
							// VFz hat diesen KollWeg gefahren:
							// Kein FV !
						}
						else
						{
							// KollWeg zum Blockweg noch nicht gefahren
							bDriveStop = true;
							// Weg des VFz anfordern
							bRequestWayCompAGV = true;	//bDrivePermCompAGV;
						}
					}
					else
					{
						// Kein KollWeg zum Blockweg
						bDriveStop = true;
						// Weg des VFz anfordern
						bRequestWayCompAGV = true;	//bDrivePermCompAGV;;
					}
#endif	//WHY_COLLWAYBLKWAY
				}
			}
			else
			{
				// Kein Crash mit der nächsten Strecke des VFz
				// Kein FV !
			}
		}
		else
		{
			// KollWeg noch nicht gefahren
			if(!bNTopPosWayLongerBlkWay)
			{
				if(pCrNT_C->HasCollWayBlkWay())
				{
				  if(pCrNT_C->CrossOverCollWayBlkWay(nCompWay))
				  {
				    // Blockweg der nächsten Strecke des AFz ist frei: Kein FV !
				  }
				  else
				  {
				    // VFz ist nicht weit genug gefahren: FV !
				    bDriveStop = true;
				  }
				}
				else
				{
				  // Kein Kollweg zum Blockweg: FV
				  bDriveStop = true;
				}
			}
			else
			{
				// Der NMp des AFz ist das Ziel, aber der PosWeg ist länger als der Blockweg:
				// FV !
				bDriveStop = true;
			}
		}
	}
	else
	{
		// Kein KollWeg: FV !
		bDriveStop = true;
	}

	if(bDriveStop)
	{
		// Bislang FV aus Kollisionswegen
		// Aber das VFz kann kollisionsfrei sein:
		if(!bDrivePermCompAGV)
		{
			// VFz hat FV
			if (pCrC_NT)
			{
				if(CheckBlkPosWay(pCrC_NT,pCrNT_C,nBlkPosWayComp,nCompWay,bStopedCompAGV))
				{
					// VFz ist kollisionsfrei im Blockweg oder in kollisionsfreiem Bereich
					// zur nächsten Strecke des AFz : Kein FV !
					bDriveStop = false;

				}
				else
				{
					// VFz ist nicht kollisionsfrei nächsten Strecke des AFz !
					if(pCrC_NT->HasAbsCollWay())
					{
						// Das AFz kann in seiner nächsten Strecke die aktuelle Strecke des VFz freifahren!
						if(pCrC_NT->RelCollWayIsAbsCollWay())
						{
						  // Der relative und der absolute Kollisionsweg sind gleich !
						  if(pCrC_NT->BlockWayWithoutCollision())
						  {
						    // Der Blockweg ist sofort frei: Kein FV !
						    bDriveStop= false;
						  }
						  else
						  {
						    // Der Blockweg des VFz kann nicht oder muß erst freigefahren werden: FV bleibt !
						  }               
						}
						else
						{
						  // Der relative und der absolute Kollisionsweg sind ungleich, d.h. das AFz geht 
						  // nochmals in Kollision mit dem VFz : FV bleibt!
						}
					}
					else
					{
					// Das AFz kann sich in seiner nächsten Strecke nicht freifahren: FV bleibt !
					}
				}
			}
			else
			{
				// Kein Gegencrash: FV bleibt !
			}
		}
		else
		{
			// VFz hat FE : FV bleibt
		}
		if(bDriveStop)
		{
			// Lac ergänzt am 26.06.02:
			if(!pCrNT_C->CrossOverFreeWay(nNextBlkPosWayTop))
			{
        		// AFz kann erfolgreich zur aktuellen VFz-Strecke blocken !
        		if(pCrNT_NC)
        		{
					if(!pCrNT_NC->CrossOverFreeWay(nNextBlkPosWayTop))
					{
            			// AFz kann erfolgreich zur nächsten VFz-Strecke blocken
            			// Kein FV !
            			bDriveStop = false;
					}
					else
					{
						// FV bleibt
					}
				}
				else
				{
					// Kein Crash mit nächster VFz-Strecke: Kein FV !
					bDriveStop = false;
				}
			}
			else
			{
        		// FV bleibt!
			}
		}
	}
	if(bDriveStop)
	{	
		m_nDriveStopReasonNCB = 85;
	}
	return bDriveStop;
}

bool CAutomaticTrafficControl::ProcessNextTop_NextComp(CCrashDistance*	pCrNC_NT,
													   int				nNextBlkPosWayComp,
													   bool				bDrivePermCompAGV,
													   CCrashDistance*	pCrNT_NC,
													   int				nNextBlkPosWayTop)
{
	bool bDriveStop = false;

	bool bCompAGVWillBeFree = true;
	bool bTopAGVWillBeFree  = true;

	if(bDrivePermCompAGV)
	{
		// VFz hat Fahrerlaubnis
		if(pCrNC_NT)
		{
			// Crash mit der nächsten A-Strecke
			if(!pCrNC_NT->CrossOverFreeWay(nNextBlkPosWayComp))
			{
				// Der Blockweg des VFz zur nächsten Strecke des AFz
				// ist auch ok : VFz wird frei sein
			}
			else
			{
				// Der Blockweg des VFz zur nächsten Strecke des AFz
				// ist zu kurz: VFz wird nicht frei sein
				bCompAGVWillBeFree = false;
			}
		}
		else
		{
			// Kein Gegencrash mit der nächsten A-Strecke:
			// VFz wird ggf nicht frei sein ! ( Block-Tabelle unvollständig !)
			bCompAGVWillBeFree = false;
		}
	}


	// Crash mit der nächsten A-Strecke
	if(!pCrNT_NC->CrossOverFreeWay(nNextBlkPosWayTop))
	{
		// Der Blockweg des AFz zur nächsten Strecke des VFz
		// ist auch ok; AFz wird frei sein
	}
	else
	{
		// Der Blockweg des AFz zur nächsten Strecke des VFz
		// ist zu kurz: AFz wird nicht frei sein
		bTopAGVWillBeFree = false;
	}

	bDriveStop = !bTopAGVWillBeFree && !bCompAGVWillBeFree;

/*	if(bDrivePermCompAGV)
	{
		// VFz hat Fahrerlaubnis
		if(pCrNC_NT)
		{
			// Crash mit der nächsten A-Strecke
			if(!pCrNC_NT->CrossOverFreeWay(nNextBlkPosWayComp))
			{
				// Der Blockweg des VFz zur nächsten Strecke des AFz
				// ist auch ok !
				// Kein FV
			}
			else
			{
				// Der Blockweg des VFz zur nächsten Strecke des AFz
				// ist zu kurz: FV
				bDriveStop = true;
			}
		}
		else
		{
			// Kein Gegencrash mit der nächsten A-Strecke:
			// FV ! ( Block-Tabelle unvollständig !)
			bDriveStop = true;
		}
	}
	else
	{
		// VFz hat Fahrverbot. Da es  mit seiner aktuellen Strecke weder die aktuelle noch die nächste Strecke des
		// AFz tangiert: Kein FV !
	}
*/

	if(bDriveStop)
	{	
		m_nDriveStopReasonNCB = 95;
	}
	return bDriveStop;
}

/*
void CAutomaticTrafficControl::GenerateListDistancesReqWayTele_Old(CDistanceReqWayTeleMap &mapDistancesReqWayTele)
{
	int nDistIndex,
		nDistances,
		nMyCp,
		nMyNCp,
		nMyBlockway;

	mapDistancesReqWayTele.RemoveAll();

	Meldepunkttabelle  *pCp;
	for(int nCpCount = 0;
	nCpCount < KKMAXMP_PLUS1;
	nCpCount++)
	{	
		if(vmptab[nCpCount] > 0)
		{	
			pCp = mptab + (vmptab[nCpCount] - 1);
			nDistIndex = pCp->str_idx - 1;
			nDistances = pCp->anz_nmp;
			nMyCp = nCpCount;
			for(int nDistCount = 0;
			nDistCount < nDistances;
			nDistCount++)
			{
				nMyNCp		= MpNmpTab[nDistIndex].nmp;
				nMyBlockway	= MpNmpTab[nDistIndex].blk_weg;
				ULONG lMyKey = nMyCp*10000 + nMyNCp;
				CDistanceReqWayTele myDRWT(1,nMyCp,nMyNCp);
				nDistIndex++;
				if(IsToRequestWayTele(nDistIndex,nMyBlockway))
				{
					mapDistancesReqWayTele[lMyKey] = myDRWT;
				}
			}
		}
	}
}
*/

void CAutomaticTrafficControl::GenerateListDistancesReqWayTele(CDistanceReqWayTeleMap &mapDistancesReqWayTele)
{
	int nDistIndex,
		nDistances,
		nMyCp,
		nMyNCp,
		nMyBlockway;

	mapDistancesReqWayTele.RemoveAll();

	CCourseAGVType* pCourseAGVType = g_pCourseData->GetPtrCourseAGVType(1);

	CCommPoint  *pCp;
	for (int nCpCount = 0;
		nCpCount < (int)pCourseAGVType->GetNumberCps();
		nCpCount++)
	{
		pCp = pCourseAGVType->GetPtrCommPointByIndex(nCpCount);
		nDistIndex = pCp->GetDistanceIndex();
		nDistances = pCp->GetNCps();
		nMyCp = pCp->GetCp();
		for (int nDistCount = 0;
			nDistCount < nDistances;
			nDistCount++)
		{
			nMyNCp = pCourseAGVType->GetPtrDistance(nDistIndex)->GetNCp();
			nMyBlockway = pCourseAGVType->GetPtrDistance(nDistIndex)->GetBlockWay();
			ULONG lMyKey = nMyCp * 10000 + nMyNCp;
			CDistanceReqWayTele myDRWT(1, nMyCp, nMyNCp);
			nDistIndex++;
			if (IsToRequestWayTele(nDistIndex, nMyBlockway))
			{
				mapDistancesReqWayTele[lMyKey] = myDRWT;
			}
		}
	}
}

bool CAutomaticTrafficControl::IsToRequestWayTele(int nDIndex, int nMyBlockway)
{
	return m_BlockTable.IsToRequestWayTele(nDIndex,nMyBlockway);
}

bool CAutomaticTrafficControl::IsActivated()
{
	return m_bActivated;
}

void CAutomaticTrafficControl::SetActivated(const bool bActivated)
{
	m_bActivated = bActivated;
}


CBlockTableAGVTypePair* CAutomaticTrafficControl::GetPtrBlockTable()
{
	return &this->m_BlockTable;
}

void CAutomaticTrafficControl::FillBlockTable(DEF_BLOCK_TABLE* pBlockTables)
{
	this->m_BlockTable.Fill(pBlockTables);
}