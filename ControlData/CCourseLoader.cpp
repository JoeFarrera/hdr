/* Include files */
#include "..\stdafx.h"
#include  <stdlib.h>
#include  <stdio.h>
#include  <malloc.h>
#include  <direct.h>

#include "konstant.h"
#include "komdat.h"

#include "CCourseLoader.h"

#include "..\tpc.h"
#include "..\msgMacro.h"





CCourseLoader::CCourseLoader()
{
	m_pCourse = NULL;
}

CCourseLoader::CCourseLoader(CString& strWorkDirectory)
{
	m_pCourse = NULL;
	this->m_strWorkDirectory = strWorkDirectory;
}

CCourseLoader::~CCourseLoader()
{
}

void CCourseLoader::DeleteCourse()
{
	if(m_pCourse != NULL)
	{
		delete m_pCourse;
		m_pCourse = NULL;
	}
}

CString CCourseLoader::LoadOnlyGenString(unsigned char& nError)
{
	CString strRet;
	CCtrlDataMemFile myMFile;
	CString strMyFile(CONTROLDATA_FILE);
	if(myMFile.Open(strMyFile))
	{
		strRet = myMFile.ReadCompactString();
	}
	nError = (strRet.GetLength() == 0) ? 1 : 0; 
	return strRet;
}

CCourseData* CCourseLoader::LoadCourse()
{

	CCtrlDataMemFile myMFile;
	CString strMyFile(CONTROLDATA_FILE);
	if(myMFile.Open(strMyFile))
	{
		DeleteCourse();
		// Kurs lesen:
		m_pCourse = new CCourseData;
		if(m_pCourse != NULL)
		{
			bool bDataOK = true;
			CString strGenString = myMFile.ReadCompactString();
			if(!strGenString.IsEmpty())
			{
				int nAGVTypes = 0;
				m_pCourse->SetGenString(strGenString);
				int nErr = m_pCourse->CheckGenString();
				bDataOK  = (nErr == 0);
				bDataOK &= myMFile.Read(nAGVTypes);
				if(bDataOK)
				{
					m_pCourse->SetAGVTypes(nAGVTypes);
					m_pCourse->SetMaxAGVType( GetMaxAGVType(m_pCourse->GetAGVTypes()));
					bDataOK = m_pCourse->InitAGVTypeCourses();
					
					if(bDataOK)
					{													
						//FzTyp-Kurse lesen:
						bDataOK &= LoadAGVTypeCourses(myMFile);
				
						//vom Fahrzeugtyp unabhängige Daten lesen:
	
						if(m_pCourse->NeedsBlockTables())
						{
							// Blockungstabellen laden:
							MSGAVISO("\n++++ LoadBlockTables");
							bDataOK = LoadBlockTables(myMFile);
						}
						
						//Zieltypen mit Pos-Infos lesen:
						MSGAVISO("\n++++ LoadDestinationTypes");
						bDataOK &= LoadDestinationTypes(myMFile);

						//Säulentypen lesen:
						MSGAVISO("\n++++ LoadColumnTypes");
						bDataOK &= LoadColumnTypes(myMFile);
						
						//Ebenentypen lesen:
						MSGAVISO("\n++++ LoadLayerTypes");
						bDataOK &= LoadLayerTypes(myMFile);
						
						//KundenzielNamen lesen:
						MSGAVISO("\n++++ LoadCustomerStationNames");
						bDataOK &= LoadCustomerStationNames(myMFile);

						//Knoten lesen:
						MSGAVISO("\n++++ LoadManualBlockAreas");
						bDataOK &= LoadManualBlockAreas(myMFile);
					}
					if (bDataOK)
					{
						// Kurs komplettieren:							
						MSGAVISO("\n----Start Prepare Course\n");
						bDataOK = m_pCourse->Prepare(this->m_strWorkDirectory);
						MSGAVISO("\n----End   Prepare Courses\n");
					}
					else
					{
						delete m_pCourse;
						m_pCourse = NULL;
					}						
				}
			}
		}
	}
	ASSERT(m_pCourse != NULL);
	return m_pCourse;
}

//***************************************************************
// GetMaxAGVType returns the highest existing vehicle type from
// the bit encoded nAGVTypes.
//***************************************************************
int CCourseLoader::GetMaxAGVType(int nAGVTypes)
{
	int nMaxAGVT = 0;
	int nMaske = 1;
	for(int nFzT = nAGVTypes;
	nFzT!= 0;
	nMaske <<= 1, nMaxAGVT++)
	{
		nFzT &= ~nMaske;   // debit to nFzT
	}
	return nMaxAGVT;
}

bool CCourseLoader::LoadDestinationTypes(CCtrlDataMemFile& myMFile)
{
	CString strID("DefDestType");
	bool bRet = myMFile.SeekIdentString(strID);
	if(bRet)
	{
		bRet = m_pCourse->LoadDestinationTypes(myMFile);
		
	}
	return bRet;
}

bool CCourseLoader::LoadDestinationTypes()
{
	CString strID("DefDestType");
	bool bRet = m_pLFile->SeekIdentString(strID);
	if(bRet)
	{
		bRet = m_pCourse->LoadDestinationTypes(m_pLFile);
		
	}
	return bRet;
}

bool CCourseLoader::LoadColumnTypes(CCtrlDataMemFile& myMFile)
{
	CString strID("DefColumnType");
	bool bRet = myMFile.SeekIdentString(strID);
	if(bRet)
	{
		bRet = m_pCourse->LoadColumnTypes(myMFile);		
	}
	return bRet;
}

bool CCourseLoader::LoadColumnTypes()
{
	CString strID("DefColumnType");
	bool bRet = m_pLFile->SeekIdentString(strID);
	if(bRet)
	{
		bRet = m_pCourse->LoadColumnTypes(m_pLFile);		
	}
	return bRet;
}

bool CCourseLoader::LoadLayerTypes(CCtrlDataMemFile& myMFile)
{
	CString strID("DefLayerType");
	bool bRet = myMFile.SeekIdentString(strID);
	if(bRet)
	{
		bRet = m_pCourse->LoadLayerTypes(myMFile);		
	}
	return bRet;
}

bool CCourseLoader::LoadLayerTypes()
{
	CString strID("DefLayerType");
	bool bRet = m_pLFile->SeekIdentString(strID);
	if(bRet)
	{
		bRet = m_pCourse->LoadLayerTypes(m_pLFile);		
	}
	return bRet;
}

bool CCourseLoader::LoadCustomerStationNames(CCtrlDataMemFile& myMFile)
{
	CString strID("CustomerStationNames");
	bool bRet = myMFile.SeekIdentString(strID);
	if(bRet)
	{
		bRet = m_pCourse->LoadCustomerStationNames(myMFile);
	}
	return bRet;
}

bool CCourseLoader::LoadCustomerStationNames()
{
	CString strID("CustomerStationNames");
	bool bRet = m_pLFile->SeekIdentString(strID);
	if(bRet)
	{
		bRet = m_pCourse->LoadCustomerStationNames(m_pLFile);
	}
	return bRet;
}

bool CCourseLoader::LoadManualBlockAreas(CCtrlDataMemFile& myMFile)
{
	CString strID("ManualBlockAreas");
	bool bRet = myMFile.SeekIdentString(strID);
	if(bRet)
	{
		bRet = m_pCourse->LoadManualBlockArea(myMFile);
	}
	return bRet;
}

bool CCourseLoader::LoadManualBlockAreas()
{
	CString strID("ManualBlockAreas");
	bool bRet = m_pLFile->SeekIdentString(strID);
	if(bRet)
	{
		bRet = m_pCourse->LoadManualBlockArea(m_pLFile);
	}
	return bRet;
}

bool CCourseLoader::LoadAGVTypeCourses(CCtrlDataMemFile& myMFile)
{
	bool bDataOK = true;
	int nAGVTypes = m_pCourse->GetAGVTypes();
	for(int nCount = 0;
	(nCount < m_pCourse->GetMaxAGVType()) && bDataOK;
	nCount++, nAGVTypes >>= 1)
	{
		if(nAGVTypes & 1)
		{
			//AGV type exist:
			bDataOK = LoadAGVTypeCourse(myMFile,nCount);			;
		}
	}
	if(bDataOK)
	{
		bDataOK = m_pCourse->PrepareAIDCpsAndDistances();
		
	}
	
	return bDataOK;
}

bool CCourseLoader::LoadAGVTypeCourses()
{
	bool bDataOK = true;
	int nAGVTypes = m_pCourse->GetAGVTypes();
	for(int nCount = 0;
	(nCount < m_pCourse->GetMaxAGVType()) && bDataOK;
	nCount++, nAGVTypes >>= 1)
	{
		if(nAGVTypes & 1)
		{
			//AGV type exist:
			bDataOK = LoadAGVTypeCourse(nCount);			;
		}
	}
	if(bDataOK)
	{
		bDataOK = m_pCourse->PrepareAIDCpsAndDistances();
		
	}
	
	return bDataOK;
}

bool CCourseLoader::LoadAGVTypeCourse(CCtrlDataMemFile& myMFile,int nAGVTIndex)
{
	CCourseAGVType* pCAGVT = m_pCourse->GetPtrCourseAGVType(nAGVTIndex+1);
	bool bDataOK = (pCAGVT != NULL);
	if (bDataOK)
	{
		char szAGVT[20];
		sprintf_s(szAGVT,", AGV Type %d",(int)pCAGVT->GetAGVType());
		
		CString strHead("\n++++ ");
		CString strAGVT(szAGVT);
		CString strToDo; 
		
		strToDo = strHead + CString("LoadAGVTypeDef") + strAGVT;
		MSGAVISO(strToDo);
		bool bDataOKAGVT = LoadAGVTypeDef (myMFile,pCAGVT); 
		
		strToDo = strHead + CString("LoadCommPoints") + strAGVT;
		MSGAVISO(strToDo);
		bool bDataOKCp   = LoadCommPoints (myMFile,pCAGVT);
		
		strToDo = strHead + CString("LoadCommPointDistances") + strAGVT;
		MSGAVISO(strToDo);
		bool bDataOKCpD  = LoadCommPointDistances (myMFile,pCAGVT);
		
		strToDo = strHead + CString("LoadDestinations") + strAGVT;
		MSGAVISO(strToDo);
		bool bDataOKDe   = LoadDestinations		  	(myMFile,pCAGVT);
			
		strToDo = strHead + CString("LoadDestPositions") + strAGVT;
		MSGAVISO(strToDo);
		bool bDataOKDPW  = LoadDestPositions		  (myMFile,pCAGVT);
		
		strToDo = strHead + CString("LoadBestWays") + strAGVT;
		MSGAVISO(strToDo);
		bool bDataOKBW   = LoadBestWays  					(myMFile,pCAGVT);

		bDataOK =	bDataOKAGVT & bDataOKCp  &
							bDataOKDe   & bDataOKCpD &
							bDataOKDPW  & bDataOKBW;
		if(bDataOK)
		{
			m_pCourse->SetNumberDestPositions(m_pCourse->GetNumberDestPositions() + pCAGVT->GetNumberDestPositions()); 
		}
	}
	return bDataOK;
}

bool CCourseLoader::LoadAGVTypeCourse(int nAGVTIndex)
{
	CCourseAGVType* pCAGVT = m_pCourse->GetPtrCourseAGVType(nAGVTIndex+1);
	bool bDataOK = (pCAGVT != NULL);
	if (bDataOK)
	{
		MSGAVISO("\n++++ LoadAGVTypeDef");
		bool bDataOKAGVT = LoadAGVTypeDef		  (pCAGVT); 
		
		MSGAVISO("\n++++ LoadCommPoints");
		bool bDataOKCp   = LoadCommPoints		  (pCAGVT);
		//	 bDataOKCp  &= LoadCommPointsPointer  (pCAGVT);
		
		MSGAVISO("\n++++ LoadCommPointDistances");
		bool bDataOKCpD  = LoadCommPointDistances (pCAGVT);
		
		MSGAVISO("\n++++ LoadDestinations");
		bool bDataOKDe   = LoadDestinations		  (pCAGVT);
			
		MSGAVISO("\n++++ LoadDestPositions");
		bool bDataOKDPW  = LoadDestPositions		  (pCAGVT);
		
		MSGAVISO("\n++++ LoadBestWays");
		bool bDataOKBW   = LoadBestWays  (pCAGVT);

		bDataOK = bDataOKAGVT & bDataOKCp  &
							bDataOKDe   & bDataOKCpD &
							bDataOKDPW  & bDataOKBW;
		if(bDataOK)
		{
			m_pCourse->SetNumberDestPositions(m_pCourse->GetNumberDestPositions() + pCAGVT->GetNumberDestPositions()); 
		}
	}
	return bDataOK;
}

bool CCourseLoader::LoadCommPoints(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT)
{
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"CommPoints(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK  = myMFile.SeekIdentString(strIdent);
	bDataOK       = myMFile.SeekIdentString(strIdent);
	if(bDataOK)
	{
		bDataOK = pCAGVT->LoadCommPoints(myMFile);
	}
	return bDataOK;
}

bool CCourseLoader::LoadCommPoints(CCourseAGVType* pCAGVT)
{
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"CommPoints(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK = m_pLFile->SeekIdentString(strIdent);
	if(bDataOK)
	{
		bDataOK = pCAGVT->LoadCommPoints(m_pLFile);
	}
	return bDataOK;
}

bool CCourseLoader::LoadCommPointDistances(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT)
{
	//pCAGVT->DeleteCommPointDistances();
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"CpDistances(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK = myMFile.SeekIdentString(strIdent);
	if(bDataOK)
	{
		bDataOK = pCAGVT->LoadCommPointDistances(myMFile);
		
	}
	return bDataOK;
}

bool CCourseLoader::LoadCommPointDistances(CCourseAGVType* pCAGVT)
{
	//pCAGVT->DeleteCommPointDistances();
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"CpDistances(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK = m_pLFile->SeekIdentString(strIdent);
	if(bDataOK)
	{
		bDataOK = pCAGVT->LoadCommPointDistances(m_pLFile);
		
	}
	return bDataOK;
}

bool CCourseLoader::LoadDestinations(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT)
{
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"Destinations(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK = myMFile.SeekIdentString(strIdent);
	if(bDataOK)
	{
		bDataOK = pCAGVT->LoadDestinations(myMFile);	
	}
	return bDataOK;
}

bool CCourseLoader::LoadDestinations(CCourseAGVType* pCAGVT)
{
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"Destinations(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK = m_pLFile->SeekIdentString(strIdent);
	if(bDataOK)
	{
		bDataOK = pCAGVT->LoadDestinations(m_pLFile);	
	}
	return bDataOK;
}

bool CCourseLoader::LoadDestPositions(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT)
{
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"PositionWays(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK = myMFile.SeekIdentString(strIdent);
	if(bDataOK)
	{
		bDataOK = pCAGVT->LoadDestPositions(myMFile);
	}		
	return bDataOK;
}

bool CCourseLoader::LoadDestPositions(CCourseAGVType* pCAGVT)
{
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"PositionWays(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK = m_pLFile->SeekIdentString(strIdent);
	if(bDataOK)
	{
		bDataOK = pCAGVT->LoadDestPositions(m_pLFile);
	}		
	return bDataOK;
}

bool CCourseLoader::LoadBestWays(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT)
{
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"BestWays(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK = myMFile.SeekIdentString(strIdent);
	if(bDataOK)
	{
		bDataOK = pCAGVT->LoadBestWays(myMFile);
		
	}
	return bDataOK;
}

bool CCourseLoader::LoadBestWays(CCourseAGVType* pCAGVT)
{
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"BestWays(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK = m_pLFile->SeekIdentString(strIdent);
	if(bDataOK)
	{
		bDataOK = pCAGVT->LoadBestWays(m_pLFile);
		
	}
	return bDataOK;
}

bool CCourseLoader::LoadAGVTypeDef(CCtrlDataMemFile& myMFile,CCourseAGVType* pCAGVT)
{
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"DefAGVType(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK = myMFile.SeekIdentString(strIdent);
	if(bDataOK)
	{
		unsigned short nAGVs = 0;
		int nAGVType,
			nFWSpeed,
			nBWSpeed,
			nCFWSpeed,
			nCBWSpeed;
		bDataOK = myMFile.Read(nAGVType ) &&
				  		myMFile.Read(nAGVs    ) &&
				  		myMFile.Read(nFWSpeed ) &&
				  		myMFile.Read(nBWSpeed ) &&
				  		myMFile.Read(nCFWSpeed) &&
				  		myMFile.Read(nCBWSpeed);
		bDataOK &= (nAGVType == (int)pCAGVT->GetAGVType());
		if(bDataOK)
		{
			pCAGVT->SetAGVs(nAGVs);
			pCAGVT->SetSpeeds(nFWSpeed,nBWSpeed,nCFWSpeed,nCBWSpeed);
		}
	}
	return bDataOK;
}

bool CCourseLoader::LoadAGVTypeDef(CCourseAGVType* pCAGVT)
{
	CString strIdent;
	char acIdent[40];
	sprintf_s(acIdent,"DefAGVType(%d)",pCAGVT->GetAGVType());
	strIdent = acIdent;
	bool bDataOK = m_pLFile->SeekIdentString(strIdent);
	if(bDataOK)
	{
		unsigned short nAGVs = 0;
		int nAGVType,
			nFWSpeed,
			nBWSpeed,
			nCFWSpeed,
			nCBWSpeed;
		bDataOK = m_pLFile->Read(nAGVType ) &&
				  m_pLFile->Read(nAGVs    ) &&
				  m_pLFile->Read(nFWSpeed ) &&
				  m_pLFile->Read(nBWSpeed ) &&
				  m_pLFile->Read(nCFWSpeed) &&
				  m_pLFile->Read(nCBWSpeed);
		bDataOK &= (nAGVType == (int)pCAGVT->GetAGVType());
		if(bDataOK)
		{
			pCAGVT->SetAGVs(nAGVs);
			pCAGVT->SetSpeeds(nFWSpeed,nBWSpeed,nCFWSpeed,nCBWSpeed);
		}
	}
	return bDataOK;
}

bool CCourseLoader::LoadBlockTables(CCtrlDataMemFile& myMFile)
{
	CString strIdent;
	strIdent = "AutoBlockTable";
	//int nAGVTA = 1,
	//    nAGVTV = 1;
	bool bDataOK = myMFile.SeekIdentString(strIdent);
	if(bDataOK)
	{
		bDataOK = m_pCourse->LoadBlockTables(myMFile);
	}
	return bDataOK;
}
