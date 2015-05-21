#ifndef _CAIDCOMMPOINT_H_
#define _CAIDCOMMPOINT_H_


class CAIDCommPoint
{
public:
	void SetDistIndex(unsigned int nDistIndex){m_nDistIndex = nDistIndex;}
	void SetNCps(unsigned char nNCps){m_nNCps = nNCps;}
	void SetOccAGV(unsigned short nOccAGV){m_nOccAGV = nOccAGV;}
	void SetOccStopAGV(unsigned short nOccStopAGV){m_nOccStopAGV = nOccStopAGV;}
	void SetAGVTypes(unsigned int nAGVTypes){m_nAGVTypes = nAGVTypes;}
	void SetAGVTypesDest(unsigned int nAGVTypesDest){m_nAGVTypesDest = nAGVTypesDest;}
	int GetCp(){return m_nCp;}
	unsigned char GetNCps(){return m_nNCps;}
	unsigned int GetAGVTypesDest(){return m_nAGVTypesDest;}
	unsigned int GetDistIndex(){return m_nDistIndex;}
	unsigned short GetOccAGV(){return m_nOccAGV;}
	unsigned short GetOccStopAGV(){return m_nOccStopAGV;}
	CAIDCommPoint();
	CAIDCommPoint(int nCp, int nCpIndex);
	virtual ~CAIDCommPoint();
	
private:
	unsigned int	m_nCp;
	unsigned char	m_nNCps;
	unsigned int	m_nCpIndex;
	unsigned short	m_nOccAGV;          // str0
	unsigned short	m_nOccStopAGV;      // fz_halt
	unsigned int    m_nAGVTypes;
	unsigned int	m_nAGVTypesDest;
	unsigned int    m_nDistIndex;
};

typedef CArray<CAIDCommPoint, CAIDCommPoint&> CAIDCommPointArray;

#endif //_CAIDCOMMPOINT_H_
