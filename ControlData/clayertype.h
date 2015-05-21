#ifndef _CLAYERTYPE_H_
#define _CLAYERTYPE_H_


class CLayerType
{
public:
	void WriteForTest(FILE* pFile,unsigned int nIndex);
	bool IsAction(int nAction);
	CLayerType();
	CLayerType(unsigned short nType,int nHeight,int nDepth,int nMaxPieces,
			   int nStartPrio,int nProperties, int nUserInfo, CString& strUserInfo);
	virtual ~CLayerType();
	
	CString         m_strUserInfo;
	int            m_nUserInfo;
	int            m_nProperties;
	int            m_nStartPrio;
	int            m_nMaxPieces;
	int            m_nDepth;
	int            m_nHeight;
	unsigned short m_nType;
	
};

typedef CArray<CLayerType, CLayerType&> CLayerTypeArray;

#endif //_CLAYERTYPE_H_
