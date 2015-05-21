#ifndef _CCOURSEDATA_H_
#define _CCOURSEDATA_H_

//-----------------------------------------------------
//Header file of class CCourseData
//Copyright E&K AUTOMATION, Indumat GmbH & Co. KG
//
//Project: Standrad
// 
//Author: J.Lachmund
//
//Changes:
//28.04.2010: Lachmund	:	GetExpectedTimeAndLengthTotal() neu
//-----------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include <list>
#include <string>
#include <iterator> 
#include <fstream>
using namespace std;

#include "CCourseAGVType.h"
#include "CSimpleDistance.h"
#include "CAIDStation.h"
#include "CAIDCommPoint.h"
#include "CAIDCpDistance.h"

#include "CAGVTypeStation.h"
#include "CManBlockArea.h"
#include "CLoaderFile.h"

typedef struct	DEF_DISTANCE_CRASH{
		int	nIndex;
		int	nFreeWay;
		int nAbsKollWay;
		int nRelKollWay;
		int nRelFreeWay;
		int nKollWayBlkWay;
				} DEF_DISTANCE_CRASH;
				
typedef struct	DEF_BLOCK_TABLE{
		unsigned int	nDistNumber;
    	short			nTopAGVT,          //nAFzT,
					    nCompAGVT;         //nVFzT;
		unsigned int* 	pDistIndexVector;
		DEF_DISTANCE_CRASH* pDistCrash;
				} DEF_BLOCK_TABLE;
										
class CCourseData
{
public:
	CAIDStation * GetAIDStationPtr(int nDest, char cSide, int nPos, int nLayer);
	bool IsAIDStation (int nDest, char cSide, int nPos, int nLayer);
	CString GetCustomerStationName(int destNumber, char lage, int pos, int layer);
	int GetLayerTypeProperties(unsigned short destType, unsigned short pos);
	void GetExpectedTimeAndLengthTotal(	unsigned char nAGVType,
																			unsigned short nStartCp,
																			unsigned short nDestCp,
																			int& nTimeTotal,
																			int& nLengthTotal);
	CString GetLayoutVersionDateTime();
	bool IsMyLayoutVersion(CString& strLayoutVersion);
	unsigned short GetManBlockAreaNumber(unsigned short nCp, unsigned short nNCp);
	bool NeedsBlockTables();
	void SaveAIDStationList(CChangedStationObjList& listChangedStationObjects);
	void SaveAIDStations(unsigned int nIndex);
	void RestoreAIDStations(CAIDStationArray& arrStations);
	bool CalculateDispoTravelTimes();
	unsigned int GetAGVTypeStationIndex(unsigned int nAIDSIndex,TYPE_STATION_DEF& tStation,unsigned int nAGVT);
	CAGVTypeStation GetAGVTypeStation(unsigned int nIndex);
	bool ConnectionExists(CString& strPickupStation,CString& strUnloadStation);
	void SetBestAGVTypeStation(TYPE_STATION_DEF& tStation,
							   unsigned char     nAGV,
							   unsigned int      nAGVT,
							   CString&           strPSName);
	DEF_BLOCK_TABLE* GetPtrBlockTables(){return m_pBlockTables;}
	void SetNumberOfBlockTablePairs(int nNumberOfBlockTablePairs){m_nNumberOfBlockTablePairs = nNumberOfBlockTablePairs;}
	int  GetNumberOfBlockTablePairs(){return m_nNumberOfBlockTablePairs;}
	void SetGenString(CString& strGenString){m_strGenString = strGenString;}
	CString GetGenString(){return m_strGenString;}
	CString GetProjectName(){return m_strProject;}
	int  GetNumberAGVs(){return m_nAGVs;}
	int  GetMaxManBlockArea(){return m_nMaxManBlockArea;}
	int  GetMaxAGVType(){return m_nMaxAGVType;}
	void SetMaxAGVType(int nMaxAGVType){m_nMaxAGVType = nMaxAGVType;}
	unsigned int GetMaxHomeAndChargeStations(){return m_nMaxHomeAndChargeStations;}
	int  GetNumberDestPositions() {return m_nDestPositions;}
	void SetNumberDestPositions(int nDestPositions){m_nDestPositions = nDestPositions;}
	int  GetAGVTypes  (){return m_nAGVTypes;}
	void SetAGVTypes  (int nAGVTypes){m_nAGVTypes = nAGVTypes;}
	int GetMaxAIDCp()  {return ((int) m_arrAIDCpIndex.GetSize() - 1);}
	unsigned int GetNumberAIDCps(){return m_nAIDCommPoints;}
	unsigned int GetNumberAIDCpDists(){return m_nAIDCpDistances;}
	CAIDStation* GetPtrAIDStation(unsigned int nAIDSIndex);
	unsigned int GetNumberAIDStations();
	
	bool LoadBlockTables		  		(CCtrlDataMemFile& myMFile);	
	bool LoadBlockTable			  		(CCtrlDataMemFile& myMFile,DEF_BLOCK_TABLE* pBlockTable);
	bool LoadDestinationTypes			(CCtrlDataMemFile& myMFile);
	bool LoadColumnTypes		  		(CCtrlDataMemFile& myMFile);
	bool LoadLayerTypes			  		(CCtrlDataMemFile& myMFile);
	bool LoadCustomerStationNames	(CCtrlDataMemFile& myMFile);
	bool LoadManualBlockArea      (CCtrlDataMemFile& myMFile);
	
	bool LoadColumnTypes		  (CLoaderFile* pLFile);
	bool LoadLayerTypes			  (CLoaderFile* pLFile);
	bool LoadDestinationTypes     (CLoaderFile* pLFile);
	bool LoadCustomerStationNames (CLoaderFile* pLFile);
	bool LoadManualBlockArea      (CLoaderFile* pLFile);
	bool LoadBlockTable			  (CLoaderFile* pLFile,DEF_BLOCK_TABLE* pBlockTable);
	void DeleteBlockTables		  ();
	int CountBits				  (unsigned int nVal);
	
	bool PrepareAIDCpsAndDistances();
	bool Prepare                  (CString& strWorkDirectory);
	
	void SendStationListToAllBuBs(CChangedStationObjList& listChangeStations);
	void ChangeStation    (unsigned short nCommand, string strPSName);
	void ChangeStation    (TYPE_STATION_DEF& tStation,
						   unsigned short nCommand,
						   bool bSendToBuBs = true,
						   bool bDataSave   = true);
	bool IsStationLoaded  (TYPE_STATION_DEF& tStation);
	bool IsStationLoaded  (CString& strCSN);
	bool IsStationClosed  (TYPE_STATION_DEF& tStation);
	bool IsStationClosed  (CString& strCSN);
	bool IsStationError   (TYPE_STATION_DEF& tStation);
	bool IsStationError   (CString& strCSN);
	bool IsStationReserved(TYPE_STATION_DEF& tStation);
	bool IsStationReserved(CString& strCSN);
	
	void ChangeAIDStation(CString& strPSName,unsigned short nCommand,unsigned int nPrioTime, CChangedStationObjList& listChangedStations);
	bool IsValidStationAction(TYPE_STATION_DEF& tStation,int nAction,unsigned int& nAGVTypes);
	CAIDStation  GetAIDStation     (int nDest, char cSide, int nPos, int nLayer);
	CAIDStation  GetAIDStation     (int nAGVT, int nDest, char cSide, int nPos, int nLayer);
	unsigned int GetAIDStationIndex(int nAGVT, int nDest, char cSide, int nPos, int nLayer,int& nError);
	unsigned int GetAIDStationIndex(CString& strCSN);
	unsigned int GetAGVTypesOfAction(int nAction,CAGVTypeStationList &listAGVTStation);
	//void GetAGVTypeStationList(string& strCSN,CAGVTypeStationList& listAGVTStations);
	bool IsValidAGVDestination      (const int nAGV,const int nDest);
	bool IsCpValid                  (const unsigned short nCp    );
	bool CheckAGVDistances          (const int nAGV,const int nCp, const int nNCp,const int nNNCp);
	int  CheckGenString ();
		
	bool           ConvertCustomerToStation(char* pCustomerName, TYPE_STATION_DEF& tStation );
	bool           ConvertStationToCustomerName(char* pCustomerName, TYPE_STATION_DEF& tStation );

	void GetPSIndexListWithChildren(CString& strPSName,CChangedStationObjList& listChangedStations);
	unsigned short CheckAIDCpIsDest(unsigned short nCp);
	unsigned short GetAIDDistOccAGV(unsigned short nCp, unsigned short nNCp);
	bool           ChangeAIDDistOccAGV(unsigned short nAGV, unsigned short nCp, unsigned short nNCp, bool bOccupy);
	unsigned short GetAIDDistStopOccAGV(unsigned short nCp);
	bool           ChangeAIDDistStopOccAGV(unsigned short nCp, bool bChange);
	CAIDCpDistance* GetAIDDistance(unsigned short nCp, unsigned short nNCp);
	int            GetAIDDistanceIndex(unsigned short nCp, unsigned short nNCp);
//	bool           MakeManBlockAreaPointerVector();
	bool           FillAIDCpDistances(CSimpleDistance* pSDist);
	bool           FillAIDCommPoints (int* pPCp);
	bool           FillAIDCommPoints (CUIntArray& arrPCp);
	unsigned short GetManBlockAreaOccAGV(unsigned short nMBArea);
	bool           ChangeManBlockAreaOccAGV(unsigned short nAGV, unsigned short nMBArea, bool bChange);
	bool           SetAGVInManBlockArea(unsigned short nAGV, unsigned short nCp, unsigned short nNCp);
	void           ResetAGVInManBlockArea(unsigned short nAGV);
	unsigned short GetAGVInManBlockArea(unsigned short nCp, unsigned short nNCp);
	int		   GetManBlockAreaOfDistance(unsigned short nCp, unsigned short nNCp);
	bool           IsDistanceInMBArea(unsigned short nMBArea, unsigned short nCp, unsigned short nNCp);
	CCourseAGVType* GetPtrCourseAGVType(unsigned int nAGVType);
	bool           InitAGVTypeCourses();
	
	CCourseData();
	virtual ~CCourseData();
		
	int   m_nChargeStations;
	int   m_nHomeStations;
	
private:
	void GetAGVTypeStationList(CString& strCSN,CAGVTypeStationList& listAGVTStations);
	int  GetMaxPossibleCps			();
	int  RemoveDoubleCps			(int* pCps);
	int  RemoveDoubleCps			(CUIntArray& arrCps);
	void SortIntArray				(int* pInt,int nLimit);
	void SortIntArray				(CUIntArray& arrInt,int nLimit);
	void WriteForTest				    (FILE* fp);
	void WriteForTest				    (CString* pstrWorkPath);
	void WriteManBlockAreaForTest		(FILE* fp);
	void WriteBlockTablesForTest		(FILE* fp);
	void WriteDestTypeForTest			(FILE* fp);
	void WriteColumnTypeForTest		    (FILE* fp);
	void WriteLayerTypesForTest		    (FILE* fp);
	void WriteCustomerStationNameForTest(FILE* fp);
	void SetAllObjectsInAIDStations      ();	
	bool SetAGVTypesActionsInAIDStations ();
	bool LinkAGVTypeStationsToAIDStations();
	bool MakeManBlockAreaObjectArray(CManBlockAreaObjectList& listMBAO);
	bool MakeManBlockAreaIndexArray();
	void MakeHomeOrChargeStationLists    ();
	unsigned int GetAllStationObjects(CString &strPSName);
	
	int  GetMaxPossibleDists		();
	void MakeListSimpleDists		(CSimpleDistance* pSDists);
	int  RemoveDoubleDists			(CSimpleDistance* pSDists,int nSDists);
	void SortSimpleDists				(CSimpleDistance* pSDist);
	
	//bool MakeAIDCommPointsPOINTER();
	
	bool MakeAIDCommPoints();
	bool MakeAIDCpIndexArray();
	bool MakeAIDCpDistances();
	
	bool MakeAIDStationArray();
	bool MakeAIDStationArray(CAIDStationList& listADIStations);
	bool IsMBAreaValid(const unsigned short nMBArea);
	bool Occupy (unsigned short nAGV,unsigned short& nOccAGV,bool bOccupy);
	CString MakeHexToString(CString& strHex);
	int GetCheckSum(CString& strData);
	void WriteManBlockAreaPointerVectorForTest	(FILE* fp);
	void SetCPAndDestAGVType(CAIDCommPoint& tAIDCp);
	
	//Variable:    
	int          m_nAGVTypes,
	          	 m_nAGVs,	
				 m_nDestPositions,
		         m_nMaxAGVType,
		         m_nMaxManBlockArea;
	unsigned int m_nMaxHomeAndChargeStations;
	int	         m_nNumberOfBlockTablePairs;
	
	DEF_BLOCK_TABLE*	        m_pBlockTables;
	  
	CCourseAGVType**          m_apCourseAGVType;
	
	CAIDStationArray          m_arrAIDStations;
	CAGVTypeStationArray      m_arrAGVTypeStations;
	
	CLayerTypeArray		        m_arrLayerTypes;
	CUIntArray			          m_arrLayerTypeIndex;
	
	CColumnTypeArray          m_arrColumnTypes;
	CUIntArray                m_arrColumnTypeIndex;
	
	CDestinationTypeArray  	  m_arrDestTypes;
	CUIntArray                m_arrDestTypeIndex;
	
	CDestTypePosList          m_listDestTypePos;
	
	CCustomerStationList      m_listCustomerStations;
	
	CUIntArray                m_arrMBAIndex;
	CManBlockAreaArray        m_arrManBlockAreas;
	CManBlockAreaObjectArray  m_arrManBlockAreaObjects;
	
	CAIDCommPointArray		    m_arrAIDCommPoints;
	CUIntArray                m_arrAIDCpIndex;
	CAIDCpDistanceArray		    m_arrAIDCpDistances;
	
	int						    m_nAIDCpDistances,
	      					  m_nAIDCommPoints;
	CString m_strUser,
	       m_strProject,
	       m_strDateTime;
	CString m_strGenString;
		  
};

extern CCourseData* pTheCourse;

#endif //_CCOURSEDATA_H_
