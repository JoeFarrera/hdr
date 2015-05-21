#include "..\stdafx.h"
#include "CDestinationType.h"

CDestinationType::CDestinationType()
{
}


CDestinationType::CDestinationType(unsigned short nDestType,
					 			   unsigned short nPosIndex,
					 			   unsigned short nPositions)
{
	m_nDestType  = nDestType;
	m_nPosIndex  = nPosIndex;
	m_nPositions = nPositions;
}

CDestinationType::~CDestinationType()
{
}

unsigned short CDestinationType::GetPosIndex()
{
	return m_nPosIndex;
}
/**
* Check action is valid for the destination type
*/
bool CDestinationType::IsAction(int nAction,short nPos,unsigned char nLayer,
				  CDestTypePosList&      listDestTypePos,
				  CUIntArray&            arrColumnTypeIndex,
				  CColumnTypeArray&      arrColumnTypes,
				  CUIntArray&            arrLayerTypeIndex,
				  CLayerTypeArray&       arrLayerTypes)
{
	bool bRet = false;
	if(nPos <= m_nPositions)
	{
		int nStartPos = 1;
		int nEndPos   = m_nPositions;
		if(nPos > 0)
		{
			nStartPos = nPos;
			nEndPos   = nPos;
		}
		int nPosIndex = m_nPosIndex + nStartPos -1;
		POSITION pos = listDestTypePos.GetHeadPosition();
		for(int nPCount = 1;
		nPCount < nPosIndex;
		nPCount++)
		{
			listDestTypePos.GetNext(pos);
		}
		
		for(int nPCount = nStartPos;
		nPCount <= nEndPos;
		nPCount++)
		{
			CDestTypePos tDTP = listDestTypePos.GetNext(pos);

			bRet |= tDTP.IsAction(nAction,nLayer,arrColumnTypeIndex,arrColumnTypes,
								  arrLayerTypeIndex,arrLayerTypes);
	}
	}
	return bRet;
}
void CDestinationType::WriteForTest(FILE* pFile,int nIndex)
{
		fprintf(pFile,"%d.DestType %d, %hd Positions, PosIndex: %d\n",
				nIndex,
				m_nDestType,
				m_nPositions,
				m_nPosIndex);
}
