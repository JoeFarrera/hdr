#include "..\stdafx.h"
#include "CManBlockArea.h"

CManBlockArea::CManBlockArea()
{
	m_nMBArea    = 0;
	m_nObjNumber = 0;
	m_nObjIndex  = 0;
	m_nOccAGV    = 0;
}

CManBlockArea::CManBlockArea(int nMBArea,int nObjNumber,unsigned int nObjIndex)
{
	m_nMBArea    = nMBArea;
	m_nObjNumber = nObjNumber;
	m_nObjIndex  = nObjIndex;
	m_nOccAGV    = 0;
}

CManBlockArea::~CManBlockArea()
{
}

bool CManBlockArea::HasDistance(int nCp,int nNCp,CManBlockAreaObjectArray& arrMBAO)
{
	bool bRet = false;
	for (int nCount = 0;
	     (nCount < m_nObjNumber) && !bRet;
	     nCount++)
	{
		bRet = (arrMBAO[m_nObjIndex + nCount].GetCp ()  == nCp ) &&
		       (arrMBAO[m_nObjIndex + nCount].GetNCp() == nNCp);
	}
	return bRet;
}
