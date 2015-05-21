#ifndef _CDPBESTWAYS_H_
#define _CDPBESTWAYS_H_



#include "CLoaderFile.h"
#include "CCtrlDataMemFile.h"

class CDpBestWays
{
public:
	void WriteForTest(FILE* pFile,int nIndex);
	unsigned int GetNCp(int nCpArea);
	bool Load(CCtrlDataMemFile& myMFile,int nCpAreas);
	bool Load(CLoaderFile*       pLFile,int nCpAreas); 
	CDpBestWays();
	virtual ~CDpBestWays();
private:
	int m_nDp;
	CUIntArray m_arrBestNCps;	// CWord instead of CUShort
};

typedef CArray<CDpBestWays, CDpBestWays&> CDpBestWaysArray;

#endif //_CDPBESTWAYS_H_
