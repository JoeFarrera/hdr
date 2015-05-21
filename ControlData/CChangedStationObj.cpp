#include "..\stdafx.h"
#include "CChangedStationObj.h"

CChangedStationObj::CChangedStationObj()
{
	m_nPSIndex       = 0;
	m_nChangeObjects = 0;
}

CChangedStationObj::~CChangedStationObj()
{
}

CChangedStationObj::CChangedStationObj(unsigned int nPSIndex,unsigned int nChangeObjects)
{
	m_nPSIndex       = nPSIndex;
	m_nChangeObjects = nChangeObjects;
}

bool CChangedStationObj::HasChangeObjects()
{
	return (m_nChangeObjects != 0);
}

void CChangedStationObj::AddObject(unsigned int nObject)
{
	m_nChangeObjects |= nObject;
}
