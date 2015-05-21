#pragma once

class CAgvSockType
{
public:
	CAgvSockType(CAGVSockThread * ptrThread, int idAgv);
	CAgvSockType(void);
	~CAgvSockType(void);
	CAGVSockThread *m_pAgvSockThread;
	UINT m_id;	// Agv no.

};

typedef CArray<CAgvSockType, CAgvSockType&> CAgvSockTypeArray;

