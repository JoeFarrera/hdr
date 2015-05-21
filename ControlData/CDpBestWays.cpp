#include "..\stdafx.h"
#include "CDpBestWays.h"

CDpBestWays::CDpBestWays()
{
}

CDpBestWays::~CDpBestWays()
{
}

bool CDpBestWays::Load(CCtrlDataMemFile& myMFile,int nCpAreas)
{
	int nDp,
	    nNCp;
	bool bDataOK = myMFile.Read(nDp);
	if(bDataOK)
	{
		m_nDp = nDp;
		m_arrBestNCps.SetSize(nCpAreas);
		for(int nCount = 0;
		(nCount < nCpAreas) && bDataOK;
		nCount++)
		{
			bDataOK = myMFile.Read(nNCp);
			if(bDataOK)
			{
				m_arrBestNCps[nCount] = nNCp;
			}
		}
	}
	return bDataOK;
}

bool CDpBestWays::Load(CLoaderFile* pLFile,int nCpAreas)
{
	int nDp,
	    nNCp;
	bool bDataOK = pLFile->Read(nDp);
	if(bDataOK)
	{
		m_nDp = nDp;
		m_arrBestNCps.SetSize(nCpAreas);
		for(int nCount = 0;
		(nCount < nCpAreas) && bDataOK;
		nCount++)
		{
			bDataOK = pLFile->Read(nNCp);
			if(bDataOK)
			{
				m_arrBestNCps[nCount] = nNCp;
			}
		}
	}
	return bDataOK;
}

unsigned int CDpBestWays::GetNCp(int nCpArea)
{
	return m_arrBestNCps[nCpArea - 1];
}

void CDpBestWays::WriteForTest(FILE* pFile,int nIndex)
{
	fprintf(pFile,"%d.Dp %d :\n",
			nIndex,
			m_nDp);
	unsigned int nCpAreas = m_arrBestNCps.GetSize();
	for(unsigned int nCount = 0;
	nCount < nCpAreas;
	nCount++)
	{
		if(nCount == 0)
		{
			fprintf(pFile,"%4d",m_arrBestNCps[nCount]);
		}
		else
		{
			if((nCount+1) == nCpAreas)
			{
				fprintf(pFile,",%4d\n",m_arrBestNCps[nCount]);
			}
			else
			{
				if(div(nCount,10).rem == 0)
				{
					fprintf(pFile,"/n%4d",m_arrBestNCps[nCount]);
				}
				else
				{
					fprintf(pFile,",%4d",m_arrBestNCps[nCount]);
				}
			}
		}
	}
}
