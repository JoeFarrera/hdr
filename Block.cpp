// Block.cpp: implementation of the CBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "Block.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlock::CBlock()
{
	m_pVer_BlStre_Root = NULL;
	m_pBlStre_Root = NULL;
	m_pBlStreExt_Root = NULL;
}

CBlock::~CBlock()
{
	if(m_pBlStre_Root != NULL)
	{
		delete [] m_pBlStre_Root;
		m_pBlStre_Root = NULL;
	}
	if(m_pBlStreExt_Root != NULL)
	{
		delete [] m_pBlStreExt_Root;
		m_pBlStreExt_Root = NULL;
	}
	if(m_pVer_BlStre_Root != NULL)
	{
		delete [] m_pVer_BlStre_Root;
		m_pVer_BlStre_Root = NULL;
	}
}

UINT CBlock::ReadBlockTab()
{
	CFile          file;
	unsigned int   nAnzStrecken = 0;
	unsigned int   ok = 0;
	CString strWorkPath;
	CString strFile;

	/* Michael 03.11.2004
	strWorkPath.LoadString (IDS_FILEPATH);
	TRACE("\nreading blocking table...");
    strFile = strWorkPath + "\\autoblk.bin";
	Michael 03.11.2004 */

	// Michael 03.11.2004 Get autoblk file from registry (CURRENT_USER/SOFTWARE/INDUMAT/tpc/Settings)

	strFile = AfxGetApp ()->GetProfileString ("Settings", "AutoblkFile");
	TRACE ("Autoblock file is: %s", strFile);


    if (file.Open (strFile, CFile::modeRead | CFile::shareExclusive))
    {
		nAnzStrecken = ReadAsAutoBlk(file);
    }
    else
    {
		TRACE("\nblocking table %s not found !",strFile);
		ASSERT (false);	// Michael 03.11.2004 No point continuing
    }
	

	if(nAnzStrecken)
	{
		ok = CheckBlockTab(nAnzStrecken);
	}
	return nAnzStrecken;


}



UINT CBlock::ReadAsAutoBlk(CFile &file)
{
	unsigned int   nRet = 0,
		i,
		nAnzBlStrecken,
		nAnzStrecken,
		*pTmpBlkTab,
		*ptmp;
	BlStre_Typ     *pBlStr;
	BlStreExt_Typ  *pBlStrExt;
	
	i = file.Read (&nAnzStrecken,sizeof(unsigned int));
	
	if (nAnzStrecken <= KKMAX_FAHR_ZEIT)
	{
		TRACE(" number of blocking-distances OK");
		
		if (m_pVer_BlStre_Root != NULL)
		{
			delete [] m_pVer_BlStre_Root;
			m_pVer_BlStre_Root = NULL;
		}
		if (m_pBlStre_Root != NULL)
		{
			delete [] m_pBlStre_Root;
			m_pBlStre_Root = NULL;
		}
		if (m_pBlStreExt_Root != NULL)
		{
			delete [] m_pBlStreExt_Root;
			m_pBlStreExt_Root = NULL;
		}
		
		/* Indexliste auf erste blockierende Strecke fuer alle Strecken einlesen */
		// m_pVer_BlStre_Root = (unsigned long *) malloc(i);
		m_pVer_BlStre_Root = new unsigned long [nAnzStrecken + 1];
		
		
		ASSERT (m_pVer_BlStre_Root != NULL);
		
		// Read index vector
		i = (nAnzStrecken+1) * sizeof(unsigned int); // Bytes to read
		
		file.Read (m_pVer_BlStre_Root,i);
		
		/* blockierende Strecken mit Index, Blockweg,AbsKollisionsweg,... einlesen */
		// Calculate total number of blocking distances in the course
		nAnzBlStrecken = *(m_pVer_BlStre_Root+nAnzStrecken);
		
		// i = nAnzBlStrecken*sizeof(BlStre_Typ);
		
		// Allocate for the old part and the extension part
		m_pBlStre_Root    = new BlStre_Typ [nAnzBlStrecken];
		m_pBlStreExt_Root = new BlStreExt_Typ [nAnzBlStrecken];
		
		i = nAnzBlStrecken * 6 * sizeof(unsigned int);
		pTmpBlkTab = new unsigned int [nAnzBlStrecken * 6];
		
		
		
		ASSERT (((m_pBlStre_Root != NULL) && (m_pBlStreExt_Root != NULL) && (pTmpBlkTab != NULL)) == TRUE);
		i = file.Read(pTmpBlkTab, i);
		
        for(i = 0, pBlStr = m_pBlStre_Root, pBlStrExt = m_pBlStreExt_Root, ptmp = pTmpBlkTab;
		i <  nAnzBlStrecken;
		i++, pBlStr++, pBlStrExt++,ptmp += 6)
        {
			pBlStr->index         = (unsigned short) *ptmp;
			pBlStr->blockweg      = (unsigned short) *(ptmp + 1);
			pBlStr->kollisionsweg = (unsigned short) *(ptmp + 2);
			
			pBlStrExt->uZKWeg         = (unsigned short) *(ptmp + 3);
			pBlStrExt->uZKFreiWeg     = (unsigned short) *(ptmp + 4);
			pBlStrExt->uKWegFuerBlWeg = (unsigned short) *(ptmp + 5);
        }
        nRet = nAnzStrecken;
        delete [] pTmpBlkTab;
		
	}
	else
	{
		CString strTemp;
		AfxMessageBox (strTemp.LoadString (IDS_BLOCKNUMBERERROR));
		nRet = 0;
	}
	
	
	
	file.Close ();
	return nRet;
	
	
}

UINT CBlock::CheckBlockTab(UINT nAnzStrecken)
{
  unsigned int i,
               j,
               k,
               index1,
               ok,
               uBlTabOk;
  /* Schleife über alle Strecken der Tabelle */
  for (i = 0,uBlTabOk = 1;
       i < nAnzStrecken;
       i++)
  {
    /* Schleife über alle blockierenden Strecke der i-ten Strecke */
    for (j = m_pVer_BlStre_Root[i];
         j < m_pVer_BlStre_Root[i+1];
         j++)
    {
      index1 = m_pBlStre_Root[j/*-1*/].index-1;
      /* Schleife über alle blockierenden Strecken der j-ten blockierenden */
      /* Strecke der i-ten Strecke */
      for (k = m_pVer_BlStre_Root[index1], ok = 0;
           (k < m_pVer_BlStre_Root[index1+1]) && !ok;
           k++)
      {
        if (m_pBlStre_Root[k/*-1*/].index == (i+1))
        {
          /* die i-te Strecke ist blockierend zur j-ten Strecke, das ist ok ! */
          ok = 1;
        }
      }
      if (!ok)
      {
		CString strTemp; 
		strTemp.Format (IDS_BLOCKDISTANCEERROR, index1 + 1, i+1);
        AfxMessageBox (strTemp);
        uBlTabOk = 0;
      }
    }
  }
  if (uBlTabOk)
  {
    TRACE ("\nBlock-Tab ist ok !");
  }
  else
  {
    TRACE ("\nBlock-Tab ist unvollstaendig !");
  }
  return uBlTabOk;

}

unsigned long* CBlock::GetVerBlStreRoot()
{
	return m_pVer_BlStre_Root;
}

BlStre_Typ* CBlock::GetBlStreRoot()
{
	return m_pBlStre_Root;
}

BlStreExt_Typ* CBlock::GetBlStreExtBlock()
{
	return m_pBlStreExt_Root;
}
