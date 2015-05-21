// DistanceReqWayTele.h: interface for the CDistanceReqWayTele class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTANCEREQWAYTELE_H__C82D2611_6690_4BDC_A319_27DC92749B38__INCLUDED_)
#define AFX_DISTANCEREQWAYTELE_H__C82D2611_6690_4BDC_A319_27DC92749B38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDistanceReqWayTele  
{
public:
	bool operator ==(const CDistanceReqWayTele &myDRWT) const;
	CDistanceReqWayTele();
	CDistanceReqWayTele(int nAGVType,int nCp,int nNCp);
	virtual ~CDistanceReqWayTele();

private:
	int m_nAGVType;
	int m_nNCp;
	int m_nCp;
};

typedef CMap <ULONG,ULONG&,CDistanceReqWayTele,CDistanceReqWayTele&>CDistanceReqWayTeleMap;

#endif // !defined(AFX_DISTANCEREQWAYTELE_H__C82D2611_6690_4BDC_A319_27DC92749B38__INCLUDED_)
