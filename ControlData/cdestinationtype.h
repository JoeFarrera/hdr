#ifndef _CDESTINATIONTYPE_H_
#define _CDESTINATIONTYPE_H_

#include "CDestTypePos.h"
#include "CColumnType.h"
#include "CLayerType.h"

class CDestinationType
{
public:
	void WriteForTest(FILE* pFile,int nIndex);
	bool IsAction(int nAction,short nPos,unsigned char nLayer,
				  CDestTypePosList&      listDestTypePos,
				  CUIntArray&            arrColumnTypeIndex,
				  CColumnTypeArray&      arrColumnTypes,
				  CUIntArray&            arrLayerTypeIndex,
				  CLayerTypeArray&       arrLayerTypes);
	unsigned short GetPositions(){return m_nPositions;}
	unsigned short GetPosIndex();
	unsigned short GetDestType(){return m_nDestType;}
	void SetPositions(unsigned short nPositions){m_nPositions = nPositions;}
	CDestinationType();
	CDestinationType(unsigned short nDestType,
					 unsigned short nPosIndex,
					 unsigned short nPositions);
	virtual ~CDestinationType();
	
private:
	unsigned short m_nPositions;
	unsigned short m_nPosIndex;
	unsigned short m_nDestType;
};

typedef CArray<CDestinationType, CDestinationType&> CDestinationTypeArray;

#endif //_CDESTINATIONTYPE_H_
