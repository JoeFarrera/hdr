#ifndef _CCOMMPOINT_H_
#define _CCOMMPOINT_H_




#include "konstant.h"

class CCommPoint
{
public:
	void PrintTravelTimes();
	unsigned short GetTravelTime(unsigned int nDestIndex);
	bool SetSizeArrayTravelTime(unsigned int nDestinations );
	void SetTravelTime(unsigned int nIndex,unsigned short nTravelTime);
	unsigned char SleepPermission(){return (m_nProperties & MP_FUER_SCHLAFEN);}
	unsigned char DoNotChangeDispo(){return (m_nProperties & MP_KEINE_UMDISPO);}
	unsigned char FreeOnFireAlarm(){return (m_nProperties & MP_BEI_BRAND_FREI);}
	bool IsDecisionPoint(){return (m_nDpIndex != 0);}
	void WriteForTest(FILE* pFile,int nIndex);
	void SetNCps(int nNCps){m_nNCps = (unsigned char)nNCps;}
	void SetDistanceIndex(unsigned int nDIndex){m_nDistanceIndex = nDIndex;}
	int GetCp(){return m_nCp;}
	unsigned short GetCpArea (){return m_nCpArea;}
	unsigned short GetDpIndex(){return m_nDpIndex;}
	unsigned char GetCtrlArea(){return m_nCtrlArea;}
	unsigned char GetCommArea(){return m_nCommArea;}
	unsigned int  GetDistanceIndex(){return m_nDistanceIndex;}
	unsigned char GetNCps(){return m_nNCps;}
	CCommPoint();
	CCommPoint( int            nCp,
				unsigned char  nCtrlArea,
				unsigned char  nCommArea,
				unsigned short nCpArea,
				unsigned short nCpIndex,
				unsigned char  nProperties,
				unsigned short nDpIndex);
	virtual ~CCommPoint();
	void operator=(const CCommPoint& other);
	
private:
	unsigned short		m_nCp;
	unsigned char		m_nNCps;
	unsigned short  	m_nCpIndex;
	unsigned char   	m_nProperties;
	unsigned char   	m_nCtrlArea;
	unsigned char   	m_nCommArea;
	unsigned short  	m_nCpArea;
	unsigned short  	m_nDpIndex;
	unsigned int		m_nDistanceIndex;
	CUIntArray			m_arrTravelTimes;	// Was CUShortArray on OS800
	
};

typedef CArray<CCommPoint, CCommPoint&> CCommPointArray;

#endif //_CCOMMPOINT_H_
