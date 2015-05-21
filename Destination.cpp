// Destination.cpp: implementation of the CDestination class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "Destination.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDestination::CDestination(TYPE_STATION_DEF* pDest)
{
	m_pDest = pDest;
}

CDestination::~CDestination()
{

}

bool CDestination::operator ==(TYPE_STATION_DEF dest)
{
	bool fRetVal;
	if (NULL != m_pDest)
	{

	if (dest.nDest == m_pDest->nDest &&
		dest.nPos == m_pDest->nPos &&
		dest.ucSide == m_pDest->ucSide 
		/* && Michael 18.10.2001 Not used...
		dest.ebene == m_pDest->ebene */)
		fRetVal = true;
	else
		fRetVal = false;
	}
	else
		fRetVal = false;

	return (fRetVal);


}

void CDestination::SetDestino(TYPE_STATION_DEF *pDest)
{
	m_pDest = pDest;

}
