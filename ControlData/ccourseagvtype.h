#ifndef _CCOURSEAGVTYPE_H_
#define _CCOURSEAGVTYPE_H_
//--------------------------------------------------------------------
//Copyright E&K AUTOMATION, Indumat GmbH & Co. KG
//
//Author: J.Lachmund
//
//Description:
//This is the Header file of the class CCourseAGVType.
//
//Documentation of changes:
//28.04.2010,	Lachmund:	GetExpectedTimeAndLengthTotal() neu
//2006-08-03:	Lachmund: new
//--------------------------------------------------------------------


#include "CCustomerStation.h"
#include "CAIDStation.h"
#include "CAGVTDestination.h"
#include "CAGVTDestPosition.h"
#include "CAGVTLayer.h"
#include "CDestinationType.h"
#include "CColumnType.h"
#include "CLayerType.h"
#include "CDestTypePos.h"
#include "CAGVTypeStation.h"
#include "CLoaderFile.h"
#include "CCommPoint.h"
#include "CCpDistance.h"
#include "CBestWays.h"
#include "CCtrlDataMemFile.h"


#define USHORT_UNENDLICH 0xFFFF
//Definition of Cp Distance from SoFTSys:

typedef struct	DEF_HOME_OR_CHARGESTATION{
	TYPE_STATION_DEF tStation;
	unsigned short	nProperties;
				} DEF_HOME_OR_CHARGESTATION;

typedef CList<DEF_HOME_OR_CHARGESTATION, DEF_HOME_OR_CHARGESTATION& >TYPE_LIST_HOCS_DEF;
typedef CArray<DEF_HOME_OR_CHARGESTATION, DEF_HOME_OR_CHARGESTATION&>CHomeOrChargeStationArray;

class CCourseAGVType
{
public:
	void GetExpectedTimeAndLengthTotal(	unsigned short nStartCp,
																			unsigned short nDestCp,
																			int& nTimeTotal,
																			int& nLengthTotal);
  void SetRealTravelTimeDistance(unsigned short nDistIndex, unsigned short nRealTravelTime);
	void PrintTravelTimes();
	void SetAGVs(int nAGVs){m_nAGVs = nAGVs;}
	void SetSpeeds(int nFWSpeed,int nBWSpeed,int nCFWSpeed,int nCBWSpeed);
	unsigned short GetTravelTime(int nCp,int nDest);
	bool CalculateDispoTravelTimes();
	CCpDistance* GetPtrDistance(int nIndex){return &(m_arrCpDistances[nIndex - 1]);}
	//DEF_CP_DISTANCE* GetPtrDistance(int nIndex){return m_pCpDistances + nIndex -1;}
	unsigned int GetCommPointIndex(int nCp){return m_arrCommPointIndex[nCp];}
	CCommPoint* GetPtrCommPoint(int nCp);
	CCommPoint* GetPtrCommPointByIndex(unsigned int nIndex);
	
	bool LoadCommPoints					(CCtrlDataMemFile& myMFile);
	bool LoadCommPointDistances	(CCtrlDataMemFile& myMFile);
	bool LoadDestinations				(CCtrlDataMemFile& myMFile);
	bool LoadDestPositions			(CCtrlDataMemFile& myMFile);
	bool LoadBestWays		   			(CCtrlDataMemFile& myMFile);
	
	
	//bool LoadCommPointDistancesPointer(CLoaderFile* pLFile);
	bool LoadCommPoints					(CLoaderFile* pLFile);
	bool LoadCommPointDistances	(CLoaderFile* pLFile);
	bool LoadDestPositions			(CLoaderFile* pLFile);
	bool LoadDestinations				(CLoaderFile* pLFile);
	bool LoadBestWaysPointer		(CLoaderFile* pLFile);
	bool LoadBestWays		   			(CLoaderFile* pLFile);
	unsigned int GetNumberDestinations(){return m_arrDestinations.GetSize();}
	CAGVTDestination* GetPtrDestination     (int nDest);
	CAGVTDestination* GetPtrDestinationByIndex(unsigned int nIndex);
	unsigned int GetNumberDestPositions(){return m_arrDestPositions.GetSize();}
	unsigned int GetNumberHOCStations();
	DEF_HOME_OR_CHARGESTATION GetHOCStation(int nIndex);
	int GetBestTravelTime(int nCp,int nDest);
	int GetMaxCp(){return m_arrCommPoints[m_arrCommPoints.GetSize() - 1].GetCp();}
	unsigned int GetNumberDistances(){return m_arrCpDistances.GetSize();}
	unsigned int GetNumberCps(){return m_arrCommPoints.GetSize();}
	int GetAGVs(){return m_nAGVs;}
	int GetAGVLength(){return m_nAGVLength;}
	int GetAGVFWSpeed(){return m_nFWSpeed;}
	int GetAGVBWSpeed(){return m_nBWSpeed;}
	int GetAGVCFWSpeed(){return m_nCFWSpeed;}
	int GetAGVCBWSpeed(){return m_nCBWSpeed;}
	short GetAGVType(){return m_nAGVType;}
	void GetPSIndexListWithChildren(unsigned int nPSIndex, CChangedStationObjList& listChangeStations);	
	unsigned int GetAIDStationIndex(int nDest, char cSide, int nPos, int nLayer,int& nError);
	unsigned int GetAllStationObjects(unsigned int nAIDPSIndex); 
	void GetAGVTypeStationList(unsigned int nAIDPSIndex,CAGVTypeStationList& listAGVTStations);
	bool IsActionOfStation(int                    nAction,
						   TYPE_STATION_DEF&      tStation,
						   CUIntArray&            arrDestTypeIndex,
						   CDestinationTypeArray& arrDestTypes,
						   CDestTypePosList&      listDestTypePos,
						   CUIntArray&            arrColumnTypeIndex,
						   CColumnTypeArray&      arrColumnTypes,
						   CUIntArray&            arrLayerTypeIndex,
						   CLayerTypeArray&       arrLayerTypes);
	void MakeLayerArray();
	void FillLayersInDestPositions(CUIntArray& arrDestTypeIndex,CDestinationTypeArray& arrDestTypes,
								   CUIntArray& arrColumnTypeIndex,CColumnTypeArray& arrColumnType,
								   CDestTypePosList& listDestTypePos);
	void SetDestPositionsInDestinations(CUIntArray& arrDestTypeIndex,CDestinationTypeArray& arrDestTypes);
	int GetDestPosIndex        (const TYPE_STATION_DEF& tStation);
	unsigned short GetDestIndex(const int nDest);
	bool IsValidDestination    (const int nDest);
	bool IsCpDestination	   (int nCp);	
  bool IsCpHOCStation      (int nCp);
  bool CheckBestWays(CStringList& listErrors);
	bool CheckAGVDistances     (const int nCp, const int nNCp, const int nNNCp);
	bool CpExists  (const int nCp);
	bool IsCpValid (const int nCp);
	bool CpLowerMaximum (const int nCp);
	void WriteForTest                       (FILE* fp);

	//Functions to access to destination data:
	bool               GetAnyStation     (TYPE_STATION_DEF& tStation);
	unsigned short     GetDestType       (TYPE_STATION_DEF& tStation);
	unsigned short     GetPosWay         (TYPE_STATION_DEF& tStation);
	
	//Functions to access to Cp- and Distance data:
	CCpDistance* GetPtrFirstDist(int nCp);
	CCpDistance* GetDistPointer     (unsigned short nCp,unsigned short nNCp);
	CCpDistance* GetNextDistPointer (unsigned short nCp,unsigned short nDestCp);
	unsigned short   GetDistIndex       (unsigned short nCp,unsigned short nNCp);
	unsigned short   GetNCpOfDp         (unsigned short nDpIndex,unsigned short nDestCp);
	unsigned short   GetNextCp          (unsigned short nCp,unsigned short nDestCp);
	unsigned short   GetBlockWay        (unsigned short nCp,unsigned short nNCp);
	unsigned short   GetBlockWay        (unsigned short nDistIndex);
	unsigned short   GetDistLen         (unsigned short nDistIndex);
	unsigned short   GetTravelTimeDistance(unsigned short nCp,unsigned short nNCp);
	unsigned short   GetTravelTimeDistance(unsigned short nDistIndex);
	unsigned short   GetRealTravelTimeDistance(unsigned short nDistIndex);
  void             GetCpSequenceToDest  (unsigned short nStartCp, unsigned short nDest, unsigned short *pCpSequence);
	
	CCourseAGVType();
	CCourseAGVType(short nAGVT);
	virtual ~CCourseAGVType();
	
	bool LinkDestPosToAIDStations(CAIDStationArray& arrADIStations,
								  CCustomerStationList& listCustomerStations);
	
	unsigned int MakeHomeOrChargeStationList(CAIDStationArray& arrAIDStations,
                                             CStringList& listHomeStations,
                                             CStringList& listChargeStations);
	
	void SetAllNextDecisionPoints	();
	
	void DeleteDestPos				();
	
	void DeleteHomeOrChargeStationList();
	
	
private:
	bool MakeCommPointIndexArray();
	void AddToHomeOrChargeStationList(CAGVTypeStationList& listAGVTStations,
									  TYPE_LIST_HOCS_DEF& listHOCStations,
									  unsigned short nProperties);
	void WriteArrayCommPointerIndexForTest  (FILE* fp);
	void WriteCpForTest                     (FILE* fp);
	void WriteDistForTest                   (FILE* pFile);
	void WriteArrayDestIndexForTest         (FILE* fp);
	void WriteDestForTest                   (FILE* fp);
	void WriteDestPosForTest                (FILE* fp);
	void WriteBestWaysForTest               (FILE* fp);
	void WriteHomeOrChargeStationListForTest(FILE* fp);
	
	void  			 DeleteCourse				();
	
	void 			 InitCpPointerVector		(int nLimit);
	void 			 DeleteCpPointerVector		();
	
	bool             MakeDestIndexArray         ();
	
	bool 			 IsCpDecisionPoint			(int nCp);
	CCpDistance* GetNextDistPointer			(int  nCp);
	CCpDistance* GetTheNonDestinationDist	(CCpDistance* pDist,int nNCps);
	
	//Variable:
	int            m_nAGVLength;
	short 	       m_nAGVType;
	int            m_nFWSpeed;
	int            m_nBWSpeed;
	int            m_nCFWSpeed;
	int            m_nCBWSpeed;
	int            m_nAGVs;

	unsigned short m_nMaxDest;
		
	CBestWays									m_BestWays;
	CCpDistanceArray					m_arrCpDistances;
	
	CAGVTDestPositionArray		m_arrDestPositions;
	CAGVTLayerArray						m_arrLayer;

	CCommPointArray						m_arrCommPoints;
	CUIntArray								m_arrCommPointIndex;
	
	CAGVTDestinationArray			m_arrDestinations;
	CUIntArray								m_arrDestIndex;
	
	CHomeOrChargeStationArray	m_arrHomeOrChargeStations;		
};

#endif //_CCOURSEAGVTYPE_H_
