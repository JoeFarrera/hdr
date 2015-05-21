#ifndef _CAGVTDESTINATION_H_
#define _CAGVTDESTINATION_H_

#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <string>
#include <iterator> 
using namespace std;

// TODO ML #include "FixArray.h"

#include "CAIDStation.h"
#include "CCustomerStation.h"
#include "CAGVTDestPosition.h"
#include "CAGVTLayer.h"
#include "CAGVTypeStation.h"

class CAGVTDestination
{
public:
	void WriteForTest(FILE* pFile, unsigned int nIndex);
	void GetPSIndexListWithChildren(unsigned int nAGVT,unsigned int nPSIndex, CChangedStationObjList& listChangeStations,CAGVTDestPositionArray& arrDestPos,CAGVTLayerArray& arrLayer);
	unsigned int GetAllStationObjects(unsigned int            nAGVT,
									  unsigned int            nPSIndex,
									  CAGVTDestPositionArray& arrDestPos,
									  CAGVTLayerArray&        arrLayer);	
	void GetAGVTypeStationList(short                   nAGVType,
							   unsigned int            nAIDPSIndex,
							   CAGVTypeStationList&    listAGVTStations,
							   CAGVTDestPositionArray& arrDestPositions,
							   CAGVTLayerArray& 	   arrLayer);
	void LinkLeftPosToPhysicalStation(CAGVTDestPositionArray& arrDestPositions,
									  CAGVTLayerArray& arrLayer,
									  CAIDStationArray& arrAIDStations,
									  CCustomerStationList& listCustomerStations);
	void LinkRightPosToPhysicalStation(CAGVTDestPositionArray& arrDestPositions,
									   CAGVTLayerArray& arrLayer,
									   CAIDStationArray& arrAIDStations,
									   CCustomerStationList& listCustomerStations);
	void SetLeftPSIndex (unsigned int nLeftPSIndex );
	void SetRightPSIndex(unsigned int nRightPSIndex);
	void SetLeftPositions (unsigned short nLeftPositions );
	void SetRightPositions(unsigned short nRightPositions);
	void SetFirstPosIndex (unsigned int nFirstPosIndex){m_nFirstPosIndex = nFirstPosIndex;}
  short GetDestThoroughfare();
	short GetDestTypeLeft ();
	short GetDestTypeRight();
	unsigned int GetFirstDestPosIndex(char cSide);
	unsigned int GetPSIndex(char cSide);
	unsigned int GetFirstPosIndex();
	unsigned int GetLeftPositions();
	unsigned int GetRightPositions(){return m_nRightPositions;}
	int GetNumber(){return m_nNumber;}
	unsigned short GetDestArea(){return m_nDestArea;}
	int GetNCp(){return m_nNCp;}
	CAGVTDestination();
	CAGVTDestination(int nDest,int nNCp,bool bNoThoroughfare,short nDestTypeLeft,short nDestTypeRight,unsigned short nDestArea);
	virtual ~CAGVTDestination();
	
private:
	unsigned int GetPositionOrLayerObjects(unsigned int            nAGVT,
										   unsigned int            nPSIndex,
										   char                    cSide,
										   CAGVTDestPositionArray& arrDestPos,
										   CAGVTLayerArray&        arrLayer);
	
	unsigned int GetAllPositions(int nPositions);	

	unsigned int   m_nLeftPossibleActions;
	unsigned int   m_nRightPossibleActions;
	unsigned short m_nLeftPositions;
	unsigned short m_nRightPositions;
	int            m_nNCp;
	int            m_nNumber;
	unsigned int   m_nLeftPSIndex;
	unsigned int   m_nRightPSIndex;
	short          m_nDestTypeRight;
	short          m_nDestTypeLeft;
	unsigned int   m_nFirstPosIndex;
	unsigned short m_nDestArea;
	bool           m_bNoThoroughfare;	
	
};

typedef CList <CAGVTDestination, CAGVTDestination&> CAGVTDestinationList;
typedef CArray<CAGVTDestination, CAGVTDestination&> CAGVTDestinationArray;

#endif //_CAGVTDESTINATION_H_
