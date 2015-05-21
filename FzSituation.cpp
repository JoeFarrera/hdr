// FzSituation.cpp: Implementierung der Klasse CFzSituation.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdafx.h"
#include "FzSituation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CFzSituation::CFzSituation()
{

}

CFzSituation::~CFzSituation()
{
	m_listVBed.RemoveAll();
}

BOOL CFzSituation::CheckSit(CFzBedingung &tABed,
							CFzBedingung &tVBed,
							BOOL         bSetFV,
							int          &nResult)
{
	BOOL bRet = !bSetFV;
	nResult   = 0;

	CFzBedingung tA(tABed,*this);
	if(tA == tABed)
	{
		// Schleife über V-Liste, da Bedingung für AFz erfüllt ist
		POSITION pos = m_listVBed.GetHeadPosition();
		while ((pos != NULL) && (bRet == !bSetFV))
		{
			CFzBedingung tVBedList = m_listVBed.GetNext(pos);
			CFzBedingung tV(tVBed,tVBedList);

			if(tV == tVBed)
			{
				bRet = bSetFV;
			}
			nResult++;
		}
	}
	return bRet;
}

CFzSituation::CFzSituation(CFzSituation &tFzSit)
{
	*this = tFzSit;
}

void CFzSituation::operator =(CFzSituation &tFzSit)
{
	m_nLMp   = tFzSit.m_nLMp;
	m_nMp    = tFzSit.m_nMp;
	m_nMpPos = tFzSit.m_nMpPos;
	m_nNMp   = tFzSit.m_nNMp;
	m_nUEMp  = tFzSit.m_nUEMp;
	m_nZiel  = tFzSit.m_nZiel;
	m_nPos   = tFzSit.m_nPos;
	m_nFV    = tFzSit.m_nFV;
	POSITION pos = tFzSit.m_listVBed.GetHeadPosition();
	while(pos != NULL)
	{
		CFzBedingung tFzBed = tFzSit.m_listVBed.GetNext(pos);
		m_listVBed.AddTail(tFzBed);
	}
}

CFzSituation::CFzSituation(CFzBedingung &tFzBed)
{
	m_nLMp   = tFzBed.m_nLMp;
	m_nMp    = tFzBed.m_nMp;
	m_nMpPos = tFzBed.m_nMpPos;
	m_nNMp   = tFzBed.m_nNMp;
	m_nUEMp  = tFzBed.m_nUEMp;
	m_nZiel  = tFzBed.m_nZiel;
	m_nPos   = tFzBed.m_nPos;
	m_nFV    = tFzBed.m_nFV;
}
