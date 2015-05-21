#include "..\stdafx.h"
#include "CCommPoint.h"

CCommPoint::CCommPoint()
{
}

CCommPoint::CCommPoint(int            nCp,
					   unsigned char  nCtrlArea,
					   unsigned char  nCommArea,
					   unsigned short nCpArea,
					   unsigned short nCpIndex,
					   unsigned char  nProperties,
					   unsigned short nDpIndex)
{
	m_nCp            = nCp;
	m_nCtrlArea      = nCtrlArea;;
	m_nCommArea      = nCommArea;
	m_nCpArea        = nCpArea;
	m_nCpIndex       = nCpIndex;
	m_nProperties    = nProperties;
	m_nDpIndex       = nDpIndex;
	m_nNCps          = 0;
	m_nDistanceIndex = 0;
}

CCommPoint::~CCommPoint()
{
}

void CCommPoint::SetTravelTime(unsigned int nIndex,unsigned short nTravelTime)
{
	m_arrTravelTimes[nIndex] = nTravelTime;
}

void CCommPoint::WriteForTest(FILE* pFile,int nIndex)
{
	fprintf(pFile, "%d.Cp: %4i, %4i, %4i, 0X%02X, %i, %i, %4i, %4i, %d\n",
			 nIndex,
	         (int)m_nCp,
	         (int)m_nNCps,
	         (int)m_nCpIndex,
	         m_nProperties,
	         (int)m_nCtrlArea,
	         (int)m_nCommArea,
	         (int)m_nCpArea,
		     (int)m_nDpIndex,
		     (int)m_nDistanceIndex);
}

bool CCommPoint::SetSizeArrayTravelTime(unsigned int nDestinations )
{
	bool bRet = true;
	if(m_arrTravelTimes.GetSize() != nDestinations)
	{
		m_arrTravelTimes.RemoveAll();
		m_arrTravelTimes.SetSize(nDestinations);
	}
	return bRet;
}

unsigned short CCommPoint::GetTravelTime(unsigned int nDestIndex)
{
	return m_arrTravelTimes[nDestIndex];
}

void CCommPoint::PrintTravelTimes()
{
	printf("\nCp%d :\n",m_nCp);
	unsigned int nLimit = m_arrTravelTimes.GetSize();
	for(unsigned int nCount = 0;
	nCount < nLimit;
	nCount++)
	{
		printf("%d,",m_arrTravelTimes[nCount]);
		if(div(nCount+1,10).rem == 0)
		{
			printf("\n");
		}
	}
}

void CCommPoint::operator=(const CCommPoint& other)
{
	this->m_nCp = other.m_nCp;
	this->m_nNCps = other.m_nNCps;
	this->m_nCpIndex = other.m_nCpIndex;
	this->m_nProperties = other.m_nProperties;
	this->m_nCtrlArea = other.m_nCtrlArea;
	this->m_nCommArea = other.m_nCommArea;
	this->m_nCpArea = other.m_nCpArea;
	this->m_nDpIndex = other.m_nDpIndex;
	this->m_nDistanceIndex = other.m_nDistanceIndex;
	this->m_arrTravelTimes.RemoveAll();
	this->m_arrTravelTimes.SetSize(other.m_arrTravelTimes.GetSize());
	this->m_arrTravelTimes.Copy(other.m_arrTravelTimes);
	this->m_arrTravelTimes.FreeExtra();
}