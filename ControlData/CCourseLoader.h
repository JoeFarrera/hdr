#ifndef _CCOURSELOADER_H_
#define _CCOURSELOADER_H_
//--------------------------------------------------------------------
//Copyright E&K AUTOMATION, INDUMAT GmbH & Co. KG
//
//Author: J.Lachmund						Project: OS800
//
//Description:	This file defines the class CCourseLoader.
//
//Documentation of changes:
//2006-08-07	Lachmund:		new
//--------------------------------------------------------------------
#include "CLoaderFile.h"
#include "CCourseData.h" 
#include "CSimpleDistance.h"

// M2015 TODO Michael 12.04.2015: Read from ini file or registry
#define CONTROLDATA_FILE "\\hdr_es_new\\ANLAG_SW\\tab\\controldata.bin"

class CCourseLoader
{
public:
	CCourseLoader();
	CCourseLoader(CString& strWorkDirectory);
	virtual ~CCourseLoader();
	CCourseData* LoadCourse();
	CString LoadOnlyGenString(unsigned char& nError);
	
private:
	//bool MakeManBlockAreaPointerVector();

	bool LoadAGVTypeCourses			();
	bool LoadAGVTypeCourse			(int nAGVTIndex);
	bool LoadCommPoints					(CCourseAGVType* pCAGVT);
	bool LoadCommPointDistances	(CCourseAGVType* pCAGVT);
	bool LoadDestinations				(CCourseAGVType* pCAGVT);
	bool LoadDestPositions			(CCourseAGVType* pCAGVT);
	bool LoadBestWays						(CCourseAGVType* pCAGVT);
	bool LoadAGVTypeDef					(CCourseAGVType* pCAGVT);
	
	bool LoadAGVTypeCourses			(CCtrlDataMemFile& myMFile);
	bool LoadAGVTypeCourse			(CCtrlDataMemFile& myMFile,int nAGVTIndex);
	bool LoadCommPoints					(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT);
	bool LoadCommPointDistances	(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT);
	bool LoadDestinations				(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT);
	bool LoadDestPositions			(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT);
	bool LoadBestWays						(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT);
	bool LoadAGVTypeDef					(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT);

	bool LoadDestinationTypes			(CCtrlDataMemFile& myMFile);
	bool LoadColumnTypes					(CCtrlDataMemFile& myMFile);
	bool LoadLayerTypes						(CCtrlDataMemFile& myMFile);
	bool LoadCustomerStationNames	(CCtrlDataMemFile& myMFile);
	bool LoadManualBlockAreas			(CCtrlDataMemFile& myMFile);

	bool LoadDestinationTypes		();
	bool LoadColumnTypes			();
	bool LoadLayerTypes				();
	bool LoadCustomerStationNames	();
	bool LoadManualBlockAreas		();
	
	bool LoadBlockTables			(CCtrlDataMemFile& myMFile);
	
	int GetMaxAGVType				(int nAGVTypes);
	void DeleteCourse				();
	
	//int CountBits					(unsigned int nVal);
	
	CLoaderFile* m_pLFile;
	CCourseData* m_pCourse;
	CString      m_strWorkDirectory;
};

#endif //_CCOURSELOADER_H_
