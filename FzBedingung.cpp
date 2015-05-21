// FzBedingung.cpp: Implementierung der Klasse CFzBedingung.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdafx.h"
#include "FzBedingung.h"

#include "macro.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CFzBedingung::CFzBedingung()
{

}

CFzBedingung::~CFzBedingung()
{

}

CFzBedingung::CFzBedingung(TYP_FZDAT &tFzDat)
{
	m_nMp    = tFzDat.mp;
	m_nNMp   = tFzDat.nmp;
	m_nLMp   = tFzDat.lmp;
	m_nMpPos = tFzDat.mppos;
	m_nUEMp  = tFzDat.uemp;
	m_nZiel  = tFzDat.fzziel.nDest;
	m_nPos   = tFzDat.fzziel.nPos;
	if(TEST_AGV_STOP(tFzDat))
	{
	   m_nFV    = 1;
	}
	else
	{
	   m_nFV    = 0;
	}
}

CFzBedingung::CFzBedingung(CFzBedingung &tBaseBed, CFzBedingung &tBedListe)
{
	*this = tBaseBed;
	if(tBedListe.m_nFV != 0)
	{
		m_nFV = tBedListe.m_nFV - 1;
	}
	if(tBedListe.m_nMp != 0)
	{
		m_nMp = tBedListe.m_nMp;
	}
	if(tBedListe.m_nNMp != 0)
	{
		m_nNMp = tBedListe.m_nNMp;
	}
	if(tBedListe.m_nLMp != 0)
	{
		m_nLMp = tBedListe.m_nLMp;
	}
	if(tBedListe.m_nMpPos != 0)
	{
		m_nMpPos = tBedListe.m_nMpPos;
	}
	if(tBedListe.m_nUEMp != 0)
	{
		m_nUEMp = tBedListe.m_nUEMp;
	}
	if(tBedListe.m_nZiel != 0)
	{
		m_nZiel = tBedListe.m_nZiel;
	}
	if(tBedListe.m_nPos != 0)
	{
		m_nPos = tBedListe.m_nPos;
	}
}

bool CFzBedingung::operator ==(const CFzBedingung &tFzBed) const
{
	return( (m_nLMp   == tFzBed.m_nLMp) &&
			(m_nMp    == tFzBed.m_nMp) &&
			(m_nMpPos == tFzBed.m_nMpPos) &&
			(m_nNMp   == tFzBed.m_nNMp) &&
			(m_nUEMp  == tFzBed.m_nUEMp) &&
			(m_nZiel  == tFzBed.m_nZiel) &&
			(m_nPos   == tFzBed.m_nPos) &&
			(m_nFV    == tFzBed.m_nFV) );
}

CFzBedingung::CFzBedingung(USHORT nLMp,
						   USHORT nMp,
						   USHORT nMpPos,
						   USHORT nNMp,
						   USHORT nUEMp,
						   USHORT nZiel,
						   USHORT nPos,
						   USHORT nFV)
{
	m_nLMp   = nLMp;
	m_nMp    = nMp;
	m_nMpPos = nMpPos;
	m_nNMp   = nNMp;
	m_nUEMp  = nUEMp;
	m_nZiel  = nZiel;
	m_nPos   = nPos;
	m_nFV    = nFV;
}
