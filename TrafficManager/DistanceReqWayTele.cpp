// DistanceReqWayTele.cpp: implementation of the CDistanceReqWayTele class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "..\tpc.h"
#include "DistanceReqWayTele.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDistanceReqWayTele::CDistanceReqWayTele()
{
	m_nAGVType	= 0;
	m_nCp		= 0;
	m_nNCp		= 0;
}

CDistanceReqWayTele::CDistanceReqWayTele(int nAGVType,int nCp,int nNCp)
{
	m_nAGVType	= nAGVType;
	m_nCp		= nCp;
	m_nNCp		= nNCp;
}

CDistanceReqWayTele::~CDistanceReqWayTele()
{

}

bool CDistanceReqWayTele::operator ==(const CDistanceReqWayTele &myDRWT) const
{
	return	(m_nAGVType == myDRWT.m_nAGVType) &&
			(m_nCp		== myDRWT.m_nCp		) &&
			(m_nNCp		== myDRWT.m_nNCp	);
}
