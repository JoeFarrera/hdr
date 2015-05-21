// Block.h: interface for the CBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCK_H__260AE4C0_41A0_4A89_A972_2D75AA7B62D9__INCLUDED_)
#define AFX_BLOCK_H__260AE4C0_41A0_4A89_A972_2D75AA7B62D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SituationSet.h"
//#include "v_block.h"
#include "TABSETFV.h"


class CBlock  
{
public:
	BlStreExt_Typ* GetBlStreExtBlock();
	BlStre_Typ* GetBlStreRoot();
	unsigned long* GetVerBlStreRoot();
	UINT ReadAsAutoBlk (CFile &file);
	UINT ReadBlockTab();
	CBlock();
	virtual ~CBlock();

private:
	UINT CheckBlockTab(UINT nAnzStrecken);
	BlStre_Typ * m_pBlStre_Root;	// Pointer to the blocking table 
	BlStreExt_Typ * m_pBlStreExt_Root;	// Pointer to the blocking table (extension part) 
	unsigned long * m_pVer_BlStre_Root;	// Pointer to index vector of distances 
};

//extern CBlock g_Block;

#endif // !defined(AFX_BLOCK_H__260AE4C0_41A0_4A89_A972_2D75AA7B62D9__INCLUDED_)
