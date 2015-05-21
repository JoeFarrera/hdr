#ifndef _CBESTWAYS_H_
#define _CBESTWAYS_H_

#include "CLoaderFile.h"
#include "CDpBestWays.h"

class CBestWays
{
public:
	void WriteBestWaysForTest(FILE* fp);
	unsigned int GetNCpOfDp(int nDpIndex,int nCpArea);
	bool Load(CLoaderFile* pLFile); 
	bool Load(CCtrlDataMemFile& myMFile);
	CBestWays();
	virtual ~CBestWays();
private:
	int m_nDps;
	int m_nCpAreas;
	CDpBestWaysArray m_arrDpBestWays;
};

#endif //_CBESTWAYS_H_
