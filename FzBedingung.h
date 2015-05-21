// FzBedingung.h: Schnittstelle für die Klasse CFzBedingung.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FZBEDINGUNG_H__0B5CDFA0_C6D4_11D5_A257_00A024A87022__INCLUDED_)
#define AFX_FZBEDINGUNG_H__0B5CDFA0_C6D4_11D5_A257_00A024A87022__INCLUDED_

#include "konstant.h"
#include "komdat.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFzBedingung  
{
public:
	CFzBedingung(USHORT nLMp,USHORT nMp,USHORT nMpPos,USHORT nNMp,USHORT nUEMp,USHORT nZiel,USHORT nPos, USHORT nFV);
	bool operator == (const CFzBedingung &tFzBed) const;
	CFzBedingung(CFzBedingung &tBaseBed,CFzBedingung &tBedListe);
	USHORT m_nLMp,
		   m_nMp,
		   m_nMpPos;
	USHORT m_nNMp,
		   m_nUEMp,
		   m_nZiel,
		   m_nPos;
	USHORT m_nFV;
	CFzBedingung();
	CFzBedingung(TYP_FZDAT &tFzDat);
	virtual ~CFzBedingung();

};

typedef CList<CFzBedingung,CFzBedingung &>CFzBedingungList;

#endif // !defined(AFX_FZBEDINGUNG_H__0B5CDFA0_C6D4_11D5_A257_00A024A87022__INCLUDED_)
