#include "..\stdafx.h"
#include "CAGVTypeStation.h"

CAGVTypeStation::CAGVTypeStation()
{
}

CAGVTypeStation::CAGVTypeStation(short nAGVType,int nNumber,char cSide,short nPos,unsigned char nLayer,int nPosWay,unsigned int nPSIndex)
{
	m_nAGVType        = nAGVType;
	m_nPosWay         = nPosWay;

	m_tStation.nDest  = nNumber;
	m_tStation.ucSide = cSide;
	m_tStation.nPos   = nPos;
	m_tStation.nLayer = nLayer;

	m_nPSIndex        = nPSIndex;
}

CAGVTypeStation::CAGVTypeStation(TYPE_STATION_DEF& tStation,short nAGVType)
{
	m_nAGVType = nAGVType;
	m_nPosWay  = 0;

	m_tStation = tStation;

	m_nPSIndex = 0;
}

CAGVTypeStation::~CAGVTypeStation()
{
}

unsigned int CAGVTypeStation::GetAGVType()
{
	return m_nAGVType;
}

TYPE_STATION_DEF CAGVTypeStation::GetStation()
{
	return m_tStation;
}

bool CAGVTypeStation::operator==(const CAGVTypeStation& tS) 
{
	return ((m_tStation.nDest  == tS.m_tStation.nDest ) &&
			(m_tStation.ucSide == tS.m_tStation.ucSide) &&
			(m_tStation.nPos   == tS.m_tStation.nPos  ) &&
			(m_tStation.nLayer == tS.m_tStation.nLayer) &&
			(m_nAGVType        == tS.m_nAGVType      ));
}
