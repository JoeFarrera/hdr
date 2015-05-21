#ifndef _CMANBLOCKAREA_H_
#define _CMANBLOCKAREA_H_

#include "CManBlockAreaObject.h"

class CManBlockArea
{
public:
	bool HasDistance(int nCp,int nNCp,CManBlockAreaObjectArray& arrMBAO);
	int            GetMBArea   (){return m_nMBArea;}
	unsigned short GetOccAGV   (){return m_nOccAGV;}
	unsigned int   GetObjIndex (){return m_nObjIndex;} 
	int            GetObjNumber(){return m_nObjNumber;}
	void SetOccAGV(unsigned short nOccAGV){ m_nOccAGV = nOccAGV;}
	CManBlockArea();
	CManBlockArea(int nMBArea,int nObjNumber,unsigned int nObjIndex);
	virtual ~CManBlockArea();
	
private:
	//Variable:
	int			           m_nMBArea;
	int			           m_nObjNumber;
	unsigned short         m_nOccAGV;
	unsigned int           m_nObjIndex;
};

typedef CArray<CManBlockArea, CManBlockArea&> CManBlockAreaArray;

#endif //_CMANBLOCKAREA_H_
