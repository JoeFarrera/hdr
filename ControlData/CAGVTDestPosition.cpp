#include "..\stdafx.h"
#include "CAGVTDestPosition.h"

CAGVTDestPosition::CAGVTDestPosition()
{
	m_nFirstLayerIndex = 0;
	m_nLayers          = 0;
	m_nPSIndexDP       = 0;
	m_cSide            = ' ';
	m_nPosWay          = 0;
	m_nPos             = 0;
}

CAGVTDestPosition::CAGVTDestPosition(int nPos,int nPosWay,char cSide)
{
	m_nFirstLayerIndex = 0;
	m_nLayers          = 0;
	m_nPSIndexDP       = 0;
	m_cSide            = cSide;
	m_nPosWay          = nPosWay;
	m_nPos             = nPos;
}

CAGVTDestPosition::~CAGVTDestPosition()
{
}

unsigned short CAGVTDestPosition::GetLayers()
{
	return m_nLayers;
}

void CAGVTDestPosition::SetFirstLayerIndex(unsigned int nFirstLayerIndex)
{
	m_nFirstLayerIndex = nFirstLayerIndex;
}

void CAGVTDestPosition::SetLayers(unsigned short nLayers)
{
	m_nLayers = nLayers;
}

void CAGVTDestPosition::GetAGVTypeStationList(short                nAGVType,
						   					  int				   nDest,
						   					  char				   cSide,
						   					  unsigned int         nAIDPSIndex,
						   					  CAGVTypeStationList& listAGVTStations,
						   					  CAGVTLayerArray& 	   arrLayer)
{
	if(nAIDPSIndex == m_nPSIndexDP)
	{
		CAGVTypeStation tAGVTStation(nAGVType,nDest,cSide,m_nPos,0,m_nPosWay,m_nPSIndexDP);
		listAGVTStations.AddTail(tAGVTStation);
	}
	else
	{
		for(int nLCount = 0, nLayerIndex = m_nFirstLayerIndex;
		nLCount < m_nLayers;
		nLCount++,nLayerIndex++)
		{
			
			if(arrLayer[nLayerIndex-1].GetPSIndex() == nAIDPSIndex)
			{
				CAGVTypeStation tAGVTStation(nAGVType,nDest,cSide,m_nPos,nLCount+1,m_nPosWay,nAIDPSIndex);
				listAGVTStations.AddTail(tAGVTStation);
			}
		}
	}
}

void CAGVTDestPosition::LinkToPhysicalStation(int                   nDest,
							   				  char                  cSide,
							   				  CAGVTLayerArray&      arrLayer,
							   				  CAIDStationArray&     arrAIDStations,
							   				  CCustomerStationList& listCustomerStations)
{
	bool bRet = true;
	CString strDummy;
	CCustomerStation tCS(nDest,cSide,m_nPos,0,strDummy);
	POSITION pos = listCustomerStations.Find(tCS);

	if(pos != NULL)
	{
		tCS = listCustomerStations.GetAt(pos);
		m_nPSIndexDP = tCS.FindAIDStation(arrAIDStations);
		int nLayerIndex = m_nFirstLayerIndex;
		for(int nLCount = 0;
		nLCount < m_nLayers;
		nLCount++,nLayerIndex++)
		{
			arrLayer[nLayerIndex-1].LinkToPhysicalStation(nDest,cSide,m_nPos,
														  arrAIDStations,
														  listCustomerStations);
		}
	}
	else
	{ 
		bRet = false; 
	}
}

unsigned int CAGVTDestPosition::GetPositionOrLayerObjects(unsigned int nAGVT,int nDest,char cSide,unsigned int nPSIndex,CAGVTLayerArray& arrLayer)
{
	unsigned int nRet = 0;
	if(m_nPSIndexDP == nPSIndex)
	{
		//nOSIndex ist Pos: Layers sind die Objekte:
		for(int nLCount = 0;
		(nLCount < m_nLayers);
		nLCount++)
		{
			nRet |= (1 << nLCount);
		}
	}
	else
	{
		//nPSIndex kann noch Layer sein:
		int nLayerIndex = m_nFirstLayerIndex;
		for(int nLCount = 0;
		(nLCount < m_nLayers) && !nRet;
		nLCount++, nLayerIndex++)
		{
			unsigned int nPSIndexLayer = arrLayer[nLayerIndex-1].GetPSIndex();
			if(nPSIndex == nPSIndexLayer)
			{
				nRet = (1 << nLCount);
			}
		}
	}
	return nRet;
}

void CAGVTDestPosition::GetPSIndexListWithChildren(unsigned int nAGVT,int nDest,char cSide,unsigned int nSeekIndex,
		                       CChangedStationObjList& listChangedStations,CAGVTLayerArray& arrLayer)
{
	CChangedStationObj tCSOPos(m_nPSIndexDP,0);

	if(nSeekIndex != 0)
	{
		if(m_nPSIndexDP == nSeekIndex)
		{
			nSeekIndex = 0;
		}
	}
	
	for(int nLCount = 0, nLayerIndex = m_nFirstLayerIndex;
	nLCount < m_nLayers;
	nLCount++, nLayerIndex++)
	{
		unsigned int nPSIndexLayer = arrLayer[nLayerIndex-1].GetPSIndex();
		if((nSeekIndex == 0) || (nSeekIndex == nPSIndexLayer))
		{
			CChangedStationObj tCSOLayer(nPSIndexLayer,1 << nLCount);
			listChangedStations.AddTail(tCSOLayer);

			//listPSIndex.AddTail((void*)nPSIndexLayer);
			//nSeekIndex = 0;
			tCSOPos.AddObject(1 << nLCount);
		}
	}
	
	if(tCSOPos.HasChangeObjects())
	{
		listChangedStations.AddTail(tCSOPos);
	}
}

void CAGVTDestPosition::WriteForTest(FILE* pFile,unsigned int nIndex)
{
	fprintf(pFile,"%d.DestPos: Side: %c, Pos:%hd, Way:%hd, Layers: %d, FirstLayerIndex: %d, PSIndex: %d\n",
			nIndex,
			m_cSide,
		    m_nPos,
			m_nPosWay,
			(int)m_nLayers,
			m_nFirstLayerIndex,
			m_nPSIndexDP);
}
