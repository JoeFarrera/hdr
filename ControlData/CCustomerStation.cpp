#include "..\stdafx.h"
#include "CCustomerStation.h"

CCustomerStation::CCustomerStation()
{
	m_nDestNr = 0;
	m_cSide   = ' ';
	m_nPos    = 0;
	m_nLayer  = 0; 
}

CCustomerStation::CCustomerStation(int nDest)
{
	m_nDestNr = nDest;
	m_cSide   = ' ';
	m_nPos    = 0;
	m_nLayer  = 0; 
}
CCustomerStation::CCustomerStation(int nDest,char cSide, unsigned char nPos, unsigned char nLayer,CString& strName)
{
	m_nDestNr  = nDest;
	m_cSide    = cSide;
	m_nPos     = nPos;
	m_nLayer   = nLayer; 
	m_strCName = strName;
}
CCustomerStation::~CCustomerStation()
{
}

bool CCustomerStation::operator==(const CCustomerStation &tCS) const
{
	bool bRet = (!tCS.m_strCName.IsEmpty()) ? (m_strCName == tCS.m_strCName):
											((m_nDestNr == tCS.m_nDestNr) &&
											 (m_cSide   == tCS.m_cSide  ) &&
											 (m_nPos    == tCS.m_nPos   ) &&
											 (m_nLayer  == tCS.m_nLayer ));
	return bRet;
}

int CCustomerStation::GetDestNr()
{
	return m_nDestNr;
}

char CCustomerStation::GetSide()
{
	return m_cSide;
}

unsigned char CCustomerStation::GetPos()
{
	return m_nPos;
}

unsigned char CCustomerStation::GetLayer()
{
	return m_nLayer;
}

CString CCustomerStation::GetCName()
{
	return m_strCName;
}

int CCustomerStation::FindAIDStation(CAIDStationArray& arrAIDStations)
{
	int nRet = 0;
	unsigned int nLimit = arrAIDStations.GetSize();
	for(unsigned int nCount = 0;
	(nCount < nLimit) && !nRet;
	nCount++)
	{
		nRet = (m_strCName == arrAIDStations[nCount].GetPSName()) ? (nCount + 1):0;
	}
	return nRet;
}
