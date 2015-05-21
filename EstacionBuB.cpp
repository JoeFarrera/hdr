// EstacionBuB.cpp: implementation of the CEstacionBuB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "EstacionBuB.h"
#include "Alarma.h"
#include ".\estacionbub.h"
#include "DBEstacion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEstacionBuB::CEstacionBuB()
{
	m_uBuBIndex = 0;
	m_mp = 0;	
	m_pos = 0;
	m_lage = 0;
	m_typ = 0;
	m_closed = 0;
	m_error = 0;
	m_prio = 0;
	m_fCargado = 0;
	m_KundenNr = new CHAR [KMAX_KUNDE + 1];
	m_codEstDes = new CHAR [KMAX_KUNDE + 1];
	m_codEstDes [0] = '\0';
	m_numAgvDes = 0;
	m_fUpdateBuB = true;
	m_codpulvac = new CHAR [KMAX_KUNDE + 1];
	m_ocuPul = -1;
	m_capPul = -1;
}

CEstacionBuB::~CEstacionBuB()
{
	delete[] m_KundenNr;	//JLac
	delete[] m_codEstDes;	//JLac
	delete[] m_codpulvac;	//JLac
}

void CEstacionBuB::Init(UINT index, UINT pos, UCHAR lage)
{		
	CCourseAGVType* pCourse = g_pCourseData->GetPtrCourseAGVType(1);
	//m_mp = statdat [index].mp;	// Ziel.nr;
	m_mp = pCourse->GetPtrDestinationByIndex(index)->GetNumber();	// Ziel.nr;
	m_uBuBIndex = index + pos + (lage == 'L' ? 0 : 1);
	m_pos = pos;		// Ziel.pos;
	m_lage = lage; // Ziel.lage;

	CAGVTDestination* pDestination = pCourse->GetPtrDestinationByIndex(index);
	if ('L' == lage)
	{
		//m_typ = (unsigned char)e_typ_tab[z_typ_tab[zt_ver_tab[statdat[index].zieltyp_l]].index].ziel_art;
		//m_typ = (unsigned char)e_typ_tab[z_typ_tab[zt_ver_tab[pDestination->GetDestTypeLeft()]].index].ziel_art;
		m_typ = g_pCourseData->GetLayerTypeProperties(pDestination->GetDestTypeLeft(),pos - 1);
		//wsprintf(m_KundenNr, (LPTSTR)&kn_tab[statdat[index].ziel_index_l + pos]);
	}
	else
	{
		//m_typ = (unsigned char)e_typ_tab[z_typ_tab[zt_ver_tab[statdat[index].zieltyp_r]].index].ziel_art;
		//m_typ = (unsigned char)e_typ_tab[z_typ_tab[zt_ver_tab[pDestination->GetDestTypeRight()]].index].ziel_art;
		m_typ = g_pCourseData->GetLayerTypeProperties(pDestination->GetDestTypeRight(), pos - 1);
		//wsprintf(m_KundenNr, (LPTSTR)&kn_tab[statdat[index].ziel_index_r + pos]);		wsprintf(m_KundenNr, (LPTSTR)&kn_tab[pDestination->GetFirstDestPosIndex(lage) + pos]);
	}
	wsprintf(m_KundenNr, g_pCourseData->GetCustomerStationName(pDestination->GetNumber(), (char)lage, (int)pos, (int)1));
	m_pAIDStation = g_pCourseData->GetAIDStationPtr (pDestination->GetNumber(), (char)lage, (int)pos, (int)1);
}

// Si se debe actualizar el BuB, prepara el telegrama
bool CEstacionBuB::SetTelegramBuB(empf_s0_tele * tele, bool fUpdateAll)
{
	bool fRetVal;
	if (m_fUpdateBuB || fUpdateAll)
	{
		// M2015
		//tele->TKopf.usIndex  = m_uBuBIndex;
		//tele->TStation.mp    = m_mp;
		//tele->TStation.pos   = m_pos;
		//tele->TStation.lage  = m_lage;
		//tele->TStation.typ   = m_typ;
		//tele->TStation.gesp  = m_closed;
		//tele->TStation.gest  = m_error;
		//tele->TStation.prio  = m_prio;
		//tele->TStation.last  = m_fCargado ? 1 : 0;
		//tele->TStation.anz_ebenen = 1; // Number of layers at the position
		//tele->TStation.fz_typ = 1;	// Agv type is always 1 (for BuB) in HdR
		//wsprintf ((char *)tele->TStation.KundenNr, m_KundenNr);
		//wsprintf ((char *)tele->TStation.codEstDes, m_codEstDes);
		//tele->TStation.numAgvDes = m_numAgvDes;
		//tele->TStation.m_ocuPul = m_ocuPul;
		//tele->TStation.m_capPul = m_capPul;



	// M2015 TODO - send this information as station telegram to BuB
	CAIDStation* pAIDS = m_pAIDStation;

	CString strPSName(pAIDS->GetPSName());
	strcpy((char*)tele->tStation.szPSName,strPSName.GetString());
	tele->tStation.nLenPSName              = strPSName.GetLength();
	tele->tStation.nPSType                 = pAIDS->GetPSType();
	tele->tStation.nPriorityTime           = pAIDS->GetPriorityTime();
	tele->tStation.nAllObjects             = pAIDS->GetAllObjects();
	tele->tStation.nAGVTypesPickup         = pAIDS->GetAGVTypesOfAction(PICKUP);
	tele->tStation.nAGVTypesUnload         = pAIDS->GetAGVTypesOfAction(UNLOAD);
	tele->tStation.nAGVTypesHomeStation    = pAIDS->GetAGVTypesOfAction(HOME);
	tele->tStation.nAGVTypesBattCharge     = pAIDS->GetAGVTypesOfAction(BATTCHARGE);
	tele->tStation.nAGVTypesPseudoStation  = pAIDS->GetAGVTypesOfAction(PSEUDO);
	tele->tStation.nAGVTypesInterimStation = pAIDS->GetAGVTypesOfAction(INTERIM);
	//pS0Tele->tStation.nLoaded                 = pAIDS->GetLoaded();
	tele->tStation.nReserved               = pAIDS->GetReserved();
	//pS0Tele->tStation.nError                  = pAIDS->GetError();
	//pS0Tele->tStation.nClosed                 = pAIDS->GetClosed();

	tele->tStation.nError = m_error;
	tele->tStation.nClosed = m_closed;
	wsprintf ((char *)tele->tStation.szKastencode, m_KundenNr);
	tele->tStation.nLoaded = m_fCargado ? 1 : 0; 


	tele->tStation.nRuf = 0;
		
			
	wsprintf ((char *)tele->tStation.codEstDes, m_codEstDes);
	tele->tStation.numAgvDes = m_numAgvDes;

	wsprintf ((char *)tele->tStation.m_codpulvac, m_codpulvac);
	if (IsCargaDescarga())
		{
			CDBEstacion dbEstacion;
			if (dbEstacion.GetEstAgv(m_mp, m_pos, m_lage))
			{
				if (dbEstacion.GetNumeroMesas() > 0)
				{
					tele->tStation.m_ocuPul = dbEstacion.m_OcuPulmon;
					tele->tStation.m_capPul = dbEstacion.m_CapPulmon;

					tele->tStation.m_automan = dbEstacion.m_Automan;
					tele->tStation.m_senact = dbEstacion.m_Senact;
					tele->tStation.m_prepal = dbEstacion.m_pEstAgv1->prepal[0];
					tele->tStation.m_mesaop = dbEstacion.m_pEstAgv1->mesaop[0];
					tele->tStation.m_errmesa = dbEstacion.m_pEstAgv1->errmesa;
					tele->tStation.m_finprod = dbEstacion.m_pEstAgv1->finprod[0];
					tele->tStation.m_resagv = dbEstacion.m_Resagv;
					tele->tStation.m_tipest = dbEstacion.m_pEstAgv1->tipest[0];
					wsprintf ((char *)tele->tStation.m_codpulvac, dbEstacion.m_CodPulVac);

					if (dbEstacion.GetNumeroMesas() > 1)
					{
						tele->tStation.m_automan = dbEstacion.m_Automan;
						tele->tStation.m_senact = dbEstacion.m_Senact;
						tele->tStation.m_prepal = dbEstacion.m_pEstAgv2->prepal[0];
						tele->tStation.m_mesaop = dbEstacion.m_pEstAgv2->mesaop[0];
						tele->tStation.m_errmesa = dbEstacion.m_pEstAgv2->errmesa;
						tele->tStation.m_finprod = dbEstacion.m_pEstAgv2->finprod[0];
						tele->tStation.m_resagv = dbEstacion.m_Resagv;
						tele->tStation.m_tipest = dbEstacion.m_pEstAgv2->tipest[0];
						wsprintf ((char *)tele->tStation.m_codpulvac, dbEstacion.m_CodPulVac);
					}
				}
			}
		}

		m_fUpdateBuB = false;
		fRetVal = true;
	}
	else
		fRetVal = false;
	return (fRetVal);
}


void CEstacionBuB::UpdateEstacion(UCHAR error, bool fCargado)
{
	// Michael 17.04.2002 A veces no se actualiza...

	bool fUpdateBuB = false;

	if (m_error != error)
		fUpdateBuB = true;
	if (m_fCargado != fCargado)
		fUpdateBuB = true;

	if (error && ! m_error)
	{
		// Inicializar el error
		m_pAlarma = new CAlarma;
		m_pAlarma->AvisarAlarma (5, 1, 2, m_mp, m_pos);
		g_Container.AddStationError ();
	}
	else if (!error && m_error)
	{
/*	XXX	if (m_pAlarma)
		{
			m_pAlarma->SetFin ();
		}
		*/
		g_Container.DeleteStationError ();

	}

	m_error = error;
	m_fCargado = fCargado;
	
	// Michael 11.12.2004 No siempre se actualiza
	m_fUpdateBuB = (fUpdateBuB) ? true : m_fUpdateBuB;

}


void CEstacionBuB::UpdateEstacion(BYTE ocuPul, BYTE capPul, LPCSTR codEstPul, UCHAR error, bool fCargado)
{
	bool fUpdateBuB = false;

	if (m_error != error)
		fUpdateBuB = true;
	if (m_fCargado != fCargado)
		fUpdateBuB = true;

	if (strcmp(codEstPul, m_codpulvac))
		fUpdateBuB = true;
	if (m_capPul != capPul)
		fUpdateBuB = true;
	if (m_ocuPul != ocuPul)
		fUpdateBuB = true;

	if (error && ! m_error)
	{
		// Inicializar el error
		m_pAlarma = new CAlarma;
		m_pAlarma->AvisarAlarma (5, 1, 2, m_mp, m_pos);
		g_Container.AddStationError ();
	}
	else if (!error && m_error)
	{
/*	XXX	if (m_pAlarma)
		{
			m_pAlarma->SetFin ();
		}
		*/
		g_Container.DeleteStationError ();

	}

	m_error = error;
	m_fCargado = fCargado;
	m_capPul = capPul;
	m_ocuPul = ocuPul;
	wsprintf ((char *)m_codpulvac, codEstPul);
	
	// Michael 11.12.2004 No siempre se actualiza
	m_fUpdateBuB = (fUpdateBuB) ? true : m_fUpdateBuB;

}


// Actualizar datos de los descansos
void CEstacionBuB::UpdateEstacion(LPCTSTR estDes, UINT nAgvDes)
{
	if (m_numAgvDes != nAgvDes)
	{
		m_numAgvDes = nAgvDes;
		m_fUpdateBuB = true;
	}
	if (strcmp (m_codEstDes, estDes) != 0)
	{
		strcpy (m_codEstDes, estDes);
		m_fUpdateBuB = true;
	}


}
