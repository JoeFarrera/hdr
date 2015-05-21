// MsgA0.h: interface for the CMsgA0 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGA0_H__C36346B2_24E7_11D5_992D_0010B547EBE6__INCLUDED_)
#define AFX_MSGA0_H__C36346B2_24E7_11D5_992D_0010B547EBE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "konstant.h"
#include "macro.h"
#include "komdat.h"
#include "zs_bed.h"
#include "bed_zs.h"


class CMsgA0 : public CObject
{
protected:
	DECLARE_DYNCREATE(CMsgA0)

public:
	Init();
	virtual void Serialize(CArchive& ar);
	CMsgA0();
	virtual ~CMsgA0();
	CString m_strText;
	CStringList m_msgList;
	empf_a0_tele m_Tele;


};

#endif // !defined(AFX_MSGA0_H__C36346B2_24E7_11D5_992D_0010B547EBE6__INCLUDED_)
