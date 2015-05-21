// CrashDistance.h: interface for the CCrashDistance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRASHDISTANCE_H__184C0946_C4AB_4BA6_9E75_E78C4BF1E6B0__INCLUDED_)
#define AFX_CRASHDISTANCE_H__184C0946_C4AB_4BA6_9E75_E78C4BF1E6B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCrashDistance  
{
public:
	bool IsToRequestWayTele(int nDIndex, int nMyBlockway);
	bool BlockWayWithoutCollision();
	bool RelCollWayIsAbsCollWay();
	bool CrossOverRelFreeArea(int nDrivenWay);
	bool CrossOverRelCollWay(int nDrivenWay);
	bool HasRelCollWay();
	bool CrossOverFreeWay(int nDrivenWay);
	bool HasFreeWay();
	bool CrossOverCollWayBlkWay(int nDrivenWay);
	bool HasCollWayBlkWay();
	bool CrossOverAbsCollWay(int nDrivenWay);
	bool HasAbsCollWay();
	bool ItsMe(int nCDistIndex);
	CCrashDistance(	int nIndex,
					int nFreeWay,
					int nAbsCollWay,
					int nRelCollWay,
					int nRelFreeWay,
					int nCollWayBlkWay);
	CCrashDistance();
	virtual ~CCrashDistance();

private:
	int m_nIndex;
	int m_nFreeWay;
	int m_nAbsCollWay;
	int m_nRelCollWay;
	int m_nRelFreeWay;
	int m_nCollWayBlkWay;
public:
	unsigned short GetIndex();
	void SetValues(int nIndex,int nFreeWay,int nAbsCollWay,int nRelCollWay,int nRelFreeWay,int nCollWayBlkWay);
};

//typedef CArray<CCrashDistance,CCrashDistance&> CCrashDistanceArray;

#endif // !defined(AFX_CRASHDISTANCE_H__184C0946_C4AB_4BA6_9E75_E78C4BF1E6B0__INCLUDED_)
