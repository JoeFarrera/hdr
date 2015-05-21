#ifndef _CCUSTOMERSTATION_H_
#define _CCUSTOMERSTATION_H_

#include "CAIDStation.h"

class CCustomerStation
{
public:
	int FindAIDStation(CAIDStationArray& arrAIDStations);
	int GetDestNr();
	unsigned char GetPos();
	unsigned char GetLayer();
	char GetSide();
	CString GetCName();
	bool operator==(const CCustomerStation &tCS) const;
	CCustomerStation();
	CCustomerStation(int nDest);
	CCustomerStation(int nDest,char cSide, unsigned char nPos, unsigned char nLayer,CString& strName);
	virtual ~CCustomerStation();
	
private:
	CString       m_strCName;
	unsigned char m_nLayer;
	unsigned char m_nPos;
	char          m_cSide;
	int           m_nDestNr;
};

typedef CList <CCustomerStation,CCustomerStation&> CCustomerStationList;

#endif //_CCUSTOMERSTATION_H_
