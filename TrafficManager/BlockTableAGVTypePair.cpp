// BlockTableAGVTypePair.cpp: implementation of the CBlockTableAGVTypePair class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "..\tpc.h"
#include "BlockTableAGVTypePair.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlockTableAGVTypePair::CBlockTableAGVTypePair()
{

}

CBlockTableAGVTypePair::~CBlockTableAGVTypePair()
{

}

CCrashDistance* CBlockTableAGVTypePair::GetPCrashDistance(int nTDistIndex, int nCDistIndex)
{
	return IndexIsValid(nTDistIndex) ? m_arrCrashDistanceArray[nTDistIndex - 1].GetPCrashDistance(nCDistIndex) : NULL;
}

bool CBlockTableAGVTypePair::IndexIsValid(int nIndex)
{
	return (nIndex > 0) && (nIndex <= m_arrCrashDistanceArray.GetSize());
}

/*
bool CBlockTableAGVTypePair::ReadBinary()
{
	bool	bRet = false;
	CFile	myFile;
	CString	strFile;

	strFile = AfxGetApp ()->GetProfileString ("Settings", "AutoblkFile");
	TRACE ("Autoblock file is: %s", strFile);


    if (myFile.Open (strFile, CFile::modeRead | CFile::shareExclusive))
    {
		CUIntArray arrDIndex;
		if(ReadIndexVector(myFile,arrDIndex))
		{
			bRet = ReadCrashDistances(myFile,arrDIndex);
		}
		myFile.Close();
	}
	else
	{
		CString strMsg;
		strMsg.Format(IDS_CANNOT_OPEN_BIN_BLOCKFILE,strFile);
		AfxMessageBox(strMsg,MB_ICONSTOP);
	}

	return bRet;
}
*/

/*
bool CBlockTableAGVTypePair::ReadIndexVector(CFile &myFile, CUIntArray &arrDIndex)
{
	bool bRet(false);
	UINT nDistances(0);
	if(myFile.Read(&nDistances,sizeof(UINT)) == sizeof(UINT))
	{
		if(nDistances <= KKMAX_FAHR_ZEIT)
		{
			UINT nIndex;
			bRet = true;
			arrDIndex.SetSize(nDistances + 1);
			for(UINT nDCount = 0;
			(nDCount <= nDistances) && bRet;
			nDCount++)
			{
				bRet = (myFile.Read(&nIndex,sizeof(UINT)) == sizeof(UINT));
				arrDIndex[nDCount] = bRet ? nIndex : 0;
			}
			if(!bRet)
			{
				AfxMessageBox(IDS_ERROR_READ_BLOCKTABLE_INDEX_VECTOR,MB_ICONSTOP);
			}
		}
		else
		{
			CString strMsg;
			strMsg.Format(IDS_BLOCKNUMBER_ERROR,(int)nDistances,(int)KKMAX_FAHR_ZEIT);
			AfxMessageBox(strMsg,MB_ICONSTOP);
		}
	}

	return bRet;
}
*/

/*
bool CBlockTableAGVTypePair::ReadCrashDistances(CFile &myFile, CUIntArray &arrDIndex)
{
	bool	bRet(true);
	UINT	aUInt[6];
	UINT	nDistances = arrDIndex.GetSize() - 1;
	int		nCrashDistances = arrDIndex[nDistances];

	m_arrCrashDistanceArray.RemoveAll();

	m_arrCrashDistanceArray.SetSize(nDistances);
	for(UINT nDCount = 0;
	(nDCount < nDistances) && bRet;
	nDCount++)
	{
		int nDCrashDistances = arrDIndex[nDCount + 1] - arrDIndex[nDCount];
		m_arrCrashDistanceArray[nDCount].SetSize(nDCrashDistances);
		for(int nCDCount = 0;
		(nCDCount < nDCrashDistances) && bRet;
		nCDCount++)
		{
			bRet = (myFile.Read(aUInt,sizeof(aUInt)) == sizeof(aUInt));
			if(bRet)
			{
				m_arrCrashDistanceArray[nDCount].SetAt(nCDCount,CCrashDistance(	aUInt[0],
																				aUInt[1],
																				aUInt[2],
																				aUInt[3],
																				aUInt[4],
																				aUInt[5]));
			}
			else
			{
				CString strMsg;
				strMsg.Format(IDS_ERROR_READ_BLOCKTABLE_CRASH_DISTANCES,nDCount + 1,nCDCount + 1);
				AfxMessageBox(strMsg,MB_ICONSTOP);
			}
		}
	}

	return bRet;
}
*/

bool CBlockTableAGVTypePair::IsToRequestWayTele(int nDIndex, int nMyBlockway)
{
	bool bRet = false;
	int nDLimit = m_arrCrashDistanceArray.GetSize();
	for(int nDCount = 0;
	(nDCount < nDLimit) && !bRet;
	nDCount++)
	{
		bRet = m_arrCrashDistanceArray[nDCount].IsToRequestWayTele(nDIndex,nMyBlockway);
	}
	return bRet;
}


CCrashDistance* CBlockTableAGVTypePair::GetGegenCrash(CCrashDistance* pCrashDist, int nDistIndex)
{
	CCrashDistance* pRet = &this->m_arrCrashDistanceArray[pCrashDist->GetIndex()][nDistIndex];
	return pRet;
}


CCrashDistanceArray* CBlockTableAGVTypePair::GetPtrArrCrashDistances(int nDistIndex)
{
	CCrashDistanceArray* pRet = NULL;
	if ((nDistIndex >= 0) && (nDistIndex < this->m_arrCrashDistanceArray.GetSize()))
	{
		// gültiger Index:
		pRet = &(this->m_arrCrashDistanceArray[nDistIndex]);
	}
	return pRet;
}

void CBlockTableAGVTypePair::Fill(DEF_BLOCK_TABLE* pBlockTables)
{
	this->m_nTopAGVType = pBlockTables->nTopAGVT;
	this->m_nCompAGVType = pBlockTables->nCompAGVT;
	this->m_arrCrashDistanceArray.SetSize(pBlockTables->nDistNumber);

	for (UINT countD = 0;
		countD < pBlockTables->nDistNumber;
		countD++)
	{
		int nStartIndex = *(pBlockTables->pDistIndexVector + countD);
		int nEndIndex = *(pBlockTables->pDistIndexVector + countD + 1);

		this->m_arrCrashDistanceArray[countD].SetSize(nEndIndex - nStartIndex);
		for (int countBlockItems = 0;
			countBlockItems < (nEndIndex - nStartIndex);
			countBlockItems++)
		{
			DEF_DISTANCE_CRASH* pDistanceCrash = (pBlockTables->pDistCrash + countBlockItems + nStartIndex);
			this->m_arrCrashDistanceArray[countD][countBlockItems].SetValues(
				pDistanceCrash->nIndex,
				pDistanceCrash->nFreeWay,
				pDistanceCrash->nAbsKollWay,
				pDistanceCrash->nRelKollWay,
				pDistanceCrash->nRelFreeWay,
				pDistanceCrash->nKollWayBlkWay);
		}
	}
}