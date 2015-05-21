// Meldepunkt.cpp: implementation of the CMeldepunkt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "Meldepunkt.h"
#include "NMeldepunkt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL (CMeldepunkt, CObject, 1)

CMeldepunkt::CMeldepunkt()
{
	m_strCustName = (_T(""));
	m_strNMP = (_T(""));


}

CMeldepunkt::~CMeldepunkt()
{
	int i = m_NmpArray.GetSize ();
	for (; i > 0; i-- )
	{
		CNMeldepunkt * ptrnMp = m_NmpArray.GetAt (i - 1);
		m_NmpArray.RemoveAt (i - 1);
		delete ptrnMp;
	}
}

// if the point is a station, get the station index
void CMeldepunkt::GetStationPtr()
{
	if (m_Mp.typ & 0x02)
	{
		m_ptrStat = &statdat [vtstat [m_numMP]];
		m_strCustName = (LPTSTR)&kn_tab [vtstat [m_numMP]];
	}

}

void CMeldepunkt::Serialize (CArchive &ar)
{
	CObject::Serialize (ar);
	m_NmpArray.Serialize (ar);
	if (ar.IsStoring ())
	{
		ar.Write (&m_Mp, sizeof (m_Mp));
		ar << m_numMP;
		ar << m_strCustName;
		ar << m_strNMP;
	}
	else
	{
		ar.Read (&m_Mp, sizeof (m_Mp));
		ar >> m_numMP;
		GetStationPtr ();
		ar >> m_strCustName;
		ar >> m_strNMP;
	}
}

