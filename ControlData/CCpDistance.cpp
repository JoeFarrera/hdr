#include "..\stdafx.h"
#include "CCpDistance.h"

CCpDistance::CCpDistance()
{
	m_nNCp            = 0;
	m_nTravelTime     = 0;
	m_nBlockWay       = 0;
	m_nLength         = 0;
	m_nNDp            = 0;
	m_nProperties     = 0;
	m_nDistIndex      = 0;
  m_nRealTravelTime = 0;
}

CCpDistance::CCpDistance(unsigned short nDistIndex,
						 unsigned short nNCp,
						 unsigned short nLength,
						 unsigned short nBlockWay,
						 unsigned short nProperties,
						 unsigned short nNDp,
						 unsigned short nTravelTime)
{
	m_nNCp            = nNCp;
	m_nTravelTime     = nTravelTime;
	m_nBlockWay       = nBlockWay;
	m_nLength         = nLength;
	m_nNDp            = nNDp;
	m_nProperties     = nProperties;
	m_nDistIndex      = nDistIndex;
	m_nRealTravelTime = nTravelTime;
}

CCpDistance::~CCpDistance()
{
}

void CCpDistance::WriteForTest(FILE* pFile,unsigned int nIndex)
{
	fprintf(pFile,"%d.Dist: NCp:%d, Len:%d, BlockWay:%d, Prop:0X%02X, NDp:%d, TravelTime:%d\n",
			m_nDistIndex,
			m_nNCp,
			m_nLength,
			m_nBlockWay,
			m_nProperties,
			m_nNDp,
			m_nTravelTime);
}
