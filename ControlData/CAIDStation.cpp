#include "..\stdafx.h"
#include "konstant.h"
#include "CAIDStation.h"

CAIDStation::CAIDStation()
{
	m_nAllObjects     = 0;
	m_nPSType         = 0;
	m_nLoaded         = 0;
	m_nError          = 0;
	m_nClosed         = 0;
	m_nReserved       = 0;
	m_nPriorityTime   = 0;
	m_nAGVTypesPickup = 0;
	m_nAGVTypesUnload					= 0;
	m_nAGVTypesPseudoStation	= 0;
	m_nAGVTypesInterimStation	= 0; 
	m_nAGVTypesBattCharge			= 0; 
	m_nAGVTypesHomeStation		= 0; 
	
	m_nAGVTypeStationIndex = 0;
	m_nAGVTypeStations     = 0;
}

CAIDStation::CAIDStation(CString& strName,unsigned char nPSType)
{
	m_strPSName = strName;
	m_nPSType   = nPSType;
	m_nAllObjects     = 0;
	m_nLoaded         = 0;
	m_nError          = 0;
	m_nClosed         = 0;
	m_nReserved       = 0;
	m_nPriorityTime   = 0;
	m_nAGVTypesPickup = 0;
	m_nAGVTypesUnload = 0;
	m_nAGVTypesPseudoStation  = 0;
	m_nAGVTypesInterimStation = 0;
	m_nAGVTypesBattCharge			= 0; 
	m_nAGVTypesHomeStation		= 0; 
	
	m_nAGVTypeStationIndex = 0;
	m_nAGVTypeStations     = 0;
}

CAIDStation::~CAIDStation()
{
}

bool CAIDStation::operator==(const CAIDStation &tAIDS) const
{
	return (m_strPSName == tAIDS.m_strPSName);
}

CString CAIDStation::GetPSName()
{
	return m_strPSName;
}

void CAIDStation::SetAGVTypesPickup(unsigned int nAGVTypesPickup)
{
	m_nAGVTypesPickup = nAGVTypesPickup;
}

void CAIDStation::SetAGVTypesUnload(unsigned int nAGVTypesUnload)
{
	m_nAGVTypesUnload = nAGVTypesUnload;
}

void CAIDStation::SetAGVTypesHomeStation(unsigned int nAGVTypesHomeStation)
{
	m_nAGVTypesHomeStation = nAGVTypesHomeStation;
}

void CAIDStation::SetAGVTypesBattCharge(unsigned int nAGVTypesBattCharge)
{
	m_nAGVTypesBattCharge = nAGVTypesBattCharge;
}

void CAIDStation::SetAGVTypesPseudoStation(unsigned int nAGVTypesPseudoStation)
{
	m_nAGVTypesPseudoStation = nAGVTypesPseudoStation;
}

void CAIDStation::SetAGVTypesInterimStation(unsigned int nAGVTypesInterimStation)
{
	m_nAGVTypesInterimStation = nAGVTypesInterimStation;
}

unsigned int CAIDStation::GetAGVTypesOfAction(int nAction)
{
	unsigned int nAGVTypes = 0;
	switch(nAction)
	{
		case PICKUP:
			nAGVTypes = m_nAGVTypesPickup;
			break;
		case UNLOAD:
			nAGVTypes = m_nAGVTypesUnload;
			break;
		case HOME:
			nAGVTypes = m_nAGVTypesHomeStation;
			break;
		case BATTCHARGE:
			nAGVTypes = m_nAGVTypesBattCharge;
			break;
		case PSEUDO:
			nAGVTypes = m_nAGVTypesPseudoStation;
			break;
		case INTERIM:
			nAGVTypes = m_nAGVTypesInterimStation;
			break;
	}
	return nAGVTypes;
}

unsigned int CAIDStation::GetAGVTypesOfActions(int nActions)
{
	unsigned int nAGVTypes = 0;
	nAGVTypes |= (nActions & PICKUP    ) ? m_nAGVTypesPickup         : 0;
	nAGVTypes |= (nActions & UNLOAD    ) ? m_nAGVTypesUnload         : 0;
	nAGVTypes |= (nActions & HOME      ) ? m_nAGVTypesHomeStation    : 0;
	nAGVTypes |= (nActions & BATTCHARGE) ? m_nAGVTypesBattCharge     : 0;
	nAGVTypes |= (nActions & PSEUDO    ) ? m_nAGVTypesPseudoStation  : 0;
	nAGVTypes |= (nActions & INTERIM   ) ? m_nAGVTypesInterimStation : 0;
	return nAGVTypes;
}

void CAIDStation::SetLoaded(bool bLoaded,unsigned int nChangeObjects)
{
	if(bLoaded)
	{
		m_nLoaded |= nChangeObjects;
	}
	else
	{
		m_nLoaded &= ~nChangeObjects;
	}
}

void CAIDStation::SetClosed(bool bClose,unsigned int nChangeObjects)
{
	if(bClose)
	{
		m_nClosed |= nChangeObjects;
	}
	else
	{
		m_nClosed &= ~nChangeObjects;
	}
}

void CAIDStation::SetReserved(bool bReserved,unsigned int nChangeObjects)
{
	if(bReserved)
	{
		m_nReserved |= nChangeObjects;
	}
	else
	{
		m_nReserved &= ~nChangeObjects;
	}
}

void CAIDStation::SetError(bool bError,unsigned int nChangeObjects)
{
	if(bError)
	{
		m_nError |= nChangeObjects;
	}
	else
	{
		m_nError &= ~nChangeObjects;
	}
}

void CAIDStation::SetPriorityTime(unsigned int nPrioTime)
{
	m_nPriorityTime = nPrioTime;
}


bool CAIDStation::IsLoaded()
{
	return ((m_nAllObjects & m_nLoaded) == m_nAllObjects);
}

bool CAIDStation::IsClosed()
{
	return ((m_nAllObjects & m_nClosed) == m_nAllObjects);
}

bool CAIDStation::IsError()
{
	return ((m_nAllObjects & m_nError) == m_nAllObjects);
}

bool CAIDStation::IsReserved()
{
	return ((m_nAllObjects & m_nReserved) == m_nAllObjects);
}

void CAIDStation::Save(FILE* pFile)
{
	char szStationName[100];
	memset(szStationName,0,100);
	
	unsigned int anValue[6] = {m_nAllObjects,m_nLoaded,m_nClosed,m_nReserved,m_nError,m_nPriorityTime};
	strcpy_s(szStationName,m_strPSName);
	fwrite(szStationName,sizeof(char), LENGTH_STATION_NAME,pFile);
	fwrite(&m_nPSType   ,sizeof(char),          1,pFile);
	fwrite(anValue      ,sizeof( int),          6,pFile);
}

bool CAIDStation::ReadSavedStation(FILE* pFile)
{
	unsigned int anValue[6];
	unsigned char nPSType;
	char szStationName[100];
	memset(szStationName,0,100);
	
	bool bDataOK = true;
	bDataOK  &= (fread(szStationName,sizeof(char), LENGTH_STATION_NAME,pFile) == LENGTH_STATION_NAME);
	bDataOK  &= (fread(&nPSType     ,sizeof(char),          1,pFile) == 1);
	bDataOK  &= (fread(anValue      ,sizeof( int),          6,pFile) == 6);
	if(bDataOK)
	{
		CString strPSName(szStationName);
		m_nAllObjects   = anValue[0];
		m_nLoaded       = anValue[1];
		m_nClosed       = anValue[2];
		m_nReserved     = anValue[3];
		m_nError	      = anValue[4];
		m_nPriorityTime = anValue[5];
		m_nPSType       = nPSType;
		m_strPSName     = strPSName;
	}
	return bDataOK;
}

void CAIDStation::Restore(CAIDStation& tAIDS)
{
	if ((m_nPSType     == tAIDS.m_nPSType) &&
		  (m_nAllObjects == tAIDS.m_nAllObjects))
	{
		m_nLoaded       = (m_nAllObjects & tAIDS.m_nLoaded  );
		m_nClosed       = (m_nAllObjects & tAIDS.m_nClosed  );
		m_nReserved     = (m_nAllObjects & tAIDS.m_nReserved);
		m_nError        = (m_nAllObjects & tAIDS.m_nError   );
		m_nPriorityTime = tAIDS.m_nPriorityTime;
	}
}
