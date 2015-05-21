#include "..\stdafx.h"
#include "CBestWays.h"

CBestWays::CBestWays()
{
}

CBestWays::~CBestWays()
{
}

bool CBestWays::Load(CCtrlDataMemFile& myMFile)
{
	int nDps,
		nCpAreas;
	bool bDataOK = myMFile.Read(nDps    ) &&
			       		 myMFile.Read(nCpAreas);
	if(bDataOK)
	{
		m_arrDpBestWays.SetSize(nDps);
		m_nDps     = nDps;
		m_nCpAreas = nCpAreas;
		if(bDataOK)
		{
			for(int nCount = 0;
			(nCount < nDps) && bDataOK;
			nCount++)
			{
				//Dp lesen:
				bDataOK = m_arrDpBestWays[nCount].Load(myMFile,nCpAreas);
				
			}
		}
	}
	return bDataOK;
}

bool CBestWays::Load(CLoaderFile* pLFile)
{
	int nDps,
		nCpAreas;
	bool bDataOK = pLFile->Read(nDps    ) &&
			       pLFile->Read(nCpAreas);
	if(bDataOK)
	{
		m_arrDpBestWays.SetSize(nDps);
		m_nDps     = nDps;
		m_nCpAreas = nCpAreas;
		if(bDataOK)
		{
			for(int nCount = 0;
			(nCount < nDps) && bDataOK;
			nCount++)
			{
				//Dp lesen:
				bDataOK = m_arrDpBestWays[nCount].Load(pLFile,nCpAreas);
				
			}
		}
	}
	return bDataOK;
}

unsigned int CBestWays::GetNCpOfDp(int nDpIndex,int nCpArea)
{
	return m_arrDpBestWays[nDpIndex - 1].GetNCp(nCpArea);
}

void CBestWays::WriteBestWaysForTest(FILE* fp)
{
	fprintf(fp,"\nBestWays : %d DecisionPoints, %d CpAreas\n\n",
			m_nDps,
			m_nCpAreas);
	for(int nCount = 0;
	nCount < m_nDps;
	nCount++)
	{
		m_arrDpBestWays[nCount].WriteForTest(fp,nCount+1);
	}
}
