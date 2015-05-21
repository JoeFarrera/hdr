// CrashDistanceArray.h: interface for the CCrashDistanceArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRASHDISTANCEARRAY_H__B5DF0017_3FB8_4925_8A56_3A64C9AE7185__INCLUDED_)
#define AFX_CRASHDISTANCEARRAY_H__B5DF0017_3FB8_4925_8A56_3A64C9AE7185__INCLUDED_

#include "CrashDistance.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCrashDistanceArray : public CArray<CCrashDistance,CCrashDistance&>  
{
public:
	bool IsToRequestWayTele(int nDIndex, int nBlockway);
	CCrashDistance* GetPCrashDistance(int nCDistIndex);
	CCrashDistanceArray();
	virtual ~CCrashDistanceArray();

};

typedef CArray<CCrashDistanceArray,CCrashDistanceArray> CArrayCrashDistanceArray;

#endif // !defined(AFX_CRASHDISTANCEARRAY_H__B5DF0017_3FB8_4925_8A56_3A64C9AE7185__INCLUDED_)
