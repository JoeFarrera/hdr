// BlockingSituation.h: Schnittstelle für die Klasse CBlockingSituation.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKINGSITUATION_H__E37BB740_BBFC_11D5_9663_0050DAE18928__INCLUDED_)
#define AFX_BLOCKINGSITUATION_H__E37BB740_BBFC_11D5_9663_0050DAE18928__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBlockingSituation  
{
public:
	USHORT m_nVUEMp;
	USHORT m_nVNMp;
	USHORT m_nVMp;
	USHORT m_nVZiel;
	USHORT m_nANMp;
	USHORT m_nAMp;
	USHORT m_nAZiel;
	UCHAR  m_ucVFeFv;
	CBlockingSituation();
	CBlockingSituation(USHORT nAMp);
	virtual ~CBlockingSituation();

};

typedef CList<CBlockingSituation,CBlockingSituation&>CBlockingSituationList;

#endif // !defined(AFX_BLOCKINGSITUATION_H__E37BB740_BBFC_11D5_9663_0050DAE18928__INCLUDED_)
