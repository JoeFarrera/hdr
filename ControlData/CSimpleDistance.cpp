#include "..\stdafx.h"
#include "CSimpleDistance.h"

CSimpleDistance::CSimpleDistance()
{
	m_nCp  = 0;
	m_nNCp = 0;
}

CSimpleDistance::CSimpleDistance(int nCp,int nNCp)
{
	m_nCp  = nCp;
	m_nNCp = nNCp;
}

CSimpleDistance::~CSimpleDistance()
{
}

bool CSimpleDistance::operator==(const CSimpleDistance& tSD)
{
	return (m_nCp == tSD.m_nCp) && (m_nNCp == tSD.m_nNCp);
}

bool CSimpleDistance::operator>(const CSimpleDistance& tSD)
{
	bool bRet = (m_nCp > tSD.m_nCp) ||
				((m_nCp == tSD.m_nCp) && (m_nNCp > tSD.m_nNCp));
	return bRet;
}
