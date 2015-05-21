// NMeldepunkt.cpp: implementation of the CNMeldepunkt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "NMeldepunkt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL (CNMeldepunkt, CObject, 1)

CNMeldepunkt::CNMeldepunkt()
{

}

CNMeldepunkt::~CNMeldepunkt()
{

}

void CNMeldepunkt::Serialize (CArchive &ar)
{
	CObject::Serialize (ar);
	if (ar.IsStoring ())
	{
		ar.Write (&m_NMp, sizeof (m_NMp));
	}
	else
	{
		ar.Read (&m_NMp, sizeof (m_NMp));
	}
}
