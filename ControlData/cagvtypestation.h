#ifndef _CAGVTYPESTATION_H_
#define _CAGVTYPESTATION_H_
/* Include files */

#include "konstant.h"
#include "komdat.h"


class CAGVTypeStation
{
public:
	TYPE_STATION_DEF GetStation();
	int GetPosWay(){return m_nPosWay;}
	unsigned int GetAGVType();
	bool operator==(const CAGVTypeStation& tS); 
	CAGVTypeStation();
	CAGVTypeStation(short nAGVType,int nNumber,char cSide,short nPos,unsigned char nLayer,int nPosWay,unsigned int nPSIndex);
	CAGVTypeStation(TYPE_STATION_DEF& tStation,short nAGVType);
	virtual ~CAGVTypeStation();

private:
	unsigned int     m_nPSIndex;
	unsigned int     m_nAGVType;
	TYPE_STATION_DEF m_tStation;
	int              m_nPosWay;
};

typedef CList<CAGVTypeStation, CAGVTypeStation& > CAGVTypeStationList;
typedef CArray<CAGVTypeStation, CAGVTypeStation&> CAGVTypeStationArray;

#endif //_CAGVTYPESTATION_H_
