// BlockingSituation.cpp: Implementierung der Klasse CBlockingSituation.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "BlockingSituation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CBlockingSituation::CBlockingSituation()
{

}
CBlockingSituation::CBlockingSituation(USHORT nAMp)
{
	m_nAMp = nAMp;
}

CBlockingSituation::~CBlockingSituation()
{

}
