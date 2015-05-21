#ifndef _CSIMPLEDISTANCE_H_
#define _CSIMPLEDISTANCE_H_

class CSimpleDistance
{
public:
	bool operator> (const CSimpleDistance& tSD);
	bool operator==(const CSimpleDistance& tSD);
	CSimpleDistance(int nCp,int nNCp);
	CSimpleDistance();
	virtual ~CSimpleDistance();
	
	int m_nCp;
	int m_nNCp;
};

#endif //_CSIMPLEDISTANCE_H_
