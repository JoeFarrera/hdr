#include "..\stdafx.h"
#include "CLayerType.h"


CLayerType::CLayerType()
{
}

CLayerType::CLayerType(unsigned short nType,int nHeight,int nDepth,int nMaxPieces,
					   int nStartPrio,int nProperties, int nUserInfo, CString& strUserInfo)
{
	m_nType       = nType;
	m_nHeight     = nHeight;
	m_nDepth      = nDepth;
	m_nMaxPieces  = nMaxPieces;
	m_nStartPrio  = nStartPrio;
	m_nProperties = nProperties;
	m_nUserInfo   = nUserInfo;
	m_strUserInfo = strUserInfo;
}

CLayerType::~CLayerType()
{
}

bool CLayerType::IsAction(int nAction)
{
	return ((m_nProperties & nAction) != 0);
}

void CLayerType::WriteForTest(FILE* pFile,unsigned int nIndex)
{
	fprintf(pFile,"%d.LayerType %hd: Properties: 0X%X\n",
			nIndex,
			m_nType,
			(int) m_nProperties);
}
