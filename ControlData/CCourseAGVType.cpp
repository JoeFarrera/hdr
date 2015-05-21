//--------------------------------------------------------------------
//Copyright E&K AUTOMATION, Indumat GmbH & Co. KG
//
//Author: J.Lachmund
//
//Description:
//This is the implemetation file of the class CCourseAGVType.
//
//Documentation of changes:
//28.04.2010,	Lachmund:	GetExpectedTimeAndLengthTotal() neu
//2006-08-03:	Lachmund: new
//--------------------------------------------------------------------

/* Include files */

#include "..\stdafx.h"

#include "konstant.h"
#include "komdat.h"

#include "CCourseAGVType.h"

CCourseAGVType::CCourseAGVType()
{
	m_nAGVType              = 0;
	m_nFWSpeed              = 0;
	m_nBWSpeed              = 0;
	m_nCFWSpeed             = 0;
	m_nCBWSpeed             = 0;
	m_nAGVs                 = 0;
	m_nAGVLength            = 0;
    m_nMaxDest              = 0;
	
	//m_pCommPoints           = NULL;
	//m_pCpDistances          = NULL;
	//m_ppCpPointerVector     = NULL;
	//m_pBestWays			    = NULL;
}

CCourseAGVType::CCourseAGVType(short nAGVT)
{
	m_nAGVType              = nAGVT;
	m_nFWSpeed              = 0;
	m_nBWSpeed              = 0;
	m_nCFWSpeed             = 0;
	m_nCBWSpeed             = 0;
	m_nAGVs                 = 0;
	m_nAGVLength            = 0;
    m_nMaxDest              = 0;
	
	//m_pBestWays			    = NULL;
}

CCourseAGVType::~CCourseAGVType()
{
	DeleteCourse();
}

void CCourseAGVType::DeleteCourse()
{
	//DeleteCommPointDistances();
}

/*
void CCourseAGVType::InitCommPointDistances()
{
	for(int nCount = 0;
	nCount < m_nDistances;
	nCount++)
	{
		m_pCpDistances[nCount].nNCp        = 0;
		m_pCpDistances[nCount].nBlockWay   = 0;
		m_pCpDistances[nCount].nTravelTime = 0;
		m_pCpDistances[nCount].nProperties = 0;
		m_pCpDistances[nCount].nLength     = 0;
		m_pCpDistances[nCount].nNDp        = 0;
	}
}


void CCourseAGVType::DeleteCommPointDistances()
{
	if(m_pCpDistances != NULL)
	{
		delete [] m_pCpDistances;
		m_pCpDistances = NULL;
	}
}
*/

void CCourseAGVType::SetAllNextDecisionPoints()
{
	int nCancelLimit = m_arrCommPoints.GetSize();
	for(int nCount = 0;
	(nCount < m_arrCpDistances.GetSize());
	nCount++)
	{
		bool bCanceled = false;
		short nNextDp = 0;
		short nNCp = m_arrCpDistances[nCount].GetNCp();
		int nCpCount = 1; 
		while((nNextDp == 0) && !bCanceled)
		{
			if(IsCpDecisionPoint(nNCp))
			{
				nNextDp = nNCp;
			}
			else
			{
				//nNCp = GetNextDistPointer(m_ppCpPointerVector[nNCp])->nNCp;
				nNCp = GetNextDistPointer(nNCp)->GetNCp();
			}
			nCpCount++;
			bCanceled = (nCpCount > nCancelLimit);		
		}
		m_arrCpDistances[nCount].SetNDp(nNextDp);
	}
}

bool CCourseAGVType::IsCpValid(const int nCp)
{
	return ((nCp == 0) || CpExists(nCp));
}

bool CCourseAGVType::CpExists(const int nCp)
{
	return(((nCp > 0) && (nCp < m_arrCommPointIndex.GetSize())) && (m_arrCommPointIndex[nCp] != 0));
}

bool CCourseAGVType::CpLowerMaximum(const int nCp)
{
	return (nCp < m_arrCommPointIndex.GetSize());
}

bool CCourseAGVType::IsCpDecisionPoint(int nCp)
{
	bool bRet = false;
	if (CpExists(nCp))
	{
		bRet =  m_arrCommPoints[m_arrCommPointIndex[nCp] - 1].IsDecisionPoint();
	}
	return bRet;
}

bool CCourseAGVType::IsCpDestination(int nCp)
{
	bool bRet = false;
	if ((nCp > 0) && (nCp <= m_nMaxDest))
	{
		bRet =  (m_arrDestIndex[nCp] != 0);
	}
	return bRet;
}

bool CCourseAGVType::IsCpHOCStation(int nCp)
{
	bool bRet = false;

	for(int nCount = 0;
	  				       nCount < m_arrHomeOrChargeStations.GetSize();
				           nCount++)
	{
    DEF_HOME_OR_CHARGESTATION tHOCS = GetHOCStation(nCount);

		if (nCp == tHOCS.tStation.nDest)
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}

CCpDistance* CCourseAGVType::GetNextDistPointer(int nCp)
{
	//DEF_CP_DISTANCE* pDist = 0;
	CCpDistance* pDist = 0;
	CCommPoint* pCp = &(m_arrCommPoints[m_arrCommPointIndex[nCp]-1]);
	int nNCps = pCp->GetNCps();
	if(nNCps > 0)
	{
		pDist = &(m_arrCpDistances[pCp->GetDistanceIndex() - 1]);
		if(nNCps != 1)
		{
			pDist = GetTheNonDestinationDist(pDist,nNCps);
		}
	}
	return pDist;
}

CCpDistance* CCourseAGVType::GetTheNonDestinationDist(CCpDistance* pDist,int nNCps)
{
	CCpDistance* pRetDist = NULL;
	
	for(int nCount = 0;
	(nCount < nNCps) && (pRetDist == NULL);
	nCount++)
	{
		if(!IsCpDestination(pDist->GetNCp()))
		{
			pRetDist = pDist;
		}
		pDist++;
	}
	return pRetDist;
}

bool CCourseAGVType::MakeDestIndexArray()
{
	m_arrDestIndex.RemoveAll();
	unsigned int nDests = m_arrDestinations.GetSize();
	m_nMaxDest = m_arrDestinations[nDests - 1].GetNumber();
	m_arrDestIndex.SetSize(m_nMaxDest + 1);
	for(int nICount = 0, nDCount = 0;
		nICount <= m_nMaxDest;
		nICount++)
	{
		if(nICount == m_arrDestinations[nDCount].GetNumber())
		{
			m_arrDestIndex[nICount] = ++nDCount;
		}
		else
		{
			m_arrDestIndex[nICount] = 0;
		}
	}
	return true;
}

void CCourseAGVType::AddToHomeOrChargeStationList(CAGVTypeStationList& listAGVTStations,
												  TYPE_LIST_HOCS_DEF& listHOCStations,
												  unsigned short nProperties)
{
	POSITION pos = listAGVTStations.GetHeadPosition();

	while(pos != NULL)
	{
		CAGVTypeStation tStation = listAGVTStations.GetNext(pos);

		DEF_HOME_OR_CHARGESTATION tHOC;
		tHOC.tStation    = tStation.GetStation();
		tHOC.nProperties = nProperties;
		listHOCStations.AddTail(tHOC);
	}
}

DEF_HOME_OR_CHARGESTATION CCourseAGVType::GetHOCStation(int nIndex)
{
	return m_arrHomeOrChargeStations[nIndex];
}

unsigned int CCourseAGVType::GetNumberHOCStations()
{
	return m_arrHomeOrChargeStations.GetSize();
}

unsigned int CCourseAGVType::MakeHomeOrChargeStationList(CAIDStationArray& arrAIDStations,
                                                         CStringList& listHomeStations,
                                                         CStringList& listChargeStations)
{
	TYPE_LIST_HOCS_DEF listHOCStations;
	unsigned int nAGVTypeMask = (1 << (m_nAGVType-1));
	for(int nCount = 0;
	nCount < arrAIDStations.GetSize();
	nCount++)
	{
		if(arrAIDStations[nCount].GetPSType() == PS_LAYER)
		{
			bool bIsHome       = (arrAIDStations[nCount].GetAGVTypesOfAction(      HOME) & nAGVTypeMask) != 0;
			bool bIsBattCharge = (arrAIDStations[nCount].GetAGVTypesOfAction(BATTCHARGE) & nAGVTypeMask) != 0;
			if(bIsHome || bIsBattCharge)
			{
				CString strStation(arrAIDStations[nCount].GetPSName());
				if (bIsHome)
				{
					POSITION pos = listHomeStations.Find(strStation);

					if(pos == NULL)
					{
						// Home-Station not in list, put it at the end
						listHomeStations.AddTail (strStation);
					}
				}
				if (bIsBattCharge)
				{
					POSITION pos = listChargeStations.Find(strStation);
					if (pos == NULL)
					{
						// Charge-Station not in list, put it at the end
						listChargeStations.AddTail(strStation);
					}
				}

				CAGVTypeStationList listAGVTStations;
				GetAGVTypeStationList(nCount+1,listAGVTStations);
				unsigned short nProperties = (bIsHome ? HOME : 0) | (bIsBattCharge ? BATTCHARGE : 0);
				AddToHomeOrChargeStationList(listAGVTStations,
											 listHOCStations,
											 nProperties);
			}
		}
	}
	int nHOCSCount = 0;

	m_arrHomeOrChargeStations.SetSize(listHOCStations.GetCount());

	POSITION pos = listHOCStations.GetHeadPosition();

	while(pos != NULL)
	{
		m_arrHomeOrChargeStations[nHOCSCount] = listHOCStations.GetNext(pos);
		nHOCSCount++;
	}
	return m_arrHomeOrChargeStations.GetSize();
}




CCpDistance* CCourseAGVType::GetNextDistPointer(unsigned short nCp,unsigned short nDestCp)
{
	CCpDistance* pDist = NULL;
	CCommPoint* pCp = GetPtrCommPoint(nCp);
	if(pCp)
	{
		if(pCp->GetDpIndex() == 0)
		{
			pDist = GetNextDistPointer(nCp);
		}
		else
		{
			unsigned short nNCp;
			nNCp = GetNCpOfDp(pCp->GetDpIndex(),nDestCp);
			pDist = GetDistPointer(nCp,nNCp);
		}
	}
	return pDist;
}

unsigned short CCourseAGVType::GetNCpOfDp(unsigned short nDpIndex,unsigned short nDestCp)
{
	unsigned short nNCp = 0;
	unsigned short nArea = GetPtrCommPoint(nDestCp)->GetCpArea();
	//nNCp = m_pBestWays->pDpsBestWays[nDpIndex - 1].pDpBestWays[nArea - 1];
	nNCp = m_BestWays.GetNCpOfDp(nDpIndex,nArea);
	return nNCp;
}

CCpDistance* CCourseAGVType::GetPtrFirstDist(int nCp)
{
	CCommPoint* pCp = GetPtrCommPoint(nCp);
	return &(m_arrCpDistances[pCp->GetDistanceIndex() - 1]);
}

CCpDistance* CCourseAGVType::GetDistPointer(unsigned short nCp,unsigned short nNCp)
{
	CCpDistance* pRetDist = NULL;
	CCommPoint* pCp = GetPtrCommPoint(nCp);
	
	if(pCp != NULL)
	{
		CCpDistance* pDist = &(m_arrCpDistances[pCp->GetDistanceIndex() - 1]);
		for (int nCount = 0;
		     nCount < pCp->GetNCps();
		     nCount++)
		{
			if (pDist->GetNCp() == nNCp)
			{ 
				pRetDist = pDist;
				break;
			}
			pDist++;
		}
	}
	return pRetDist;
}

unsigned short CCourseAGVType::GetDistIndex(unsigned short nCp,unsigned short nNCp)
{
	unsigned short   nDistIndex;
	CCpDistance* pDist;
	
	pDist = GetDistPointer(nCp,  nNCp);
    nDistIndex = (pDist == 0)? 0: pDist->GetDistIndex();
	return nDistIndex;
}

unsigned short CCourseAGVType::GetDestIndex(const int nDest)
{
	unsigned short   nRet = 0;
	if(IsCpDestination(nDest))
	{
		nRet = m_arrDestIndex[nDest];
	}
	return nRet;
}

int CCourseAGVType::GetDestPosIndex(const TYPE_STATION_DEF& tStation)
{
	unsigned short   nRet = 0;
	if(IsCpDestination(tStation.nDest))
	{
		unsigned int nDestIndex = m_arrDestIndex[tStation.nDest];
		nRet  = m_arrDestinations[nDestIndex -1].GetFirstPosIndex();
		nRet += tStation.nPos -1; 
	}
	return nRet;
}

unsigned short CCourseAGVType::GetNextCp(unsigned short nCp,unsigned short nDestCp)
{
	unsigned short   nNCp = 0;
	CCpDistance* pDist;
	if (CpExists(nCp) &&
	    IsCpDestination(nDestCp) )
	{
		pDist = GetNextDistPointer(nCp,  nDestCp);
	    nNCp = (pDist == 0)? 0: pDist->GetNCp();
	}
	return nNCp;
}

unsigned short CCourseAGVType::GetBlockWay(unsigned short nCp,unsigned short nNCp)
{
	unsigned short   nBlockWay;
	CCpDistance* pDist;
	
	pDist = GetDistPointer(nCp,  nNCp);
    nBlockWay = (pDist == 0)? 0: pDist->GetBlockWay();
	return nBlockWay;
}

unsigned short CCourseAGVType::GetBlockWay(unsigned short nDistIndex)
{
	unsigned short   nBlockWay;
	CCpDistance* pDist = &(m_arrCpDistances[nDistIndex - 1]);
	
    nBlockWay = (pDist == 0)? 0: pDist->GetBlockWay();
	return nBlockWay;
}

unsigned short CCourseAGVType::GetDistLen(unsigned short nDistIndex)
{
	unsigned short   nDistLen;
	CCpDistance* pDist = &(m_arrCpDistances[nDistIndex - 1]);
	
    nDistLen = (pDist == 0)? 0: pDist->GetLength();
	return nDistLen;
}

unsigned short CCourseAGVType::GetTravelTimeDistance(unsigned short nCp,unsigned short nNCp)
{
	unsigned short   nTravelTime;
	CCpDistance* pDist;
	
	pDist = GetDistPointer(nCp,  nNCp);
    nTravelTime = (pDist == NULL)? 0: pDist->GetTravelTime();
	return nTravelTime;
}

unsigned short CCourseAGVType::GetTravelTimeDistance(unsigned short nDistIndex)
{
	unsigned short   nTravelTime;
	CCpDistance* pDist = &(m_arrCpDistances[nDistIndex - 1]);
	
    nTravelTime = (pDist == NULL)? 0: pDist->GetTravelTime();
	return nTravelTime;
}

unsigned short CCourseAGVType::GetRealTravelTimeDistance(unsigned short nDistIndex)
{
	unsigned short   nTravelTime;
	CCpDistance* pDist = &(m_arrCpDistances[nDistIndex - 1]);
	
    nTravelTime = (pDist == NULL)? 0: pDist->GetRealTravelTime();
	return nTravelTime;
}

int CCourseAGVType::GetBestTravelTime(int nCp,int nDest)
{
	int          nRet   = 0;
	int nStep  = 0;
	int          nTopCp = nCp;
	while((nTopCp != nDest) && (nStep < m_arrCommPoints.GetSize()))
	{
		CCpDistance* pDist = GetNextDistPointer(nTopCp, nDest);
		if(pDist != NULL)
		{
			nTopCp = pDist->GetNCp();
			nRet  += pDist->GetTravelTime();
			nStep++;
		}
		else
		{
			nStep = m_arrCommPoints.GetSize();
		}
	}
	if(nTopCp != nDest) nRet = -1;
	{
		// it's a decision point:
	}
	return nRet;
}

unsigned short CCourseAGVType::GetPosWay(TYPE_STATION_DEF& tStation)
{
	unsigned short   nPosWay = 0;
	
	if ((tStation.nDest >           0) &&
	    (tStation.nDest <= m_nMaxDest)  )
	{
		unsigned int nDestIndex = m_arrDestIndex[tStation.nDest];
		if(nDestIndex != 0)
		{
			int nPosLimit = 0;
			int nIndex = 0;
			if (tStation.ucSide == 'L')
			{
				nPosLimit = m_arrDestinations[nDestIndex-1].GetLeftPositions();
				nIndex = tStation.nPos - 1;
			}
			else
			{
				if (tStation.ucSide == 'R')
				{
					nPosLimit = m_arrDestinations[nDestIndex-1].GetRightPositions();
					nIndex = m_arrDestinations[nDestIndex-1].GetLeftPositions() + tStation.nPos - 1;
				}
			}
			if((tStation.nPos > 0) && (tStation.nPos <= nPosLimit))
			{
				unsigned int nDestPosIndex =  m_arrDestinations[nDestIndex-1].GetFirstPosIndex() + nIndex;
				nPosWay = m_arrDestPositions[nDestPosIndex - 1].GetPosWay();
			}
		}
	}
	return nPosWay;
}

unsigned short CCourseAGVType::GetDestType(TYPE_STATION_DEF& tStation)
{
	unsigned short uRet = 0;
	if(IsCpDestination(tStation.nDest))
	{
		unsigned int nDestIndex = m_arrDestIndex[tStation.nDest];
		short nDestTypeLeft  = m_arrDestinations[nDestIndex-1].GetDestTypeLeft ();
		short nDestTypeRight = m_arrDestinations[nDestIndex-1].GetDestTypeRight();
		uRet = (tStation.ucSide == 'L') ? nDestTypeLeft:nDestTypeRight;
	}
	return uRet;
}

bool CCourseAGVType::GetAnyStation(TYPE_STATION_DEF& tStation)
{
	bool bRet = false;
	if(IsCpDestination(tStation.nDest))
	{
		unsigned int nDestIndex = m_arrDestIndex[tStation.nDest];
		tStation.ucSide = m_arrDestinations[nDestIndex-1].GetDestTypeLeft() ? 'L':'R';
	}
	return bRet;
}

void CCourseAGVType::GetCpSequenceToDest(unsigned short nStartCp,
                                         unsigned short nDest,
                                         unsigned short *pCpSequence)
{
  unsigned short  nCount,
                  nCp;
  int             nCPs = m_arrCommPoints.GetSize();
  
  *pCpSequence = nStartCp;
  *(pCpSequence + 1) = 0;
  
  for(nCount = 1, nCp = *(pCpSequence);
      (nCp != nDest) && (nCount < nCPs);
      nCount++)
  {
    *(pCpSequence + nCount+ 1) = 0;
    nCp = GetNextCp(nCp, nDest);

    *(pCpSequence + nCount) = nCp;
  }

	if (nCount < nCPs)
	{
	   nCp = GetNextCp(nCp, nDest);
	  *(pCpSequence + nCount) = nCp;
	  *(pCpSequence + nCount + 1) = 0;
	}
	else
	{
		*pCpSequence = 0;
	}
}

bool CCourseAGVType::LinkDestPosToAIDStations(CAIDStationArray& arrADIStations,
											  CCustomerStationList& listCustomerStations)
{
	bool bRet = true;
	for(int nDCount = 0;
	nDCount < m_arrDestinations.GetSize();
	nDCount++)
	{
		int nDest = m_arrDestinations[nDCount].GetNumber();
		

		//Linke Ziele:
		if(m_arrDestinations[nDCount].GetDestTypeLeft())
		{
			CString strName;
			CCustomerStation tCS(nDest,'L',0,0,strName);
			POSITION pos = listCustomerStations.Find(tCS);

			if(pos != NULL)
			{
				tCS = listCustomerStations.GetAt(pos);
				int nPSIndex = tCS.FindAIDStation(arrADIStations);
				
				m_arrDestinations[nDCount].SetLeftPSIndex(nPSIndex);
				m_arrDestinations[nDCount].LinkLeftPosToPhysicalStation(m_arrDestPositions,
																		m_arrLayer,
																		arrADIStations, 
																		listCustomerStations);
			}
		}
		//Rechte Ziele:
		if(m_arrDestinations[nDCount].GetDestTypeRight())
		{
			CString strName;
			CCustomerStation tCS(nDest,'R',0,0,strName);
			POSITION pos = listCustomerStations.Find(tCS);

			if(pos != NULL)
			{
				tCS = listCustomerStations.GetAt(pos);
				int nPSIndex = tCS.FindAIDStation(arrADIStations);
				m_arrDestinations[nDCount].SetRightPSIndex(nPSIndex);
				m_arrDestinations[nDCount].LinkRightPosToPhysicalStation(m_arrDestPositions,
																		 m_arrLayer,
																		 arrADIStations, 
																		 listCustomerStations);
			}
		}
	}
	return bRet;
}

void CCourseAGVType::WriteForTest(FILE* fp)
{
	fprintf(fp, "\nFzTyp %d\n",m_nAGVType);
	fprintf(fp, "\n*************************************");
	fprintf(fp, "\nAnzahl Fahrzeuge:  %4i", m_nAGVs);
	fprintf(fp, "\nFahrzeug-Typ       %4i", m_nAGVType);
	fprintf(fp, "\nFahrzeug-Laenge    %4i", m_nAGVLength);
	fprintf(fp, "\nSpeed vorwaerts    %4i", m_nFWSpeed);
	fprintf(fp, "\nSpeed rueckwaerts  %4i", m_nBWSpeed);
	fprintf(fp, "\nCSpeed vorwaerts   %4i", m_nCFWSpeed);
	fprintf(fp, "\nCSpeed rueckwaerts %4i", m_nCBWSpeed);
	fprintf(fp, "\nMPs                %4i", m_arrCommPoints.GetSize());
	fprintf(fp, "\nMax MP             %4i", GetMaxCp());
	fprintf(fp, "\nStrecken           %4i", m_arrCpDistances.GetSize());
	fprintf(fp, "\nZiele              %4i", m_arrDestinations.GetSize());
	fprintf(fp, "\nMax Ziel           %4i", m_nMaxDest);
	fprintf(fp, "\nZiel-Positionen    %4i", m_arrDestPositions.GetSize());
	fprintf(fp, "\nEH und LP          %4i", m_arrHomeOrChargeStations.GetSize());
	fprintf(fp, "\n\n");

	WriteArrayCommPointerIndexForTest  (fp);	
	WriteCpForTest                     (fp);
	WriteDistForTest                   (fp);
	WriteArrayDestIndexForTest         (fp);
	WriteDestForTest                   (fp);
	WriteDestPosForTest                (fp);
	WriteBestWaysForTest               (fp);
	WriteHomeOrChargeStationListForTest(fp);
	
}

void CCourseAGVType::WriteArrayCommPointerIndexForTest(FILE* fp)
{
	fprintf(fp,"\nArrayCommPointerIndex :\n\n");
	for (int nCount = 0;
		 nCount < m_arrCommPointIndex.GetSize();
	 	 nCount++)
	{
		fprintf(fp, "%d. %d\n",
		         nCount,
		         m_arrCommPointIndex[nCount]);
	}
}

void CCourseAGVType::WriteCpForTest(FILE* fp)
{
	fprintf(fp,"\nCommPoints :\n\n");
	for (int nCount = 0;
		 nCount < m_arrCommPoints.GetSize();
	 	 nCount++)
	{
		m_arrCommPoints[nCount].WriteForTest(fp, nCount+1);
	}
}

void CCourseAGVType::WriteDistForTest(FILE* pFile)
{
	fprintf(pFile,"\nCPDistances :\n\n");
	for(int nCount = 0;
	nCount < m_arrCpDistances.GetSize();
	nCount++)
	{
		m_arrCpDistances[nCount].WriteForTest(pFile,nCount + 1);
	}
}

void CCourseAGVType::WriteArrayDestIndexForTest(FILE* fp)
{
	fprintf(fp,"\nArrayDestIndex :\n\n");
	for (int nCount = 0;
		 nCount < m_arrDestIndex.GetSize();
	 	 nCount++)
	{
		fprintf(fp, "Destination %d: %d.Destination\n",
		         nCount,
		         m_arrDestIndex[nCount]);
	}
}

void CCourseAGVType::WriteDestForTest(FILE* fp)
{
	fprintf(fp,"\nDestinations :\n\n");
	for(int nCount = 0;
	nCount < m_arrDestinations.GetSize();
	nCount++)
	{
		m_arrDestinations[nCount].WriteForTest(fp,nCount + 1);
	}
}

void CCourseAGVType::WriteDestPosForTest(FILE* fp)
{
	fprintf(fp,"\nDestPositions :\n\n");
	for(int nPCount = 0;
	nPCount < m_arrDestPositions.GetSize();
	nPCount++)
	{
		m_arrDestPositions[nPCount].WriteForTest(fp,nPCount+1);
	}
}

void CCourseAGVType::WriteBestWaysForTest(FILE* fp)
{
	m_BestWays.WriteBestWaysForTest(fp);
}

void CCourseAGVType::WriteHomeOrChargeStationListForTest(FILE* fp)
{
	fprintf(fp,"\nHomeOrChargeStation :\n\n");
	for (int nCount = 0;
		 nCount < m_arrHomeOrChargeStations.GetSize();
	 	 nCount++)
	{
		fprintf(fp,"%d.Station %d, Side:%c, Pos:%hd, Layer:%d, Properties: %d\n",
		         nCount+1,
		         m_arrHomeOrChargeStations[nCount].tStation.nDest,
		         m_arrHomeOrChargeStations[nCount].tStation.ucSide,
		         m_arrHomeOrChargeStations[nCount].tStation.nPos,
		         (short)m_arrHomeOrChargeStations[nCount].tStation.nLayer,
		         m_arrHomeOrChargeStations[nCount].nProperties);
	}
}

bool CCourseAGVType::CheckAGVDistances(const int nCp, const int nNCp, const int nNNCp)
{
	bool bRet = false;
	if(nCp)
	{
		bRet = true;
		if (nNCp)
		{
	 		bRet &= (CpExists(nCp) && CpExists(nNCp) && (GetDistIndex(nCp,nNCp) > 0)); 
			if(bRet && nNNCp)
			{
				bRet &= (CpExists(nNNCp) && (GetDistIndex(nNCp,nNNCp) > 0));
			}
		}
		else if (nNNCp)
		{
			bRet = false;
		}
	}
	return bRet;
}
bool CCourseAGVType::IsValidDestination(const int nDest)
{
	return((nDest == 0) || ((nDest > 0) && (nDest <= m_nMaxDest) && (m_arrDestIndex[nDest] != 0)));
}

void CCourseAGVType::SetDestPositionsInDestinations(CUIntArray& arrDestTypeIndex,CDestinationTypeArray& arrDestTypes)
{
	int nDestinations = m_arrDestinations.GetSize();
	for(int nDCount = 0;
	nDCount < nDestinations;
	nDCount++)
	{
		short nDestType;

		//Linke Ziele:
		m_arrDestinations[nDCount].SetLeftPositions(0);
		nDestType = m_arrDestinations[nDCount].GetDestTypeLeft();
		if(nDestType)
		{
			unsigned short nPositions = arrDestTypes[arrDestTypeIndex[nDestType]-1].GetPositions();
			m_arrDestinations[nDCount].SetLeftPositions(nPositions);
		}
		//Rechte Ziele:
		m_arrDestinations[nDCount].SetRightPositions(0);
		nDestType = m_arrDestinations[nDCount].GetDestTypeRight();
		if(nDestType)
		{
			unsigned short nPositions = arrDestTypes[arrDestTypeIndex[nDestType]-1].GetPositions();
			m_arrDestinations[nDCount].SetRightPositions(nPositions);
		}
	}
}

void CCourseAGVType::FillLayersInDestPositions(CUIntArray& arrDestTypeIndex,CDestinationTypeArray& arrDestTypes,
								   			   CUIntArray& arrColumnTypeIndex,CColumnTypeArray& arrColumnType,
								   			   CDestTypePosList& listDestTypePos)
{
	int nDestinations = m_arrDestinations.GetSize();
	for(int nDCount = 0;
	nDCount < nDestinations;
	nDCount++)
	{
		short        nDestType;
		unsigned int nPosIndex;
		
		//Linke Ziele:
		nDestType = m_arrDestinations[nDCount].GetDestTypeLeft();
		if(nDestType)
		{
			nPosIndex           = m_arrDestinations[nDCount].GetFirstPosIndex();
			unsigned short nPositions   = arrDestTypes[arrDestTypeIndex[nDestType] - 1].GetPositions();
			unsigned short nDTPosIndex  = arrDestTypes[arrDestTypeIndex[nDestType] - 1].GetPosIndex();

			POSITION pos = listDestTypePos.GetHeadPosition();
			for(int nITCount = 1;
			(nITCount < nDTPosIndex) && (pos != NULL);
			nITCount++)
			{
				listDestTypePos.GetNext(pos);
			}
			
			for(int nPCount = 0;
			nPCount < nPositions;
			nPCount++, nPosIndex++)
			{
				if(pos != NULL)
				{
					CDestTypePos tDPT = listDestTypePos.GetNext(pos);
					if(tDPT.m_cLayerColumn == 'E')
					{
						m_arrDestPositions[nPosIndex-1].SetLayers(1);
					}
					else
					{
						unsigned short nCType = tDPT.m_nType;
						int nColIndex = arrColumnTypeIndex[nCType];
						m_arrDestPositions[nPosIndex-1].SetLayers(arrColumnType[nColIndex - 1].GetLayers());
					}
				}
				//Zum Test:
				/*
				if(m_arrDestPositions[nPosIndex-1].GetLayers() == 0)
				{
					printf("Ziel%d, links, Pos%d: 0 Layers !!?\n",nDest,nPCount+1);
				}
				*/
			}
		}
		//Rechte Ziele :
		nDestType = m_arrDestinations[nDCount].GetDestTypeRight();
		if(nDestType)
		{
			nPosIndex = m_arrDestinations[nDCount].GetFirstPosIndex() + m_arrDestinations[nDCount].GetLeftPositions();
			unsigned short nPositions   = arrDestTypes[arrDestTypeIndex[nDestType] - 1].GetPositions();
			unsigned short nDTPosIndex  = arrDestTypes[arrDestTypeIndex[nDestType] - 1].GetPosIndex();

			POSITION pos = listDestTypePos.GetHeadPosition();
			for(int nITCount = 1;
			(nITCount < nDTPosIndex) && (pos != NULL);
			nITCount++)
			{
				listDestTypePos.GetNext(pos);
			}
			
			for(int nPCount = 0;
			nPCount < nPositions;
			nPCount++,nPosIndex++)
			{
				if(pos != NULL)
				{
					CDestTypePos tDPT = listDestTypePos.GetNext(pos);
					if(tDPT.m_cLayerColumn == 'E')
					{
						m_arrDestPositions[nPosIndex-1].SetLayers(1);
					}
					else
					{
						unsigned short nCType = tDPT.m_nType;
						int nColIndex = arrColumnTypeIndex[nCType];
						m_arrDestPositions[nPosIndex-1].SetLayers(arrColumnType[nColIndex - 1].GetLayers());
					}
				}
				//Zum Test:
				/*
				if(m_arrDestPositions[nPosIndex-1].GetLayers() == 0)
				{
					printf("Ziel%d, rechts, Pos%d: 0 Layers !!?\n",nDest,nPCount+1);
				}
				*/
			}
		}
	}
}

void CCourseAGVType::MakeLayerArray()
{
	int nLayers = 0;
	unsigned int nLimit = m_arrDestPositions.GetSize();
	unsigned int nCount;
	for(nCount = 0;
	nCount < nLimit;
	nCount++)
	{
		nLayers += m_arrDestPositions[nCount].GetLayers();
	}
	m_arrLayer.SetSize(nLayers);
	int nLCount = 0;
	int nLayerIndex = 1;
	for(nCount  = 0;
	nCount < nLimit;
	nCount++)
	{
		int nPLimit = m_arrDestPositions[nCount].GetLayers();
		m_arrDestPositions[nCount].SetFirstLayerIndex(nLayerIndex);
		for(int nPLCount = 0;
		nPLCount < nPLimit;
		nPLCount++)
		{
			m_arrLayer[nLCount++] = CAGVTLayer((unsigned char)(nPLCount+1));
		}
		nLayerIndex += nPLimit;
	}
}
bool CCourseAGVType::IsActionOfStation(int                    nAction,
						   			   TYPE_STATION_DEF&      tStation,
						   			   CUIntArray&            arrDestTypeIndex,
						   			   CDestinationTypeArray& arrDestTypes,
						   			   CDestTypePosList&      listDestTypePos,
						   			   CUIntArray&            arrColumnTypeIndex,
						   			   CColumnTypeArray&      arrColumnTypes,
						   			   CUIntArray&            arrLayerTypeIndex,
						   			   CLayerTypeArray&       arrLayerTypes)
{
	bool bRet = false;
	int nDIndex = m_arrDestIndex[tStation.nDest];
	if(nDIndex > 0)
	{
		int nDestType = (tStation.ucSide == 'L') ? m_arrDestinations[nDIndex - 1].GetDestTypeLeft():m_arrDestinations[nDIndex - 1].GetDestTypeRight();
		if(nDestType > 0)
		{
			int nDTIndex = arrDestTypeIndex[nDestType];
			if(nDTIndex > 0)
			{
				bRet = arrDestTypes[nDTIndex - 1].IsAction(nAction,tStation.nPos,tStation.nLayer,
														  listDestTypePos,
														  arrColumnTypeIndex,arrColumnTypes,
														  arrLayerTypeIndex ,arrLayerTypes);
			}
		}
	}
	return bRet;
}

void CCourseAGVType::GetAGVTypeStationList(unsigned int nAIDPSIndex,CAGVTypeStationList& listAGVTStations)
{
	int nLimit = m_arrDestinations.GetSize();
	for(int nDCount = 0;
	nDCount < nLimit;
	nDCount++)
	{
		m_arrDestinations[nDCount].GetAGVTypeStationList(m_nAGVType, nAIDPSIndex,listAGVTStations,m_arrDestPositions,m_arrLayer);
	}
}

unsigned int CCourseAGVType::GetAllStationObjects(unsigned int nAIDPSIndex)
{
	unsigned int nRet = 0;
	int nLimit = m_arrDestinations.GetSize();
	for(int nDCount = 0;
	(nDCount < nLimit) && !nRet;
	nDCount++)
	{
		nRet = m_arrDestinations[nDCount].GetAllStationObjects(m_nAGVType,nAIDPSIndex,m_arrDestPositions,m_arrLayer);
	}
	return nRet;
}

unsigned int CCourseAGVType::GetAIDStationIndex(int nDest, char cSide, int nPos, int nLayer,int& nError)
{
	unsigned int nPSIndex = 0;
	int nDIndex = m_arrDestIndex[nDest];
	nError = nDIndex ? 0:1;
	if(!nError)
	{
		if(nPos == 0)
		{
			nPSIndex = m_arrDestinations[nDIndex - 1].GetPSIndex(cSide);
		}
		else
		{
			int nPosIndex = m_arrDestinations  [nDIndex   - 1].GetFirstDestPosIndex(cSide) + nPos -1;
			nPSIndex      = m_arrDestPositions[nPosIndex - 1].GetFirstPhysicalStationIndex() + nLayer;
		}
	}
	return nPSIndex;
}

void CCourseAGVType::GetPSIndexListWithChildren(unsigned int nPSIndex, CChangedStationObjList& listChangeStations)
{
	int nLimit = m_arrDestinations.GetSize();
	for(int nDCount = 0;
	nDCount < nLimit;
	nDCount++)
	{
		m_arrDestinations[nDCount].GetPSIndexListWithChildren(m_nAGVType,nPSIndex,listChangeStations,m_arrDestPositions,m_arrLayer);
	}
}

CAGVTDestination* CCourseAGVType::GetPtrDestination(int nDest)
{
	CAGVTDestination* pRet = NULL;
	if(IsCpDestination(nDest))
	{
		pRet = &(m_arrDestinations[m_arrDestIndex[nDest] - 1]);
	}
	return pRet;
}

CAGVTDestination* CCourseAGVType::GetPtrDestinationByIndex(unsigned int nIndex)
{
	return &(m_arrDestinations[nIndex]);
}

bool CCourseAGVType::MakeCommPointIndexArray()
{
	bool bRet = true;
	unsigned int nCps = m_arrCommPoints.GetSize();
	if(nCps > 0)
	{
		int nMaxCp = m_arrCommPoints[nCps-1].GetCp();
		m_arrCommPointIndex.SetSize(nMaxCp + 1);
		int nCpCount = 0;
		for(int nICount = 0;
		nICount <= nMaxCp;
		nICount++)
		{
			if(nICount == m_arrCommPoints[nCpCount].GetCp())
			{
				nCpCount++;
				m_arrCommPointIndex[nICount] = nCpCount;
			}
			else
			{
				m_arrCommPointIndex[nICount] = 0;
			}
		} 
	}
	return bRet;
}

bool CCourseAGVType::LoadCommPoints(CCtrlDataMemFile& myMLFile)
{
	int nAnz;
	bool bDataOK = myMLFile.Read(nAnz);

	m_arrCommPoints.SetSize(nAnz);
	if(bDataOK)
	{
		int			nCp,
						nUserInfo,
						nCpArea;
		short		nCtrlArea,
				 		nCommArea;
		unsigned int	nProperties; 
		unsigned short	nDpIndex = 0;
		for(int nCount = 0;
						(nCount < nAnz) && bDataOK;
						nCount++)
		{
			bDataOK =	myMLFile.Read(nCp        ) &&
								myMLFile.Read(nCtrlArea  ) &&
								myMLFile.Read(nCommArea  ) &&
								myMLFile.Read(nUserInfo  ) &&
								myMLFile.Read(nCpArea    ) &&
								myMLFile.Read(nProperties);
			if(bDataOK)
			{
				bDataOK = nCpArea > 0;
				
				if (nCpArea == 0)
				{
					printf("\n++++ CpArea of CP %d, AGVType %d is 0; Error in Layout !!", nCp, (int)m_nAGVType );   
				}

				CCommPoint tCp(nCp,
							         (unsigned  char)nCtrlArea,
							         (unsigned  char)nCommArea,
							         (unsigned short)nCpArea,
							         nCount,
							         nProperties & 0x0f,
							         (nProperties & 0x10) ? (++nDpIndex) : 0);
				m_arrCommPoints[nCount] = tCp;
			}
		}
		if(bDataOK)
		{
			bDataOK = MakeCommPointIndexArray();
		}
	}
	return bDataOK;
}

bool CCourseAGVType::LoadCommPoints(CLoaderFile* pLFile)
{
	int nAnz;
	bool bDataOK = pLFile->Read(nAnz);

	m_arrCommPoints.SetSize(nAnz);
	if(bDataOK)
	{
		int 		nCp,
						nUserInfo,
						nCpArea;
		short 	nCtrlArea,
				 		nCommArea;
		unsigned int	nProperties; 
		unsigned short	nDpIndex = 0;
		for(int nCount = 0;
						(nCount < nAnz) && bDataOK;
						nCount++)
		{
			bDataOK =	pLFile->Read(nCp        ) &&
						pLFile->Read(nCtrlArea  ) &&
						pLFile->Read(nCommArea  ) &&
						pLFile->Read(nUserInfo  ) &&
						pLFile->Read(nCpArea    ) &&
						pLFile->Read(nProperties);
			if(bDataOK)
			{
				bDataOK = nCpArea > 0;
				
				if (nCpArea == 0)
				{
					printf("\n++++ CpArea of CP %d, AGVType %d is 0; Error in Layout !!", nCp, (int)m_nAGVType );   
				}

				CCommPoint tCp(nCp,
	 									   (unsigned  char)nCtrlArea,
	 									   (unsigned  char)nCommArea,
	 									   (unsigned short)nCpArea,
	 									   nCount,
	 									   nProperties & 0x0f,
	 									   (nProperties & 0x10) ? (++nDpIndex) : 0);
				m_arrCommPoints[nCount] = tCp;
			}
		}
		if(bDataOK)
		{
			bDataOK = MakeCommPointIndexArray();
		}
	}
	return bDataOK;
}

bool CCourseAGVType::LoadCommPointDistances(CCtrlDataMemFile& myMFile)
{
	int nAnz;
	bool bDataOK = myMFile.Read(nAnz);

	if(bDataOK)
	{
		m_arrCpDistances.SetSize(nAnz);
		int nCpCount = 0;
		CCommPoint* pCp = GetPtrCommPointByIndex(nCpCount); 
		int nCp,
			nNCp,
			nUserInfo,
			nBlockWay,
			nLength,
			nTravelTime,
			nUserInfoAGVT,
			nProperties,
			nOldCp = 0;
		int nDistCount = 0;
		for (int nCount = 0;
	    	(nCount < nAnz) && bDataOK;
		     nCount++)
		{
			bDataOK =	myMFile.Read(nCp          ) &&
								myMFile.Read(nNCp         ) &&
								myMFile.Read(nUserInfo    ) &&
								myMFile.Read(nBlockWay    ) &&
								myMFile.Read(nTravelTime  ) &&
								myMFile.Read(nLength      ) &&
								myMFile.Read(nUserInfoAGVT) &&
								myMFile.Read(nProperties  );
			if(bDataOK)
			{
				// First Cp
				if (nCount == 0)
				{
					nOldCp = nCp;
					pCp->SetDistanceIndex(nCount+1);
				}
				
				if(nOldCp != nCp)
				{
					if(nDistCount != 0)
					{
						pCp->SetNCps(nDistCount);
					}
					//pCp++;
					nCpCount++;
					pCp = GetPtrCommPointByIndex(nCpCount);
					if (pCp->GetCp() == nCp)
					{
						//pCp->pCpDistance = pCpDist;
						pCp->SetDistanceIndex(nCount+1);
						nDistCount = 1;
						nOldCp = nCp;
					}
					else
					{
						bDataOK = false;
					}
				}
				else
				{
					nDistCount++;
				}
				//Umrechnen von 1/10 in 1/2 Sekunden:
				div_t tDiv = div(nTravelTime,5);
				CCpDistance tDist(nCount + 1,
								 nNCp,
								 nLength,
								 nBlockWay,
								 nProperties,
								 0,
								 tDiv.quot + ((tDiv.rem > 0)? 1 : 0));
				m_arrCpDistances[nCount] = tDist;
			}
		}
		if(!bDataOK)
		{
			m_arrCpDistances.RemoveAll();
		}
		else
		{
			pCp->SetNCps(nDistCount);
			// SetAllNextDecisionPoints();
		}
	}
	return bDataOK;
}

bool CCourseAGVType::LoadCommPointDistances(CLoaderFile* pLFile)
{
	int nAnz;
	bool bDataOK = pLFile->Read(nAnz);

	if(bDataOK)
	{
		m_arrCpDistances.SetSize(nAnz);
		int nCpCount = 0;
		CCommPoint* pCp = GetPtrCommPointByIndex(nCpCount); 
		int nCp,
			nNCp,
			nUserInfo,
			nBlockWay,
			nLength,
			nTravelTime,
			nUserInfoAGVT,
			nProperties,
			nOldCp = 0;
		int nDistCount = 0;
		for (int nCount = 0;
	    	(nCount < nAnz) && bDataOK;
		     nCount++)
		{
			bDataOK = 	pLFile->Read(nCp          ) &&
						pLFile->Read(nNCp         ) &&
						pLFile->Read(nUserInfo    ) &&
						pLFile->Read(nBlockWay    ) &&
						pLFile->Read(nTravelTime  ) &&
						pLFile->Read(nLength      ) &&
						pLFile->Read(nUserInfoAGVT) &&
						pLFile->Read(nProperties  );
			if(bDataOK)
			{
				// First Cp
				if (nCount == 0)
				{
					nOldCp = nCp;
					pCp->SetDistanceIndex(nCount+1);
				}
				
				if(nOldCp != nCp)
				{
					if(nDistCount != 0)
					{
						pCp->SetNCps(nDistCount);
					}
					//pCp++;
					nCpCount++;
					pCp = GetPtrCommPointByIndex(nCpCount);
					if (pCp->GetCp() == nCp)
					{
						//pCp->pCpDistance = pCpDist;
						pCp->SetDistanceIndex(nCount+1);
						nDistCount = 1;
						nOldCp = nCp;
					}
					else
					{
						bDataOK = false;
					}
				}
				else
				{
					nDistCount++;
				}
				//Umrechnen von 1/10 in 1/2 Sekunden:
				div_t tDiv = div(nTravelTime,5);
				CCpDistance tDist(nCount + 1,
								 nNCp,
								 nLength,
								 nBlockWay,
								 nProperties,
								 0,
								 tDiv.quot + ((tDiv.rem > 0)? 1 : 0));
				m_arrCpDistances[nCount] = tDist;
			}
		}
		if(!bDataOK)
		{
			m_arrCpDistances.RemoveAll();
		}
		else
		{
			pCp->SetNCps(nDistCount);
			// SetAllNextDecisionPoints();
		}
	}
	return bDataOK;
}
/*
bool CCourseAGVType::LoadCommPointDistancesPointer(CLoaderFile* pLFile)
{
	int nAnz;
	bool bDataOK = pLFile->Read(nAnz);
	m_nDistances =  nAnz;

	m_pCpDistances = new DEF_CP_DISTANCE[nAnz];
	bDataOK = (m_pCpDistances != NULL);
	if(bDataOK)
	{
		InitCommPointDistances();
		DEF_CP_DISTANCE* pCpDist = m_pCpDistances;
		int nCpCount = 0;
		CCommPoint* pCp = GetPtrCommPointByIndex(nCpCount); 
		int nCp,
			nNCp,
			nUserInfo,
			nBlockWay,
			nLength,
			nTravelTime,
			nUserInfoAGVT,
			nProperties,
			nOldCp;
		int nDistCount = 0;
		for (int nCount = 0;
	    	(nCount < nAnz) && bDataOK;
		     nCount++)
		{
			bDataOK = 	pLFile->Read(nCp          ) &&
						pLFile->Read(nNCp         ) &&
						pLFile->Read(nUserInfo    ) &&
						pLFile->Read(nBlockWay    ) &&
						pLFile->Read(nTravelTime  ) &&
						pLFile->Read(nLength      ) &&
						pLFile->Read(nUserInfoAGVT) &&
						pLFile->Read(nProperties  );
			if(bDataOK)
			{
				// First Cp
				if (nCount == 0)
				{
					nOldCp = nCp;
					pCp->SetDistanceIndex(nCount+1);
				}
				
				if(nOldCp != nCp)
				{
					if(nDistCount != 0)
					{
						pCp->SetNCps(nDistCount);
					}
					//pCp++;
					nCpCount++;
					pCp = GetPtrCommPointByIndex(nCpCount);
					if (pCp->GetCp() == nCp)
					{
						//pCp->pCpDistance = pCpDist;
						pCp->SetDistanceIndex(nCount+1);
						nDistCount = 1;
						nOldCp = nCp;
					}
					else
					{
						bDataOK = false;
					}
				}
				else
				{
					nDistCount++;
				}
				//Umrechnen von 1/10 in 1/2 Sekunden:
				div_t tDiv = div(nTravelTime,5);
				
				pCpDist->nNCp        = nNCp;
				pCpDist->nBlockWay   = nBlockWay;
				pCpDist->nLength     = nLength;
				pCpDist->nTravelTime = tDiv.quot + ((tDiv.rem > 0)? 1 : 0);
				pCpDist->nProperties = nProperties;
				pCpDist->nDistIndex  = nCount + 1;
				pCpDist++;
			}
		}
		if(!bDataOK)
		{
			//DeleteCommPointDistances();
		}
		else
		{
			pCp->SetNCps(nDistCount);
			SetAllNextDecisionPoints();
		}
	}
	return bDataOK;
}
*/

bool CCourseAGVType::LoadDestinations(CCtrlDataMemFile& myMFile)
{
	int nAnz;
	bool bDataOK = myMFile.Read(nAnz);
	
	m_arrDestinations.SetSize(nAnz);

	if(bDataOK)
	{
		int   nDest,
			    nNCp;
		bool  bNoThoroughfare;
		short nDestTypeLeft,
			    nDestTypeRight;

		for(int nCount = 0;
        		(nCount < nAnz) && bDataOK;
		        nCount++)
		{
			bDataOK =	myMFile.Read(nDest          ) &&
								myMFile.Read(nNCp           ) &&
								myMFile.Read(bNoThoroughfare) &&
								myMFile.Read(nDestTypeLeft  ) &&
								myMFile.Read(nDestTypeRight );
			if(bDataOK)
			{
				unsigned short nDestArea = GetPtrCommPoint(nDest)->GetCpArea();
				CAGVTDestination tDest(nDest,nNCp,bNoThoroughfare,nDestTypeLeft,nDestTypeRight,nDestArea);
				m_arrDestinations[nCount] = tDest;
			}
		}
	}
	if(bDataOK)
	{			
		bDataOK = MakeDestIndexArray();
		SetAllNextDecisionPoints();
	}
	return bDataOK;
}

bool CCourseAGVType::LoadDestinations(CLoaderFile* pLFile)
{
	int nAnz;
	bool bDataOK = pLFile->Read(nAnz);
	
	m_arrDestinations.SetSize(nAnz);

	if(bDataOK)
	{
		int   nDest,
			    nNCp;
		bool  bNoThoroughfare;
		short nDestTypeLeft,
			    nDestTypeRight;
		for(int nCount = 0;
		        (nCount < nAnz) && bDataOK;
	        	nCount++)
		{
			bDataOK = pLFile->Read(nDest          ) &&
								pLFile->Read(nNCp           ) &&
								pLFile->Read(bNoThoroughfare) &&
								pLFile->Read(nDestTypeLeft  ) &&
								pLFile->Read(nDestTypeRight );
			if(bDataOK)
			{
				unsigned short nDestArea = GetPtrCommPoint(nDest)->GetCpArea();
				CAGVTDestination tDest(nDest,nNCp,bNoThoroughfare,nDestTypeLeft,nDestTypeRight,nDestArea);
				m_arrDestinations[nCount] = tDest;
			}
		}
	}
	if(bDataOK)
	{			
		bDataOK = MakeDestIndexArray();
		SetAllNextDecisionPoints();
	}
	return bDataOK;
}

bool CCourseAGVType::LoadDestPositions(CLoaderFile* pLFile)
{
	int nAnz;
	bool bDataOK = pLFile->Read(nAnz);

	m_arrDestPositions.SetSize(nAnz);
	if(bDataOK)
	{
		int		nDest,
				nDestOld = 0;
		char	cSide;
		int		nPos,
				nPosWay;
		for(int nCount = 0;
		(nCount < nAnz) && bDataOK;
		nCount++)
		{
			bDataOK =	pLFile->Read(nDest  ) &&
								pLFile->Read(cSide  ) &&
								pLFile->Read(nPos   ) &&
								pLFile->Read(nPosWay);
			if(bDataOK)
			{
				CAGVTDestPosition tDP(nPos,nPosWay,cSide);
				m_arrDestPositions[nCount] = tDP;
				if(nDestOld != nDest)
				{
					int nDIndex = GetDestIndex(nDest);
					m_arrDestinations[nDIndex - 1].SetFirstPosIndex(nCount + 1);
					nDestOld = nDest;
				}
			}
		}
	}
	if(!bDataOK)
	{
		m_arrDestPositions.RemoveAll();
	}
	return bDataOK;
}

bool CCourseAGVType::LoadDestPositions(CCtrlDataMemFile& myMFile)
{
	int nAnz;
	bool bDataOK = myMFile.Read(nAnz);

	m_arrDestPositions.SetSize(nAnz);
	if(bDataOK)
	{
		int		nDest,
				nDestOld = 0;
		char	cSide;
		int		nPos,
				nPosWay;
		for(int nCount = 0;
		(nCount < nAnz) && bDataOK;
		nCount++)
		{
			bDataOK =	myMFile.Read(nDest  ) &&
								myMFile.Read(cSide  ) &&
								myMFile.Read(nPos   ) &&
								myMFile.Read(nPosWay);
			if(bDataOK)
			{
				CAGVTDestPosition tDP(nPos,nPosWay,cSide);
				m_arrDestPositions[nCount] = tDP;
				if(nDestOld != nDest)
				{
					int nDIndex = GetDestIndex(nDest);
					m_arrDestinations[nDIndex - 1].SetFirstPosIndex(nCount + 1);
					nDestOld = nDest;
				}
			}
		}
	}
	if(!bDataOK)
	{
		m_arrDestPositions.RemoveAll();
	}
	return bDataOK;
}

bool CCourseAGVType::LoadBestWays(CCtrlDataMemFile& myMFile)
{
	return m_BestWays.Load(myMFile);
}

bool CCourseAGVType::LoadBestWays(CLoaderFile* pLFile)
{
	return m_BestWays.Load(pLFile);
}

CCommPoint* CCourseAGVType::GetPtrCommPoint(int nCp)
{
	unsigned int nIndex = m_arrCommPointIndex[nCp];
	return (nIndex == 0)? NULL : &(m_arrCommPoints[nIndex - 1]);
}

CCommPoint* CCourseAGVType::GetPtrCommPointByIndex(unsigned int nIndex)
{
	CCommPoint* pRet = (nIndex < (UINT)m_arrCommPoints.GetSize()) ? &(m_arrCommPoints[nIndex]): NULL;
	return pRet;
}

bool CCourseAGVType::CalculateDispoTravelTimes()
{
	bool bDataOK = true;
	unsigned int nDestinations = m_arrDestinations.GetSize();
	for(int nCpCount = 0;
	nCpCount < m_arrCommPoints.GetSize();
	nCpCount++)
	{
		bDataOK &= m_arrCommPoints[nCpCount].SetSizeArrayTravelTime(nDestinations);
		
		for(unsigned int nDCount = 0;
		nDCount < nDestinations;
		nDCount++)
		{
			int nCp   = m_arrCommPoints  [nCpCount].GetCp();
			int nDest = m_arrDestinations[nDCount ].GetNumber();
			unsigned short nTravelTime = GetBestTravelTime(nCp,nDest);
			m_arrCommPoints[nCpCount].SetTravelTime(nDCount,nTravelTime);
		}
	}
	return bDataOK;
}

unsigned short CCourseAGVType::GetTravelTime(int nCp,int nDest)
{
	unsigned short nTravelTime = USHORT_UNENDLICH;
	unsigned int nCpIndex      = m_arrCommPointIndex[nCp];
	unsigned int nDestIndex    = m_arrDestIndex[nDest];
	if((nCpIndex != 0) && (nDestIndex != 0))
	{
		nTravelTime = m_arrCommPoints[nCpIndex - 1].GetTravelTime(nDestIndex - 1);
	}
	return nTravelTime;
}

void CCourseAGVType::PrintTravelTimes()
{
	for(int nCpCount = 0;
	nCpCount < m_arrCommPoints.GetSize();
	nCpCount++)
	{
			m_arrCommPoints[nCpCount].PrintTravelTimes();
	}
}

void CCourseAGVType::SetSpeeds(int nFWSpeed,int nBWSpeed,int nCFWSpeed,int nCBWSpeed)
{
	m_nFWSpeed  = nFWSpeed;
	m_nBWSpeed  = nBWSpeed;
	m_nCFWSpeed = nCFWSpeed;
	m_nCBWSpeed = nCBWSpeed;
}

void CCourseAGVType::SetRealTravelTimeDistance(unsigned short nDistIndex, unsigned short nRealTravelTime)
{
 	m_arrCpDistances[nDistIndex-1].SetRealTravelTime(nRealTravelTime);
}

void CCourseAGVType::GetExpectedTimeAndLengthTotal(	unsigned short nStartCp,
																										unsigned short nDestCp,
																										int& nTimeTotal,
																										int& nLengthTotal)
{
	nTimeTotal		= 0;
	nLengthTotal	= 0;
	
	int nSteps  = 0;
	int          nTopCp = nStartCp;
	while((nTopCp != nDestCp) && (nSteps < m_arrCommPoints.GetSize()))
	{
		CCpDistance* pDist = GetNextDistPointer(nTopCp, nDestCp);
		if(pDist != NULL)
		{
			nTopCp				= pDist->GetNCp();
			nTimeTotal  	+= pDist->GetTravelTime();
			nLengthTotal	+= pDist->GetLength();
			nSteps++;
		}
		else
		{
			//break while by nSteps:
			nSteps = m_arrCommPoints.GetSize();
		}
	}
	if(nTopCp != nDestCp)
	{
		nTimeTotal		= 0;
		nLengthTotal	= 0;
	}
}
