// MsgA0.cpp: implementation of the CMsgA0 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tpc.h"
#include "MsgA0.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CMsgA0, CObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgA0::CMsgA0()
{

}

CMsgA0::~CMsgA0()
{

}

void CMsgA0::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		// Enviar telegrama al BuB
		
		// ar.Write (m_Tele.TKopf.ucId, sizeof (m_Tele.TKopf.ucId));
		ar.Write ((const void *)&m_Tele.TKopf, sizeof (m_Tele.TKopf));

		ar.Write ((const void *)&m_Tele.AtDat, sizeof (m_Tele.AtDat));

/*
		ar << m_Tele.TKopf.usType;
        ar << m_Tele.TKopf.usValid;
        ar << m_Tele.TKopf.usIndex;
        ar << m_Tele.TKopf.usSize;
        ar << m_Tele.TKopf.usMaxItems;
		
		ar << m_Tele.AtDat.bnr;                 
		ar << m_Tele.AtDat.herkunft;            
		ar << m_Tele.AtDat.type;                
		ar << m_Tele.AtDat.art;                 
		ar << m_Tele.AtDat.lauf_nr;             
		ar << m_Tele.AtDat.at_lauf_nr;          
		ar << m_Tele.AtDat.af_status;           
		ar << m_Tele.AtDat.zw_status_ziel;      
		
		ar << m_Tele.AtDat.zw_status_kont; 
		
		ar << m_Tele.AtDat.prio_org;       
		ar << m_Tele.AtDat.akt_org;        
		ar << m_Tele.AtDat.prio_akt;       
		ar << m_Tele.AtDat.aktion;         
		ar << m_Tele.AtDat.lasttyp;        
		
		
		ar.Write (m_Tele.AtDat.id_nr, sizeof (m_Tele.AtDat.id_nr));    
		ar.Write (m_Tele.AtDat.SonID, sizeof (m_Tele.AtDat.SonID));
		ar.Write (m_Tele.AtDat.HzString,sizeof (m_Tele.AtDat.HzString));
		ar.Write (m_Tele.AtDat.BzString, sizeof (m_Tele.AtDat.BzString));

		ar << m_Tele.AtDat.hz.nr;
        ar << m_Tele.AtDat.hz.pos;
		ar << m_Tele.AtDat.hz.lage;
		ar << m_Tele.AtDat.hz.ebene;
		ar << m_Tele.AtDat.bz.nr;
        ar << m_Tele.AtDat.bz.pos;
		ar << m_Tele.AtDat.bz.lage;
		ar << m_Tele.AtDat.bz.ebene;
		ar << m_Tele.AtDat.az.nr;
        ar << m_Tele.AtDat.az.pos;
		ar << m_Tele.AtDat.az.lage;
		ar << m_Tele.AtDat.az.ebene;
		ar << m_Tele.AtDat.akt_z.nr;
        ar << m_Tele.AtDat.akt_z.pos;
		ar << m_Tele.AtDat.akt_z.lage;
		ar << m_Tele.AtDat.akt_z.ebene;
		ar << m_Tele.AtDat.org_z.nr;
        ar << m_Tele.AtDat.org_z.pos;
		ar << m_Tele.AtDat.org_z.lage;
		ar << m_Tele.AtDat.org_z.ebene;
		ar << m_Tele.AtDat.fznr;           
		ar << m_Tele.AtDat.quitt;          
		ar << m_Tele.AtDat.Kennung;             
		ar << m_Tele.AtDat.Status;              
		ar.Write (m_Tele.AtDat.HostIdNr, sizeof (m_Tele.AtDat.HostIdNr));
		ar.Write (m_Tele.AtDat.Quelle[0].nr,sizeof (m_Tele.AtDat.Quelle[0].nr));    
		ar.Write (m_Tele.AtDat.Quelle[1].nr,sizeof (m_Tele.AtDat.Quelle[1].nr));    
		ar << m_Tele.AtDat.Quelle[0].pos;
		ar << m_Tele.AtDat.Quelle[1].pos;
		ar << m_Tele.AtDat.Quelle[0].ebene;
		ar << m_Tele.AtDat.Quelle[1].ebene;
		ar << m_Tele.AtDat.Quelle[0].lage;
		ar << m_Tele.AtDat.Quelle[1].lage;
		ar.Write (m_Tele.AtDat.Senke[0].nr,sizeof (m_Tele.AtDat.Senke[0].nr));    
		ar.Write (m_Tele.AtDat.Senke[1].nr,sizeof (m_Tele.AtDat.Senke[1].nr));    
		ar << m_Tele.AtDat.Senke[0].pos;
		ar << m_Tele.AtDat.Senke[1].pos;
		ar << m_Tele.AtDat.Senke[0].ebene;
		ar << m_Tele.AtDat.Senke[1].ebene;
		ar << m_Tele.AtDat.Senke[0].lage;
		ar << m_Tele.AtDat.Senke[1].lage;
		ar << m_Tele.AtDat.datum.jahr;
		ar << m_Tele.AtDat.datum.monat;
		ar << m_Tele.AtDat.datum.tag;
		ar << m_Tele.AtDat.datum.stunde;
		ar << m_Tele.AtDat.datum.minute;
		ar << m_Tele.AtDat.datum.sekunde;
		ar << m_Tele.AtDat.ds_status;      
		ar << m_Tele.AtDat.ds_fznr;        
		ar << m_Tele.AtDat.gesp_at;        
		ar << m_Tele.AtDat.gesp_hz;        
		ar << m_Tele.AtDat.gesp_bz;        
		ar << m_Tele.AtDat.HostStatus;

  */

		m_strText = _T("A0 sent");

/*		LPTSTR p = m_strText.GetBuffer(m_Tele.nTeleLaenge+3);
		for (unsigned int i= 0; i<m_Tele.nTeleLaenge; i++)
		{
			if (m_Tele.nData[i] <= 9)
				p[i] = m_Tele.nData[i] +0x30;
			else p[i] = m_Tele.nData[i];
		}
		
		p[m_Tele.nTeleLaenge] = 0;
		m_strText.ReleaseBuffer();
		m_msgList.AddTail(m_strText);

*/	}
	else
	{
		TRACE (_T("MsgA0::Serialize (): Attempt to read\n"));
/*
		
		LPTSTR p = m_strText.GetBuffer(m_Tele.nTeleLaenge+3);
		
		for (unsigned int i= 0; i<m_Tele.nTeleLaenge; i++)
		{
			if (m_Tele.nData[i] <= 0x09)
				p[i] = m_Tele.nData[i] +0x30;
			else p[i] = m_Tele.nData[i];
			
		}
		
		
		p[m_Tele.nTeleLaenge] = 0;
		m_strText.ReleaseBuffer();
		m_msgList.AddTail(m_strText);
*/		
		
	}

}

CMsgA0::Init()
{
	m_Tele.TKopf.ucId [0] = 'A';
	m_Tele.TKopf.ucId [1] = '0';
	m_Tele.TKopf.usMaxItems = 1;
	// que más

}
