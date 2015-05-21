#ifndef _CDESTTYPEPOS_H_
#define _CDESTTYPEPOS_H_


#include "CColumnType.h"
#include "CLayerType.h"

class CDestTypePos
{
public:
	void WriteForTest(FILE*pFile,int nIndex);
	bool IsAction(int nAction,unsigned char nLayer,
				  CUIntArray&            arrColumnTypeIndex,
				  CColumnTypeArray&      arrColumnTypes,
				  CUIntArray&            arrLayerTypeIndex,
				  CLayerTypeArray&       arrLayerTypes);
	CDestTypePos();
	CDestTypePos(unsigned char  nMedium,
				 char           cLayerColumn,
				 unsigned short nType,
				 int			nPosWay,
				 unsigned short nDestRoutine);
	virtual ~CDestTypePos();
	
	unsigned short m_nDestRoutine;
	int            m_nPosWay;
	unsigned short m_nType;
	char           m_cLayerColumn;
	unsigned char  m_nMedium;
	
};

typedef CList <CDestTypePos,CDestTypePos&> CDestTypePosList;

#endif //_CDESTTYPEPOS_H_
