#include "..\stdafx.h"
#include "CColumnType.h"

CColumnType::CColumnType()
{
	m_nCType = 0;
	for(int i = 0;
	i < 10;
	i++)
	{
		m_auLayerType[i] = 0;
	}
}

CColumnType::CColumnType(unsigned short nType,unsigned short nLType)
{
	m_nCType = nType;
	for(int i = 0;
	i < 10;
	i++)
	{
		m_auLayerType[i] = 0;
	}
	m_auLayerType[0] = nLType;
}

CColumnType::CColumnType(unsigned short nType,unsigned short* pLType)
{
	m_nCType = nType;
	for(int i = 0;
	i < 10;
	i++)
	{
		m_auLayerType[i] = *(pLType+i);
	}
}

CColumnType::~CColumnType()
{
}

unsigned short CColumnType::GetLayers()
{
	unsigned short nCount;
	for(nCount = 0;
	(nCount < 10) && (m_auLayerType[nCount] != 0);
	nCount++)
	{
	}
	return nCount;
}

bool CColumnType::IsAction(int              nAction,
				 		   CUIntArray&      arrLayerTypeIndex,
				  		   CLayerTypeArray& arrLayerTypes)
{
	bool bRet = false;
	int nLimit = GetLayers();
	for(int nLCount = 0;
	nLCount < nLimit;
	nLCount++)
	{
		int nLIndex = arrLayerTypeIndex[m_auLayerType[nLCount]];
		if(nLIndex > 0)
		{
			CLayerType tLT = arrLayerTypes[nLIndex - 1];
			bRet |= tLT.IsAction(nAction);
		}
	}
	return bRet;
}

void CColumnType::WriteForTest(FILE* pFile,unsigned int nIndex)
{
	fprintf(pFile,"%d.ColumnType %d\n",
			nIndex,
			m_nCType);
	for(int nLCount = 10;
	nLCount > 0;
	nLCount--)
	{
		if(m_auLayerType[nLCount - 1] != 0)
		{
			fprintf(pFile,"    %d.Layer: Type %d\n",
					nLCount,
					(int) m_auLayerType[nLCount - 1]);
		}
	}
}
