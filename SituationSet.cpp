// SituationSet.cpp: Implementierung der Klasse CSituationSet.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "SituationSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

CSituationSet::CSituationSet()
{

}

CSituationSet::~CSituationSet()
{
	m_listSituation.RemoveAll();
}

BOOL CSituationSet::CheckSet(CFzBedingung &tABed, CFzBedingung &tVBed)
{
	BOOL bRet = !m_bSetFV;
	m_nResultSit = 0;

	POSITION pos = m_listSituation.GetHeadPosition();
	while((pos != NULL) && (bRet == ! m_bSetFV))
	{
		CFzSituation tFzSit = m_listSituation.GetNext(pos);
		bRet = tFzSit.CheckSit(tABed,
			                   tVBed,
							   m_bSetFV,
							   m_nResultBed);
		tFzSit.m_listVBed.RemoveAll();
		m_nResultSit++;
	}
	return bRet;
}

void CSituationSet::Read(CString strFile)
{
	CStdioFile   tFile;
	CString      strZeile;
	BOOL         bRead,
		         bSit = FALSE;
	USHORT       auVal[10];
	CFzSituation tFzSit;
	CString      strTyp;

	// 22.02.2002 May be reinitialized
	m_listSituation.RemoveAll ();

	if(tFile.Open(strFile,CFile::modeRead))
	{
		do
		{
			bRead = tFile.ReadString(strZeile);
			strZeile.TrimLeft();
			// In strZeile Whitespaces entfernen !
			for(int i = 0;
			i < 10;
			i++)
			{
				auVal[i] = 0;
			}
			if(!strZeile.IsEmpty() && strZeile[0] != ';')
			{
				if(bRead)
				{
					strTyp.LoadString(IDS_ASITUATION);
					int i = sscanf(strZeile,strTyp, &auVal[0],
						&auVal[1],
						&auVal[2],
						&auVal[3],
						&auVal[4],
						&auVal[5],
						&auVal[6],
						&auVal[7]);
					if(i > 0)
					{
						// ASit gefunden
						if(bSit)
						{
							// FzSituation in Liste aufnehmen,
							// wenn Bedingungsliste nicht leer !
							if(tFzSit.m_listVBed.GetCount() > 0)
							{
								m_listSituation.AddTail(tFzSit);
								tFzSit.m_listVBed.RemoveAll();
							}
							else
							{
								// MessageBox: Situation-Defintion ohne Vergleichsbedingungen
							}
						}
						tFzSit = CFzSituation(CFzBedingung(auVal[0],
							auVal[1],
							auVal[2],
							auVal[3],
							auVal[4],
							auVal[5],
							auVal[6],
							auVal[7]));
						bSit = TRUE;
						
					}
					else
					{
						// nach VSit suchen
						strTyp.LoadString(IDS_VSITUATION);
						int i = sscanf(strZeile,strTyp, &auVal[0],
							&auVal[1],
							&auVal[2],
							&auVal[3],
							&auVal[4],
							&auVal[5],
							&auVal[6],
							&auVal[7]);
						if(i >= 1)
						{
							// VSit gefunden !
							if(bSit)
							{
								CFzBedingung tBed(auVal[0],
									auVal[1],
									auVal[2],
									auVal[3],
									auVal[4],
									auVal[5],
									auVal[6],
									auVal[7]);
								tFzSit.m_listVBed.AddTail(tBed);
							}
							else
							{
								// MessageBox: Vergleich-Situation gefunden ohne
								// ASituation !
							}
						}
						else
						{
							// MessageBox : Unerwartete/unbekannte Zeile:
						}
					}
				}
			}
		}while (bRead);
		if(bSit)
		{
			// Letzte FzSituation in Liste aufnehmen,
			// wenn Bedingungsliste nicht leer !
			if(tFzSit.m_listVBed.GetCount() > 0)
			{
				m_listSituation.AddTail(tFzSit);
				tFzSit.m_listVBed.RemoveAll();
			}
			else
			{
				// MessageBox: Situation-Defintion ohne Vergleichsbedingungen
			}
		}
		
		tFile.Close();
	}
	else
	{
		// MessageBox: Keine Datei gefunden !
	}
}
