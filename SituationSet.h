// SituationSet.h: Schnittstelle für die Klasse CSituationSet.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SITUATIONSET_H__0B5CDFA2_C6D4_11D5_A257_00A024A87022__INCLUDED_)
#define AFX_SITUATIONSET_H__0B5CDFA2_C6D4_11D5_A257_00A024A87022__INCLUDED_

#include "FzSituation.h"	// Hinzugefügt von der Klassenansicht
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSituationSet  
{
public:
	int m_nResultBed;
	int m_nResultSit;
	void Read(CString strFile);
	BOOL CheckSet(CFzBedingung &tABed,CFzBedingung &tVBed);
	BOOL m_bSetFV;
	CFzSituationList m_listSituation;
	CSituationSet();
	virtual ~CSituationSet();

};

#endif // !defined(AFX_SITUATIONSET_H__0B5CDFA2_C6D4_11D5_A257_00A024A87022__INCLUDED_)
