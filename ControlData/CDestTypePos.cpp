#include "..\stdafx.h"
#include "CDestTypePos.h"

CDestTypePos::CDestTypePos()
{
}

CDestTypePos::CDestTypePos(unsigned char  nMedium,
				 			   char           cLayerColumn,
				 			   unsigned short nType,
				 			   int			  nPosWay,
				 			   unsigned short nDestRoutine)
{
	m_nMedium      = nMedium;
	m_cLayerColumn = cLayerColumn;
	m_nType        = nType;
	m_nPosWay      = nPosWay;
	m_nDestRoutine = nDestRoutine;
}

CDestTypePos::~CDestTypePos()
{
}

bool CDestTypePos::IsAction(int nAction,unsigned char nLayer,
				  			CUIntArray&            arrColumnTypeIndex,
				  			CColumnTypeArray&      arrColumnTypes,
				  			CUIntArray&            arrLayerTypeIndex,
				  			CLayerTypeArray&       arrLayerTypes)
{
	bool bRet = false;
	CColumnType tCT;
	if(m_cLayerColumn != 'E')
	{
		//ColumnType:
		int nCIndex = arrColumnTypeIndex[m_nType];
		if(nCIndex > 0)
		{
			tCT = arrColumnTypes[nCIndex - 1];
		}

	}
	else
	{
		//LayerType:
		tCT = CColumnType (0,m_nType);
	}
	bRet |= tCT.IsAction(nAction,arrLayerTypeIndex,arrLayerTypes);

	return bRet;
}

void CDestTypePos::WriteForTest(FILE* pFile,int nIndex)
{
	fprintf(pFile,"     %d.Position: Column/Layer: %c, Type: %hd, PosWay: %hd\n",
			nIndex,
			m_cLayerColumn,
			m_nType,
			m_nPosWay);
}
