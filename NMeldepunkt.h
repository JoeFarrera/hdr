// NMeldepunkt.h: interface for the CNMeldepunkt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMELDEPUNKT_H__D1C06F75_A10A_4085_A721_685B6EB66976__INCLUDED_)
#define AFX_NMELDEPUNKT_H__D1C06F75_A10A_4085_A721_685B6EB66976__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KONSTANT.H"
#include "typedef.h"


class CNMeldepunkt : public CObject  
{
public:
	DECLARE_SERIAL (CNMeldepunkt)
	NMP_tabelle m_NMp;
	CNMeldepunkt();
	virtual ~CNMeldepunkt();
	virtual void Serialize (CArchive &ar);

};

#endif // !defined(AFX_NMELDEPUNKT_H__D1C06F75_A10A_4085_A721_685B6EB66976__INCLUDED_)
