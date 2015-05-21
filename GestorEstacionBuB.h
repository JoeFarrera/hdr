// GestorEstacionBuB.h: interface for the CGestorEstacionBuB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GESTORESTACIONBUB_H__14D600EC_5890_4D63_A87E_22D9D7C83FF8__INCLUDED_)
#define AFX_GESTORESTACIONBUB_H__14D600EC_5890_4D63_A87E_22D9D7C83FF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EstacionBuB.h"

class CGestorEstacionBuB  
{
public:
	void InitStations();
	void SetPtrCourse(CCourseAGVType* pCourse);
	void SetEstacionBuB (UINT mp, UINT pos, UCHAR lage, LPCTSTR estDes, UINT nAgvDes);
	CEstacionBuB * SetEstacionBuB (UINT mp, UINT pos, UCHAR lage, bool fError, bool fCargado);
	void UpdateEstacion();
	CGestorEstacionBuB();
	virtual ~CGestorEstacionBuB();
	CTypedPtrArray <CObArray, CEstacionBuB*> m_EstacionPtrArray; // Array of orden pointers
private:
	CCourseAGVType* m_pCourse;
public:
	CEstacionBuB * FindEstacionBuB(LPCTSTR psClientName);
};

#endif // !defined(AFX_GESTORESTACIONBUB_H__14D600EC_5890_4D63_A87E_22D9D7C83FF8__INCLUDED_)
