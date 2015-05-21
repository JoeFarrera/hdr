// BlockTableAGVTypePair.h: interface for the CBlockTableAGVTypePair class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKTABLEAGVTYPEPAIR_H__EA2F33C0_6CF2_409A_8A99_F187A3996FE3__INCLUDED_)
#define AFX_BLOCKTABLEAGVTYPEPAIR_H__EA2F33C0_6CF2_409A_8A99_F187A3996FE3__INCLUDED_

#include "CrashDistanceArray.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBlockTableAGVTypePair  
{
public:
	void Fill(DEF_BLOCK_TABLE* pBlockTables);
	bool IsToRequestWayTele(int nDIndex,int nMyBlockway);
	//bool ReadBinary();		JLac
	CCrashDistance* GetPCrashDistance(int nTDistIndex,int nCDistIndex);
	CBlockTableAGVTypePair();
	virtual ~CBlockTableAGVTypePair();

private:
	bool ReadCrashDistances(CFile& myFile,CUIntArray& arrDIndex);
	bool ReadIndexVector(CFile& myFile,CUIntArray& arrDIndex);
	bool IndexIsValid(int nIndex);
	int m_nTopAGVType;
	int m_nCompAGVType;
	CArrayCrashDistanceArray m_arrCrashDistanceArray;
public:
	CCrashDistance* GetGegenCrash(CCrashDistance* pCrashDist, int nDistIndex);
	CCrashDistanceArray* GetPtrArrCrashDistances(int nDistIndex);
};

#endif // !defined(AFX_BLOCKTABLEAGVTYPEPAIR_H__EA2F33C0_6CF2_409A_8A99_F187A3996FE3__INCLUDED_)
