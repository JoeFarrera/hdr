// CrashDistance.cpp: implementation of the CCrashDistance class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "..\tpc.h"
#include "CrashDistance.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrashDistance::CCrashDistance()
{

}

CCrashDistance::CCrashDistance(	int nIndex,
								int nFreeWay,
								int nAbsCollWay,
								int nRelCollWay,
								int nRelFreeWay,
								int nCollWayBlkWay)
{
	m_nIndex			= nIndex;
	m_nFreeWay			= nFreeWay;
	m_nAbsCollWay		= nAbsCollWay;
	m_nRelCollWay		= nRelCollWay;
	m_nRelFreeWay		= nRelFreeWay;
	m_nCollWayBlkWay	= nCollWayBlkWay;
}

CCrashDistance::~CCrashDistance()
{

}

bool CCrashDistance::ItsMe(int nCDistIndex)
{
	return (m_nIndex == nCDistIndex);
}

bool CCrashDistance::HasAbsCollWay()
{
	return (m_nAbsCollWay > 0);
}

bool CCrashDistance::CrossOverAbsCollWay(int nDrivenWay)
{
	return (nDrivenWay >= m_nAbsCollWay);
}

bool CCrashDistance::HasCollWayBlkWay()
{
	return (m_nCollWayBlkWay > 0);
}

bool CCrashDistance::CrossOverCollWayBlkWay(int nDrivenWay)
{
	return (nDrivenWay >= m_nCollWayBlkWay);
}

bool CCrashDistance::HasFreeWay()
{
	return (m_nFreeWay > 0);
}

bool CCrashDistance::CrossOverFreeWay(int nDrivenWay)
{
	return (nDrivenWay > m_nFreeWay);
}

bool CCrashDistance::HasRelCollWay()
{
	return (m_nRelCollWay > 0);
}

bool CCrashDistance::CrossOverRelCollWay(int nDrivenWay)
{
	return (nDrivenWay >= m_nRelCollWay);
}

bool CCrashDistance::CrossOverRelFreeArea(int nDrivenWay)
{
	return (nDrivenWay >= (m_nRelCollWay + m_nRelFreeWay));
}

bool CCrashDistance::RelCollWayIsAbsCollWay()
{
	return (m_nRelCollWay == m_nAbsCollWay);
}

bool CCrashDistance::BlockWayWithoutCollision()
{
	return (m_nCollWayBlkWay == 1);
}

bool CCrashDistance::IsToRequestWayTele(int nDIndex, int nMyBlockway)
{
	return ((m_nIndex == nDIndex ) &&
			(m_nCollWayBlkWay > 1) &&
			(m_nCollWayBlkWay < nMyBlockway) &&
			((nMyBlockway - m_nCollWayBlkWay) >= 500));
}


unsigned short CCrashDistance::GetIndex()
{
	return this->m_nIndex;
}


void CCrashDistance::SetValues(int nIndex, int nFreeWay, int nAbsCollWay, int nRelCollWay, int nRelFreeWay, int nCollWayBlkWay)
{
	this->m_nIndex = nIndex;
	this->m_nFreeWay = nFreeWay;
	this->m_nAbsCollWay = nAbsCollWay;
	this->m_nRelCollWay = nRelCollWay;
	this->m_nRelFreeWay = nRelFreeWay;
	this->m_nCollWayBlkWay = nCollWayBlkWay;
}
