#ifndef _CAIDSTATION_H_
#define _CAIDSTATION_H_

#define PS_DEST		0
#define PS_POS		1
#define PS_LAYER	2

class CAIDStation
{
public:
	void Save(FILE* pFile);
	void Restore(CAIDStation& tAIDS);
	bool ReadSavedStation(FILE* pFile);
	bool IsLoaded();
	bool IsClosed();
	bool IsError();
	bool IsReserved();
	void SetClosed  (bool bClose   ,unsigned int nChangeObjects);
	void SetLoaded  (bool bLoaded  ,unsigned int nChangeObjects);
	void SetReserved(bool bReserved,unsigned int nChangeObjects);
	void SetError   (bool bError   ,unsigned int nChangeObjects);
  void SetPriorityTime(unsigned int nPrioTime);
	unsigned int GetAGVTypesOfAction(int nAction);
	unsigned int GetAGVTypesOfActions(int nActions);
	void SetAllObjects(unsigned int nAllObjects){m_nAllObjects = nAllObjects;}
	void SetAGVTypesPickup        (unsigned int nAGVTypesPickup);
	void SetAGVTypesUnload        (unsigned int nAGVTypesUnload);
	void SetAGVTypesHomeStation   (unsigned int nAGVTypesHomeStation);
	void SetAGVTypesBattCharge    (unsigned int nAGVTypesBattCharge);
	void SetAGVTypesPseudoStation (unsigned int nAGVTypesPseudoStation);
	void SetAGVTypesInterimStation(unsigned int nAGVTypesInterimStation);
	void SetAGVTypeStations       (unsigned short nStations){m_nAGVTypeStations = nStations;}
	void SetAGVTypeStationIndex   (unsigned int   nIndex){m_nAGVTypeStationIndex = nIndex;}
	CString GetPSName();
	unsigned char GetPSType      (){return m_nPSType;}
	unsigned char GetPriorityTime(){return m_nPriorityTime;}
	unsigned int  GetAllObjects  (){return m_nAllObjects;}
	unsigned int  GetLoaded      (){return m_nLoaded;}
	unsigned int  GetClosed      (){return m_nClosed;}
	unsigned int  GetReserved    (){return m_nReserved;}
	unsigned int  GetError       (){return m_nError;}
	unsigned short GetAGVTypeStations    (){return m_nAGVTypeStations;}
	unsigned int   GetAGVTypeStationIndex(){return m_nAGVTypeStationIndex;}
	bool operator==(const CAIDStation &tAIDS) const;
	CAIDStation();
	CAIDStation(CString& strName,unsigned char nPSType);
	virtual ~CAIDStation();
	
private:
	unsigned int m_nAllObjects;
	unsigned int m_nAGVTypesPickup;
	unsigned int m_nAGVTypesUnload;
	unsigned int m_nAGVTypesHomeStation;
	unsigned int m_nAGVTypesBattCharge;
	unsigned int m_nAGVTypesPseudoStation;
	unsigned int m_nAGVTypesInterimStation;
	unsigned int m_nPriorityTime;
	unsigned int m_nLoaded;
	unsigned int m_nReserved;
	unsigned int m_nError;
	unsigned int m_nClosed;
	unsigned char m_nPSType;
	CString m_strPSName;
	unsigned int   m_nAGVTypeStationIndex;
	unsigned short m_nAGVTypeStations;
};

typedef CList <CAIDStation, CAIDStation&> CAIDStationList;
typedef CArray<CAIDStation, CAIDStation&> CAIDStationArray;

#endif //_CAIDSTATION_H_
