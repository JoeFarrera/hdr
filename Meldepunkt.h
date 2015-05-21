// Meldepunkt.h: interface for the CMeldepunkt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MELDEPUNKT_H__3C92220C_38BA_46D0_A4A4_36CFF6B5DBBE__INCLUDED_)
#define AFX_MELDEPUNKT_H__3C92220C_38BA_46D0_A4A4_36CFF6B5DBBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KONSTANT.H"
#include "KOMDAT.H"
#include "a1_konst.c86"
#include "typedef.h"

#include "NMeldepunkt.h"

class CMeldepunkt : public CObject  
{
public:
	DECLARE_SERIAL (CMeldepunkt)
	virtual void Serialize (CArchive &ar);
	CString m_strCustName;
	void GetStationPtr();
	TYP_STATDAT * m_ptrStat;
	int m_numMP;	// número del MP
	CString m_strNMP;	// String del MP (lista de Next MP)
	CMeldepunkt();	
	virtual ~CMeldepunkt();
	Meldepunkttabelle m_Mp;	// Communication point structure
	CTypedPtrArray <CObArray, CNMeldepunkt*> m_NmpArray; // Pointer to next point array (array of pointers)


private:
};

#endif // !defined(AFX_MELDEPUNKT_H__3C92220C_38BA_46D0_A4A4_36CFF6B5DBBE__INCLUDED_)
