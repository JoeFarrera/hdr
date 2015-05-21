// FzSituation.h: Schnittstelle für die Klasse CFzSituation.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FZSITUATION_H__0B5CDFA1_C6D4_11D5_A257_00A024A87022__INCLUDED_)
#define AFX_FZSITUATION_H__0B5CDFA1_C6D4_11D5_A257_00A024A87022__INCLUDED_

#include "FzBedingung.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFzSituation : public CFzBedingung 
{
public:
	CFzSituation(CFzBedingung & tFzBed);
	void operator =(CFzSituation &tFzSit);
	CFzSituation(CFzSituation  &tFzSit);
	BOOL CheckSit(CFzBedingung &tABed,
		          CFzBedingung &tVFz,
				  BOOL         bSetFV,
				  int          &nResult);
	CFzBedingungList m_listVBed;
	CFzSituation();
	virtual ~CFzSituation();
};

typedef CList<CFzSituation,CFzSituation &>CFzSituationList;

#endif // !defined(AFX_FZSITUATION_H__0B5CDFA1_C6D4_11D5_A257_00A024A87022__INCLUDED_)
