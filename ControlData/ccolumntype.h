#ifndef _CCOLUMNTYPE_H_
#define _CCOLUMNTYPE_H_


#include "CLayerType.h"

class CColumnType
{
public:
	void WriteForTest(FILE* pFile,unsigned int nIndex);
	bool IsAction(int              nAction,
				  CUIntArray&      arrLayerTypeIndex,
				  CLayerTypeArray& arrLayerTypes);
	unsigned short GetLayers();
	unsigned short m_auLayerType[10];
	unsigned short GetType(){return m_nCType;}
	CColumnType();
	CColumnType(unsigned short nType,unsigned short nLType);
	CColumnType(unsigned short nType,unsigned short* pLType);
	virtual ~CColumnType();
	
private :
	unsigned short m_nCType;
};

typedef CArray<CColumnType, CColumnType&> CColumnTypeArray;

#endif //_CCOLUMNTYPE_H_
