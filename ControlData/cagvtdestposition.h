#ifndef _CAGVTDESTPOSITION_H_
#define _CAGVTDESTPOSITION_H_


#include "CAGVTypeStation.h"
#include "CAGVTLayer.h"
#include "CAIDStation.h"
#include "CCustomerStation.h"
#include "CChangedStationObj.h"

class CAGVTDestPosition
{
public:
	void WriteForTest(FILE* pFile,unsigned int nIndex);
	void GetPSIndexListWithChildren(unsigned int nAGVT,int nDest,char cSide,unsigned int nSeekIndex,
		                       		CChangedStationObjList& listChangedStations,CAGVTLayerArray& arrLayer);

	unsigned int GetPositionOrLayerObjects(unsigned int nAGVT,
										   int nDest,
										   char cSide,
										   unsigned int nPSIndex,
										   CAGVTLayerArray& arrLayer);
	
	void LinkToPhysicalStation(int                   nDest,
							   char                  cSide,
							   CAGVTLayerArray&      arrLayer,
							   CAIDStationArray&     arrAIDStations,
							   CCustomerStationList& listCustomerStations);
	void GetAGVTypeStationList(short                nAGVType,
							   int					nDest,
							   char					cSide,
							   unsigned int         nAIDPSIndex,
							   CAGVTypeStationList& listAGVTStations,
							   CAGVTLayerArray& 	arrLayer);
	void SetLayers(unsigned short nLayers);
	void SetFirstLayerIndex(unsigned int nFirstLayerIndex);
	unsigned short GetLayers();
	int GetPosWay(){return m_nPosWay;}
	unsigned int GetFirstPhysicalStationIndex(){return m_nPSIndexDP;}
	CAGVTDestPosition();
	CAGVTDestPosition(int nPos,int nPosWay,char cSide);
	virtual ~CAGVTDestPosition();
	
private:
	unsigned int   m_nFirstLayerIndex;
	unsigned short m_nLayers;
	unsigned int   m_nPSIndexDP;
	char           m_cSide;
	int            m_nPosWay;
	int            m_nPos;
	
};

typedef CArray<CAGVTDestPosition, CAGVTDestPosition&> CAGVTDestPositionArray;

#endif //_CAGVTDESTPOSITION_H_
