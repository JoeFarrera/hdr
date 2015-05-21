#ifndef _CAIDCPDISTANCE_H_
#define _CAIDCPDISTANCE_H_


class CAIDCpDistance
{
public:
	void SetOccAGV(unsigned short nOccAGV){m_nOccAGV = nOccAGV;}
	unsigned short GetOccAGV(){return m_nOccAGV;}
	int GetNCp(){return m_nNCp;}
	unsigned int GetDistIndex(){return m_nDistIndex;}
	CAIDCpDistance();
	CAIDCpDistance(int nNCp,unsigned int nDistIndex,unsigned short nOccAGV);
	virtual ~CAIDCpDistance();

private:	
	int            m_nNCp;
	unsigned int   m_nDistIndex;
	unsigned short m_nOccAGV;          // str
	
};

typedef CArray<CAIDCpDistance, CAIDCpDistance&> CAIDCpDistanceArray;

#endif //_CAIDCPDISTANCE_H_
