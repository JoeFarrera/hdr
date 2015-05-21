// Destination.h: interface for the CDestination class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESTINATION_H__3163F69A_54C0_4C2B_84F5_05753C8C045C__INCLUDED_)
#define AFX_DESTINATION_H__3163F69A_54C0_4C2B_84F5_05753C8C045C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDestination  
{
public:
	void SetDestino (TYPE_STATION_DEF * pDest);
	CDestination(TYPE_STATION_DEF * pDest = NULL);
	virtual ~CDestination();
	bool operator==(TYPE_STATION_DEF dest);
	TYPE_STATION_DEF * m_pDest;	// Destination pointer

private:
};

#endif // !defined(AFX_DESTINATION_H__3163F69A_54C0_4C2B_84F5_05753C8C045C__INCLUDED_)
