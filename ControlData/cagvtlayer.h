#ifndef _CAGVTLAYER_H_
#define _CAGVTLAYER_H_

#include "CAIDStation.h"
#include "CCustomerStation.h"

class CAGVTLayer
{
public:
	void LinkToPhysicalStation(int                   nDest,
							   char                  cSide,
							   short                 nPos,
							   CAIDStationArray&     arrAIDStations,
							   CCustomerStationList& listCustomerStations);
	unsigned int GetPSIndex(){return m_nPSIndex;}
	CAGVTLayer();
	CAGVTLayer(unsigned char nLayer);
	virtual ~CAGVTLayer();
	
private:
	unsigned char m_nLayer;
	unsigned int  m_nPSIndex; 
	
};

typedef CArray<CAGVTLayer, CAGVTLayer&> CAGVTLayerArray;

#endif //_CAGVTLAYER_H_
