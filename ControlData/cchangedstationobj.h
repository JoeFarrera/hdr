#ifndef _CCHANGEDSTATIONOBJ_H_
#define _CCHANGEDSTATIONOBJ_H_


class CChangedStationObj
{
public:
	void AddObject(unsigned int nObject);	
	bool HasChangeObjects();
	unsigned int GetChangeObjects(){return m_nChangeObjects;}
	unsigned int GetPSIndex(){return m_nPSIndex;}
	CChangedStationObj();
	CChangedStationObj(unsigned int nPSIndex,unsigned int nChangeObjects);
	virtual ~CChangedStationObj();

private:
	unsigned int m_nChangeObjects;
	unsigned int m_nPSIndex;
};

typedef CList <CChangedStationObj,CChangedStationObj&> CChangedStationObjList;

#endif //_CCHANGEDSTATIONOBJ_H_
