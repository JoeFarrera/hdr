#ifndef _CCPDISTANCE_H_
#define _CCPDISTANCE_H_


// TODO ML #include "FixArray.h"

class CCpDistance
{
public:
	void WriteForTest(FILE* pFile,unsigned int nIndex);
	unsigned short GetNCp(){return m_nNCp;}
	unsigned short GetDistIndex(){return m_nDistIndex;}
	unsigned short GetBlockWay(){return m_nBlockWay;}
	unsigned short GetLength(){return m_nLength;}
	unsigned short GetTravelTime(){return m_nTravelTime;}
	unsigned short GetRealTravelTime(){return m_nRealTravelTime;}
	void SetRealTravelTime(unsigned short nRealTravelTime){m_nRealTravelTime = nRealTravelTime;}
  void SetNDp(unsigned short nNDp){m_nNDp = nNDp;}
	CCpDistance();
	CCpDistance(unsigned short nDistIndex,
				unsigned short nNCp,
				unsigned short nLength,
				unsigned short nBlockWay,
				unsigned short nProperties,
				unsigned short nNDp,
				unsigned short nTravelTime);
	virtual ~CCpDistance();
private:
	unsigned short	m_nNCp;
	unsigned short	m_nTravelTime;
	unsigned short	m_nBlockWay;
	unsigned short	m_nLength;
	unsigned short  m_nNDp;
	unsigned short	m_nProperties;
	unsigned short	m_nDistIndex;
  unsigned short  m_nRealTravelTime;
};

typedef CArray<CCpDistance, CCpDistance&> CCpDistanceArray;

#endif //_CCPDISTANCE_H_
