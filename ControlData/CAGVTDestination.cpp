#include "..\stdafx.h"
#include "CAGVTDestination.h"

CAGVTDestination::CAGVTDestination()
{
	m_nNumber               = 0;
	m_nNCp                  = 0;
	m_bNoThoroughfare       = 0;
	m_nDestTypeLeft         = 0;
	m_nLeftPSIndex          = 0;
	m_nLeftPositions        = 0;
	m_nLeftPossibleActions  = 0;
	m_nDestTypeRight        = 0;
	m_nRightPSIndex         = 0;
	m_nRightPositions       = 0;
	m_nRightPossibleActions = 0;
	m_nFirstPosIndex        = 0;
	m_nDestArea             = 0;
}
CAGVTDestination::CAGVTDestination(int nDest,int nNCp,bool bNoThoroughfare,short nDestTypeLeft,short nDestTypeRight,unsigned short nDestArea)
{
	m_nNumber               = nDest;
	m_nNCp                  = nNCp;
	m_bNoThoroughfare       = bNoThoroughfare;
	m_nDestTypeLeft         = nDestTypeLeft;
	m_nLeftPSIndex          = 0;
	m_nLeftPositions        = 0;
	m_nLeftPossibleActions  = 0;
	m_nDestTypeRight        = nDestTypeRight;
	m_nRightPSIndex         = 0;
	m_nRightPositions       = 0;
	m_nRightPossibleActions = 0;
	m_nFirstPosIndex        = 0;
	m_nDestArea             = nDestArea;
}
CAGVTDestination::~CAGVTDestination()
{
}

short CAGVTDestination::GetDestThoroughfare()
{
	// Durchfahrt erlaubt
	return !m_bNoThoroughfare;
}

short CAGVTDestination::GetDestTypeLeft()
{
	return m_nDestTypeLeft;
}

short CAGVTDestination::GetDestTypeRight()
{
	return m_nDestTypeRight;
}

unsigned int CAGVTDestination::GetFirstPosIndex()
{
	return m_nFirstPosIndex;
}

unsigned int CAGVTDestination::GetLeftPositions()
{
	return m_nLeftPositions;
}

void CAGVTDestination::SetLeftPSIndex(unsigned int nLeftPSIndex)
{
	m_nLeftPSIndex = nLeftPSIndex;
}

void CAGVTDestination::SetRightPSIndex(unsigned int nRightPSIndex)
{
	m_nRightPSIndex = nRightPSIndex;
}

void CAGVTDestination::SetRightPositions (unsigned short nRightPositions)
{
	m_nRightPositions = nRightPositions;
}

void CAGVTDestination::SetLeftPositions (unsigned short nLeftPositions)
{
	m_nLeftPositions = nLeftPositions;
}

void CAGVTDestination::LinkLeftPosToPhysicalStation(CAGVTDestPositionArray& arrDestPositions,
													 CAGVTLayerArray& arrLayer,
													 CAIDStationArray& arrAIDStations,
													 CCustomerStationList& listCustomerStations)
{
	int nCount,
		nPosIndex = m_nFirstPosIndex;
	for(nCount = 0;
	nCount < m_nLeftPositions;
	nCount++,nPosIndex++)
	{	
		arrDestPositions[nPosIndex-1].LinkToPhysicalStation(m_nNumber,'L',arrLayer,
															arrAIDStations,listCustomerStations);

	}	
}

void CAGVTDestination::LinkRightPosToPhysicalStation(CAGVTDestPositionArray& arrDestPositions,
													 CAGVTLayerArray& arrLayer,
													 CAIDStationArray& arrAIDStations,
													 CCustomerStationList& listCustomerStations)
{
	int nCount,
		nPosIndex = m_nFirstPosIndex + m_nLeftPositions;
	for(nCount = 0;
	nCount < m_nRightPositions;
	nCount++,nPosIndex++)
	{	
		arrDestPositions[nPosIndex-1].LinkToPhysicalStation(m_nNumber,'R',arrLayer,
															arrAIDStations,listCustomerStations);

	}	
}

void CAGVTDestination::GetAGVTypeStationList(short                   nAGVType,
							   				 unsigned int            nAIDPSIndex,
							   				 CAGVTypeStationList&    listAGVTStations,
							   				 CAGVTDestPositionArray& arrDestPositions,
							   				 CAGVTLayerArray& 	     arrLayer)
{
	if(nAIDPSIndex == m_nLeftPSIndex)
	{
		CAGVTypeStation tAGVTStation(nAGVType,m_nNumber,'L',0,0,0,m_nLeftPSIndex);
		listAGVTStations.AddTail(tAGVTStation);
	}
	if(nAIDPSIndex == m_nRightPSIndex)
	{
		CAGVTypeStation tAGVTStation(nAGVType,m_nNumber,'R',0,0,0,m_nRightPSIndex);
		listAGVTStations.AddTail(tAGVTStation);
	}
	if(m_nDestTypeLeft != 0)
	{
		for(int nPCount = 0,nPosIndex = m_nFirstPosIndex;
		nPCount < m_nLeftPositions;
		nPCount++,nPosIndex++)
		{
			arrDestPositions[nPosIndex-1].GetAGVTypeStationList(nAGVType,m_nNumber,'L',nAIDPSIndex,listAGVTStations,arrLayer);
		}
	}
	if(m_nDestTypeRight != 0)
	{
		for(int nPCount = 0,nPosIndex = m_nFirstPosIndex + m_nLeftPositions;
		nPCount < m_nRightPositions;
		nPCount++,nPosIndex++)
		{
			arrDestPositions[nPosIndex-1].GetAGVTypeStationList(nAGVType,m_nNumber,'R',nAIDPSIndex,listAGVTStations,arrLayer);
		}
	}
}

unsigned int CAGVTDestination::GetAllStationObjects(unsigned int nAGVT,unsigned int nPSIndex,CAGVTDestPositionArray& arrDestPos,CAGVTLayerArray& arrLayer)
{
	unsigned int nRet = 0;
	bool bIsLeftDest  = (nPSIndex == m_nLeftPSIndex );
	bool bIsRightDest = (nPSIndex == m_nRightPSIndex);
	if(bIsLeftDest || bIsRightDest)
	{
		if(bIsLeftDest)
		{
			nRet = GetAllPositions(m_nLeftPositions);
		}
		else
		{
			nRet = GetAllPositions(m_nRightPositions);
		}
	}
	else
	{
		//nPSIndex ist eine Position oder ein Layer:
		if(m_nDestTypeLeft != 0)
		{
			nRet = GetPositionOrLayerObjects(nAGVT,
											 nPSIndex,
											 'L',
											 arrDestPos,
											 arrLayer);
		}
		if((m_nDestTypeRight != 0) && (nRet == 0))
		{
			nRet = GetPositionOrLayerObjects(nAGVT,
											 nPSIndex,
											 'R',
											 arrDestPos,
											 arrLayer);
		}
	}
	return nRet;
}

unsigned int CAGVTDestination::GetAllPositions(int nPositions)
{
	unsigned int nRet = 0;
	for(int nPCount = 0;
	nPCount < nPositions;
	nPCount++)
	{
		nRet |= (1 << nPCount);
	}
	return nRet;
}

unsigned int CAGVTDestination::GetPositionOrLayerObjects(unsigned int nAGVT,unsigned int nPSIndex,char cSide,CAGVTDestPositionArray& arrDestPos,CAGVTLayerArray& arrLayer)
{
	unsigned int nRet  = 0;
	int nPositions     = (cSide == 'L') ? m_nLeftPositions:m_nRightPositions;
	int nFirstPosIndex = (cSide == 'L') ? m_nFirstPosIndex:(m_nFirstPosIndex + m_nLeftPositions);
	for(int nPCount = 0,nPosIndex = nFirstPosIndex;
	(nPCount < nPositions) && !nRet;
	nPCount++,nPosIndex++)
	{
			nRet = arrDestPos[nPosIndex-1].GetPositionOrLayerObjects(nAGVT,
																	 m_nNumber,
																	 cSide,
																	 nPSIndex,
																	 arrLayer);

	}
	return nRet;
}

unsigned int CAGVTDestination::GetFirstDestPosIndex(char cSide)
{
	return (cSide == 'L') ? m_nFirstPosIndex : (m_nFirstPosIndex + m_nLeftPositions);
}

unsigned int CAGVTDestination::GetPSIndex(char cSide)
{
	// Side:
	return (cSide == 'L') ? m_nLeftPSIndex : m_nRightPSIndex;
}

void CAGVTDestination::GetPSIndexListWithChildren(unsigned int nAGVT,unsigned int nPSIndex, CChangedStationObjList& listChangeStations,CAGVTDestPositionArray& arrDestPos,CAGVTLayerArray& arrLayer)
{
	unsigned int nSeekIndexLeft  = nPSIndex,
		         nSeekIndexRight = nPSIndex;
	if(nPSIndex == m_nLeftPSIndex)
	{
		nSeekIndexLeft = 0;
	}
	if(nPSIndex == m_nRightPSIndex)
	{
		nSeekIndexRight = 0;
	}
	if(m_nDestTypeLeft != 0)
	{
		//Ein CChangedStationObj-Objekt bilden mit eigenem PSIndex der linken Seite;
		CChangedStationObj tCSODest(m_nLeftPSIndex,0);
		for(int nPCount = 0,nPosIndex = m_nFirstPosIndex;
		(nPCount < m_nLeftPositions) && ((nSeekIndexLeft == 0) || ((nSeekIndexLeft != 0) && !tCSODest.HasChangeObjects()));
		nPCount++,nPosIndex++)
		{
			int nListCountOld = listChangeStations.GetCount();
			arrDestPos[nPosIndex-1].GetPSIndexListWithChildren(nAGVT,
															   m_nNumber,
															   'L',
															   nSeekIndexLeft,
															   //listPSIndex,
															   listChangeStations,
															   arrLayer);
			if(nListCountOld < listChangeStations.GetCount())
			{
				//listPSIndex.AddTail((void*)m_nLeftPSIndex);
				tCSODest.AddObject(1 << nPCount);
			}
		}
		if(tCSODest.HasChangeObjects())
		{
			listChangeStations.AddTail(tCSODest);
		}
	}
	if(m_nDestTypeRight != 0)
	{
		//Ein CChangedStationObj-Objekt bilden mit eigenem PSIndex der rechten Seite;
		CChangedStationObj tCSODest(m_nRightPSIndex,0);
		for(int nPCount = 0,nPosIndex = m_nFirstPosIndex + m_nLeftPositions;
		(nPCount < m_nRightPositions) && ((nSeekIndexRight == 0) || ((nSeekIndexRight != 0) && !tCSODest.HasChangeObjects()));
		nPCount++,nPosIndex++)
		{
			int nListCountOld = listChangeStations.GetCount();
			arrDestPos[nPosIndex-1].GetPSIndexListWithChildren(nAGVT,
															   m_nNumber,
															   'R',
															   nSeekIndexRight,
															   //listPSIndex,
															   listChangeStations,
															   arrLayer);
			if(nListCountOld < listChangeStations.GetCount())
			{
				//listPSIndex.AddTail((void*)m_nRightPSIndex);
				tCSODest.AddObject(1 << nPCount);
			}
		}
		if(tCSODest.HasChangeObjects())
		{
			listChangeStations.AddTail(tCSODest);
		}
	}
}

void CAGVTDestination::WriteForTest(FILE* pFile, unsigned int nIndex)
{
	fprintf(pFile,"%d.Dest.%d: Area:%d, FirstPosIndex: %d, NCp: %d\n"
	              "            TypeL:%d, LeftPositions : %d, LeftPossibleActions : 0X%02X, LeftPSIndex : %d\n"
	              "            TypeR:%d, RightPositions: %d, RightPossibleActions: 0X%02X, RightPSIndex: %d\n",
			nIndex,
			m_nNumber,
			(int)m_nDestArea,
			m_nFirstPosIndex,
			m_nNCp,
			(int)m_nDestTypeLeft,
			(int)m_nLeftPositions,
			m_nLeftPossibleActions,
			m_nLeftPSIndex,
			(int)m_nDestTypeRight,
			(int)m_nRightPositions,
			m_nRightPossibleActions,
			m_nRightPSIndex);
}
