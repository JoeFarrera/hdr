// CrashDistanceArray.cpp: implementation of the CCrashDistanceArray class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "..\tpc.h"
#include "CrashDistanceArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrashDistanceArray::CCrashDistanceArray()
{

}

CCrashDistanceArray::~CCrashDistanceArray()
{

}

CCrashDistance* CCrashDistanceArray::GetPCrashDistance(int nCDistIndex)
{
	int				nLimit	= GetSize();
	CCrashDistance*	pMyCD	= NULL;
	CCrashDistance*	pCD		= GetData();

	for(int nCCount = 0;
	nCCount < nLimit;
	nCCount++)
	{
		if(pCD->ItsMe(nCDistIndex))
		{
			pMyCD = pCD;
		}
		pCD++;
	}

	return pMyCD;
}

bool CCrashDistanceArray::IsToRequestWayTele(int nDIndex, int nBlockway)
{
	bool bRet = false;
	int nLimit = GetSize();
	CCrashDistance*	pCD		= GetData();
	for(int nCount = 0;
	(nCount < nLimit) && !bRet;
	nCount++)
	{
		bRet = pCD->IsToRequestWayTele(nDIndex,nBlockway);
		pCD++;
	}
	return bRet;
}
