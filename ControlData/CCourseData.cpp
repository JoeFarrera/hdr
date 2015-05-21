//-----------------------------------------------------
//Implementation file of class CCourseData
//Copyright E&K AUTOMATION, Indumat GmbH & Co. KG
//
//Project: Standard/Development
// 
//Author: J.Lachmund
//
//Changes:
//28.04.2010: Lachmund	:	GetExpectedTimeAndLengthTotal() neu
//19.04.2007: Lachmund	:	SaveAIDStations() neu
//-----------------------------------------------------
/* Include files */
#include "..\stdafx.h"
#include  <stdlib.h>
#include  <string.h>
#include  <stdio.h>
#include  <malloc.h>
#include  <direct.h>
#include  <io.h>

#include "konstant.h"
#include "komdat.h"

#include "CCourseData.h"
#include "..\tpc.h"
#include "..\msgMacro.h"



//the one and only topical course:
CCourseData* pTheCourse = NULL;

CCourseData::CCourseData()
{
	m_nAGVs                		= 0;
	m_nChargeStations      		= 0;
	m_nMaxHomeAndChargeStations	= 0;
	m_nDestPositions            = 0;
	m_nMaxManBlockArea          = 0;
	
	m_pBlockTables		   		= NULL;
	//m_pAIDCommPoints	  		= NULL;
}

CCourseData::~CCourseData()
{
	if(m_pBlockTables)
	{
		DeleteBlockTables();
	}
}

bool CCourseData::InitAGVTypeCourses()
{
	m_apCourseAGVType = (CCourseAGVType**)malloc(m_nMaxAGVType*sizeof(CCourseAGVType*));
	bool bDataOK = (m_apCourseAGVType != NULL);
	if(bDataOK)
	{
		unsigned int nMask = 1;
		for(int nCount = 0;
		(nCount < m_nMaxAGVType) && bDataOK;
		nCount++,nMask <<= 1)
		{
			*(m_apCourseAGVType+nCount) = NULL;
			if(m_nAGVTypes & nMask)
			{
				//Speicher für FzTyp-Kurs:
				*(m_apCourseAGVType+nCount) = new CCourseAGVType(nCount+1);
				bDataOK &= (*(m_apCourseAGVType+nCount) != NULL);
			}
		}
	}
	return bDataOK;
}

bool CCourseData::NeedsBlockTables()
{
	return (m_nAGVs > 1);
}

CCourseAGVType* CCourseData::GetPtrCourseAGVType(unsigned int nAGVType)
{
	CCourseAGVType* pRet = NULL;
	if((nAGVType > 0) && (nAGVType <= (unsigned int)m_nMaxAGVType))
	{
		pRet = m_apCourseAGVType[nAGVType-1];
	}
	return pRet;
}

unsigned int CCourseData::GetNumberAIDStations()
{
	return m_arrAIDStations.GetSize();
}

CAIDStation* CCourseData::GetPtrAIDStation(unsigned int nAIDSIndex)
{
	bool bValidIndex = ((nAIDSIndex >0) && (nAIDSIndex <= (unsigned int)m_arrAIDStations.GetSize()));
	CAIDStation* pRet =  bValidIndex ? &(m_arrAIDStations[nAIDSIndex - 1]) : NULL;
	return pRet;
}

int CCourseData::CheckGenString()
{
	int nErr = 0;
	CString strVersion = m_strGenString;
	// Längenprüfung:
	int nLenOCS = strVersion.GetLength();
	if (nLenOCS > 49)
	{
		int nLenProjekt;
		string strLen = strVersion.Mid(37,2);
		// TODO: Test - sscanf->sscanf_s
		int i = sscanf_s(strLen.c_str(),"%2X",&nLenProjekt);
		if(i == 1)
		{
			if(nLenOCS > (49 + nLenProjekt))
			{
				int nLenUser;
				strLen = strVersion.Mid(39 + nLenProjekt,2);
				i = sscanf_s(strLen.c_str(),"%2X",&nLenUser);
				if(i == 1)
				{
					//Userlänge ermittelt:
					if(nLenOCS == (49 + nLenProjekt + nLenUser))
					{
						//Länge ist in Ordnung!
						string strSoFTSys = strVersion.Mid(0,13);
						if(strSoFTSys == "$$$SoFTSys$$$")
						{
							nLenOCS = strVersion.GetLength() - 8; // Länge ohne Checksumme
							CString strCheckSum = strVersion.Mid(nLenOCS,8);
							int nCheckSum;
							i = sscanf_s(strCheckSum,"%8X",&nCheckSum);
							if (i == 1)
							{
								// Checksumme prüfen:
								CString strData = strVersion.Mid(0,nLenOCS);
								int nCS = GetCheckSum(strData);
								if(nCS == nCheckSum)
								{
									//Datum und Zeit ermitteln:
									string strRandom = strVersion.Mid(29,8);
									string strDatum  = strVersion.Mid(13,8);
									string strZeit   = strVersion.Mid(21,8);
									int nRandom,
										nDatum,
										nZeit;
									i = sscanf_s(strRandom.c_str(),"%8X",&nRandom) +
										sscanf_s(strDatum .c_str(),"%8X",&nDatum ) +
										sscanf_s(strZeit  .c_str(),"%8X",&nZeit  );
									if(i == 3)
									{
										nDatum    += nRandom;
										nZeit     -= nRandom;

										div_t dt   = div(nDatum,10000);
										int nJahr  = dt.quot;
										int nTag   = div(dt.rem,100).quot;
										int nMonat = div(dt.rem,100).rem;

										dt           = div(nZeit,10000);
										int nStunde  = dt.quot;
										int nMinute  = div(dt.rem,100).quot;
										int nSekunde = div(dt.rem,100).rem;
										
										CString strHexProjekt = strVersion.Mid(39,nLenProjekt);
										CString strHexUser    = strVersion.Mid(39 + nLenProjekt + 2, nLenUser);

										m_strProject = MakeHexToString(strHexProjekt);
										m_strUser    = MakeHexToString(strHexUser   );

										//CTime tT (nJahr,nMonat,nTag,nStunde,nMinute,nSekunde);
										char szDZ[50];
										sprintf_s(szDZ,"%02d.%02d.%d, %02d:%02d:%02d",nTag,nMonat,nJahr,nStunde,nMinute,nSekunde);// = tT.Format("%d.%m.%Y, %H:%M:%S");
										m_strDateTime = CString(szDZ);
									}
									else
									{
										nErr = 9; // Datum/Zeit manipuliert!
									}
								}
								else
								{
									nErr = 8;	//Checksumme fasch!
								}
							}
							else
							{
								nErr = 7;	//Keine Checksumme
							}
						}
						else
						{
							nErr = 6;	// Keine SoFTSys Kennung
						}
					}
					else
					{
						nErr = 5;	//Länge für User zu lang
					}
				}
				else
				{
					nErr = 4;	// Keine Länge für User
				}
			}
			else
			{
				nErr = 3;	//Länge für Projekt zu lang
			}
		}
		else
		{
			nErr = 2;  //Keine Länge für Projekt
		}
	}
	else
	{
		nErr = 1;	// absolut zu kurz
	}
	return nErr;		
}

int CCourseData::GetCheckSum(CString& strData)
{
	int nCheckSum = 0;

	int nLen = strData.GetLength();

	for(int i = 0;
	i < nLen;
	i++)
	{
		nCheckSum += strData[i];
	}

	return nCheckSum;
}

CString CCourseData::MakeHexToString(CString& strHex)
{
	CString str;
	CString strHexChar;
	int     i,
		    nChar;
	int     nLen = strHex.GetLength();
	nLen = div(nLen,2).quot;
	for(int nCount = 0;
	nCount < nLen;
	nCount++)
	{
		strHexChar = strHex.Mid(nCount*2,2);
		i = sscanf_s(strHexChar,"%02X",&nChar);
		if(isascii(nChar))
		{
			str += (char)nChar;
		}

	}
	return str;
}

bool CCourseData::CheckAGVDistances(const int nAGV,const int nCp, const int nNCp,const int nNNCp)
{
	bool bRet = false;
	CCourseAGVType* pCAGVT = pTheCourse->m_apCourseAGVType[fzdat[nAGV].fztyp - 1];
    if (pCAGVT)
	{
		bRet = pCAGVT->CheckAGVDistances(nCp,nNCp,nNNCp);
	}		
	return bRet;
}

bool CCourseData::IsValidAGVDestination(const int nAGV,const int nDest)
{
	bool bRet = false;
	CCourseAGVType* pCAGVT = pTheCourse->m_apCourseAGVType[fzdat[nAGV].fztyp - 1];
    if (pCAGVT)
	{
		bRet = pCAGVT->IsValidDestination(nDest);
	}		
	return bRet;
}

bool CCourseData::IsValidStationAction(TYPE_STATION_DEF& tStation,int nAction,unsigned int& nAGVTypes)
{
	bool bRet = false;
	int anActions[6] = {PICKUP,UNLOAD,HOME,BATTCHARGE,PSEUDO,INTERIM};
	nAGVTypes = 0xffffffff;
	CAIDStation tAIDS = GetAIDStation(tStation.nDest,tStation.ucSide,tStation.nPos,tStation.nLayer);
	if(tAIDS.GetPSName().GetLength() > 0)
	{
		//SChleife über alle Aktionen:
		for(int nACount = 0;
		nACount < 6;
		nACount++)
		{
			if(nAction & anActions[nACount])
			{
				nAGVTypes &= tAIDS.GetAGVTypesOfAction(anActions[nACount]);
			}
		}
		bRet = (nAGVTypes != 0);
	}
	return bRet;
}
bool CCourseData::FillAIDCommPoints(CUIntArray& arrPCp)
{
	bool bRet = true;
	m_arrAIDCommPoints.SetSize(m_nAIDCommPoints);
	if(bRet)
	{
		for(int nCount = 0;
		nCount < m_nAIDCommPoints;
		nCount++)
		{
			CAIDCommPoint tAIDCp(arrPCp[nCount],nCount+1);
			SetCPAndDestAGVType(tAIDCp);
			m_arrAIDCommPoints[nCount] = tAIDCp;
		}	
	}
	return bRet;
}

void CCourseData::SetCPAndDestAGVType(CAIDCommPoint& tAIDCp)
{
	unsigned short nMaske = 1;
	
	unsigned int nAGVTypes     = 0,
				 nAGVTypesDest = 0;
	int          nCp = tAIDCp.GetCp();			 
	for(int nCount = 0;
	nCount < m_nMaxAGVType;
	nCount++,nMaske <<= 1)
	{
		CCourseAGVType* pCAGVT = m_apCourseAGVType[nCount];
		if(pCAGVT != NULL)
		{
			nAGVTypes     |= !pCAGVT->CpExists(nCp)           ? 0 : nMaske;
			nAGVTypesDest |= (pCAGVT->GetDestIndex(nCp) == 0) ? 0 : nMaske;
		}
	}
	tAIDCp.SetAGVTypes    (nAGVTypes    );
	tAIDCp.SetAGVTypesDest(nAGVTypesDest);
}

bool CCourseData::FillAIDCpDistances(CSimpleDistance* pSDist)
{
	bool bRet = true;
	m_arrAIDCpDistances.SetSize(m_nAIDCpDistances);

		int nOldCp = 0;
		int nNCps  = 0;
		unsigned int nCpCount = 0;
		for(int nCount = 0;
		nCount < m_nAIDCpDistances;
		nCount++)
		{
			if(nOldCp != pSDist->m_nCp)
			{
				if(nOldCp != 0)
				{
					m_arrAIDCommPoints[nCpCount - 1].SetNCps(nNCps);
				}
				m_arrAIDCommPoints[nCpCount].SetNCps(nNCps);
				m_arrAIDCommPoints[nCpCount].SetDistIndex(nCount);
				nOldCp = pSDist->m_nCp;
				nCpCount++;
				nNCps  = 0;
			}
			nNCps++;
			CAIDCpDistance tAIDCpD(pSDist->m_nNCp,nCount,0);
			m_arrAIDCpDistances[nCount] = tAIDCpD;

			pSDist++;
		}

		m_arrAIDCommPoints[nCpCount - 1].SetNCps(nNCps);	
	return bRet;
}

bool CCourseData::MakeAIDCpIndexArray()
{
	bool bRet = true;
	int nMaxAIDCp = m_arrAIDCommPoints[m_nAIDCommPoints - 1].GetCp();
	m_arrAIDCpIndex.SetSize(nMaxAIDCp + 1);
	int nCount = 0;	
	for(int nCpCount = 0;
		nCpCount <= nMaxAIDCp;
		nCpCount++)
	{
		m_arrAIDCpIndex[nCpCount] = (nCpCount == m_arrAIDCommPoints[nCount].GetCp()) ? (nCount + 1) : 0;
		nCount += (m_arrAIDCpIndex[nCpCount] == 0) ? 0 : 1; 
	}

	return bRet;
}

bool CCourseData::MakeManBlockAreaIndexArray()
{
	bool bRet = true;
	m_arrMBAIndex.RemoveAll();
	int nMBAs   = m_arrManBlockAreas.GetSize();
	if(nMBAs > 0)
	{
		//We have one or more ManBlockAreas:
		int nMaxMBA = m_arrManBlockAreas[nMBAs - 1].GetMBArea();
		m_arrMBAIndex.SetSize(nMaxMBA + 1);
			int nMBACount = 0, nExistCount = 0;
			
			m_arrMBAIndex[0] = 0;
			for(int nCount = 1;
        			nCount <= nMaxMBA;
			        nCount++)
			{
				nExistCount += (nCount == m_arrManBlockAreas[nMBACount].GetMBArea()) ? 1 : 0; 
				m_arrMBAIndex[nCount] =  (nCount == m_arrManBlockAreas[nMBACount].GetMBArea()) ? nExistCount : 0;
				nMBACount += (m_arrMBAIndex[nCount] == 0) ? 0 : 1; 
			}
			// Grösste Knoten-Nr eintragen
			m_nMaxManBlockArea = nMaxMBA;
	}
	return bRet;
}

unsigned short CCourseData::CheckAIDCpIsDest(unsigned short nCp)
{
	unsigned short nRet = 0;
	if (IsCpValid(nCp))
	{
		nRet = m_arrAIDCommPoints[m_arrAIDCpIndex[nCp]].GetAGVTypesDest();
	}
	return nRet;
}


bool CCourseData::ConvertCustomerToStation(char* pCustomerName, TYPE_STATION_DEF& tStation )
{
	//ACHTUNG: diese funktion ist so nicht richtig, da mehrere Stationen
	//möglich sind:
	//int  nCustomerStationNames = m_listCustomerStations.size();
	bool bRet = false;
	CString strCName(pCustomerName);
	CCustomerStation tCS(0,' ',0,0,strCName);

	POSITION pos = m_listCustomerStations.Find (tCS);
	if(pos != NULL)
	{
		tCS = m_listCustomerStations.GetAt(pos);
		tStation.nDest  = tCS.GetDestNr();
		tStation.nLayer = tCS.GetLayer();
		tStation.nPos   = tCS.GetPos();
		tStation.ucSide = tCS.GetSide();
		bRet = true;
	}
	/*
	for (int nCount = 0;
	     (nCount < nCustomerStationNames) && (bRet == false);
	     nCount++)
	{
		if (strcmp(pCustomerName, m_pCustomerStationName[nCount].pCustomerName) == 0)
		{
			tStation = m_pCustomerStationName[nCount].tStationDef;
			bRet = true;
		}
	}
	*/
	return bRet;
}

bool CCourseData::ConvertStationToCustomerName(char* pCustomerName, TYPE_STATION_DEF& tStation )
{
	bool bRet = false;
	
	CString	strName;
	CCustomerStation tCS(tStation.nDest,tStation.ucSide,
			(unsigned char)tStation.nPos,tStation.nLayer,strName);
	POSITION pos = m_listCustomerStations.Find(tCS);

	if(pos != NULL)
	{
		tCS = m_listCustomerStations.GetAt(pos);
		CString name = tCS.GetCName();
		strcpy_s(pCustomerName, name.GetLength(), name);
		bRet = true;
	}
	return bRet;
}

unsigned short CCourseData::GetAIDDistOccAGV(unsigned short nCp, unsigned short nNCp)
{
	unsigned short nAGV = 0;
	if (IsCpValid(nCp))
	{
		CAIDCpDistance* pDist = GetAIDDistance(nCp,nNCp);
		unsigned int nCpIndex = m_arrAIDCpIndex[nCp];
		
		if(pDist || nCpIndex)
		{
			nAGV = pDist ? pDist->GetOccAGV() : m_arrAIDCommPoints[nCpIndex-1].GetOccAGV();
		}
	}
	return nAGV;
}

bool CCourseData::Occupy(unsigned short nAGV,unsigned short& nOccAGV,bool bOccupy)
{
	bool bRet = false;
	bool bChangeOccAGV = bOccupy ? (nOccAGV == 0): (nOccAGV == nAGV);

	nOccAGV = bChangeOccAGV ? (bOccupy ? nAGV : 0) : nOccAGV;
	bRet = bChangeOccAGV;
	
	return bRet;
}

bool CCourseData::ChangeAIDDistOccAGV(unsigned short nAGV, unsigned short nCp, unsigned short nNCp, bool bOccupy)
{
	bool bRet = false;
	
	unsigned int nCpIndex = m_arrAIDCpIndex[nCp];
	CAIDCpDistance* pDist = GetAIDDistance(nCp,nNCp);

	if(pDist || nCpIndex)
	{
		unsigned short nOccAGV = pDist ? pDist->GetOccAGV() :  m_arrAIDCommPoints[nCpIndex-1].GetOccAGV();
		bRet =  Occupy(nAGV,nOccAGV,bOccupy);
		if(bRet)
		{
			if(pDist != NULL)
			{
				pDist->SetOccAGV(nOccAGV);
				/*
				if(bOccupy)
				{
					printf("FTF%d belegt M%04dM%04d.\n",(int)nAGV,(int)nCp,(int)nNCp);
				}
				else
				{
					printf("FTF%d gibt M%04dM%04d frei.\n",(int)nAGV,(int)nCp,(int)nNCp);
				}
				*/
			}
			else
			{
				m_arrAIDCommPoints[nCpIndex-1].SetOccAGV(nOccAGV);
				/*
				if(bOccupy)
				{
					printf("FTF%d belegt Cp%04d.\n",(int)nAGV,(int)nCp);
				}
				else
				{
					printf("FTF%d gibt Cp%04d frei.\n",(int)nAGV,(int)nCp);
				}
				*/
			}
		}
	}
	return bRet;
}


unsigned short CCourseData::GetAIDDistStopOccAGV(unsigned short nCp)
{
	unsigned short nAGV = 0;
	if (IsCpValid(nCp))
	{
		nAGV = m_arrAIDCommPoints[m_arrAIDCpIndex[nCp]-1].GetOccStopAGV();
	}
	return nAGV;
}

bool CCourseData::ChangeAIDDistStopOccAGV(unsigned short nCp, bool bChange)
{
	bool bRet = false;
	if (IsCpValid(nCp))
	{
		m_arrAIDCommPoints[m_arrAIDCpIndex[nCp]-1].SetOccStopAGV(bChange ? 0xF0 : 0);
		bRet = true; 
	}
	return bRet;
}

bool CCourseData::IsCpValid(const unsigned short nCp)
{
	return (((nCp > 0) && (nCp < m_arrAIDCpIndex.GetSize())) && (m_arrAIDCpIndex[nCp] != NULL));
}

CAIDCpDistance* CCourseData::GetAIDDistance(unsigned short nCp, unsigned short nNCp)
{
	CAIDCpDistance* pRet = NULL;
	bool bCpValid  = IsCpValid( nCp);
	bool bNCpValid = IsCpValid(nNCp);
	if(bCpValid && bNCpValid)
	{
		unsigned int nCpIndex   = m_arrAIDCpIndex[nCp] - 1;
		unsigned int nDistIndex = m_arrAIDCommPoints[nCpIndex].GetDistIndex();
		for (int nCount = 0;
		     (nCount < m_arrAIDCommPoints[nCpIndex].GetNCps()) && (pRet == NULL);
		     nCount++,nDistIndex++)
		{
			pRet = (m_arrAIDCpDistances[nDistIndex].GetNCp() == nNCp) ? &(m_arrAIDCpDistances[nDistIndex]): NULL;
		}
	} 
	return pRet;
}

int CCourseData::GetAIDDistanceIndex(unsigned short nCp, unsigned short nNCp)
{
	int nRet = 0;
	bool bCpValid  = IsCpValid( nCp);
	bool bNCpValid = IsCpValid(nNCp);
	if(bCpValid && bNCpValid)
	{
		unsigned int nCpIndex   = m_arrAIDCpIndex[nCp] - 1;
		unsigned int nDistIndex = m_arrAIDCommPoints[nCpIndex].GetDistIndex();
		for (int nCount = 0;
		     (nCount < m_arrAIDCommPoints[nCpIndex].GetNCps()) && (nRet == 0);
		     nCount++,nDistIndex++)
		{
			nRet = (m_arrAIDCpDistances[nDistIndex].GetNCp() == nNCp) ? m_arrAIDCpDistances[nDistIndex].GetDistIndex(): 0;
		}
	} 
	return nRet;
}

bool CCourseData::IsMBAreaValid(const unsigned short nMBArea)
{
	return (((nMBArea > 0) && (nMBArea <= m_nMaxManBlockArea)) && (m_arrMBAIndex[nMBArea] != 0));
}

unsigned short CCourseData::GetManBlockAreaOccAGV(unsigned short nMBArea)
{
	//unsigned short nAGV = 0xFFFF;
	unsigned short nAGV = 0;
	if(IsMBAreaValid(nMBArea))
	{
		nAGV = m_arrManBlockAreas[m_arrMBAIndex[nMBArea] - 1].GetOccAGV();
	}
	return nAGV;
}

bool CCourseData::ChangeManBlockAreaOccAGV(unsigned short nAGV, unsigned short nMBArea, bool bOccupy)
{
	bool bRet = false;
	if (IsMBAreaValid(nMBArea))
	{
		int nMBAIndex             = m_arrMBAIndex[nMBArea] - 1;
		unsigned short nOccAGVOld = m_arrManBlockAreas[nMBAIndex].GetOccAGV();
		bool bChangeOccAGV        = bOccupy ? (nOccAGVOld == 0) : (nOccAGVOld == nAGV);
		unsigned short nOccAGVNew = bOccupy ? nAGV : 0;
		bRet                      = bChangeOccAGV;
		m_arrManBlockAreas[nMBAIndex].SetOccAGV( bChangeOccAGV ? nOccAGVNew : nOccAGVOld);
	}
	return bRet;
}

bool CCourseData::IsDistanceInMBArea(unsigned short nMBArea, unsigned short nCp, unsigned short nNCp)
{
	bool bRet = false;

	if (IsMBAreaValid(nMBArea))
	{
		int nIndex = m_arrMBAIndex[nMBArea]-1;
		bRet = m_arrManBlockAreas[nIndex].HasDistance(nCp,nNCp,m_arrManBlockAreaObjects);
	}
	return bRet;
}

bool CCourseData::SetAGVInManBlockArea(unsigned short nAGV, unsigned short nCp, unsigned short nNCp)
{
	bool bRet = false;
	
    CAIDCpDistance* pDist = GetAIDDistance(nCp, nNCp);
    
	if ((pDist != NULL) || (m_arrAIDCpIndex[nCp] != 0))
	{
		for (int nCount = 0;
		     nCount < m_arrManBlockAreas.GetSize();// m_nNumberManBlockArea;
		     nCount++)
		{
			if (IsDistanceInMBArea(m_arrManBlockAreas[nCount].GetMBArea(), nCp, nNCp))
			{
				bRet |= ChangeManBlockAreaOccAGV(nAGV, m_arrManBlockAreas[nCount].GetMBArea(), true);
			}
		}
	}
	return bRet;
}

void CCourseData::ResetAGVInManBlockArea(unsigned short nAGV)
{
    bool bRet = false;
	for (int nCount = 0;
	     nCount < m_arrManBlockAreas.GetSize();
	     nCount++)
	{
		bRet = ChangeManBlockAreaOccAGV(nAGV, m_arrManBlockAreas[nCount].GetMBArea(), false);
	}
}

int CCourseData::GetManBlockAreaOfDistance(unsigned short nCp, unsigned short nNCp)
{
	int nBlockArea = 0;
	bool bRet = false;
	CAIDCpDistance* pDist = NULL;

	pDist = GetAIDDistance(nCp, nNCp);

	if ((pDist != NULL) || (m_arrAIDCpIndex[nCp] != 0))
	{
		for (int nCount = 0;
			(nCount < m_arrManBlockAreas.GetSize()) && !bRet;
			nCount++)
		{
			if (IsDistanceInMBArea(m_arrManBlockAreas[nCount].GetMBArea(), nCp, nNCp))
			{
				nBlockArea = m_arrManBlockAreas[nCount].GetMBArea();
				break;
			}
		}
	}

	return nBlockArea;
}

unsigned short CCourseData::GetAGVInManBlockArea(unsigned short nCp, unsigned short nNCp)
{
  unsigned short nAGV = 0;
  bool bRet = false;
  CAIDCpDistance* pDist = NULL;
    
	pDist = GetAIDDistance(nCp, nNCp);
    
	if ((pDist != NULL) ||(m_arrAIDCpIndex[nCp] != 0))
	{
		for (int nCount = 0;
		     (nCount < m_arrManBlockAreas.GetSize()) && !bRet;
		     nCount++)
		{
			if (IsDistanceInMBArea(m_arrManBlockAreas[nCount].GetMBArea(), nCp, nNCp))
			{
				nAGV = m_arrManBlockAreas[nCount].GetOccAGV();
				break;
			}
		}
	}	
	return nAGV;
}

bool CCourseData::IsStationLoaded(TYPE_STATION_DEF& tStation)
{
	bool bRet = false;
	CAIDStation tAIDS = GetAIDStation(tStation.nDest,
									  tStation.ucSide,
									  tStation.nPos,
									  tStation.nLayer);
	bRet = tAIDS.IsLoaded();
	return bRet;
}

bool CCourseData::IsStationLoaded(CString& strCSN)
{	
	bool bRet = false;
	int nPSIndex = GetAIDStationIndex(strCSN);
	if (nPSIndex)
	{	
		CAIDStation tAIDS = m_arrAIDStations[nPSIndex - 1];

		bRet = tAIDS.IsLoaded();
	}
	return bRet;
}

bool CCourseData::IsStationClosed(TYPE_STATION_DEF& tStation)
{
	bool bRet = false;
	CAIDStation tAIDS = GetAIDStation(tStation.nDest,
									  tStation.ucSide,
									  tStation.nPos,
									  tStation.nLayer);
	bRet = tAIDS.IsClosed();
	return bRet;
}

bool CCourseData::IsStationClosed(CString& strCSN)
{	
	bool bRet = false;
	int nPSIndex = GetAIDStationIndex(strCSN);
	if (nPSIndex)
	{	
		CAIDStation tAIDS = m_arrAIDStations[nPSIndex - 1];

		bRet = tAIDS.IsClosed();
	}
	return bRet;
}


bool CCourseData::IsStationError(TYPE_STATION_DEF& tStation)
{
	bool bRet = false;
	CAIDStation tAIDS = GetAIDStation(tStation.nDest,
									  tStation.ucSide,
									  tStation.nPos,
									  tStation.nLayer);
	bRet = tAIDS.IsError();
	return bRet;
}


bool CCourseData::IsStationError(CString& strCSN)
{	
	bool bRet = false;
	int nPSIndex = GetAIDStationIndex(strCSN);
	if (nPSIndex)
	{	
		CAIDStation tAIDS = m_arrAIDStations[nPSIndex - 1];

		bRet = tAIDS.IsError();
	}
	return bRet;
}

bool CCourseData::IsStationReserved(TYPE_STATION_DEF& tStation)
{
	bool bRet = false;
	CAIDStation tAIDS = GetAIDStation(tStation.nDest,
									  tStation.ucSide,
									  tStation.nPos,
									  tStation.nLayer);
	bRet = tAIDS.IsReserved();
	return bRet;
}


bool CCourseData::IsStationReserved(CString& strCSN)
{	
	bool bRet = false;
	int nPSIndex = GetAIDStationIndex(strCSN);
	if (nPSIndex)
	{	
		CAIDStation tAIDS = m_arrAIDStations[nPSIndex - 1];

		bRet = tAIDS.IsReserved();
	}
	return bRet;
}

void CCourseData::MakeHomeOrChargeStationLists()
{
    CStringList listHomeStations;
    CStringList listChargeStations;
	m_nMaxHomeAndChargeStations = 0;

    for (int nCount = 0;
         nCount < m_nMaxAGVType;
         nCount++)
    {
    	CCourseAGVType* pCAGVT = m_apCourseAGVType[nCount];
    	if(pCAGVT != NULL)
    	{
    		 unsigned int nHOCS = pCAGVT->MakeHomeOrChargeStationList(m_arrAIDStations,
    		                                                          listHomeStations,
    		                                                          listChargeStations);
    		 m_nMaxHomeAndChargeStations = (nHOCS > m_nMaxHomeAndChargeStations)? nHOCS:m_nMaxHomeAndChargeStations;
    	}
    }
	m_nChargeStations = listChargeStations.GetCount();
	m_nHomeStations   = listHomeStations.GetCount();
}

void CCourseData::WriteManBlockAreaPointerVectorForTest(FILE* fp)
{
	fprintf(fp,"ManBlockAreas: %d\n\n",m_nMaxManBlockArea);
	for(int nCount = 0;
	nCount < m_nMaxManBlockArea;
	nCount++)
	{
		fprintf(fp,"Area %d: %p\n",
				nCount,
				m_arrMBAIndex[nCount]);
	}
}

void CCourseData::WriteManBlockAreaForTest(FILE* fp)
{
	fprintf(fp,"\nManualBlockAreas: %d\n\n",m_arrManBlockAreas.GetSize());
	for(int nCount = 0;
	nCount < m_arrManBlockAreas.GetSize();
	nCount++)
	{
		fprintf(fp,"%d.Area %d, OccAGV:%hd, Objects:%d, AbsObjIndex:%d\n",
				nCount+1,
				m_arrManBlockAreas[nCount].GetMBArea(),
				(int)m_arrManBlockAreas[nCount].GetOccAGV(),
				m_arrManBlockAreas[nCount].GetObjNumber(),
				m_arrManBlockAreas[nCount].GetObjIndex());
		int nLimit = m_arrManBlockAreas[nCount].GetObjNumber();
		int nIndex = m_arrManBlockAreas[nCount].GetObjIndex();
		for(int nOCount = 0;
		nOCount < nLimit;
		nOCount++)
		{
				if(m_arrManBlockAreaObjects[nIndex+nOCount].GetNCp() != 0)
				{
					//A distance:
					fprintf(fp,"            %d.Distance: Cp%04dCp%04d\n",
							nOCount+1,
							m_arrManBlockAreaObjects[nIndex+nOCount].GetCp(),
							m_arrManBlockAreaObjects[nIndex+nOCount].GetNCp());
				}
				else
				{
					//A CommPoint:
					fprintf(fp,"            %d.CommPoint: Cp%04d\n",
							nOCount+1,
							m_arrManBlockAreaObjects[nIndex+nOCount].GetCp());
				}
		}
	}
}

void CCourseData::WriteBlockTablesForTest(FILE* fp)
{
	if(NeedsBlockTables())
	{
		int nNumberBlockTables = m_nMaxAGVType * m_nMaxAGVType;
		fprintf(fp,"\nBlockTables: %d\n",nNumberBlockTables);
		DEF_BLOCK_TABLE* pBT = m_pBlockTables;
		for(int nCount = 0;
		nCount < nNumberBlockTables;
		nCount++)
		{
			int nAllCrashCount = 0;
			fprintf(fp,"\n%d.BlockTable: TopAGVT %d/CompAGVT %d, %d distances with %d crashs\n",
					nCount +1,
					pBT->nTopAGVT,
					pBT->nCompAGVT,
					pBT->nDistNumber,
					pBT->pDistIndexVector[pBT->nDistNumber]);
			int nDistNumber = pBT->nDistNumber;	
			for(int nDCount = 0;
			nDCount < nDistNumber;
			nDCount++)
			{
				int nCrash = pBT->pDistIndexVector[nDCount+1] - pBT->pDistIndexVector[nDCount];
				fprintf(fp,"\n%d.Distance has %d crash distances:\n",nDCount+1,nCrash);
				DEF_DISTANCE_CRASH* pCrash = pBT->pDistCrash;
				for(int nCCount = 0;
				nCCount < nCrash;
				nCCount++)
				{
					fprintf(fp,"%d.Crash (%d.): FreeWay:%d, AbsKollWay:%d, RelKollWay:%d, RelFreeWay:%d, KollWayBlkWay:%d\n",
							++nAllCrashCount,
							nCCount+1,
							pCrash->nIndex,
							pCrash->nFreeWay,
							pCrash->nAbsKollWay,
							pCrash->nRelKollWay,
							pCrash->nRelFreeWay,
							pCrash->nKollWayBlkWay);
							pCrash++;
				}
			}
			pBT++;
		}
		fprintf(fp,"\n---------End of BlockTables------------\n");
	}	
}

void CCourseData::WriteDestTypeForTest(FILE* fp)
{
	fprintf(fp,"\nDestTypes: %d\n\n",m_arrDestTypes.GetSize());
	for(int nCount = 0;
	nCount < m_arrDestTypes.GetSize();
	nCount++)
	{
		m_arrDestTypes[nCount].WriteForTest(fp,nCount+1);
	}
	
	POSITION pos = m_listDestTypePos.GetHeadPosition();

	int nAbsPCount = 0;
	fprintf(fp,"\nDestTypePositions: \n\n");
	for(int nCount = 0;
	nCount < m_arrDestTypes.GetSize();
	nCount++)
	{
		fprintf(fp,"DestType %d\n",
				m_arrDestTypes[nCount].GetDestType());
		for(int nRelPCount = 0;
		(nRelPCount < m_arrDestTypes[nCount].GetPositions()) && (pos != NULL);
		nRelPCount++)
		{
			CDestTypePos tDTP = m_listDestTypePos.GetNext(pos);
			tDTP.WriteForTest(fp,++nAbsPCount);
		}
	}		
}

void CCourseData::WriteColumnTypeForTest(FILE* fp)
{
	fprintf(fp,"\nColumnTypes: %d\n\n",m_arrColumnTypes.GetSize());
	for (int nCount = 0;
	nCount < m_arrColumnTypes.GetSize();
	nCount++)
	{
		m_arrColumnTypes[nCount].WriteForTest(fp,nCount+1);
	}
}

void CCourseData::WriteLayerTypesForTest(FILE* fp)
{
	fprintf(fp,"\nLayerTypes: %d\n\n",m_arrLayerTypes.GetSize());
	for(int nCount = 0;
	nCount < m_arrLayerTypes.GetSize();
	nCount++)
	{
		m_arrLayerTypes[nCount].WriteForTest(fp,nCount+1);
	}
}

void CCourseData::WriteCustomerStationNameForTest(FILE* fp)
{
	fprintf(fp,"\nCustomerStationNames: %d\n\n",m_listCustomerStations.GetCount());
	POSITION pos = m_listCustomerStations.GetHeadPosition();

	int nCount = 1;
	while(pos != NULL)
	{
		CCustomerStation tCS = m_listCustomerStations.GetNext(pos);
		fprintf(fp,"%d. Destination %d, Side: %c, Pos: %hd, Layer: %d, Name:'%s'\n",
				nCount++,
				tCS.GetDestNr(),
				tCS.GetSide(),
				tCS.GetPos(),
				tCS.GetLayer(),
				tCS.GetCName());
		
	}
}

bool CCourseData::MakeAIDStationArray()
{
	bool bRet = true;
	CAIDStationList listAIDStations;

	POSITION pos = m_listCustomerStations.GetHeadPosition();
	
	while(pos != NULL)
	{
		CCustomerStation tCS = m_listCustomerStations.GetNext(pos);

		int nPos   = tCS.GetPos  ();
		int nLayer = tCS.GetLayer();
		unsigned char nPSType = (nPos == 0) ? PS_DEST:((nLayer == 0) ? PS_POS : PS_LAYER);
		CString strName = tCS.GetCName();
		CAIDStation tAIDS(strName,nPSType);
		POSITION posAIDStation = listAIDStations.Find(tAIDS);
		if(posAIDStation == NULL)
		{
			//AIDStation ist noch nicht vorhanden
			listAIDStations.AddTail(tAIDS);
		}
	}
	MakeAIDStationArray(listAIDStations);
	return bRet;
}

//-------------------------------------------------------------------
// Diese Funktion veranlasst, dass in den Fahrzeugtyp-Kursen jede 
// Station einen Verweis (Index) in das Array der Fahrzeugtypunabhängigen
// Stationen (m_listAIDStations) erhält. 
//-------------------------------------------------------------------
bool CCourseData::LinkAGVTypeStationsToAIDStations()
{
	bool bRet = true;
	
    for (int nCount = 0;
         nCount < m_nMaxAGVType;
         nCount++)
    {
  		CCourseAGVType *pCAGVT = m_apCourseAGVType[nCount];
    	if(pCAGVT)
    	{
    		pCAGVT->SetDestPositionsInDestinations(m_arrDestTypeIndex,m_arrDestTypes);
 			pCAGVT->FillLayersInDestPositions(m_arrDestTypeIndex,m_arrDestTypes,
 											  m_arrColumnTypeIndex,m_arrColumnTypes,
 											  m_listDestTypePos);
    		pCAGVT->MakeLayerArray();
    		bRet &= pCAGVT->LinkDestPosToAIDStations(m_arrAIDStations,m_listCustomerStations);
    	}
    }
	return bRet;
}

bool CCourseData::MakeAIDStationArray(CAIDStationList& listAIDStations)
{
	int nAIDStations = listAIDStations.GetCount();
	bool bRet = true;
	m_arrAIDStations.SetSize(nAIDStations);
	CAIDStation tAIDS;
	POSITION pos = listAIDStations.GetHeadPosition();

	for(int nPSCount = 0;
		nPSCount < nAIDStations;
		nPSCount++)
	{
		tAIDS = listAIDStations.GetNext(pos);
		m_arrAIDStations[nPSCount] = tAIDS;
	}

	return bRet;
}

//-------------------------------------------------------------------
//GetAGVTypeStationList() seeks for AIDStation with the name strCSN 
//all AGVType stations. You find these AGVType stations in the list
//listAGVTStations.
//-------------------------------------------------------------------
void CCourseData::GetAGVTypeStationList(CString& strCSN,CAGVTypeStationList& listAGVTStations)
{
	unsigned int nAIDPSIndex = GetAIDStationIndex(strCSN);
	if(nAIDPSIndex > 0)
	{
		for(int nCount = 0;
		nCount < m_nMaxAGVType;
		nCount++)
		{
			if(m_apCourseAGVType[nCount] != NULL)
			{
				m_apCourseAGVType[nCount]->GetAGVTypeStationList(nAIDPSIndex,listAGVTStations);
			}
		}
	}
}

CAIDStation CCourseData::GetAIDStation(int nAGVT, int nDest, char cSide, int nPos, int nLayer)
{
	CAIDStation tAIDS;
	int  nError = 0;
	unsigned int nPSIndex = GetAIDStationIndex(nAGVT,nDest,cSide,nPos,nLayer,nError);
	if(nPSIndex)
	{
		tAIDS = m_arrAIDStations[nPSIndex-1];
	}
	return tAIDS;
}
// Michael M2015 Test if it really is a customer station
bool CCourseData::IsAIDStation (int nDest, char cSide, int nPos, int nLayer)
{
	CString strDummy;
	CCustomerStation tCS(nDest,cSide,nPos,nLayer,strDummy);
	POSITION pos = m_listCustomerStations.Find(tCS);
	if(pos != NULL)
	{
		return true;
	}
	else
		return false;

}

// Michael M2015 Pointer to station
CAIDStation * CCourseData::GetAIDStationPtr(int nDest, char cSide, int nPos, int nLayer)
{
	CString strDummy;
	CCustomerStation tCS(nDest,cSide,nPos,nLayer,strDummy);
	POSITION pos = m_listCustomerStations.Find(tCS);
	if(pos != NULL)
	{
		tCS = m_listCustomerStations.GetAt(pos);
		unsigned int nPSIndex = tCS.FindAIDStation(m_arrAIDStations);
		return (&m_arrAIDStations[nPSIndex - 1]);
	}
	return NULL;
}


CAIDStation CCourseData::GetAIDStation(int nDest, char cSide, int nPos, int nLayer)
{
	CAIDStation tAIDS;
	CString strDummy;
	CCustomerStation tCS(nDest,cSide,nPos,nLayer,strDummy);
	POSITION pos = m_listCustomerStations.Find(tCS);
	if(pos != NULL)
	{
		tCS = m_listCustomerStations.GetAt(pos);
		unsigned int nPSIndex = tCS.FindAIDStation(m_arrAIDStations);
		tAIDS = m_arrAIDStations[nPSIndex - 1];
	}
	return tAIDS;
}

unsigned int CCourseData::GetAIDStationIndex(int nAGVT, int nDest, char cSide, int nPos, int nLayer,int& nError)
{
	unsigned int nRet = 0;
	if(m_apCourseAGVType[nAGVT-1] != NULL)
	{
		nRet = m_apCourseAGVType[nAGVT-1]->GetAIDStationIndex(nDest,cSide,nPos,nLayer,nError);
	}
	return nRet;
}

unsigned int CCourseData::GetAIDStationIndex(CString& strCSN)
{
	unsigned int nRet = 0;
	int nLimit = m_arrAIDStations.GetSize();
	for(int nPSCount = 0;
	(nPSCount < nLimit) && (nRet == 0);
	nPSCount++)
	{
		nRet = (strCSN == m_arrAIDStations[nPSCount].GetPSName()) ? (nPSCount+1):0;
	}
	return nRet;
}

//-------------------------------------------------------------------
// Diese Funktion ermittelt Eigenschaften der AID Stationen und schreibt
// diese in die Membervariablen ein:
//		m_nAGVTypesPickup - welche Fahrzeugtypen an dieser Station holen
//      m_nAGVTypesUnload - welche Fahrzeugtypen an dieser Station bringen
//		m_nAGVTypesHomeStation - welche Fahrzeugtypen haben hier Endhalt
//      m_nAGVTypesBattCharge  - welche Fahrzeugtypen haben hier Ladestation
//      m_nAGVTypesPseudoStation - welche Fahrzeugtypen nutzen die Station als Pseudoziel
//      m_nAGVTypeInterimStation - welche Fahrzeugtypen nutzen diese Station als Zwischenziel
//-------------------------------------------------------------------
bool CCourseData::SetAGVTypesActionsInAIDStations()
{
	CAGVTypeStationList listAllAGVTypeStations;
	unsigned int nAGVTypeStationIndex = 1;
	bool bRet = true;
	int nLimit = m_arrAIDStations.GetSize();
	for(int nPSCount = 0;
	nPSCount < nLimit;
	nPSCount++)
	{
		CString strPSName = m_arrAIDStations[nPSCount].GetPSName();
		CAGVTypeStationList listAGVTStations;
		GetAGVTypeStationList(strPSName,listAGVTStations);
		
		POSITION pos = listAGVTStations.GetHeadPosition();

		while(pos != NULL)
		{
			CAGVTypeStation station = listAGVTStations.GetNext(pos);
			listAllAGVTypeStations.AddTail (station);
		}
		
		unsigned int nStations = listAGVTStations.GetCount();
		unsigned int nIndex    = (nStations > 0) ? nAGVTypeStationIndex : 0;
		nAGVTypeStationIndex  += nStations;

		unsigned int nAGVTypesPickup         = GetAGVTypesOfAction(PICKUP    ,listAGVTStations);
		unsigned int nAGVTypesUnload         = GetAGVTypesOfAction(UNLOAD    ,listAGVTStations);
		unsigned int nAGVTypesHomeStation    = GetAGVTypesOfAction(HOME      ,listAGVTStations);
		unsigned int nAGVTypesBattCharge     = GetAGVTypesOfAction(BATTCHARGE,listAGVTStations);
		unsigned int nAGVTypesPseudoStation  = GetAGVTypesOfAction(PSEUDO    ,listAGVTStations);
		unsigned int nAGVTypesInterimStation = GetAGVTypesOfAction(INTERIM   ,listAGVTStations);
		m_arrAIDStations[nPSCount].SetAGVTypesPickup        (nAGVTypesPickup        );
		m_arrAIDStations[nPSCount].SetAGVTypesUnload        (nAGVTypesUnload        );
		m_arrAIDStations[nPSCount].SetAGVTypesHomeStation   (nAGVTypesHomeStation   );
		m_arrAIDStations[nPSCount].SetAGVTypesBattCharge    (nAGVTypesBattCharge    );
		m_arrAIDStations[nPSCount].SetAGVTypesPseudoStation (nAGVTypesPseudoStation );
		m_arrAIDStations[nPSCount].SetAGVTypesInterimStation(nAGVTypesInterimStation);
		m_arrAIDStations[nPSCount].SetAGVTypeStations       (nStations);
		m_arrAIDStations[nPSCount].SetAGVTypeStationIndex   (nIndex);
	}
	
	int nIndex = 0;
	m_arrAGVTypeStations.SetSize(listAllAGVTypeStations.GetCount());
	POSITION pos = listAllAGVTypeStations.GetHeadPosition();

	while(pos != NULL)
	{
		CAGVTypeStation station = listAllAGVTypeStations.GetNext(pos);
		
		m_arrAGVTypeStations[nIndex] = station;
		nIndex++;
	}
	
	return bRet;
}

unsigned int CCourseData::GetAGVTypesOfAction(int nAction,CAGVTypeStationList &listAGVTStation)
{
	unsigned int nRet = 0;
	POSITION pos = listAGVTStation.GetHeadPosition();

	while(pos != NULL)
	{
		CAGVTypeStation tAGVTDP = listAGVTStation.GetNext(pos);
		unsigned int nAGVT = tAGVTDP.GetAGVType() - 1;
		CCourseAGVType* pCAGVT = m_apCourseAGVType[nAGVT];
		TYPE_STATION_DEF tStation = tAGVTDP.GetStation();
		bool bIsAction = pCAGVT->IsActionOfStation(nAction,
												   tStation,
												   m_arrDestTypeIndex,
												   m_arrDestTypes,
												   m_listDestTypePos,
												   m_arrColumnTypeIndex,
												   m_arrColumnTypes,
												   m_arrLayerTypeIndex,
												   m_arrLayerTypes);
		nRet |= bIsAction ? ( 1 << nAGVT) : 0;
	}
	return nRet;
}

void CCourseData::SetAllObjectsInAIDStations()
{
	int nLimit = m_arrAIDStations.GetSize();
	for(int nPSCount = 0;
	nPSCount < nLimit;
	nPSCount++)
	{
		CString strPSName = m_arrAIDStations[nPSCount].GetPSName();
		m_arrAIDStations[nPSCount].SetAllObjects(GetAllStationObjects(strPSName));
	}
}

unsigned int CCourseData::GetAllStationObjects(CString &strPSName)
{
	unsigned int nRet = 0;
	unsigned int nAIDPSIndex = GetAIDStationIndex(strPSName);
	if(nAIDPSIndex > 0)
	{
		for(int nCount = 0;
		nCount < m_nMaxAGVType;
		nCount++)
		{
			if(m_apCourseAGVType[nCount] != 0)
			{
				nRet |= m_apCourseAGVType[nCount]->GetAllStationObjects(nAIDPSIndex);
			}
		}
	}
	return nRet;
}

void CCourseData::GetPSIndexListWithChildren(CString& strPSName,CChangedStationObjList& listChangedStations)
{
	unsigned int nAIDPSIndex = GetAIDStationIndex(strPSName);
	if(nAIDPSIndex > 0)
	{
		for(int nCount = 0;
		nCount < m_nMaxAGVType;
		nCount++)
		{
			if(m_apCourseAGVType[nCount] != 0)
			{
				m_apCourseAGVType[nCount]->GetPSIndexListWithChildren(nAIDPSIndex,listChangedStations);
			}
		}
	}
}

//void CCourseData::SendStationListToAllBuBs(CChangedStationObjList& listChangeStations)
//{
//	CChangedStationObjList::iterator itCSO = listChangeStations.begin();
//	CChangedStationObjList::iterator itEnd = listChangeStations.end  ();
//	while(itCSO != itEnd)
//	{
//		CChangedStationObj tCSO = *itCSO;
//	  	komp_aend_dat.pAIDStation[tCSO.GetPSIndex()-1] = 1;
//	    itCSO++;
//	}
//}

// TODO ML - Possibly not required
//void CCourseData::ChangeStation(unsigned short nCommand, string strPSName)
//{
//	unsigned char nPrioTime = 0; 
//	bool bInfoToOrderManager = false;             /* Merker fuer Auftragsverwaltung */
//	CChangedStationObjList listChangeStations;
//
//	pTheCourse->ChangeAIDStation(strPSName, nCommand, nPrioTime, listChangeStations);
//
//	if(listChangeStations.size() > 0)  
//	{
//	  	bInfoToOrderManager = ((nCommand == TK_RESERVIEREN      ) ||
//	  	                       (nCommand == TK_NICHT_RESERVIEREN) ||
//	  	                       (nCommand == TK_SPERREN          ) ||
//	  	                       (nCommand == TK_FREIGEBEN        ));
//	  
//		//Syslog veranlassen:
//		SyslogFromStationList(listChangeStations,nCommand);
//		
//		if(bInfoToOrderManager)
//		{
//			// Send Mail to OrderManager:
//			SendMailStationChangedToOrderManager(strPSName,nCommand);
//		}
//		
//    // Inform all BuB's:
//    SendStationListToAllBuBs(listChangeStations);
//
//    //für jeden Listeneintrag eine Mail an Datensicherung:
//    SaveAIDStationList(listChangeStations);
//	}
//}


//void CCourseData::ChangeStation(TYPE_STATION_DEF& tStation,
//								unsigned short    nCommand,
//								bool              bSendToBuBs,
//								bool              bDataSave)
//{
//	unsigned char nPrioTime = 0; 
//	
//	CAIDStation tAIDS = GetAIDStation(tStation.nDest,
//									  tStation.ucSide,
//									  tStation.nPos,
//									  tStation.nLayer);
//									 
//	CChangedStationObjList listChangedStations;
//	
//	string strPSName(tAIDS.GetPSName());
//	ChangeAIDStation(strPSName,nCommand,nPrioTime,listChangedStations);
//	
//	if(bSendToBuBs)
//	{
//		SendStationListToAllBuBs(listChangedStations);
//	}
//	if(bDataSave)
//	{
//		//für jeden Listeneintrag eine Mail an Datensicherung:
//		SaveAIDStationList(listChangedStations);
//	}
//}

void CCourseData::ChangeAIDStation(CString& strPSName, unsigned short nCommand, unsigned int nPrioTime, CChangedStationObjList& listChangedStations)
{
	GetPSIndexListWithChildren(strPSName,listChangedStations);

	//für Test:
	unsigned int nAIDS = listChangedStations.GetCount();
	nAIDS = nAIDS;
	
	CChangedStationObj tCSO;

	POSITION pos = listChangedStations.GetHeadPosition();
	while(pos != NULL)
	{
		tCSO = listChangedStations.GetNext(pos);

		unsigned int nPSIndex = tCSO.GetPSIndex();
		switch(nCommand)
		{
			case TK_SPERREN:
			{
				m_arrAIDStations[nPSIndex - 1].SetClosed(true,tCSO.GetChangeObjects());
				break;
			}
			case TK_FREIGEBEN:
			{
				m_arrAIDStations[nPSIndex - 1].SetClosed(false,tCSO.GetChangeObjects());
				break;
			}
			case TK_BELEGEN:
			{
				m_arrAIDStations[nPSIndex - 1].SetLoaded(true,tCSO.GetChangeObjects());
				break;
			}
			case TK_NICHT_BELEGEN:
			{
				m_arrAIDStations[nPSIndex - 1].SetLoaded(false,tCSO.GetChangeObjects());
				break;
			}
			case TK_RESERVIEREN:
			{
				m_arrAIDStations[nPSIndex - 1].SetReserved(true,tCSO.GetChangeObjects());
				break;
			}
			case TK_NICHT_RESERVIEREN:
			{
				m_arrAIDStations[nPSIndex - 1].SetReserved(false,tCSO.GetChangeObjects());
				break;
			}
			case TK_ERROR:
			{
				m_arrAIDStations[nPSIndex - 1].SetError(true,tCSO.GetChangeObjects());
				break;
			}
			case TK_NO_ERROR:
			{
				m_arrAIDStations[nPSIndex - 1].SetError(false,tCSO.GetChangeObjects());
				break;
			}			
			case TK_BASE_PRIORITY:
			{
				m_arrAIDStations[nPSIndex - 1].SetPriorityTime(nPrioTime);
				break;
			}			
		}
	}
}

//Ermitten der Summe aller Cps aus Kursen aller Fahrzeugtypen
int CCourseData::GetMaxPossibleCps()
{
	int nRet = 0;
	for(int nCount = 0;
	nCount < m_nMaxAGVType;
	nCount++)
	{
		if(m_apCourseAGVType[nCount] != NULL)
		{
			nRet += m_apCourseAGVType[nCount]->GetNumberCps();
		}
	}
	return nRet;
}

//Mehrfache Cp entfernen:
int CCourseData::RemoveDoubleCps(int* pCps)
{
	int nRet      = m_nAIDCommPoints;
	int nOldLimit = m_nAIDCommPoints;
	int nCount1,
		nCount2,
		nFirst = 0;
	for(nCount1 = 0;
	nCount1 < nOldLimit - 1;
	nCount1++)
	{
		for(nCount2 = nCount1 + 1;
		nCount2 < nOldLimit;
		nCount2++)
		{
			if(*(pCps + nCount1)!= -1)
			{
				if(*(pCps + nCount1) == *(pCps + nCount2))
				{
					//Cp entfernen:
					if(nFirst == 0)
					{
						nFirst = nCount2;
					}
					*(pCps + nCount2) = -1;
					nRet--;
				}
			}
		}
	}
	//wir haben noch nRet Cps im Array mit -1 inmitten:
	if(nFirst > 0)
	{
		// -1 nach hinten:
		nCount1 = nFirst;
		for(nCount2 = nCount1 + 1;
		nCount2 < nOldLimit;
		nCount2++)
		{
			if (*(pCps + nCount2) != -1)
			{
				*(pCps + nCount1) = *(pCps + nCount2);
				*(pCps + nCount2) = -1;
				nCount1++;
			}
		}
	}
	return nRet;
}

//Mehrfache Cp entfernen:
int CCourseData::RemoveDoubleCps(CUIntArray& arrCps)
{
	int nRet      = m_nAIDCommPoints;
	int nOldLimit = m_nAIDCommPoints;
	int nCount1,
		nCount2,
		nFirst = 0;
	for(nCount1 = 0;
	nCount1 < nOldLimit - 1;
	nCount1++)
	{
		for(nCount2 = nCount1 + 1;
		nCount2 < nOldLimit;
		nCount2++)
		{
			if(arrCps[nCount1] != -1)
			{
				if(arrCps[nCount1] == arrCps[nCount2])
				{
					//Cp entfernen:
					if(nFirst == 0)
					{
						nFirst = nCount2;
					}
					arrCps[nCount2] = -1;
					nRet--;
				}
			}
		}
	}
	//wir haben noch nRet Cps im Array mit -1 inmitten:
	if(nFirst > 0)
	{
		// -1 nach hinten:
		nCount1 = nFirst;
		for(nCount2 = nCount1 + 1;
		nCount2 < nOldLimit;
		nCount2++)
		{
			if (arrCps[nCount2] != -1)
			{
				arrCps[nCount1] = arrCps[nCount2];
				arrCps[nCount2] = -1;
				nCount1++;
			}
		}
	}
	return nRet;
}

// sortiert die Integer in Feld pInt, das nLimit Integer enthält:
void CCourseData::SortIntArray(int* pInt,int nLimit)
{
	int nCount1,
		nCount2;
	for(nCount1 = 0;
	nCount1 < (nLimit - 1);
	nCount1++)
	{
		for(nCount2 = nCount1 + 1;
		nCount2 < nLimit;
		nCount2++)
		{
			if(*(pInt + nCount1) > *(pInt + nCount2))
			{
				int nInt = *(pInt + nCount1);
				*(pInt + nCount1) = *(pInt + nCount2);
				*(pInt + nCount2)  = nInt;
			}
		}
	}
}

// sortiert die Integer in Feld pInt, das nLimit Integer enthält:
void CCourseData::SortIntArray(CUIntArray& arrInt,int nLimit)
{
	int nCount1,
		nCount2;
	for(nCount1 = 0;
	nCount1 < (nLimit - 1);
	nCount1++)
	{
		for(nCount2 = nCount1 + 1;
		nCount2 < nLimit;
		nCount2++)
		{
			if(arrInt[nCount1] > arrInt[nCount2])
			{
				int nInt        = arrInt[nCount1];
				arrInt[nCount1] = arrInt[nCount2];
				arrInt[nCount2] = nInt;
			}
		}
	}
}

// Erstellen der FzTyp-unabhängigen Meldepunkttabelle:
bool CCourseData::MakeAIDCommPoints()
{
	int nPossibleCps = GetMaxPossibleCps();
	CUIntArray arrPCp;
	bool bRet = true;
	arrPCp.SetSize(nPossibleCps + 1);

	m_nAIDCommPoints = 0;
	for(int nCount = 0;
		nCount < m_nMaxAGVType;
		nCount++)
	{
		if(m_apCourseAGVType[nCount] != NULL)
		{
			CCourseAGVType* pCAGVT = m_apCourseAGVType[nCount];
			m_nAGVs          += pCAGVT->GetAGVs();
			//AGV Type exists:
			for(unsigned int nCpCount = 0;
				nCpCount < pCAGVT->GetNumberCps();
				nCpCount++)
			{
				arrPCp[m_nAIDCommPoints] = pCAGVT->GetPtrCommPointByIndex(nCpCount)->GetCp();
				m_nAIDCommPoints++;
			}
		}
	}
	m_nAIDCommPoints = RemoveDoubleCps(arrPCp);
	//Cps sortieren:
	SortIntArray(arrPCp,m_nAIDCommPoints);
	//Eindeutige Cps füllen:
	FillAIDCommPoints(arrPCp);

	return bRet;
}

//Ermitten der Summe aller Strecken aus Kursen aller Fahrzeugtypen
int CCourseData::GetMaxPossibleDists()
{
	int nRet = 0;
	for(int nCount = 0;
	nCount < m_nMaxAGVType;
	nCount++)
	{
		if(m_apCourseAGVType[nCount] != NULL)
		{
			nRet += m_apCourseAGVType[nCount]->GetNumberDistances() + 1;
		}
	}
	return nRet;
}

void CCourseData::MakeListSimpleDists(CSimpleDistance* pSDists)
{
	CSimpleDistance* pSD = pSDists;

	for(int nCount = 0;
	nCount < m_nMaxAGVType;
	nCount++)
	{
		if(m_apCourseAGVType[nCount] != NULL)
		{
			CCourseAGVType* pCAGVT = m_apCourseAGVType[nCount];
			//AGV Type exists:
			for(unsigned int nCpCount = 0;
			nCpCount < pCAGVT->GetNumberCps();
			nCpCount++)
			{
				CCommPoint* pCp = pCAGVT->GetPtrCommPointByIndex(nCpCount);
				int nNCps = pCp->GetNCps();
				unsigned int nDistanceIndex = pCp->GetDistanceIndex();
				//Loop over all NCps of Cp:
				//DEF_CP_DISTANCE* pDist = pCAGVT->m_pCpDistances + nDistanceIndex -1;
				CCpDistance* pDist = pCAGVT->GetPtrDistance(nDistanceIndex);
				for(int nNCpCount = 0;
				nNCpCount < nNCps;
				nNCpCount++,pDist++)
				{ 
					pSD->m_nCp  = pCp->GetCp();
					pSD->m_nNCp = pDist->GetNCp();
					pSD++;
				}
			}
		}
	}
}

int CCourseData::RemoveDoubleDists(CSimpleDistance* pSDists,int nSDists)
{
	int nDists    = nSDists;
	int nOldLimit = nSDists;
	int nCount1,
		nCount2,
		nFirstNull = 0;
	CSimpleDistance tSDNull(-1,-1);
	for(nCount1 = 0;
	nCount1 < nOldLimit -1;
	nCount1++)
	{
		for(nCount2 = nCount1+1;
		nCount2 < nOldLimit;
		nCount2++)
		{
			if(!(*(pSDists + nCount1) == tSDNull))
			{
				if(*(pSDists + nCount1) == *(pSDists + nCount2))
				{
					//SDist entfernen:
					if(nFirstNull == 0)
					{
						nFirstNull = nCount2;
					}
					*(pSDists + nCount2) = tSDNull;
					nDists--;
				}
			}
		}
	}
	//wir haben noch nDists SDist's im Array mit Null inmitten:
	if(nFirstNull > 0)
	{
		//Nullen nach hinten:
		nCount1 = nFirstNull;
		for(nCount2 = nCount1 + 1;
		nCount2 < nOldLimit;
		nCount2++)
		{
			if(!(*(pSDists + nCount2)== tSDNull))
			{
				*(pSDists + nCount1) = *(pSDists + nCount2);
				*(pSDists + nCount2) = tSDNull;
				nCount1++;
			}
		}
	}
	return nDists;
}

void CCourseData::SortSimpleDists(CSimpleDistance* pSDist)
{
	int nLimit = m_nAIDCpDistances;
	int nCount1,
		nCount2;
	for(nCount1 = 0;
	nCount1 < (nLimit - 1);
	nCount1++)
	{
		for(nCount2 = nCount1 + 1;
		nCount2 < nLimit;
		nCount2++)
		{
			if(*(pSDist + nCount1) > *(pSDist + nCount2))
			{
				CSimpleDistance tSD = *(pSDist + nCount1);
				*(pSDist + nCount1) = *(pSDist + nCount2);
				*(pSDist + nCount2)  = tSD;
			}
		}
	}
}

bool CCourseData::MakeAIDCpDistances()
{
	int nPossibleDists = GetMaxPossibleDists();
	CSimpleDistance* pSDist = new CSimpleDistance[nPossibleDists];
	bool bRet = (pSDist != NULL);
	if(bRet)
	{
		//Alle Strecken in eine Liste:
		MakeListSimpleDists(pSDist);
		
		//Mehrfache Strecken entfernen:
		m_nAIDCpDistances = RemoveDoubleDists(pSDist,nPossibleDists);
		
		//Strecken sortieren:
		SortSimpleDists(pSDist);
		
		//Liste der CAIDCpDistance Strecken erstellen:
		bRet = FillAIDCpDistances(pSDist);
		
		delete [] pSDist;
	}
	return bRet;
}

bool CCourseData::PrepareAIDCpsAndDistances()
{
	bool bDataOK  = MakeAIDCommPoints();
	bDataOK      &= MakeAIDCpIndexArray();
	bDataOK      &= MakeAIDCpDistances();
	
	return bDataOK;
}

bool CCourseData::Prepare(CString& strWorkDirectory)
{
	bool bDataOK = true;
	
	//Array of AID-Stations:
	bDataOK &= MakeAIDStationArray             ();
	MSGAVISO("----AIDStationArray is ready.\n");
	bDataOK &= LinkAGVTypeStationsToAIDStations();
	MSGAVISO("----AGVTypeStations are linked to AIDStations.\n");
	
	bDataOK &= SetAGVTypesActionsInAIDStations ();
	MSGAVISO("----AIDStations have AGVTypes for all actions.\n");
	if(bDataOK)
	{
		SetAllObjectsInAIDStations ();
		MSGAVISO("----AIDStations have all objects.\n");
	}
	
	// Tabelle für Endhalte und Ladestationen erstellen
	MakeHomeOrChargeStationLists();
	MSGAVISO("----Now we have HomeOrChargeStationLists.\n");

/*	man braucht die Ausgabe nicht mehr unbedingt; kostet bei Colordruck nur Zeit 15.04.08 Ri
	if(bDataOK)
	{
		WriteForTest(pstrWorkDirectory);
		printf("----Course is written for test.\n");
	}
*/
	
	return bDataOK;
}

bool CCourseData::LoadLayerTypes(CCtrlDataMemFile& myMFile)
{
	bool bRet = true;
	int nAnz;
	bRet = myMFile.Read(nAnz);

	m_arrLayerTypes.SetSize(nAnz);

	if (bRet)
	{	
		unsigned short usType;
		int            nDestProperties;
		int            nDepth,
					   nHeight,
					   nMaxPieces,
					   nStartPrio,
					   nUserInfo;
		CString         strUserInfo;
		CLayerType tLT;
		int nCount;
		for(nCount = 0;
		(nCount < nAnz) && bRet;
		nCount++)
		{
			bRet =	myMFile.Read(usType         ) &&
							myMFile.Read(nHeight        ) &&
							myMFile.Read(nDepth         ) &&
							myMFile.Read(nMaxPieces     ) &&
							myMFile.Read(nStartPrio     ) &&
							myMFile.Read(nDestProperties) &&
							myMFile.Read(nUserInfo      );
			if(bRet)
			{
				strUserInfo = myMFile.ReadCompactString();
				tLT = CLayerType(usType,nHeight,nDepth,nMaxPieces,nStartPrio,nDestProperties,nUserInfo,strUserInfo);
				m_arrLayerTypes[nCount] = tLT;
			}
		}
		if(bRet)
		{
			//IndexArray bilden:
			int nMaxType = tLT.m_nType;
			m_arrLayerTypeIndex.SetSize(nMaxType+1);
			int nIndex = 0;
			m_arrLayerTypeIndex[0] = nIndex;
			for(nCount = 1;
			nCount <= nMaxType;
			nCount++)
			{
				if(nCount == m_arrLayerTypes[nIndex].m_nType)
				{
					nIndex++;
					m_arrLayerTypeIndex[nCount] = nIndex;
				}
				else
				{
					m_arrLayerTypeIndex[nCount] = 0;
				}
			}
		}
	}
	return bRet;
}

bool CCourseData::LoadLayerTypes(CLoaderFile* pLFile)
{
	bool bRet = true;
	int nAnz;
	bRet = pLFile->Read(nAnz);

	m_arrLayerTypes.SetSize(nAnz);
	if(bRet)
	{	
		unsigned short usType;
		int            nDestProperties;
		int            nDepth,
					   nHeight,
					   nMaxPieces,
					   nStartPrio,
					   nUserInfo;
		CString         strUserInfo;
		CLayerType tLT;
		int nCount;
		for(nCount = 0;
		(nCount < nAnz) && bRet;
		nCount++)
		{
			bRet =	pLFile->Read(usType         ) &&
					pLFile->Read(nHeight        ) &&
					pLFile->Read(nDepth         ) &&
					pLFile->Read(nMaxPieces     ) &&
					pLFile->Read(nStartPrio     ) &&
					pLFile->Read(nDestProperties) &&
					pLFile->Read(nUserInfo      );
			if(bRet)
			{
				bRet = pLFile->ReadCompactString(strUserInfo);
				if(bRet)
				{
					tLT = CLayerType(usType,nHeight,nDepth,nMaxPieces,nStartPrio,nDestProperties,nUserInfo,strUserInfo);
					m_arrLayerTypes[nCount] = tLT;
				}
			}
		}
		if(bRet)
		{
			//IndexArray bilden:
			int nMaxType = tLT.m_nType;
			m_arrLayerTypeIndex.SetSize(nMaxType+1);
			int nIndex = 0;
			m_arrLayerTypeIndex[0] = nIndex;
			for(nCount = 1;
			nCount <= nMaxType;
			nCount++)
			{
				if(nCount == m_arrLayerTypes[nIndex].m_nType)
				{
					nIndex++;
					m_arrLayerTypeIndex[nCount] = nIndex;
				}
				else
				{
					m_arrLayerTypeIndex[nCount] = 0;
				}
			}
		}
	}
	return bRet;
}

bool CCourseData::LoadColumnTypes(CCtrlDataMemFile& myMFile)
{
	bool bRet = true;
	int nAnz;
	bRet = myMFile.Read(nAnz);
	if(nAnz > 0)
	{
		m_arrColumnTypes.SetSize(nAnz);
		if(bRet)
		{
			CColumnType tCT;
			unsigned short usType;
			unsigned short anET[10];
			int nCount;
			for(nCount = 0;
			(nCount < nAnz) && bRet;
			nCount++)
			{
				bRet =	myMFile.Read(usType    ) &&
								myMFile.Read(anET[0]   ) &&
								myMFile.Read(anET[1]   ) &&
								myMFile.Read(anET[2]   ) &&
								myMFile.Read(anET[3]   ) &&
								myMFile.Read(anET[4]   ) &&
								myMFile.Read(anET[5]   ) &&
								myMFile.Read(anET[6]   ) &&
								myMFile.Read(anET[7]   ) &&
								myMFile.Read(anET[8]   ) &&
								myMFile.Read(anET[9]   );
				if(bRet)
				{
					tCT = CColumnType(usType,anET);
					m_arrColumnTypes[nCount] = tCT;
				}
			}
			if(bRet)
			{
				// Index-Array erstellen:
				int nMaxType = tCT.GetType();
				m_arrColumnTypeIndex.SetSize(nMaxType+1);
				int nIndex = 0;
				m_arrColumnTypeIndex[0] = nIndex;
				for(nCount = 1;
				nCount <= nMaxType;
				nCount++)
				{
					if(nCount == m_arrColumnTypes[nIndex].GetType())
					{
						nIndex++;
						m_arrColumnTypeIndex[nCount] = nIndex;
					}
					else
					{
						m_arrColumnTypeIndex[nCount] = 0;
					}
				}
			}
		}
	}
	return bRet;
}

bool CCourseData::LoadColumnTypes(CLoaderFile* pLFile)
{
	bool bRet = true;
	int nAnz;
	bRet = pLFile->Read(nAnz);
	if(nAnz > 0)
	{
		m_arrColumnTypes.SetSize(nAnz);
		if(bRet)
		{
			CColumnType tCT;
			unsigned short usType;
			unsigned short anET[10];
			int nCount;
			for(nCount = 0;
			(nCount < nAnz) && bRet;
			nCount++)
			{
				bRet =	pLFile->Read(usType    ) &&
						pLFile->Read(anET[0]   ) &&
						pLFile->Read(anET[1]   ) &&
						pLFile->Read(anET[2]   ) &&
						pLFile->Read(anET[3]   ) &&
						pLFile->Read(anET[4]   ) &&
						pLFile->Read(anET[5]   ) &&
						pLFile->Read(anET[6]   ) &&
						pLFile->Read(anET[7]   ) &&
						pLFile->Read(anET[8]   ) &&
						pLFile->Read(anET[9]   );
				if(bRet)
				{
					tCT = CColumnType(usType,anET);
					m_arrColumnTypes[nCount] = tCT;
				}
			}
			if(bRet)
			{
				// Index-Array erstellen:
				int nMaxType = tCT.GetType();
				m_arrColumnTypeIndex.SetSize(nMaxType+1);
				int nIndex = 0;
				m_arrColumnTypeIndex[0] = nIndex;
				for(nCount = 1;
				nCount <= nMaxType;
				nCount++)
				{
					if(nCount == m_arrColumnTypes[nIndex].GetType())
					{
						nIndex++;
						m_arrColumnTypeIndex[nCount] = nIndex;
					}
					else
					{
						m_arrColumnTypeIndex[nCount] = 0;
					}
				}
			}
		}
	}
	return bRet;
}

bool CCourseData::LoadDestinationTypes(CCtrlDataMemFile& myMFile)
{
	bool bRet = true;
	int nAnz;
	bRet = myMFile.Read(nAnz);
	if(bRet)
	{
		m_arrDestTypes.SetSize(nAnz + 1);
		if(bRet)
		{
			//Zieltypen lesen:	
			for(int nCount = 0;
			(nCount <= nAnz) && bRet;
			nCount++)
			{
				unsigned short nDT;
				unsigned short nIndex;
				bRet =	myMFile.Read(nDT   ) &&
								myMFile.Read(nIndex);
				if(bRet)
				{
					CDestinationType tDT(nDT,nIndex,0);
					m_arrDestTypes[nCount]= tDT;
				}
			}
		}
		if(bRet)
		{
			// Pos-Anzahl eintragen
			int nCountPos = 0;
			for(int nCount = 0;
			nCount < nAnz;
			nCount++)
			{
				int nPositions = m_arrDestTypes[nCount+1].GetPosIndex() - m_arrDestTypes[nCount].GetPosIndex();
				m_arrDestTypes[nCount].SetPositions(nPositions);
				nCountPos += nPositions;
			}
			
			// Positionen lesen:
			m_listDestTypePos.RemoveAll();
			unsigned char		nMedium,
											cLayerColumn;
			unsigned short	nType,
											nDestRoutine;
			int							nPosWay;
			CDestTypePos		tDTP;
			
			for(int nCount = 1;
			(nCount <= nCountPos) && bRet;
			nCount++)
			{
				bRet =	myMFile.Read(nMedium     ) &&
								myMFile.Read(cLayerColumn) &&
								myMFile.Read(nType       ) &&
								myMFile.Read(nPosWay     ) &&
								myMFile.Read(nDestRoutine);
				if(bRet)
				{
					tDTP = CDestTypePos(nMedium,cLayerColumn,nType,nPosWay,nDestRoutine);
					m_listDestTypePos.AddTail(tDTP);
				}
			}
		}
		if(bRet)
		{
			//Verweis-Vektor aufbauen:
			int nMaxDT = m_arrDestTypes[nAnz - 1].GetDestType() + 1;
			m_arrDestTypeIndex.RemoveAll();
			m_arrDestTypeIndex.SetSize(nMaxDT); 
			if(bRet)
			{
				for(int nCount = 0;
				nCount < nMaxDT;
				nCount++)
				{
					m_arrDestTypeIndex[nCount] = 0;
				}
				for(int nCount = 0;
				nCount < nAnz;
				nCount++)
				{
					int nType = m_arrDestTypes[nCount].GetDestType();
					m_arrDestTypeIndex[nType] = nCount + 1;
				}
			}
		}
	}
	return bRet;
}

bool CCourseData::LoadDestinationTypes(CLoaderFile* pLFile)
{
	bool bRet = true;
	int nAnz;
	bRet = pLFile->Read(nAnz);
	if(bRet)
	{
		m_arrDestTypes.SetSize(nAnz);
		if(bRet)
		{
			//Zieltypen lesen:	
			for(int nCount = 0;
			(nCount <= nAnz) && bRet;
			nCount++)
			{
				unsigned short nDT;
				unsigned short nIndex;
				bRet = 	pLFile->Read(nDT   ) &&
						pLFile->Read(nIndex);
				if(bRet)
				{
					CDestinationType tDT(nDT,nIndex,0);
					m_arrDestTypes[nCount]= tDT;
				}
			}
		}
		if(bRet)
		{
			// Pos-Anzahl eintragen
			int nCountPos = 0;
			for(int nCount = 0;
			nCount < nAnz;
			nCount++)
			{
				int nPositions = m_arrDestTypes[nCount+1].GetPosIndex() - m_arrDestTypes[nCount].GetPosIndex();
				m_arrDestTypes[nCount].SetPositions(nPositions);
				nCountPos += nPositions;
			}
			
			// Positionen lesen:
			m_listDestTypePos.RemoveAll();
			unsigned char 	nMedium,
							cLayerColumn;
			unsigned short	nType,
							nDestRoutine;
			int				nPosWay;
			CDestTypePos tDTP;
			
			for(int nCount = 1;
			(nCount <= nCountPos) && bRet;
			nCount++)
			{
				bRet =	pLFile->Read(nMedium     ) &&
						pLFile->Read(cLayerColumn) &&
						pLFile->Read(nType       ) &&
						pLFile->Read(nPosWay     ) &&
						pLFile->Read(nDestRoutine);
				if(bRet)
				{
					tDTP = CDestTypePos(nMedium,cLayerColumn,nType,nPosWay,nDestRoutine);
					m_listDestTypePos.AddTail(tDTP);
				}
			}
		}
		if(bRet)
		{
			//Verweis-Vektor aufbauen:
			int nMaxDT = m_arrDestTypes[nAnz - 1].GetDestType() + 1;
			m_arrDestTypeIndex.RemoveAll();
			m_arrDestTypeIndex.SetSize(nMaxDT); 
			if(bRet)
			{
				for(int nCount = 0;
				nCount < nMaxDT;
				nCount++)
				{
					m_arrDestTypeIndex[nCount] = 0;
				}
				for(int nCount = 0;
				nCount < nAnz;
				nCount++)
				{
					int nType = m_arrDestTypes[nCount].GetDestType();
					m_arrDestTypeIndex[nType] = nCount + 1;
				}
			}
		}
	}
	return bRet;
}

bool CCourseData::LoadCustomerStationNames(CCtrlDataMemFile& myMFile)
{
	bool bRet = true;	
	int nAnz;
	bRet = myMFile.Read(nAnz);
	int 			nDest;
	
	unsigned char   cSide;
	unsigned char 	nPos,
					nLayer;
	for(int nCount = 0;
	(nCount < nAnz) && bRet;
	nCount++)
	{ 
		bRet  = myMFile.Read(nDest );
		bRet &= myMFile.Read(cSide );
		bRet &= myMFile.Read(nPos  );
		bRet &= myMFile.Read(nLayer);
		if(bRet)
		{
			CString strCSN;
			strCSN =  myMFile.ReadCompactString();
			strCSN.TrimLeft();
			strCSN.TrimRight();
			if(strCSN.IsEmpty())
			{
				char acDest[50];
				sprintf_s(acDest,"D%d%cP%dL%d",(int)nDest,cSide,(int)nPos,(int)nLayer);
				CString strDest(acDest);
				strCSN = strDest;
			}
			CCustomerStation tCS(nDest,cSide,nPos,nLayer,strCSN);
			m_listCustomerStations.AddTail(tCS);
		}
	}
	return bRet;
}

bool CCourseData::LoadCustomerStationNames(CLoaderFile* pLFile)
{
	bool bRet = true;	
	int nAnz;
	bRet = pLFile->Read(nAnz);
	int 			nDest;
	
	unsigned char   cSide;
	unsigned char 	nPos,
					nLayer;
	for(int nCount = 0;
	(nCount < nAnz) && bRet;
	nCount++)
	{ 
		bRet  = pLFile->Read(nDest );
	    bRet &= pLFile->Read(cSide );
		bRet &= pLFile->Read(nPos  );
		bRet &= pLFile->Read(nLayer);
		if(bRet)
		{
			CString strCSN;
			bRet =  pLFile->ReadCompactString(strCSN);
			if(bRet)
			{
				CString strName(strCSN);
				strName.TrimLeft();
				strName.TrimRight();
				if(strName.IsEmpty())
				{
					char acDest[50];
					sprintf_s(acDest,"D%d%cP%dL%d",(int)nDest,cSide,(int)nPos,(int)nLayer);
					CString strDest(acDest);
					strName = strDest;
				}
				CCustomerStation tCS(nDest,cSide,nPos,nLayer,strName);
				m_listCustomerStations.AddTail(tCS);
			}
		}
	}
	return bRet;
}

bool CCourseData::LoadManualBlockArea(CCtrlDataMemFile& myMFile)
{
	CManBlockAreaObjectList listMBAO;
	
	bool bRet = true;
	int nAnz;
	bRet = myMFile.Read(nAnz);

	m_arrManBlockAreas.SetSize(nAnz);
	if(bRet)
	{
		int nArea,
		    nAreaObj,
		    nAllObjCount = 0;
		for(int nCount = 0;
    		(nCount < nAnz) && bRet;
		    nCount++)
		{
			bRet =	myMFile.Read(nArea   ) &&
							myMFile.Read(nAreaObj);
			if(bRet)
			{
				CManBlockArea tMBA = CManBlockArea(nArea,nAreaObj,nAllObjCount);
				m_arrManBlockAreas[nCount] = tMBA;
				int nOCount, nCp, nNCp;
				
				for(nOCount = 0;
  				  (nOCount < nAreaObj) && bRet;
				    nOCount++)
				{
					bRet =	myMFile.Read(nCp ) &&
    							myMFile.Read(nNCp);
					if(bRet)
					{
						CManBlockAreaObject tMBAO(nCp,nNCp);
						listMBAO.AddTail(tMBAO);
					}
				}
				nAllObjCount += nOCount;
			}
		}
	}
	if (bRet)
	{
		bRet &= MakeManBlockAreaObjectArray(listMBAO);
		bRet &= MakeManBlockAreaIndexArray ();
	}
	return bRet;
}

bool CCourseData::LoadManualBlockArea(CLoaderFile* pLFile)
{
	CManBlockAreaObjectList listMBAO;
	
	bool bRet = true;
	int nAnz;
	bRet = pLFile->Read(nAnz);

	m_arrManBlockAreas.SetSize(nAnz);
	if(bRet)
	{
		int nArea,
		    nAreaObj,
		    nAllObjCount = 0;
		for(int nCount = 0;
    		(nCount < nAnz) && bRet;
		    nCount++)
		{
			bRet =	pLFile->Read(nArea   ) &&
					pLFile->Read(nAreaObj);
			if(bRet)
			{
				CManBlockArea tMBA = CManBlockArea(nArea,nAreaObj,nAllObjCount);
				m_arrManBlockAreas[nCount] = tMBA;
				int nOCount, nCp, nNCp;
				
				for(nOCount = 0;
  				  (nOCount < nAreaObj) && bRet;
				    nOCount++)
				{
					bRet =	pLFile->Read(nCp ) &&
    							pLFile->Read(nNCp);
					if(bRet)
					{
						CManBlockAreaObject tMBAO(nCp,nNCp);
						listMBAO.AddTail(tMBAO);
					}
				}
				nAllObjCount += nOCount;
			}
		}
	}
	if (bRet)
	{
		bRet &= MakeManBlockAreaObjectArray(listMBAO);
		bRet &= MakeManBlockAreaIndexArray ();
	}
	return bRet;
}

bool CCourseData::LoadBlockTable(CCtrlDataMemFile& myMFile,DEF_BLOCK_TABLE* pBlockTable)
{
	unsigned int nDist = 0;
	bool bDataOK = myMFile.Read(nDist);
	if(bDataOK)
	{
		pBlockTable->nDistNumber = nDist;
		pBlockTable->pDistIndexVector = new unsigned int[nDist+1];
		bDataOK = (pBlockTable->pDistIndexVector != NULL);
		if(bDataOK)
		{
			//Streckenvektor laden:
			bDataOK = myMFile.Read(pBlockTable->pDistIndexVector,sizeof(unsigned int),nDist+1);
			if(bDataOK)
			{
				unsigned int nCrashDist = pBlockTable->pDistIndexVector[nDist];
				pBlockTable->pDistCrash = new DEF_DISTANCE_CRASH[nCrashDist];
				bDataOK = (pBlockTable->pDistCrash != NULL);
				if(bDataOK)
				{
					int	aInt[6];
					for(int nCount = 0;
					(nCount < (int)nCrashDist) && bDataOK;
					nCount++)
					{
						bDataOK = (myMFile.Read(aInt,sizeof(int)*6) == sizeof(int)*6);
						if(bDataOK)
						{
							pBlockTable->pDistCrash[nCount].nIndex         = aInt[0];
							pBlockTable->pDistCrash[nCount].nFreeWay       = aInt[1];
							pBlockTable->pDistCrash[nCount].nAbsKollWay    = aInt[2];
							pBlockTable->pDistCrash[nCount].nRelKollWay    = aInt[3];
							pBlockTable->pDistCrash[nCount].nRelFreeWay    = aInt[4];
							pBlockTable->pDistCrash[nCount].nKollWayBlkWay = aInt[5];
						}
					}
				}
			}
		}
	}
	return bDataOK;
}

bool CCourseData::LoadBlockTable(CLoaderFile* pLFile,DEF_BLOCK_TABLE* pBlockTable)
{
	unsigned int nDist = 0;
	bool bDataOK = pLFile->Read(nDist);
	if(bDataOK)
	{
		pBlockTable->nDistNumber = nDist;
		pBlockTable->pDistIndexVector = new unsigned int[nDist+1];
		bDataOK = (pBlockTable->pDistIndexVector != NULL);
		if(bDataOK)
		{
			//Streckenvektor laden:
			bDataOK = pLFile->Read(pBlockTable->pDistIndexVector,nDist+1);
			if(bDataOK)
			{
				unsigned int nCrashDist = pBlockTable->pDistIndexVector[nDist];
				pBlockTable->pDistCrash = new DEF_DISTANCE_CRASH[nCrashDist];
				bDataOK = (pBlockTable->pDistCrash != NULL);
				if(bDataOK)
				{
					int	aInt[6];
					for(int nCount = 0;
					(nCount < (int)nCrashDist) && bDataOK;
					nCount++)
					{
						bDataOK = pLFile->Read(aInt,6);
						if(bDataOK)
						{
							pBlockTable->pDistCrash[nCount].nIndex         = aInt[0];
							pBlockTable->pDistCrash[nCount].nFreeWay       = aInt[1];
							pBlockTable->pDistCrash[nCount].nAbsKollWay    = aInt[2];
							pBlockTable->pDistCrash[nCount].nRelKollWay    = aInt[3];
							pBlockTable->pDistCrash[nCount].nRelFreeWay    = aInt[4];
							pBlockTable->pDistCrash[nCount].nKollWayBlkWay = aInt[5];
						}
					}
				}
			}
		}
	}
	return bDataOK;
}

bool CCourseData::LoadBlockTables(CCtrlDataMemFile& myMFile)
{
	bool bDataOK = true;
	int nAGVTA = 1,
	    nAGVTV = 1;
	int nAGVTs = CountBits(m_nAGVTypes);
	int nTables = nAGVTs * nAGVTs;
	m_pBlockTables = new DEF_BLOCK_TABLE[nTables];
	bDataOK = (m_pBlockTables != NULL);
	if(bDataOK)
	{
		for(int nCount = 0;
		(nCount < nTables) && bDataOK;
		nCount++)
		{
			if(nCount > 0)
			{
				bDataOK = myMFile.Read(nAGVTA) &&
									myMFile.Read(nAGVTV);
			}
			m_pBlockTables[nCount].nTopAGVT  = nAGVTA;
			m_pBlockTables[nCount].nCompAGVT = nAGVTV;
			bDataOK &= LoadBlockTable(myMFile,m_pBlockTables + nCount);
			if(nCount == 0)
			{
				int nNextTables;
				bDataOK = myMFile.Read(nNextTables);
				if(bDataOK)
				{
					bDataOK = ((nNextTables + 1) == nTables);
				}
			}
		}
		if(!bDataOK)
		{
			DeleteBlockTables();
		}
		else
		{
			m_nNumberOfBlockTablePairs = nTables;
		}
	}
	return bDataOK;
}

void CCourseData::DeleteBlockTables()
{
	for(int nCount = 0;
	nCount < m_nNumberOfBlockTablePairs;
	nCount++)
	{
		delete [] m_pBlockTables->pDistCrash;
		delete [] m_pBlockTables->pDistIndexVector;
	}
	delete [] m_pBlockTables;
	m_pBlockTables = NULL;
}

bool CCourseData::MakeManBlockAreaObjectArray(CManBlockAreaObjectList& listMBAO)
{
	m_arrManBlockAreaObjects.SetSize(listMBAO.GetCount());
	int nOCount = 0;
	POSITION pos = listMBAO.GetHeadPosition();

	while(pos != NULL)
	{
		CManBlockAreaObject tMBAO = listMBAO.GetNext(pos);
		m_arrManBlockAreaObjects[nOCount++] = tMBAO;

	}
	return true;
}

void CCourseData::WriteForTest(CString* pstrWorkPath)
{
	FILE *fp;
	
	_chdir((char *)pstrWorkPath);
	errno_t err = fopen_s(&fp, "SoftSys.dat", "w");
	if (err == 0)
	{
		WriteForTest(fp);
		
    	fclose(fp);      
	}
}
void CCourseData::WriteForTest(FILE* fp)
{
	fprintf(fp,"*--------------------------------------------------------------------------\n");
	fprintf(fp,"* SoFTSys: The Course                                                     *\n");
	fprintf(fp,"*--------------------------------------------------------------------------\n");
	
	fprintf(fp,"\n  Allgemeine Daten:");
	fprintf(fp,"\nGenString: '%s'"       , m_strGenString);
	fprintf(fp,"\nFahrzeug-Anzahl    %4i", m_nAGVs);
	fprintf(fp,"\nLadestationen      %4i", m_nChargeStations);
	fprintf(fp,"\nFahrzeug-Typen     %2X", m_nAGVTypes);
	fprintf(fp,"\nMax Fahrzeug-Typ   %4i", m_nMaxAGVType);
	fprintf(fp,"\nEbenen-Typen       %4i", m_arrLayerTypes.GetSize());
	fprintf(fp,"\nColumn-Types       %4i", m_arrColumnTypes.GetSize());
	fprintf(fp,"\nZiel-Typen         %4i", m_arrDestTypes.GetSize());
	fprintf(fp,"\nKnoten             %4i", m_arrManBlockAreas.GetSize());
	fprintf(fp,"\nMax Knoten         %4i", m_nMaxManBlockArea);
	fprintf(fp,"\nKundenzielnamen    %4i", m_listCustomerStations.GetCount());
	fprintf(fp,"\nMPs                %4i", m_nAIDCommPoints);
	fprintf(fp,"\nMax MP             %4i", m_arrAIDCpIndex.GetSize() - 1);
	fprintf(fp,"\nStrecken           %4i", m_nAIDCpDistances);
	fprintf(fp,"\nMax Anz LP pro Typ %4i", m_nMaxHomeAndChargeStations);
	fprintf(fp, "\n\n");
	
	for (int nType = 0;
		 nType < m_nMaxAGVType;
	     nType++)
	{
		CCourseAGVType* pCAGVT = m_apCourseAGVType[nType];
		if (pCAGVT != NULL)
		{
			pCAGVT->WriteForTest(fp);
		}
	}
	WriteManBlockAreaPointerVectorForTest 	(fp);
	WriteManBlockAreaForTest				(fp);
	WriteBlockTablesForTest        			(fp);
	WriteDestTypeForTest           			(fp);
	WriteColumnTypeForTest         			(fp);
	WriteLayerTypesForTest         			(fp);
	WriteCustomerStationNameForTest			(fp);
	
	fprintf(fp,"\n*--------------------------------------------------------------------------");
	fprintf(fp,"\n* End of Course                                                           *");
	fprintf(fp,"\n*--------------------------------------------------------------------------\n");
}

void CCourseData::SetBestAGVTypeStation(TYPE_STATION_DEF& tStation,
										unsigned char     nAGV,
							  	  		unsigned int      nAGVT,
							  	  		CString&           strPSName)
{
	CAGVTypeStationList listAGVTypeStations;
	
	
	GetAGVTypeStationList(strPSName,listAGVTypeStations);
	if(listAGVTypeStations.GetCount() > 0)
	{
		CAGVTypeStation tMyStation;
		int nBestValue(0x7fffffff);
		POSITION pos = listAGVTypeStations.GetHeadPosition();
		POSITION posSave = pos;
		if(listAGVTypeStations.GetCount() > 1)
		{
			int nCp = fzdat[nAGV].mp;
			CCourseAGVType* pCAGVT = this->GetPtrCourseAGVType(nAGVT);

			while(pos != NULL)
			{
				tMyStation = listAGVTypeStations.GetNext(pos);
				if(tMyStation.GetAGVType() == nAGVT)
				{
					int nTopValue = pCAGVT->GetBestTravelTime(nCp,tMyStation.GetStation().nDest);
					if((nTopValue >= 0) && (nBestValue > nTopValue))
					{
						nBestValue = nTopValue;
						posSave     = pos;
					}
				}
			}
		}
		tStation = listAGVTypeStations.GetAt(posSave).GetStation();
	}
}

int CCourseData::CountBits(unsigned int nVal)
{
	int nAnzBits = sizeof(nVal)*8;
	int nRet = 0;

	for(int i = 0;
	    i < nAnzBits;
		i++)
	{
			if (nVal & (1 << i))
		{
			nRet++;
		}
	}
	return nRet;
}

bool CCourseData::ConnectionExists(CString& strPickupStation,CString& strUnloadStation)
{
	bool bRet = false;
	unsigned int nAIDPIndex = GetAIDStationIndex(strPickupStation);
	unsigned int nAIDUIndex = GetAIDStationIndex(strUnloadStation);

	unsigned int   nPIndex = m_arrAIDStations[nAIDPIndex-1].GetAGVTypeStationIndex();
	unsigned short nPLimit = m_arrAIDStations[nAIDPIndex-1].GetAGVTypeStations(); 
	for(unsigned short nPCount = 0;
	(nPCount < nPLimit) && !bRet;
	nPCount++)
	{
		CAGVTypeStation tPS = m_arrAGVTypeStations[nPIndex + nPCount - 1];
		unsigned int   nUIndex = m_arrAIDStations[nAIDUIndex-1].GetAGVTypeStationIndex();
		unsigned short nULimit = m_arrAIDStations[nAIDUIndex-1].GetAGVTypeStations(); 
		for(unsigned short nUCount = 0;
		(nUCount < nULimit) && !bRet;
		nUCount++)
		{
			CAGVTypeStation tUS = m_arrAGVTypeStations[nUIndex + nUCount - 1];
			if(tPS.GetAGVType() == tUS.GetAGVType())
			{
				CCourseAGVType* pCAGVT = GetPtrCourseAGVType(tPS.GetAGVType());
				if(pCAGVT != NULL)
				{
					int nTravelTime = pCAGVT->GetBestTravelTime(tPS.GetStation().nDest,tUS.GetStation().nDest);
					bRet = (nTravelTime!= -1);
				}
			}
		}
	}	
	return bRet;
}

CAGVTypeStation CCourseData::GetAGVTypeStation(unsigned int nIndex)
{
	return m_arrAGVTypeStations[nIndex];
}

unsigned int CCourseData::GetAGVTypeStationIndex(unsigned int nAIDSIndex,TYPE_STATION_DEF& tStation,unsigned int nAGVT)
{
	unsigned int nRet = 0;
	unsigned int nStart   = m_arrAIDStations[nAIDSIndex-1].GetAGVTypeStationIndex();
	unsigned short nLimit = m_arrAIDStations[nAIDSIndex-1].GetAGVTypeStations();
	for(int nSCount = 0;
	(nSCount < nLimit) && (nRet == 0);
	nSCount++)
	{
		CAGVTypeStation tS(tStation,(short)nAGVT);
		if(m_arrAGVTypeStations[nStart + nSCount -1] == tS)
		{
			nRet = nStart + nSCount;
		}
	}
	return nRet;
}

bool CCourseData::CalculateDispoTravelTimes()
{
	bool bDataOK = true;
	for(int nCount = 0;
	    (nCount < GetMaxAGVType()) && bDataOK;
	    nCount++)
	{
  		CCourseAGVType* pCAGVT = GetPtrCourseAGVType(nCount+1);
  		if(pCAGVT != NULL)
  		{
  			bDataOK &= pCAGVT->CalculateDispoTravelTimes();
  			//ausgeblendet 16.01.08 Ri
  			//pCAGVT->PrintTravelTimes();
  		}
	}
	
	/* Fertigmeldung ausgeben */
	
    // TODO SystemMessage(mod_dispo, 13, 0,
    //              0, 0, 0, 0);

	MSGAVISO ("SystemMessage(mod_dispo, 13, 0, 0, 0, 0, 0");


	return bDataOK;
}

void CCourseData::RestoreAIDStations(CAIDStationArray& arrStations)
{
	unsigned int nLimit = arrStations.GetSize();
	for(unsigned int nCount = 0;
	    nCount < nLimit;
	    nCount++)
	{
		CString strPSName = arrStations[nCount].GetPSName();
		unsigned int nIndex = GetAIDStationIndex(strPSName);
		if(nIndex != 0)
		{
			m_arrAIDStations[nIndex - 1].Restore(arrStations[nCount]);
		}
	}
}




unsigned short CCourseData::GetManBlockAreaNumber(unsigned short nCp, unsigned short nNCp)
{
  unsigned short nMBArea = 0;
  bool bRet = false;
  CAIDCpDistance* pDist = NULL;
    
	pDist = GetAIDDistance(nCp, nNCp);
    
	if ((pDist != NULL) ||(m_arrAIDCpIndex[nCp] != 0))
	{
		for (int nCount = 0;
		     (nCount < m_arrManBlockAreas.GetSize()) && !bRet;
		     nCount++)
		{
			if (IsDistanceInMBArea(m_arrManBlockAreas[nCount].GetMBArea(), nCp, nNCp))
			{
				nMBArea = m_arrManBlockAreas[nCount].GetMBArea();
				break;
			}
		}
	}	
	return nMBArea;
}

bool CCourseData::IsMyLayoutVersion(CString& strLayoutVersion)
{
	return (m_strGenString == strLayoutVersion);
}

CString CCourseData::GetLayoutVersionDateTime()
{
	return m_strDateTime;
}

void CCourseData::GetExpectedTimeAndLengthTotal(
	unsigned char nAGVType,
	unsigned short nStartCp,
	unsigned short nDestCp,
	int& nTimeTotal,
	int& nLengthTotal)
{
	nTimeTotal = 0;
	nLengthTotal = 0;
	CCourseAGVType* pCAGVT = GetPtrCourseAGVType(nAGVType);
	if (pCAGVT != NULL)
	{
		pCAGVT->GetExpectedTimeAndLengthTotal(nStartCp,
			nDestCp,
			nTimeTotal,
			nLengthTotal);
	}
}
int CCourseData::GetLayerTypeProperties(unsigned short destType, unsigned short pos)
{
	int destTypeIndex = this->m_arrDestTypeIndex[destType] - 1;
	int destTypePosIndex = this->m_arrDestTypes[destTypeIndex].GetPosIndex() + pos - 1;
	POSITION posDestTypePos = this->m_listDestTypePos.FindIndex(destTypePosIndex);
	CDestTypePos destTypePos = this->m_listDestTypePos.GetAt(posDestTypePos);
	int layerTypeIndex = this->m_arrLayerTypeIndex[destTypePos.m_nType] - 1;

	return this->m_arrLayerTypes[layerTypeIndex].m_nProperties;
}

CString CCourseData::GetCustomerStationName(int destNumber, char lage, int pos, int layer)
{
	CString strCustomerStationName = this->GetAIDStation(destNumber, lage, pos, layer).GetPSName();
	return strCustomerStationName;
}