// Agv.cpp: implementation of the CAgv class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "Telegrama.h"
#include "SerialCom.h"
#include "WlanMsg.h"
#include "Msg.h"

#include "Agv.h"
#include "GestorAgv.h"
#include "tpcDoc.h"
#include "tpcView.h"
#include "orden.h"
#include "destination.h"
#include "DBEstacion.h"
#include "DBEstadoPLC.h"
#include "GestorOrden.h"
#include "PuntoDescanso.h"
#include "DBEstadistica.h"
#include "CCourseAGVType.h"


#include "msgMacro.h"
#include "macro.h"
//#include "a1_ep_mp.a86"
//#include "a1_mpqv.inc"
//#include "a1_mpstr.a86"
// #include "a1_z_typ.c86"
//#include "a1_zt_v.c86"
//#include "a1_mpnmp.inc"
// #include "a1_fzeit.p86"

#include "SituationSet.h"
// #include "v_block.h"
#include "Tabsetfv.h"
//Xavi, 02/11/05:
#include ".\agv.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		TIMEOUTINTERVAL	500			// Millisegundos
#define		PERDIDOINTERVAL	5000


CCourseAGVType* CAgv::pAGVTCourse;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAgv::CAgv(UINT nId, 
		   CSerialCom * pSerCom,
		   CDistanceReqWayTeleMap* pmapDistancesReqWayTele)
{

	
	m_id = nId;
	m_pSerCom = pSerCom;
	m_fWasBlocked = false;
	m_uErrorEstacion = 0;
	m_knot = 0;
	m_nextKnot = 0;
	m_nAgvEmpujando = 0;
	m_countEsperar = 0;
	m_fHasBeenPolled = false;
	m_numError = 0;
	m_fEnCircuito = false;
	m_fCargaBateriaBuB = false;	// 04.01.2002 Quizas debe ser persistent ?
	m_uDeadlockCounter = 0;
	m_fSendQuitum = false;
	m_lastUmleitMp = 0;
	m_cOrdenesOk = 0;
	m_nMantenimiento = 0;	// Michael 14.06.2005 T5K
	//Xavi, 02/11/05: Inicialitzem el temporitzador de l'historic d'AGVs a NULL
	tiempo = NULL;

	m_pmapDistancesReqWayTele = pmapDistancesReqWayTele;
	
	m_lastWegTeleValue = 0;
	m_lastWegWasSame = false;


	//Test GetBlockWay:
	//USHORT nBlkWay	= GetBlockWay(  1,  2);		//3300
	//nBlkWay			= GetBlockWay(251,252);		//3000
	//nBlkWay			= GetBlockWay(252,202);		//6128
	//nBlkWay			= GetBlockWay(253,203);		//6176

	//nBlkWay			= GetBlockWay(302,245);		//5533
}

CAgv::~CAgv()
{
	CString format;
	format.Format("%s%d", "LastPosAgv_", m_id);
	AfxGetApp ()->WriteProfileInt ("Settings", format, m_pFzdat->mp);

}

// Poll el AGV si le toca
LARGE_INTEGER CAgv::Poll()
{

	int fRetVal;

	

	m_fTimeOutThisPoll = false;

	m_CurTime = GetTimePlusInterval (0);

	if (m_CurTime.QuadPart >= m_minNextPoll.QuadPart)
	{
	/* Puede ser que se está esperando los 5 segundos para que el Agv
	se duerme en el cargador de bateria. No se trata de realizar un poll
	en este caso ya que hay que decirselo al PLC solo para que active
		la carga de bateria */
		switch (GetEstado ())
		{
		case CARGARBATWAITPLC:
		case CARGARBAT:
			UpdateEstado ();
			break;
		default:
			// Michael 29.04.2009 Mods 2009
			if (m_fSendWeg){
				if (SendWegTelegram())
				{
					m_fSendWeg = false;
					// Michael 28.09.2010 Might have received reply already
						if (m_fSendQuitum)
						{
							// Enviar quitum
							MSGTRACE1 ("Enviando Quitum al Agv: %d", m_id);
							fRetVal = m_pSerCom->SendTelegrama (m_Tel.StrQuitum (m_id));
						}

				}
			}
			else
			{

				// se debe realizar un poll
				m_Tel.SetPollingTelegram (m_id);
				m_fSendQuitum = false;

				// Michael 20.09.2001
				m_pSerCom->ResetReceiveBuffer ();	// Vaciar el buffer de telegramas
				fRetVal = m_pSerCom->SendTelegrama (m_Tel.m_strTelegrama);
				if (fRetVal)
				{
					// El telegrama requiere respuesta
					fRetVal = m_pSerCom->ReceiveTelegrama (m_Tel.m_strRespuesta);
					if (g_Container.m_fSimulateCom)
					{
						fRetVal = SimulatePoll ();
					}
					switch (fRetVal)
					{

					case WAIT_OBJECT_0:
						// se ha recibido el telegrama de respuesta
						m_Tel.SetTeleRecibido ();
						InterpretarTelegramaRecibido ();
						if (m_fSendQuitum || ! IsBlocked ())
						{
							// Enviar quitum
							MSGTRACE1 ("Enviando Quitum al Agv: %d", m_id);
							fRetVal = m_pSerCom->SendTelegrama (m_Tel.StrQuitum (m_id));
							// Michael 19.02.2002 if (m_fWasBlocked)
							if (m_fWasBlocked && ! IsBlocked ())
								UnblockDrive ();
						}
						break;
					case WAIT_TIMEOUT:
						m_Tel.SetTeleTout ();
						ActionTimeoutPoll ();
						m_fTimeOutThisPoll = true;
						MSGERROR1 ("Timeout polling AGV: %d", m_id);
						break;
					default:
						m_Tel.SetTeleTout ();
						MSGERROR2 ("ReceiveTelegrama ha devuelto valor desconocido: %d para AGV: %d",
							fRetVal,
							m_id);
						break;
					}
				}
				else
				{
					m_Tel.SetTeleNull ();
				}
			}
		} // case CARGARBATWAITPLC
		if (m_fWillUpdateBuB)
		{
			g_Container.m_pMainWnd->PostMessage (WM_USER_AGV_UPDATE_BUB, 0, (UINT)m_id);
			m_fWillUpdateBuB = false;
		}
		return (SetNextPollInterval ());

	}
	else
		return (m_minNextPoll);	// No le toca todavia

}



LARGE_INTEGER CAgv::GetTimePlusInterval(UINT interval)
{
	LARGE_INTEGER liTime;

	GetSystemTimeAsFileTime ((FILETIME *)&liTime);	// Can this be done ?

	liTime.QuadPart += interval * 10000;	// Convertir milliseconds to 100 nanoseconds
	return (liTime);


}

// Interpretar telegrama recibido en respuesta a un poll
void CAgv::InterpretarTelegramaRecibido()
{
	typ_tele_kopf* pTeleHeader; // Telegram header
	CString strMessage;
	bool fUpdateEstado; // Indicar si se debe de actualizar el estado del AGV como resultado del telegrama

	pTeleHeader = (typ_tele_kopf*) m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_kopf));

	if (! CheckTeleAgv ((char *)pTeleHeader->fznr))
		return;


	RESET_FZ_VERMISST(*m_pFzdat); // Reset vehicle missing as there is communication

	switch (pTeleHeader->tele_art)
	{
	case 'w':
		/*	This telegram will only be sent if requested
			Allows drive optimization
			Distance gone up to date on the current way
			XXX Clarify this
		*/

		typ_tele_weg* tele_weg;

		tele_weg = (typ_tele_weg *) m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_weg));

		fUpdateEstado = InterpretarTeleWeg (*tele_weg);
		break;
	case 'z':
		// Kleines Zustands Telegramme. State of affairs !!!
		// The current reported point (mmmmp), the current status (sss)
		// and the departed distance of the FTF (ii)

		typ_tele_kz     *tele_kz;
		tele_kz = (typ_tele_kz*) m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_kz));

		fUpdateEstado = InterpretarTeleKleineZustand (*tele_kz);

		break;
	case 's':
		{
		// kleines Status-Tele.
		typ_tele_ks* tele_ks = (typ_tele_ks*)m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_ks));
		MSGTRACE1 ("InterpretarTelegramaRecibido () Agv:%d s telegram", m_id);
		
		// M2015 if (tele_ks->status_0 != m_pFzdat->fahr_byte_ist)
		if (tele_ks->status_0 != m_pFzdat->Status0)
		{
			fUpdateEstado = true;
			m_fWillUpdateBuB = true;
			QuizasInsertarBloqueo(tele_ks->status_0); //Xavi, 02/11/05
			// M2015 m_pFzdat->fahr_byte_ist = tele_ks->status_0;  // STATUSWORT 0
			// TODO M2015 Check that status 0 is really tele_ks_status_0
			m_pFzdat->Status0 = tele_ks->status_0;
		}
		}
		break;
	case 'S':
		{
			// grosses Status-Telegramm
			typ_tele_gs * tele_gs = (typ_tele_gs*) m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_gs));
			fUpdateEstado = InterpretarTeleGrosseStatus (*tele_gs);
			updateEstadoGrossStatus (fUpdateEstado);
		}
		break;
    case 'E':
		{
			// ERROR-Tele.
			typ_tele_error * tele_er = (typ_tele_error*) m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_error));

			fUpdateEstado = InterpretarTeleError (*tele_er);
		}
		break;

    case 'A':
    {
      // Manual order input
      typ_tele_at * tele_at = (typ_tele_at*) m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_at));
	  fUpdateEstado = InterpretarTeleManualOrder (*tele_at);

	}
     break;

    case 'F':
		{
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (2, 14, 2, 
			m_id, 
			m_pFzdat->mp);

		// Agv taken out of System
		MSGWARNING1 ("InterpretarTelegramaRecibido()->telegrama F: Agv: %d quitado del sistema",
			m_id);
		SetEstado (AGVFUERASISTEMA);	// XXX Not so fast...
		m_fSendQuitum = true;
		fUpdateEstado = true;
		}
		break;

    default:
    {
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (2, 13, 2, m_id, pTeleHeader->tele_art);
		 MSGERROR1 ("InterpretarTelegramaRecibido()->pTeleHeader->tele_art:%c no interpretado",
			 pTeleHeader->tele_art);
		 m_fSendQuitum = false;
    }
	break;
	} // switch
	if (fUpdateEstado)
		UpdateEstado ();

}



void CAgv::updateEstadoGrossStatus (bool fUpdateEstado) 
{
	if (fUpdateEstado)
	{
		if (0 != m_pFzdat->nmp)
		{
			bool fStop = ShouldBlock ();
			if (fStop)
			{
				if (m_pFzdat->auftziel.nDest != 0 && IsDeadlocked ())
				{
					bool bNCp(true);
					if (USHORT altNmp = Umleiten (bNCp))
						if (!g_Container.m_fSimulateCom)
							if(bNCp)
								SendUmleitTelegram (altNmp);
							else
								SimulateUmleitTelegram (altNmp,bNCp);
				}
				else 
					if (!QuizasCaminoAlt ())
						// Se debe ir por ariba y no puede
						fStop = true;
				// Michael 28.04.2009 Mods 2009
					else
						if (!QuizasCaminoAlt203())
							fStop = true;
				// Michael 02.03.2012
						else 
						{
							if (IsCaminoAlt303())
							{
								fStop = true; // 03.0.2012 Force block to update status
								if (!QuizasCaminoAlt303())
									fStop = true; // redundant
							}
						}

			}
			// Michael 23.02.2002
			else
			{
				m_lastUmleitMp = 0;	// Reset
				if (!QuizasCaminoAlt ())
					fStop = true;
				// Michael 28.04.2009 Mods 2009
				else
					if (!QuizasCaminoAlt203())
						fStop = true;
				// Michael 02.03.2012
				// Michael 02.03.2012
					else 
					{
						if (IsCaminoAlt303())
						{
							fStop = true; // 03.0.2012 Force block to update status
							if (!QuizasCaminoAlt303())
								fStop = true; // redundant
						}
					}


			}
			// Michael 23.02.2002 fin

			SetBlocked (fStop == TRUE ? true : false);



		}
		else
		{
			// nmp == 0
			SetBlocked (true);
		}
	}

}

// Convertir Ascii to int
int CAgv::GetAtoi(LPCTSTR pStr, int len)
{
	CString strAscii (pStr, len);
	return atoi (strAscii);

}

// Insertar un AGV en el sistema
void CAgv::InsertarAGV(UINT mp, UCHAR mpPos)
{
 /* Michael.
   Registrar el AGV como presente.
   Comprobar que el MP existe
   Actualizar radio y area para este MP
   Ver que mensajes del sistema enviar...
*/
    m_pFzdat->mp      = mp;
    m_pFzdat->mppos   = mpPos;
	m_pFzdat->lmp  = 0;
	m_pFzdat->nmp  = 0;
	m_pFzdat->uemp = 0;
	m_pFzdat->wegstr = 0;

	m_pOrden = NULL;
	m_nCargador = 0;

	// Michael 30.10.2001
	*(pCrMengeVerz[m_pFzdat->fznr - 1]) = 0;

	SET_FZ_IN_SYSTEM(*m_pFzdat);
	m_fWillUpdateBuB = true; // Avisar al BuB de los acontecimientos

}

// Devolver puntero a datos
TYP_FZDAT * CAgv::GetFzdatPtr()
{
	return (m_pFzdat);

}

void CAgv::SetZiel(TYPE_STATION_DEF *ptrZiel,
        unsigned short nr,
        unsigned char  pos,
	    unsigned char  lage,
        unsigned char ebene)
{
	ptrZiel->nDest = nr;
	ptrZiel->nPos = pos;
	ptrZiel->ucSide = lage;
	ptrZiel->nLayer = ebene;

}

void CAgv::Init()
{
	m_fWlanAgv = false;
	m_fWlanUpdateEstado = false;
	m_fSendWeg = false;
	m_nCargador = 0;
	m_estado = AGVSINDATOS;	// Ver eso...
	m_numTimeout = 0;
	m_maxNextPoll.QuadPart = 0;
	m_minNextPoll.QuadPart = 0;
	// Initialize AGV data
	// memset (m_pFzdat, 0, sizeof (*m_pFzdat));
	m_pFzdat->fznr = m_id;
	m_pFzdat->fztyp = 1;	// Only one vehicle type in HdR
	m_pFzdat->mplage = 'L';
	m_pFzdat->aktion = STATUS_LEER;
	
	// M2015 TODO
	//m_pFzdat->mp = 52 + m_id;	// M2015 TODO
	//if (m_pFzdat->mp > 60)
	//	m_pFzdat->mp -= 60;
	//m_pFzdat->mppos = 1;
	m_pFzdat->mp = 0;
	m_pFzdat->mppos = 0;
	
	

	m_pFzdat->lmp = 0;
	m_pFzdat->nmp = 0;
	m_pFzdat->uemp = 0;
	m_pFzdat->at_nr = 0;
	m_pFzdat->aufident [0] = '\0';
	m_pFzdat->FVvonBedienung = 0;
	SetZiel (&m_pFzdat->fzziel);
	SetZiel (&m_pFzdat->hz);
	SetZiel (&m_pFzdat->bz);
	SetZiel (&m_pFzdat->auftziel);
	for (int i = 0; i < 5; i++)
	{
		m_pFzdat->errofz [i].modul_nr = 0;
		m_pFzdat->errofz [i].fehler_nr = 0;
	}
	m_pFzdat->FVvonBedienung = 0;
	//M2015
	//m_pFzdat->mistfz0 = 0;
	//m_pFzdat->mistfz1 = 0;
	//m_pFzdat->mistfz2 = 0;

	m_pFzdat->Status0 = 0;
	m_pFzdat->Status1 = 0;
	m_pFzdat->Status2 = 0;
	m_pFzdat->Status3 = 0;
	m_pFzdat->Status4 = 0;

	//M2015
	//m_pFzdat->msollfz0 = 0;
	//m_pFzdat->msollfz1 = 0;



	// XXX-Lac01:
	m_pFzdat->fz_blk  = 0;

		// XXX-Lac02:
	m_pFzdat->wegstr = 0;

	SetBlocked (true);	// Agv blocked initially
	m_pFzdat->Status0 |= BIT6;	// Set battery full

	m_pOrden = NULL;

	// Michael 30.10.2001
	*(pCrMengeVerz[m_pFzdat->fznr - 1]) = 0;



	
	for (int i = 0; i < sizeof (m_pFzdat->ZielString); i++)
		m_pFzdat->ZielString [i] = '-';


	m_pWlanMsg = NULL;
	

	if (SetWlanProperties())
	{
	}
	else
	{
	if (m_id > 0)
		m_Tel.SetPollingTelegram (m_id);	// Preparar un telegrama de polling
	}

	RESET_FZ_IN_SYSTEM(*m_pFzdat); 

}



// Devolver el siguiente mp para un MP actual y un destino
UINT CAgv::GetNmp(unsigned short mp, unsigned short ziel)
{

	UINT nmp = pAGVTCourse->GetNextCp(mp, ziel);

	return (nmp);
}


/* Devolver distancia que el AGV puede viajar desde el MP
	Será: Posición si la posición es el destino
	Blocking distance si la posición no es el destino
	NOTA: Indumat definition era: unsigned short CAgv::GetWeg(unsigned char fznr)
*/

unsigned short CAgv::GetWeg(TYP_FZDAT * fzdat)
{
	//Meldepunkttabelle  *p_mptab;
	CCommPoint* pCommPoint;

	//NMP_tabelle  *p_nmptab;
	CCpDistance* pCpDistance;

	unsigned char  anz_nmp;
	unsigned short i;
	unsigned short mp, nmp;
	unsigned short blk_weg = 0;

	TYPE_STATION_DEF ziel;

	mp = fzdat->mp;
	nmp = fzdat->nmp;
	ziel = fzdat->fzziel;

	if (!ziel.nDest)
	{
		ziel.nDest = fzdat->mp;
		ziel.nPos = fzdat->mppos;
		ziel.nLayer = 0;
		ziel.ucSide = fzdat->mplage;
	}

	if (nmp == 0 || mp == ziel.nDest)
	{
		blk_weg = 1; // XXX GetPosWeg (ziel);
	}
	else
	{
		//p_mptab = mptab + vmptab[mp];
		pCommPoint = pAGVTCourse->GetPtrCommPoint(mp);
		//anz_nmp = p_mptab->anz_nmp;
		anz_nmp = pCommPoint->GetNCps();
		//p_nmptab = MpNmpTab + (p_mptab->str_idx - 1);
		int nDistIndex = pCommPoint->GetDistanceIndex();


		// NMP-Strecke suchen
		for (i = 1; i <= anz_nmp; i++)
		{
			pCpDistance = pAGVTCourse->GetPtrDistance(nDistIndex);
			//if (nmp == p_nmptab->nmp)
			if (nmp == pCpDistance->GetNCp())
			{
				//blk_weg = p_nmptab->blk_weg;
				blk_weg = pCpDistance->GetBlockWay();
				break;
			}
			else
			{
				//p_nmptab++;
				nDistIndex++;
			}
		}
	}

	return(blk_weg);
}

/*
// Get distance of a destination position
unsigned short CAgv::GetPosWeg(TYPE_STATION_DEF Ziel)
{
  unsigned char z_index = 0,
      				  pos_anz = 0;

  unsigned int ZielIndex;

	ZielIndex = vtstat[Ziel.nDest];

	if (ZielIndex >  0 &&
	  	ZielIndex <= KMAX_STATIONEN)
	{
    #ifdef POS_WEG_DATEI
    unsigned short PosIndex;

		// Es wird der Positionsweg ueber die Pos-Weg-Tabelle ermittelt

		// Index in Zieltyptabelle berechnen (ueber Zieltypverweistabelle)

		if (Ziel.ucSide == 'L')
		{
			PosIndex = pPosZielTab[ZielIndex-1].PosIndexLinks;
			pos_anz = statdat[ZielIndex].pos_anz_l;
		}
		else
		{
			if (Ziel.ucSide == 'R')
			{
				PosIndex = pPosZielTab[ZielIndex-1].PosIndexRechts;
				pos_anz = statdat[ZielIndex].pos_anz_r;
			}
			else
			{
				return 0xFFFF;
			}
		}

		// Posweg ermitteln
		if (Ziel.nPos > 0 && Ziel.nPos <= pos_anz)
		{
			return (pPosWegTab[(PosIndex-1) + (Ziel.nPos-1)]);
		}
		else
		{
			return 0xFFFF;
		}
    #else

		// Positionsweg ueber Zieltypverweistabelle

		// Index in Zieltyptabelle berechnen (ueber Zieltypverweistabelle)
		if (Ziel.ucSide == 'L')
		{
			// linkes Ziel
			z_index = zt_ver_tab[statdat[ZielIndex].zieltyp_l];	// type index
			pos_anz = statdat[ZielIndex].pos_anz_l; // Number of positions
		}
		else
		{
			if (Ziel.ucSide == 'R')
			{
				// rechtes Ziel
				z_index = zt_ver_tab[statdat[ZielIndex].zieltyp_r];
				pos_anz = statdat[ZielIndex].pos_anz_r;
			}
		}
		if ((z_index != 0) && (Ziel.nPos > 0) && (Ziel.nPos <= pos_anz))
		{
			// Posweg ermitteln (z_index + pos - 1)
			return (z_typ_tab[z_index+Ziel.nPos-1].pos_weg);
		}
		else
		{
			return 0xFFFF;
		}
    #endif
	}
	else
	{
		return 0xFFFF;
	}
}
*/

// Get distance of a destination position
unsigned short CAgv::GetPosWeg(TYPE_STATION_DEF Ziel)
{
	return pAGVTCourse->GetPosWay(Ziel);
}

/*
//------------------------------------------------------------------------------
// Datum       : 25.02.97
//
// Beschreibung:  Das Program get_nmp ermittelt den NMP aus:
//              - den Tabellen Mp und MP-Strecken
//              - den Tabellen EP-FB und EP-MP
//              - dem aktuellen Meldepunkt
//              - dem aktuellen Fahrzeugziel
//
// Parameter   : mp   - aktuelle MP-Nummer
//               ziel - anzufahrendes Ziel
//
// Rueckgabewert: Zeiger auf naechste Strecke,
//                NULL, wenn mp oder ziel falsch ist, oder ein Fehler im
//                Fahrkurs vorliegt.
//------------------------------------------------------------------------------
unsigned int CAgv::GetNmpWeg(unsigned int mp, unsigned int ziel, long *weg)
{
	unsigned int ret  = 0;
	NMP_tabelle *pnmp = GetPNmp(mp,ziel);

	if (pnmp)
	{
		*weg += pnmp->str_len;
		ret  = pnmp->nmp;
	}
	return ret;

}
*/

//------------------------------------------------------------------------------
// Datum       : 25.02.97
//
// Beschreibung:  Das Program get_nmp ermittelt den NMP aus:
//              - den Tabellen Mp und MP-Strecken
//              - den Tabellen EP-FB und EP-MP
//              - dem aktuellen Meldepunkt
//              - dem aktuellen Fahrzeugziel
//
// Parameter   : mp   - aktuelle MP-Nummer
//               ziel - anzufahrendes Ziel
//
// Rueckgabewert: Zeiger auf naechste Strecke,
//                NULL, wenn mp oder ziel falsch ist, oder ein Fehler im
//                Fahrkurs vorliegt.
//------------------------------------------------------------------------------
unsigned int CAgv::GetNmpWeg(unsigned int mp, unsigned int ziel, long *weg)
{
	unsigned int ret = 0;
	CCpDistance *pCpDistance = GetPNmp(mp, ziel);

	if (pCpDistance != NULL)
	{
		*weg += pCpDistance->GetLength();
		ret = pCpDistance->GetNCp();
	}

	return ret;
}

/*
NMP_tabelle* CAgv::GetPNmp(UINT mp, UINT ziel)
{
	// lokale Variablen
	unsigned int   epnr;	// Decision point index
	unsigned char  zber;	// Destination area
	unsigned int   nmp;          // nächster Meldepunkt
	int  nmp_index;    // Index in EP_MP_Tabelle
	unsigned char  i;
	unsigned char  anz_nmp;	// Number of next coms points

	// Typendefinition der Streckentabelle
	Meldepunkttabelle  *strtab;

	// Typendefinition des NMP-Teils in der Streckentabelle
	NMP_tabelle *nmp_tab,
            	*pnmp;

	nmp_tab = 0;

	// Nur bearbeiten, wenn mp und ziel übergeben wurden.

	if (mp && ziel)
	{
		// Adresse in MP-Tabelle(mpstre) ermitteln
		// XXX Michael 11.09.2001 strtab  = mptab + vmptab [mp]; // Get mp data
		strtab  = mptab + (vmptab [mp] - 1); // Get mp data
		if (vtstat[ziel] != 0xFFFF)
		{
			// The destination really exists
			epnr = strtab->ep_nr;	// Decision point index
			zber = statdat[vtstat[ziel]].f_bereich;	// Area index

			if (zber)
			{
				nmp_tab    = MpNmpTab + strtab->str_idx - 1;	// First next point
				anz_nmp    = strtab->anz_nmp;	// Number of next points

				if (epnr)
				{
					// The MP is a decision point
					// Entscheidungspunkt


					// NMP aus EP-FB-Tabelle !

					// Entscheidungspunkt- und Zielbereichsnummer erniedrigen, da
					// es keinen Entscheidungspunkt und Zielbereich mit der Nummer
					// "0" gibt!
					epnr--;
					zber--;
					nmp_index = ep_fb_tab [epnr] [zber];

					if (nmp_index)
					{
						nmp = ep_mp_tab [epnr] [nmp_index - 1];
						// Get pointer to MpNmpTab for this point from the first next point
						for (;
      						nmp != nmp_tab->nmp;
			      			nmp_tab++)
						{
						}
					}
					else
					{
						// Fehler im Fahrkurs !
						MSGERROR ("Next point not found for decision point");
						nmp_tab = 0;
					}
				}
				else
				{
					// Mp mit einem NMp oder Zielsuchpunkt !
					// It is a destination seek point or a normal point
					// which isn't a decision point

					// Ermitteln des NMP aus MP-Streckentabelle
					anz_nmp    = strtab->anz_nmp; // Number of next points
					nmp        = nmp_tab->nmp;		// Number of the first nmp

					// Zielsuchpunkt: Ermitteln, ob n„chster NMP Ziel ist.
					for (i = 0, pnmp = nmp_tab;
						i < anz_nmp;
						i++)
					{
						if (nmp != ziel)
						{
							// The next point is different from the destination
							strtab = mptab + vmptab [nmp] - 1; // mptab is 0 based
							// Get the MP data for the next point
							if (strtab->typ == 1)
							{
								// Vorzugsstrecke gefunden
								// It is not a destination. This is the point
								pnmp = nmp_tab;
							}
							else
							{
								// The next point cannot be a destination if it is
								// not the mission destination ???
								nmp_tab++;
								nmp = nmp_tab->nmp;
							}
						}
						else
						{
							// Ziel gefunden !
							// The next point is the destination
							pnmp = nmp_tab;
							break;
						}
					}

					nmp_tab = pnmp;
				}
			}
			else
			{
				// ungueltiger Feinbereich !
				// The destination doesn't have an area
			}
		}
		else
		{
			// ziel ist kein Ziel !
			// the mp or destination is null
		}
	}

	return nmp_tab;
}
*/

/*
CCpDistance* CAgv::GetPNmp(UINT mp, UINT ziel)
{
	// lokale Variablen
	unsigned int   epnr;	// Decision point index
	unsigned char  zber;	// Destination area
	unsigned int   nmp;          // nächster Meldepunkt
	int  nmp_index;    // Index in EP_MP_Tabelle
	unsigned char  i;
	unsigned char  anz_nmp;	// Number of next coms points

	// Typendefinition der Streckentabelle
	CCommPoint  *pCommPoint;				//strtab

	// Typendefinition des NMP-Teils in der Streckentabelle
	CCpDistance *pCpDistance,				//nmp_tab
				*pCpDistancePreferred;		//pnmp

	pCpDistance = 0;

	// Nur bearbeiten, wenn mp und ziel übergeben wurden.

	if (mp && ziel)
	{
		// Adresse in MP-Tabelle(mpstre) ermitteln
		// XXX Michael 11.09.2001 strtab  = mptab + vmptab [mp]; // Get mp data
		//strtab = mptab + (vmptab[mp] - 1); // Get mp data
		pCommPoint = CAgv::pCourse->GetPtrCommPoint(mp);

		if (vtstat[ziel] != 0xFFFF)
		{
			// The destination really exists
			epnr = strtab->ep_nr;	// Decision point index
			zber = statdat[vtstat[ziel]].f_bereich;	// Area index

			if (zber)
			{
				nmp_tab = MpNmpTab + strtab->str_idx - 1;	// First next point
				anz_nmp = strtab->anz_nmp;	// Number of next points

				if (epnr)
				{
					// The MP is a decision point
					// Entscheidungspunkt


					// NMP aus EP-FB-Tabelle !

					// Entscheidungspunkt- und Zielbereichsnummer erniedrigen, da
					// es keinen Entscheidungspunkt und Zielbereich mit der Nummer
					// "0" gibt!
					epnr--;
					zber--;
					nmp_index = ep_fb_tab[epnr][zber];

					if (nmp_index)
					{
						nmp = ep_mp_tab[epnr][nmp_index - 1];
						// Get pointer to MpNmpTab for this point from the first next point
						for (;
							nmp != nmp_tab->nmp;
							nmp_tab++)
						{
						}
					}
					else
					{
						// Fehler im Fahrkurs !
						MSGERROR("Next point not found for decision point");
						nmp_tab = 0;
					}
				}
				else
				{
					// Mp mit einem NMp oder Zielsuchpunkt !
					// It is a destination seek point or a normal point
					// which isn't a decision point

					// Ermitteln des NMP aus MP-Streckentabelle
					anz_nmp = strtab->anz_nmp; // Number of next points
					nmp = nmp_tab->nmp;		// Number of the first nmp

					// Zielsuchpunkt: Ermitteln, ob n„chster NMP Ziel ist.
					for (i = 0, pnmp = nmp_tab;
						i < anz_nmp;
						i++)
					{
						if (nmp != ziel)
						{
							// The next point is different from the destination
							strtab = mptab + vmptab[nmp] - 1; // mptab is 0 based
							// Get the MP data for the next point
							if (strtab->typ == 1)
							{
								// Vorzugsstrecke gefunden
								// It is not a destination. This is the point
								pnmp = nmp_tab;
							}
							else
							{
								// The next point cannot be a destination if it is
								// not the mission destination ???
								nmp_tab++;
								nmp = nmp_tab->nmp;
							}
						}
						else
						{
							// Ziel gefunden !
							// The next point is the destination
							pnmp = nmp_tab;
							break;
						}
					}

					nmp_tab = pnmp;
				}
			}
			else
			{
				// ungueltiger Feinbereich !
				// The destination doesn't have an area
			}
		}
		else
		{
			// ziel ist kein Ziel !
			// the mp or destination is null
		}
	}

	return nmp_tab;
}
*/

CCpDistance* CAgv::GetPNmp(UINT mp, UINT ziel)
{
	return pAGVTCourse->GetNextDistPointer(mp, ziel);
}

void CAgv::GetOrden (int orden) {

	int index = 0;
	 char * szId = new char [KMAXID_AT];
	 errno_t error = _itoa_s(orden, szId, KMAXID_AT, 10);			//JLac
	COrden * ptrOrden = g_GestorOrden.GetOrdenPtr ((unsigned char *)szId, &index);
	if (ptrOrden) {
		SetOrden(ptrOrden);
	}

	delete [] szId;

}


// Asociar una orden con el AGV para su posterior envio
void CAgv::SetOrden(COrden *orden)
{

	m_pOrden = orden;
	if (!orden->IsManual ())
	{
	//m_estado = ORDENNUEVO; // Xavi, 02/11/05
	SetEstadoActual(ORDENNUEVO);
	SetZiel (&m_pFzdat->auftziel, orden->m_at.hz.nDest,
			(unsigned char)orden->m_at.hz.nPos,
			orden->m_at.hz.ucSide,
			orden->m_at.hz.nLayer);

	}
	else
	{

		// Una orden manual
		SetZiel (&m_pFzdat->auftziel, orden->m_at.az.nDest,
			(unsigned char)orden->m_at.az.nPos,
				orden->m_at.az.ucSide,
				orden->m_at.az.nLayer);

		if (orden->m_at.akt_org == 1)

		{
			// Pickup
			//m_estado = ORDENNUEVO; // Xavi, 02/11/05
			SetEstadoActual(ORDENNUEVO);
		}
		else
		{
			// y o nada
			//m_estado = ORDENNUEVOMANUAL; //Xavi, 02/11/05
			SetEstadoActual(ORDENNUEVOMANUAL);
		}
	}

	strcpy ((char *)m_pFzdat->aufident, (char *)orden->m_at.id_nr);

/*	// 25.10.2001
	USHORT nFz = m_pFzdat->fznr;
	// Get vector of communication points to destination
	GetMpFolge(nFz,pMpFolgeVerz[nFz-1],m_pFzdat->auftziel.nr);
	// Set the crash area empty
	*(pCrMengeVerz[nFz-1]) = 0;
 XXX-Lac04 */
	m_minNextPoll.QuadPart = 0;	// Para que sea asignado de inmediato
	g_pGestor->m_evtOrden.SetEvent ();

	// M2015
	SetWlanUpdateEstado();


}

LPCSTR CAgv::GetStringEstado()
{
	switch (m_estado)
	{
		case AGVFUERASISTEMA:	return "AGVFUERASISTEMA";
		case AGVPERDIDO:		return "AGVPERDIDO";
		case AGVSINDATOS:		return "AGVSINDATOS";
		case ORDENNUEVO:		return "ORDENNUEVO";
		case ORDENNUEVOMANUAL:	return "ORDENNUEVOMANUAL";
		case IRCARGARBAT:		return "IRCARGARBAT";
		case CARGARBAT:			return "CARGARBAT";
		case CARGARBATWAITPLC:	return "CARGARBATWAITPLC";
		case IRPUNTOESP:		return "IRPUNTOESP";
		case PUNTOESP:			return "PUNTOESP";
		case IRCARGARORD:		return "IRCARGARORD";
		case CARGARORD:			return "CARGARORD";
		case IRDESCARGARORD:	return "IRDESCARGARORD";
		case DESCARGARORD:		return "DESCARGARORD";
		// Michael 14.06.2005 T5K
		case IRMANTENIMIENTO:	return "IRMANTENIMIENTO";
		case MANTENIMIENTO:		return "MANTENIMIENTO";
		default:				return "NO CONOCIDO";
	}


}


// Tratar time out en un telegrama de poll
void CAgv::ActionTimeoutPoll()
{
	if (m_CurTime.QuadPart >= m_maxNextPoll.QuadPart)
	{
		// Se ha excedido el tiempo máximo de poll
		m_numTimeout++;
		if (m_numTimeout == 5)
		{
			CAlarma * ptrAlarma = new CAlarma;
			ptrAlarma->AvisarAlarma (2, 1, 1, m_id, m_pFzdat->mp);
			m_numTimeout = 0;
			SetEstado (AGVPERDIDO);
		}
	}
}


// Actualizar el estado del AGV como resultado de un telegrama
void CAgv::UpdateEstado()
{
	CSingleLock lockAgv (&m_cs);	// Bloquear acceso al AGV...
	//TRACE1 ("CAGV::UpdateEstado->Esperando Aceso AGV %d\n", m_id);
	lockAgv.Lock ();
	//TRACE1 ("CAGV::UpdateEstado->Aceso Agv %d granted...\n", m_id);

	switch (m_estado)
	{
	case AGVPERDIDO:
		SetEstado (AGVSINDATOS);
	case AGVSINDATOS:
		// Michael 15.02.2002
		if (MESA1CARGADA (*m_pFzdat) || MESA2CARGADA (*m_pFzdat))
		{
			MSGWARNING1 ("Agv: %d sin orden y con carga - enviando a rechazo",
				m_id);
			CAlarma * ptrAlarma = new CAlarma;
			ptrAlarma->AvisarAlarma (2, 21, 2, m_id, m_pFzdat->mp);
			g_GestorOrden.InsertarOrdenRechazo (this);
		}
		else
		{
			if (IsAparcado ())
				//m_estado = PUNTOESP; // Xavi, 02/11/05
				SetEstadoActual(PUNTOESP);
			else
			{
				if (g_Container.IsEnFuncionamiento ())
				{
					if (g_pGestor->StealOrden (this))
						break;
				}
				if (m_cOrdenesOk >= 2 || m_countEsperar >= 5)
				{
					m_cOrdenesOk = 0;
					m_countEsperar = 0;
					if (EnviarPuntoEspera ())
					{
						//m_estado = IRPUNTOESP; //Xavi, 02/11/05
						SetEstadoActual(IRPUNTOESP);
						break;
					}
				}
				else
					m_countEsperar++;
			}
		}
		break;
	case PUNTOESP:
		if (SCargarBateria () && EnviarCargador ())
		{
			//m_estado = IRCARGARBAT; //Xavi, 02/11/05
			SetEstadoActual(IRCARGARBAT);

		}
		// Michael 14.06.2005 T5K
		else if (SEnviarMantenimiento() && EnviarMantenimiento())
			//estado = IRMANTENIMIENTO;//Xavi, 02/11/05
			SetEstadoActual(IRMANTENIMIENTO);
		// Michael 14.06.2005 T5K fin
		else
			if (AgvConDestino ())
			{
				if (EnviarPuntoEspera (m_pFzdat->mp))
					//m_estado = IRPUNTOESP;//Xavi, 02/11/05
					SetEstadoActual(IRPUNTOESP);
				else
					SetEstado (AGVSINDATOS);
			}
			else
				if (IsEmpujado ())
				{
					ResetEmpujado ();
					if (IsOkEmpujar ())
					{
						if (EnviarPuntoEspera (m_pFzdat->mp))
							//m_estado = IRPUNTOESP;//Xavi, 02/11/05
							SetEstadoActual(IRPUNTOESP);
						else
							SetEstado (AGVSINDATOS);
					}
				}
				else
				{
					if (IsSalirPuntoDescanso ())
					{
						if (EnviarPuntoEspera ())
							//m_estado = IRPUNTOESP;//Xavi, 02/11/05
							SetEstadoActual(IRPUNTOESP);
						else
							SetEstado (AGVSINDATOS);
					}
				}
				
				break;
	case IRPUNTOESP:
		if (DestinoDiferente (m_estado))
		{
			; // Ya se hace directamente
		}
		else if (SLlegadoDestino ())
		{
			//m_estado = PUNTOESP;//Xavi, 02/11/05
			SetEstadoActual(PUNTOESP);
		}
		else if (SCargarBateria ())	
		{
			if (EnviarCargador ())
				//m_estado = IRCARGARBAT; //Xavi, 02/11/05
				SetEstadoActual(IRCARGARBAT);
		}
		// Michael 14.06.2005 T5K
		else if (SEnviarMantenimiento() && EnviarMantenimiento())
			//m_estado = IRMANTENIMIENTO;//Xavi, 02/11/05
			SetEstadoActual(IRMANTENIMIENTO);
		// Michael 14.06.2005 T5K fin
		if (IsEmpujado ())
		{
			EmpujarAgvBloqueo();
		}
		// Michael 31.12.2001: Quitar órdenes...
		if (IRPUNTOESP == m_estado && g_Container.IsEnFuncionamiento ())
		{
			if (g_pGestor->StealOrden (this))
				break;
		}
		break;
	case ORDENNUEVO:
		if (PENDINGCANCEL == m_pOrden->GetEstado ())
		{
			SetCancelled ();
			SetEstado (AGVSINDATOS);
		}
		else if (PickupOrden ())
		{
			//m_estado = IRCARGARORD;//Xavi, 02/11/05
			SetEstadoActual(IRCARGARORD);
		}
		else
		{
			;
		}
		break;
	case ORDENNUEVOMANUAL:
		if (PENDINGCANCEL == m_pOrden->GetEstado ())
		{
			SetCancelled ();
			SetEstado (AGVSINDATOS);
		}
		else
		{
			SetDestinoManual ();
			//m_estado = IRDESCARGARORD;//Xavi, 02/11/05
			SetEstadoActual(IRDESCARGARORD);
		}
		break;
	case IRCARGARORD:
		if (PENDINGCANCEL == m_pOrden->GetEstado ())
		{
			SetCancelled ();
			SetEstado (AGVSINDATOS);
		}
		else if (DestinoDiferente (IRCARGARORD))
		{
			; // Ya se hace directamente
		}
		else
			if (SLlegadoDestino ())
			{
				if (OkDestinoCarga ())
				{
					if (CargarOrden ())
					{
						//m_estado = CARGARORD; //Xavi, 02/11/05
						SetEstadoActual(CARGARORD);
					}
					else
					{
						;
					}
				}
			}
			break;
	case CARGARORD:
		// Michael 12.11.2001 Considerar la posibilidad de cancelar la orden
		if (PENDINGCANCEL == m_pOrden->GetEstado ())
		{
			SetCancelled ();
			SetEstado (AGVSINDATOS);
		}
		else
		{
			if (SOrdenCargada ())
			{
				m_uErrorEstacion = 0;	// por si hubiera error en la estación
				if (IsOrdenAgvCargado ())
				{
					// Una orden de Agv será carga solo
					SetDelivered ();
					SetEstado (AGVSINDATOS);
				}
				else
				{
					if (SetUnloadDestination ())
					{
						//m_estado = IRDESCARGARORD;//Xavi, 02/11/05
						SetEstadoActual(IRDESCARGARORD);
					}
					else
					{
						;
					}
				}
			}
			else
			{
				if (SOrdenCargadaParcial ())
				{
					if (PickupOrden ())
						//m_estado = IRCARGARORD;//Xavi, 02/11/05
						SetEstadoActual(IRCARGARORD);
					else		
						; // Que ??
				}
				else
				{
					// Michael 15.01.2002 - Action run would seem to get lost at times
					if (FZ_AKTION_GESPERRT (*m_pFzdat))
					{
						// Action is stopped
						MSGWARNING1 ("Agv: %d en CARGARORD con action stopped", m_id);
						SetAction (true);
					}	
					else
						// Michael 22.02.2002 Por algun motivo no se mira si la posición es diferente
						// Michael 22.02.2002 Por si no está viendo nada !!!
						if (! FZ_HAT_FEHLER (*m_pFzdat) 
							&& ! FZ_AUFTRAG_ERLEDIGT (*m_pFzdat)
							&& ! FZ_AUFTRAG_ERLEDIGT_2 (*m_pFzdat)
							&& SLlegadoDestino ()	// Por si ha salido de la estación
							&& g_Container.SystemDriveAllowed () // Michael 28.02.2002
							&& !IsDriveForbiddenByBuB ())
						{
							// Es que no debe contestar al poll en este caso...
							m_fSendQuitum = true; // Enviar otro quitum
							MSGERROR2 ("Agv: %d estado: %s Enviando quitum de nuevo",
								m_id, GetStringEstado ());
						}
				}
			}
		}
		break;
	case IRDESCARGARORD:
		if (DestinoDiferente (IRDESCARGARORD))
			;
		else
			if (SLlegadoDestino ())
			{
				if (OkDestinoEspecial ())
				{
					if (DescargarOrden ())
						//m_estado = DESCARGARORD;//Xavi, 02/11/05
						SetEstadoActual(DESCARGARORD);
					// si no prueba otra vez
				}
				else if (OkDestinoDescarga ())
				{
					if (DescargarOrden ())
						//m_estado = DESCARGARORD;//Xavi, 02/11/05
						SetEstadoActual(DESCARGARORD);
					// si no prueba otra vez
				}
			}
			break;
	case DESCARGARORD:
		if (SOrderDone ())
		{
			m_uErrorEstacion = 0;	// por si hubiera error en la estación
			// Se debe descargar de forma "manual" tambien
			SetDelivered ();
			SetCountEsperar ();	// Para que se pueda esperar
			SetEstado (AGVSINDATOS);
		}
		else
		{
			if (SOrderDoneParcial ())
			{
				if (SetUnloadDestination ())
				{
					//m_estado = IRDESCARGARORD;//Xavi, 02/11/05
					SetEstadoActual(IRDESCARGARORD);
				}
			}
			else
			{
				// Michael 15.01.2002 - Action run would seem to get lost at times
				if (FZ_AKTION_GESPERRT (*m_pFzdat))
				{
					// Action is stopped
					MSGWARNING1 ("Agv: %d en DESCARGARORD con action stopped", m_id);
					SetAction (true);
				}
				else
				{
					if (DestinoDiferente (DESCARGARORD))
						//m_estado = IRDESCARGARORD;//Xavi, 02/11/05
						SetEstadoActual(IRDESCARGARORD);
					else
						// Michael 22.02.2002 Por si no está viendo nada !!!
						if (! FZ_HAT_FEHLER (*m_pFzdat) 
							&& ! ORDERDONEMESA1 (*m_pFzdat)
							&& ! ORDERDONEMESA2 (*m_pFzdat)
							&& SLlegadoDestino ()	// Por si ha salido de la estación
							&& g_Container.SystemDriveAllowed () // Michael 28.02.2002
							&& !IsDriveForbiddenByBuB ())
						{
							// Es que no debe contestar al poll en este caso...
							m_fSendQuitum = true; // Enviar otro quitum
							MSGERROR2 ("Agv: %d estado: %s Enviando quitum de nuevo",
								m_id, GetStringEstado ());
						}
				}
			}
		}
		break;
	case IRCARGARBAT:
		if (!SCargarBateria ())
		{
			MSGAVISO1 ("Agv: %d petición de carga de bateria anulada", m_id);
			SetZiel (&m_pFzdat->auftziel);	// Borrar destino
			SetEstado (AGVSINDATOS);
		}
		else
		{
			
			if (DestinoDiferente (m_estado))
				;
			else
				if (SLlegadoDestino ())
				{
					if (m_pFzdat->fzziel.nDest != 0)
						SetAction (true);	// action free
					// Estado mismo
					else
					{
						SleepAgv ();
						//m_estado = CARGARBATWAITPLC;//Xavi, 02/11/05
						SetEstadoActual(CARGARBATWAITPLC);
					}
				}
		}
		break;
	case CARGARBATWAITPLC:
		if (SwitchOnCargadorPLC ())
		{
			//m_estado = CARGARBAT;//Xavi, 02/11/05
			SetEstadoActual(CARGARBAT);
			m_nCargador = m_pFzdat->mp;	// Actualizar donde esté
			m_fWillUpdateBuB = true;
		}
		else
		{
			CAlarma * ptrAlarma = new CAlarma;
			ptrAlarma->AvisarAlarma (2, 15, 2, m_id, m_pFzdat->mp);
			MSGERROR1 ("Agv: %d No se ha activado la carga de bateria", m_id);
		}
		
		// M2015 TODO
		SleepAgv();


		break;
	case CARGARBAT:
		if (SetCargado ())
		{
			// M2015 TODO
			WakeUpAgv();

			SetEstado (AGVSINDATOS);
			m_fWillUpdateBuB = true;
		}
		break;
	// Michael 14.06.2005 Mods T5K
	case IRMANTENIMIENTO:
		if (!SEnviarMantenimiento())
		{
			MSGAVISO1 ("Agv: %d petición de ir a mantenimiento anulada", m_id);
			SetZiel (&m_pFzdat->auftziel);	// Borrar destino
			SetEstado (AGVSINDATOS);
		}
		else
		{
		if (DestinoDiferente (m_estado))
			;
		else
			if (SLlegadoDestino ())
			{
				// Quitar el AGV
				SetEstado (MANTENIMIENTO); 

			}
		}
		break;
	case MANTENIMIENTO:
		if (!SEnviarMantenimiento())
		{
			MSGAVISO1 ("Agv: %d mantenimiento acabado", m_id);
			SetZiel (&m_pFzdat->auftziel);	// Borrar destino
			SetEstado (AGVSINDATOS);
		}
		break;
	} // Switch
	
	lockAgv.Unlock ();
	
}




// Asignar Orden al AGV
bool CAgv::PickupOrden()
{
	bool fRes;

	if (!IsWlan()) {

		typ_tele_info teleinfo;
		CString str;

		teleinfo.tele_art = 'I';



		str.Format ("%02d", m_id);
		strncpy ((char *)teleinfo.fznr, str, 2);

		TYPE_STATION_DEF * ptrZiel;

		if ('S' == m_pOrden->m_at.herkunft)
			// Es una orden de AGV manual
			ptrZiel = &m_pOrden->m_at.az;
		else
			ptrZiel = &m_pOrden->m_at.hz;



		str.Format ("%04d", ptrZiel->nDest);
		strncpy ((char *)teleinfo.auftziel, str, 4);

		teleinfo.auftpos = ptrZiel->nPos + 0x30; // Int to Char
		teleinfo.auftebene = ptrZiel->nLayer + 0x30; // Int to Char
		teleinfo.auftlage = ptrZiel->ucSide;

		teleinfo.kommando_0 = 0;
		teleinfo.kommando_0 |= BIT7;	// Always 1

		// Michael 30.10.2001 But why always blocked ???
		SetBlocked (true);
		SetTeleBlocked (&teleinfo.kommando_0, true);

		teleinfo.kommando_0 |= BIT3;	// Action stopped (Don't load)

		teleinfo.kommando_1 = BIT7;		// Always 1

		// Si la mesa es doble, no importa si es una o dos paletas
		if (2 == m_pOrden->m_ptrEstOri->GetNumeroMesas ())
		{
			if (m_pOrden->m_at.fMesa1)
				teleinfo.kommando_1 |= BIT1;	// Pickup Conveyor 1
			if (m_pOrden->m_at.fMesa2)
				teleinfo.kommando_1 |= BIT3;	// Pickup Conveyor 2
		}
		else
			SetMesaToPickup (&teleinfo.kommando_1);	// Ya que es complejo


		if (m_pOrden->m_at.fSimCarga)
			teleinfo.kommando_1 |= BIT6;	// Simulate Pickup


	if (!g_Container.m_fSimulateCom)
		fRes = SendInfoTelegram (teleinfo);
	else
		fRes = SimulateInfoTelegram (teleinfo);
	}
	else
	{
		// WLan Order
		typ_tele_info_wlan teleinfo;
		CString str;

		teleinfo.tele_art = 'I';
		
		str.Format ("%02d", m_id);
		strncpy ((char *)teleinfo.fznr, str, 2);

		TYPE_STATION_DEF * ptrZiel;

		if ('S' == m_pOrden->m_at.herkunft)
			// Es una orden de AGV manual
			ptrZiel = &m_pOrden->m_at.az;
		else
			ptrZiel = &m_pOrden->m_at.hz;

		str.Format ("%04d", ptrZiel->nDest);
		strncpy ((char *)teleinfo.auftziel, str, 4);


		sprintf((char *)teleinfo.auftpos, "%02d",m_pFzdat->auftziel.nPos);
		// Michael M2015 layer is always 1 for new AGVs
		// TODO - see can this be changed as seems to be 0 for old agvs m_pFzdat->auftziel.nLayer = 1;

		teleinfo.auftebene = m_pFzdat->auftziel.nLayer + 0x30; // Int to Char
		teleinfo.auftebene = 1 + 0x30; // Int to Char

		teleinfo.auftlage = m_pFzdat->auftziel.ucSide;

		str.Format ("%04d", m_pFzdat->nmp);
		strncpy ((char *)teleinfo.nmp, str, 4);
		str.Format ("%04d", m_pFzdat->uemp);
		strncpy ((char *)teleinfo.uemp, str, 4);
		
		
		strncpy ((char *)teleinfo.EntryPoint, "0000", 4);



		teleinfo.kommando_0 = 0;
		teleinfo.kommando_0 |= BIT7;	// Always 1

		// Michael 30.10.2001 But why always blocked ???
		
		// Michael 25.05.2015 Don't interfere with current state of Agv SetBlocked (true);

		SET_CTRL_STOP(*m_pFzdat);
		SET_AKTION_SPERR(*m_pFzdat);	// Action stopped (Don't load)

		teleinfo.kommando_1 = BIT7;		// Always 1

		// Si la mesa es doble, no importa si es una o dos paletas
		if (2 == m_pOrden->m_ptrEstOri->GetNumeroMesas ())
		{
			if (m_pOrden->m_at.fMesa1)
				teleinfo.kommando_1 |= BIT1;	// Pickup Conveyor 1
			if (m_pOrden->m_at.fMesa2)
				teleinfo.kommando_1 |= BIT3;	// Pickup Conveyor 2
		}
		else
			SetMesaToPickup (&teleinfo.kommando_1);	// Ya que es complejo


		teleinfo.kommando_2 = m_pFzdat->Status2;
		


		if (m_pOrden->m_at.fSimCarga)
			teleinfo.kommando_1 |= BIT6;	// Simulate Pickup

	if (!g_Container.m_fSimulateCom)
	{
		m_WlanTel.SetInfoTelegram (teleinfo, m_pWlanMsg);
		fRes = true;
	}
// 	else
//		fRes = SimulateInfoTelegram (teleinfo);
	}



	if (fRes)
	{
		m_fWillUpdateBuB = true;
		m_pOrden->SetEstado (COLLECT);
	}
	return (fRes);

}




// Enviar un telegrama de info al AGV
bool CAgv::SendInfoTelegram(typ_tele_info tele)
{
	int fRetVal;
	int i;
	bool fAceptado;

	// Guardar los datos del commando por si...
	//M2015
	//m_pFzdat->msollfz0 = tele.kommando_0;
	//m_pFzdat->msollfz1 = tele.kommando_1;

	m_pFzdat->Kommando0 = tele.kommando_0;
	m_pFzdat->Kommando1 = tele.kommando_1;


	for (i = 0, fAceptado = false; i < 3 && !fAceptado; i++)
	{
	m_Tel.SetInfoTelegram (tele);
	fRetVal = m_pSerCom->SendTelegrama (m_Tel.m_strTelegrama);
	if (fRetVal)
		{
		// El telegrama requiere respuesta
		fRetVal = m_pSerCom->ReceiveTelegrama (m_Tel.m_strRespuesta);

		CString strTrace;
		switch (fRetVal)
		{
		case WAIT_OBJECT_0:
			// se ha recibido el telegrama de respuesta
			m_Tel.SetTeleRecibido ();
			m_numTimeout = 0;	// Reset timeouts

			if (ComprobarRespuestaInfo (tele))
			{
				if (ShouldBlock ())
					SetBlocked (true);
				else
					if (IsBlocked ())
					{
						SetBlocked (false);
						// Michael 17.10.2001 Sobre este envio: fRetVal = m_pSerCom->SendTelegrama (m_Tel.StrQuitum (m_id));
					}
				fAceptado = true;
			}
			break;
		case WAIT_TIMEOUT:
			m_Tel.SetTeleTout ();
			strTrace.Format ("Timeout waiting for Info reply Agv: %d", m_id);
			MSGTRACE (strTrace);
			break;
		default:
			// XXX mirar posibilidades aquí
			m_Tel.SetTeleTout ();
			strTrace.Format ("ReceiveTelegrama ha devuelto: %d (PickupOrden)", fRetVal);
			MSGERROR (strTrace);
			break;
			}
		}
		else
		{
			m_Tel.SetTeleNull ();
		}
	} // for (..)
	return (fAceptado);

}




// Comprobar que la respuesta al teleinfo telegram es correcto
bool CAgv::ComprobarRespuestaInfo(typ_tele_info tele)
{
	bool fRet = false;	// Valor a devolver
	typ_tele_kopf* pTeleHeader; // Telegram header

	pTeleHeader = (typ_tele_kopf*) m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_kopf));

	if (! CheckTeleAgv ((char *)pTeleHeader->fznr))
		return (false);

	switch (pTeleHeader->tele_art)
	{
	case 'S':
		{ // brackets to avoid initialization problem
		typ_tele_gs * tele_gs = (typ_tele_gs*) m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_gs));
		InterpretarTeleGrosseStatus (*tele_gs);
		fRet = true;
		}
		break;
	default:
		MSGERROR1 ("ComprobarRespuestaInfo telegrama tipo: %c", pTeleHeader->tele_art);
		fRet = false;	// Possibly there are error telegrams ?
	}

	return (fRet);
}



// Ver si el AGV está en destino
bool CAgv::SLlegadoDestino()
{
	bool fRetVal = false;
	if ((m_pFzdat->mp == m_pFzdat->fzziel.nDest)	// M2015 Is this set ok??
		&& m_pFzdat->nmp == 0) // Nmp is 0 at station
		fRetVal = true;
	else
	{
		// If was already in the station fzziel is false
		if (m_pFzdat->fzziel.nDest == 0 && m_pFzdat->nmp == 0
			&& m_pFzdat->auftziel.nDest == m_pFzdat->mp)
			fRetVal = true;
		else
			fRetVal = false;
	}
	return (fRetVal);
}


// Cargar una orden sobre el AGV
bool CAgv::CargarOrden()
{
	// Permitir acción del Agv
	bool fRes = SetAction (true);
	if (fRes)
	{
		// Actualizar la orden como ya cargado para delivery
		// Michael 18.10.2001 Not here - see SOrdenCargada	m_pOrden->SetEstado (DELIVER);
		m_fWillUpdateBuB = true;
	}
	return (fRes);
}

// Ver si la orden ha sido cargado correctamente para 1 o 2 Conveyors
bool CAgv::SOrdenCargada()
{
	bool fOrderDone;

	if (m_pOrden->m_at.fSimCarga || g_Container.m_fSimulateCom)
	{	
		fOrderDone = true;
		// Simulate presencia carga
		if (g_Container.m_fSimulateCom)
		{
			//SimulateCargada((bool)m_pOrden->m_at.fMesa1, (bool)m_pOrden->m_at.fMesa2);
			SimulateCargada(m_pOrden->m_at.fMesa1 == TRUE, m_pOrden->m_at.fMesa2 == TRUE);	//JLac
		}
	}
	else
	{
		
		if ('S' == m_pOrden->m_at.herkunft && 0 == m_pOrden->m_at.akt_org)
			// Orden manual - no hay nada que cargar
			fOrderDone = true;
		else
		{
			fOrderDone = false;
			if (m_pOrden->m_at.fMesa1)
				if (!m_pOrden->m_at.fSimCarga)
					fOrderDone = MESA1CARGADA (*m_pFzdat) != 0 ? true : false; // Order done 1
				else
					fOrderDone = ORDERDONEMESA1 (*m_pFzdat) != 0 ? true : false;
			else
				fOrderDone = true;
			if (fOrderDone)
				if (m_pOrden->m_at.fMesa2)
					if (!m_pOrden->m_at.fSimCarga)
						fOrderDone = MESA2CARGADA (*m_pFzdat) != 0 ? true : false; // Order done 2
					else
						fOrderDone = ORDERDONEMESA2 (*m_pFzdat) != 0 ? true : false;

		}
	}

	if (fOrderDone)
	{
		MSGTRACE3 ("Agv:%d Orden Cargada Mesa 1: %s Mesa 2: %s",
					m_id,
					m_pOrden->m_at.fMesa1 ? "TRUE" : "FALSE",
					m_pOrden->m_at.fMesa2 ? "TRUE" : "FALSE");
		m_pOrden->SetEstado (DELIVER);
	}


	return (fOrderDone);
}


// Enviar orden de llevar a destino una orden ya cargada
bool CAgv::SetUnloadDestination()
{
	bool fRes;

	if (!IsWlan())
	{
		typ_tele_info teleinfo;
		CString str;
		TYPE_STATION_DEF * ptrZiel;

		// Actualizar actual destination

		if ('S' == m_pOrden->m_at.herkunft)
			// Es una orden de AGV manual
			ptrZiel = &m_pOrden->m_at.az;
		else
			ptrZiel = &m_pOrden->m_at.bz;

		SetZiel (&m_pFzdat->auftziel, ptrZiel->nDest,
			(unsigned char)ptrZiel->nPos,
			ptrZiel->ucSide,
			ptrZiel->nLayer);

		USHORT nFz = m_pFzdat->fznr;

		MSGTRACE2 ("Agv:%d SetUnload Destination: %d",
			m_id,
			ptrZiel->nDest);

		teleinfo.tele_art = 'I';

		str.Format ("%02d", m_id);
		strncpy ((char *)teleinfo.fznr, str, 2);

		str.Format ("%04d", ptrZiel->nDest);
		strncpy ((char *)teleinfo.auftziel, str, 4);

		teleinfo.auftpos = ptrZiel->nPos + 0x30; // Int to Char
		teleinfo.auftebene = ptrZiel->nLayer + 0x30; // Int to Char
		teleinfo.auftlage = ptrZiel->ucSide;


		// Michael 01.10.2001: Set Agv blocked initially ?

		teleinfo.kommando_0 = 0;
		SetBlocked (true);
		SetTeleBlocked (&teleinfo.kommando_0, true);

		teleinfo.kommando_0 |= BIT3;	// XXX Michael 9.12.2004 Wasn't doing this Action stopped (Don't load)
		teleinfo.kommando_0 |= BIT7; // Always 1

		teleinfo.kommando_1 = 0;

		if ('S' == m_pOrden->m_at.herkunft)
		{
			// Orden manual
			if (2 == m_pOrden->m_at.akt_org)
			{
				// Deposito
				teleinfo.kommando_1 = 0;
				if (m_pOrden->m_at.fMesa1)
					teleinfo.kommando_1 = BIT2;		// Conveyor 1 delivery

				if (m_pOrden->m_at.fMesa2)
					teleinfo.kommando_1 |= BIT4;	// Conveyor 2 delivery
			}
			// Si no - solo ir a destino
		}
		else
		{
			if (2 == m_pOrden->m_ptrEstDes->GetNumeroMesas ())
			{
				if (m_pOrden->m_at.fMesa1)
					teleinfo.kommando_1 = BIT2;		// Conveyor 1 delivery

				if (m_pOrden->m_at.fMesa2)
					teleinfo.kommando_1 |= BIT4;	// Conveyor 2 delivery
			}
			else
				SetMesaToDeliver (&teleinfo.kommando_1);
		}

		teleinfo.kommando_1 |= BIT7;	// Always 1

		if (m_pOrden->m_at.fSimCarga)
			teleinfo.kommando_1 |= BIT6;	// Simulate delivery

		if (!g_Container.m_fSimulateCom)
			fRes = SendInfoTelegram (teleinfo);
		else
			fRes = SimulateInfoTelegram (teleinfo);
	}
	else
	{
		typ_tele_info_wlan teleinfo;
		CString str;
		TYPE_STATION_DEF * ptrZiel;

		// Actualizar actual destination

		if ('S' == m_pOrden->m_at.herkunft)
			// Es una orden de AGV manual
			ptrZiel = &m_pOrden->m_at.az;
		else
			ptrZiel = &m_pOrden->m_at.bz;

		SetZiel (&m_pFzdat->auftziel, ptrZiel->nDest,
			(unsigned char)ptrZiel->nPos,
			ptrZiel->ucSide,
			ptrZiel->nLayer);

		USHORT nFz = m_pFzdat->fznr;

		MSGTRACE2 ("Agv:%d SetUnload Destination: %d",
			m_id,
			ptrZiel->nDest);

		teleinfo.tele_art = 'I';

		str.Format ("%02d", m_id);
		strncpy ((char *)teleinfo.fznr, str, 2);

		str.Format ("%04d", ptrZiel->nDest);
		strncpy ((char *)teleinfo.auftziel, str, 4);

		sprintf((char *)teleinfo.auftpos, "%02d",m_pFzdat->auftziel.nPos);

		// Michael M2015 layer is always 1 for new AGVs
		// TODO - see can this be changed as seems to be 0 for old agvs m_pFzdat->auftziel.nLayer = 1;

		teleinfo.auftebene = m_pFzdat->auftziel.nLayer + 0x30; // Int to Char
		teleinfo.auftebene = 1 + 0x30; // Int to Char

		teleinfo.auftlage = m_pFzdat->auftziel.ucSide;

		str.Format ("%04d", m_pFzdat->nmp);
		strncpy ((char *)teleinfo.nmp, str, 4);
		str.Format ("%04d", m_pFzdat->uemp);
		strncpy ((char *)teleinfo.uemp, str, 4);
		
		
		strncpy ((char *)teleinfo.EntryPoint, "0000", 4);

		// Michael 01.10.2001: Set Agv blocked initially ?

		// Michael 25.05.2015 Don't block here SetBlocked (true);
		
		SET_CTRL_STOP(*m_pFzdat);
		SET_AKTION_SPERR(*m_pFzdat);	// Action stopped (Don't load)

		teleinfo.kommando_0 = BIT7; // Always 1

		teleinfo.kommando_1 = 0;

		if ('S' == m_pOrden->m_at.herkunft)
		{
			// Orden manual
			if (2 == m_pOrden->m_at.akt_org)
			{
				// Deposito
				teleinfo.kommando_1 = 0;
				if (m_pOrden->m_at.fMesa1)
					teleinfo.kommando_1 = BIT2;		// Conveyor 1 delivery

				if (m_pOrden->m_at.fMesa2)
					teleinfo.kommando_1 |= BIT4;	// Conveyor 2 delivery
			}
			// Si no - solo ir a destino
		}
		else
		{
			if (2 == m_pOrden->m_ptrEstDes->GetNumeroMesas ())
			{
				if (m_pOrden->m_at.fMesa1)
					teleinfo.kommando_1 = BIT2;		// Conveyor 1 delivery

				if (m_pOrden->m_at.fMesa2)
					teleinfo.kommando_1 |= BIT4;	// Conveyor 2 delivery
			}
			else
				SetMesaToDeliver (&teleinfo.kommando_1);
		}

		teleinfo.kommando_1 |= BIT7;	// Always 1
		
		teleinfo.kommando_2 = m_pFzdat->Status2;

		if (m_pOrden->m_at.fSimCarga)
			teleinfo.kommando_1 |= BIT6;	// Simulate delivery

		if (!g_Container.m_fSimulateCom)
			m_WlanTel.SetInfoTelegram (teleinfo, m_pWlanMsg);
		fRes = true;
//		else
//			fRes = SimulateInfoTelegram (teleinfo);

	} // WLan
	if (fRes)
	{
		m_fWillUpdateBuB = true;
	}
	return (fRes);

}

// Ver si una orden (ya cargado) ha sido descargado en destion
bool CAgv::SOrderDone()
{
	bool fOrderDone = true;
	if ('S' == m_pOrden->m_at.herkunft && m_pOrden->m_at.akt_org == 0)
		// Orden manual - no hay nada que descargar
		fOrderDone = true;
	else
	{
		// Hay que mirar que mesas del Agv
		if (m_pOrden->m_at.fMesa1)
			fOrderDone = ! MESA1CARGADA (*m_pFzdat);		// Conveyor 1 Order done
		if (m_pOrden->m_at.fMesa2)
			fOrderDone = fOrderDone && ! MESA2CARGADA (*m_pFzdat);
	}
	return (fOrderDone);
}

bool CAgv::SimulateInfoTelegram(typ_tele_info tele)
{

	MSGTRACE1 ("SimulateInfoTelegram Agv: %d", m_id);

	// XXX-Lac04: Save destination number
	
	unsigned short nFzzielOld = m_pFzdat->fzziel.nDest;


    m_pFzdat->fzziel.nDest = GetAtoi((LPCTSTR)tele.auftziel, sizeof(tele.auftziel));
    m_pFzdat->fzziel.nPos   = tele.auftpos;
    m_pFzdat->fzziel.nLayer = tele.auftebene;
	m_pFzdat->fzziel.ucSide = tele.auftlage;

	QuizasInsertarBloqueo(tele.kommando_0);//Xavi, 02/11/05
	// M2015
    //m_pFzdat->fahr_byte_ist = tele.kommando_0;
    //m_pFzdat->akt_byte_ist  = tele.kommando_1;
	m_pFzdat->Status0 = tele.kommando_0;
	// M2015 TODO Check kommando_1 is Status1
    m_pFzdat->Status1  = tele.kommando_1;
    // m_pFzdat->last_byte_ist = 0;
	if (!(tele.kommando_0 & BIT3) && (tele.kommando_1 & BIT1))
	{
		// Pickup conveyor 1
		// M2015 TODO m_pFzdat->last_byte_ist |= BIT2;
		SET_BELADEN(*m_pFzdat);
		m_pFzdat->aktion = BIT1;		// Pickup
	}
	else if (!(tele.kommando_0 & BIT3) && (tele.kommando_1 & BIT2))
	{
		// Deliver conveyor 1
		m_pFzdat->aktion = BIT2;
	}
	else
		// TODO 2014 this is conveyor loaded m_pFzdat->last_byte_ist &= OFFBIT2;
	if (!(tele.kommando_0 & BIT3) && (tele.kommando_1 & BIT3))
		// Pickup conveyor 2
		// M2015 TODO m_pFzdat->last_byte_ist |= BIT3;
		SET_BELADEN_2 (*m_pFzdat);
	else if (!(tele.kommando_0 & BIT3) && (tele.kommando_1 & BIT4))
		// deliver Conveyor 2
		m_pFzdat->aktion = BIT2;
	else
		// TODO 2014 m_pFzdat->last_byte_ist &= OFFBIT3;
	
	// M2015 TODO What is this bit for? m_pFzdat->last_byte_ist |= BIT7;
	m_pFzdat->Status2 |= BIT7;

	// m_pFzdat->wegstr = GetAtoi((char *)tele_gs->wegstrecke, sizeof(tele_gs->wegstrecke)) * FZ_WEG_KORREKTUR;

	m_pFzdat->nmp = GetNmp (m_pFzdat->mp, m_pFzdat->fzziel.nDest);

	// Michael 10.01.2002
	m_pFzdat->uemp = GetNmp (m_pFzdat->nmp, m_pFzdat->fzziel.nDest);


	// XXX-Lac04: Start
	if(nFzzielOld != m_pFzdat->fzziel.nDest)
	{
		if(m_pFzdat->fzziel.nDest != 0)
		{
			USHORT nFz = m_pFzdat->fznr;
			GetMpFolge(m_pFzdat,pMpFolgeVerz[nFz-1],m_pFzdat->fzziel.nDest);
			*(pCrMengeVerz[nFz-1]) = 0;
		}
	}
	// XXX-Lac04: End

	return (true);

}

// Simulate arrival of an 'S' telegram from the AGV

int CAgv::SimulatePoll()
{
	int iRetVal;


	//if (m_id <= KKFZMAX)
	if (m_id <= (UINT)g_pCourseData->GetNumberAGVs())
	{
		iRetVal = WAIT_OBJECT_0;
		if (!TEST_FZ_IM_SYSTEM (*m_pFzdat))
		{
			UINT insertPoint, insertPointReg;
			insertPoint = 52 + m_id;
			if (m_id > 8)
			{
				insertPoint = m_id - 7; 
			}
			CString format;
			format.Format("%s%d", "LastPosAgv_", m_id);
			insertPointReg = AfxGetApp ()->GetProfileInt ("Settings", format, 0);
			if (insertPointReg == 0)
				AfxGetApp ()->WriteProfileInt ("Settings", format, insertPoint);
			else
				insertPoint = insertPointReg;

			
			InsertarAGV (insertPoint, 1);	
			SetEstado (AGVSINDATOS);
		}

		// Quitar el order done
		// M2015 m_pFzdat->mistfz2 &= OFFBIT1;
		RESET_AUFTRAG_ERLEDIGT (*m_pFzdat);
		// M2015 m_pFzdat->mistfz2 &= OFFBIT6;
		RESET_AUFTRAG_ERLEDIGT_2 (*m_pFzdat);

		TYP_FZDAT fzDatCopy;
		fzDatCopy.Status0 = m_pFzdat->Status0;
		fzDatCopy.Status1 = m_pFzdat->Status1;
		fzDatCopy.Status2 = m_pFzdat->Status2;
		fzDatCopy.Status3 = m_pFzdat->Status3;

		typ_tele_gs tele_gs;
		tele_gs.tele_kopf.tele_art = 'S';

		tele_gs.amp_pos = 0;	// XXX-Lac02

		CString strTemp;

		strTemp.Format ("%02d", m_id);
		strncpy ((char *)tele_gs.tele_kopf.fznr, strTemp, 2);



		// (typ_tele_gs*) m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_gs));
		UINT lastMp = m_pFzdat->mp;

		if (lastMp != m_pFzdat->fzziel.nDest && m_pFzdat->fzziel.nDest != 0)
		{
			if (m_pFzdat->nmp != 0)
			{
				m_pFzdat->wegstr = 0;	// XXX-Lac02

				if(! IsBlocked ()) // Michael
				{
					// AGV can drive
					strTemp.Format ("%04d", m_pFzdat->nmp);
					strncpy ((char *)tele_gs.amp, strTemp, 4);
					UINT unmp = GetNmp (m_pFzdat->nmp, m_pFzdat->fzziel.nDest);
					strTemp.Format ("%04d", unmp);
					strncpy ((char *)tele_gs.nmp, strTemp, 4);

					// XXX-Lac: AGV send also UEMP
					UINT uemp = GetNmp (unmp, m_pFzdat->fzziel.nDest);
					strTemp.Format ("%04d", uemp);
					strncpy ((char *)tele_gs.uemp, strTemp, 4);
					//XXX-Lac02:
					if(m_pFzdat->wegstr <= 1000)
					{
						m_pFzdat->wegstr = 1000;
					}

				}
				else   // XXX-Lac01
				{
					// AGV must stop
					strTemp.Format ("%04d", m_pFzdat->mp);
					strncpy ((char *)tele_gs.amp, strTemp, 4);
					
					// Michael 02.01.2002
					// UINT xnmp = GetNmp (m_pFzdat->mp, m_pFzdat->fzziel.nr);
					// strTemp.Format ("%04d", xnmp);
					strTemp.Format ("%04d", m_pFzdat->nmp);
					strncpy ((char *)tele_gs.nmp, strTemp, 4);
					
					// xnmp = GetNmp (m_pFzdat->nmp, m_pFzdat->fzziel.nr);
					// strTemp.Format ("%04d", xnmp);
					strTemp.Format ("%04d", m_pFzdat->uemp);
					strncpy ((char *)tele_gs.uemp, strTemp, 4);
					// XXX-Lac02:
					USHORT nBlockweg = GetBlkWeg(m_pFzdat->mp,m_pFzdat->nmp);
					if(m_pFzdat->wegstr < nBlockweg)
					{
						m_pFzdat->wegstr = nBlockweg;
					}
				}
			}
			else
			{
				strTemp.Format ("%04d", m_pFzdat->mp);
				strncpy ((char *)tele_gs.amp, strTemp, 4);

				// XXX-Lac: Set NMp and UEMp
				UINT nNMp = GetNmp (m_pFzdat->mp, m_pFzdat->fzziel.nDest);
				strTemp.Format ("%04d", nNMp);
				strncpy ((char *)tele_gs.nmp, strTemp, 4);

				UINT nUeMp = GetNmp (nNMp, m_pFzdat->fzziel.nDest);
				strTemp.Format ("%04d", nUeMp);
				strncpy ((char *)tele_gs.uemp, strTemp, 4);

			}
		}
		else
		{
			strTemp.Format ("%04d", m_pFzdat->mp);
			strncpy ((char *)tele_gs.amp, strTemp, 4);
			tele_gs.amp_pos = m_pFzdat->mppos;


			if (m_estado != AGVSINDATOS)
			{
				if (m_pFzdat->fzziel.nDest)
				{
					// XXX-Lac02:
					tele_gs.amp_pos = (unsigned char)m_pFzdat->fzziel.nPos;

					strncpy ((char *)tele_gs.nmp, "0000", 4);
					// Esta en destino
					// Si la orden tenia accion, realizalo...
					// Si era cargar, carga
					// Si era descargar, descarga !!!
					// M2015 if (! (m_pFzdat->mistfz0 & BIT3))
					if (! FZ_AKTION_GESPERRT(*m_pFzdat))	// Action stopped
					{
						// Action run
						// M2015 if (m_pFzdat->mistfz1 & BIT1)
						if (FZ_HOLT(*m_pFzdat))
						{
							// M2015 mistfz2 |= BIT2;	// Conveyor 1 loaded
							SET_BELADEN(fzDatCopy);
							SET_BELADEN_2(fzDatCopy);
						}
						// M2015 else if (m_pFzdat->mistfz1 & BIT2)	// Conveyor 1 delivery
						else if (FZ_BRINGT(*m_pFzdat))	// Conveyor 1 delivery
						{
							// M2015 mistfz2 &= OFFBIT2;	// Conveyor 1 unloaded
							RESET_BELADEN(fzDatCopy);
							RESET_BELADEN_2(fzDatCopy);
							m_pFzdat->aktion = 0;	// Sin acción
						}
						// M2015 if (m_pFzdat->mistfz1 & BIT3)		// Conveyor 2 pickup
						// Michael TODO incorporated above, for both tables
						//if (m_pFzdat->mistfz1 & BIT3)		// Conveyor 2 pickup
						//	mistfz2 |= BIT3;	// Conveyor 2 loaded
						//else if (m_pFzdat->mistfz1 & BIT4)	// Conveyor 2 delivert
						//{
						//	mistfz2 &= OFFBIT3;	// Conveyor 2 unloaded
						//	m_pFzdat->aktion = 0;
						//}
					}
					SET_FZ_AUFTRAG_ERLEDIGT (fzDatCopy);
					SET_FZ_AUFTRAG_ERLEDIGT_2 (fzDatCopy);
					SET_AGV_DRIVE(fzDatCopy);

				}
				// M2015 mistfz0 |= BIT6;	// Battery full
				SET_FZ_BATTERIE_GELADEN(fzDatCopy);

			}
		}
		tele_gs.status_0 = fzDatCopy.Status0;
		tele_gs.status_1 = fzDatCopy.Status1;
		tele_gs.status_2 = fzDatCopy.Status2;

		strTemp.Format ("%04d", m_pFzdat->fzziel.nDest);
		strncpy ((char *)tele_gs.fzziel, strTemp, 4);
		tele_gs.fzpos = (unsigned char)m_pFzdat->fzziel.nPos;
		tele_gs.fzebene = m_pFzdat->fzziel.nLayer;
		tele_gs.fzlage = m_pFzdat->fzziel.ucSide;

		// XXX-Lac02:

		CString strFormat,
			    strLength;
		strLength.Format("%d",sizeof(tele_gs.wegstrecke));
		strFormat = "%0" + strLength + "d";
		strTemp.Format(strFormat, m_pFzdat->wegstr/FZ_WEG_KORREKTUR);
		strncpy((char*)tele_gs.wegstrecke, strTemp, sizeof(tele_gs.wegstrecke));


		// Michael 21.11.2001 -- Set 0 el destino
		if (m_pFzdat->mp >= 53 && m_pFzdat->mp <= 60)
		{
			//m_pFzdat->wegstr = z_typ_tab[zt_ver_tab[statdat [vtstat[m_pFzdat->mp]].zieltyp_l]].pos_weg;
			TYPE_STATION_DEF station;
			station.nDest = m_pFzdat->mp;
			station.ucSide = 'L';
			station.nPos = 1;
			station.nLayer = 1;
			m_pFzdat->wegstr = pAGVTCourse->GetPosWay(station);
		}

		// Michael 21.11.2001
        unsigned short wegstr = m_pFzdat->wegstr/FZ_WEG_KORREKTUR;
		tele_gs.wegstrecke[1] = wegstr & 0x3f;
		//tele_gs.wegstrecke[0] = (wegstr & 0xff40) >> 6;
		tele_gs.wegstrecke[0] = (wegstr & 0xffc0) >> 6;



		// Copy telegram to input buffer
		LPTSTR pszbuf = m_Tel.m_strRespuesta.GetBuffer (sizeof (tele_gs));
		memcpy (pszbuf, (const void *)&tele_gs, sizeof (tele_gs));
		pszbuf [sizeof (tele_gs)] = '\0';
		m_Tel.m_strRespuesta.ReleaseBuffer ();

	}
	else
	{
		iRetVal = WAIT_TIMEOUT;
	}



	return iRetVal;
}

/*
// Set time interval in milliseconds for the next poll of the AGV
LARGE_INTEGER CAgv::SetNextPollInterval()
{
	int interval;
	bool wasWeg = m_fSendWeg;
	m_fSendWeg = false;

	switch (m_estado)
	{
	case AGVFUERASISTEMA:
		interval = 10000;	// 10 seconds
		break;
	case AGVPERDIDO:
		interval = 20000;	// 20 Seconds
		break;
	case AGVSINDATOS:
		interval = 2000;	// 2 seconds
		break;
	case CARGARBAT:
		interval = 5000;
		break;
	case PUNTOESP:
		interval = 2000;
		break;
	case CARGARBATWAITPLC:
		interval = 5000;	// Esperamos 5 segundos antes de enchufar
		break;

	default:
	{
			   if (!m_fTimeOutThisPoll)
			   {
				   // Michael 16.09.2001 NMP_tabelle * pNmp = GetPNmp (m_pFzdat->mp, m_pFzdat->fzziel.nr);

				   if (0 != m_pFzdat->nmp)
				   {
					   NMP_tabelle * pNmp = GetPNmpFromNmp(m_pFzdat->mp, m_pFzdat->nmp);

					   if (pNmp)
					   {
						   if (IsBlocked())
						   {
							   interval = 500;	// medio segundo si el AGV está bloqueado
						   }
						   else
						   {
							   // Michael TODO Mods 2009: If weg telegram already sent, then distance should take the distance travelled into account.
							   // TODO
							   interval = (fahrzeit_tab_fest[pNmp->zeit_index] * 100) - 2000; // tenths of second	XXX Check this
							   if (interval < 0)
								   interval = 0;
							   // Michael 29.04.2009 Mods 2009
							   //if (IsWegSection() )
							   // J.Lachmund: 10.12.2009
							   if (IsDistanceToRequestWegTele())
							   {
								   if (!IsBlockingDistanceTravelled() && !m_lastWegWasSame)
								   {
									   interval = 1000;	// Send a weg telegram every second
									   m_fSendWeg = true;
								   }
								   else
								   {
									   m_fSendWeg = false;
									   //interval = 500;	// Milliseconds
									   //J.Lachmund, 25.11.2010:
									   int nToDrive = pNmp->str_len - m_pFzdat->wegstr;
									   if (nToDrive < 0) nToDrive = 0;
									   interval = (fahrzeit_tab_fest[pNmp->zeit_index] * 100) * nToDrive / pNmp->str_len;
									   interval = (interval < 500) ? 500 : (interval - 500);
								   }
							   }
							   // Simulation 2014 Compensate times
							   if (g_Container.m_fSimulateCom)
							   {
								   double nextint = interval * 1.4;
								   interval = (int)nextint;
							   }

						   }
						   // Poner tiempo para BuB
						   m_pFzdat->fahrzeit = (unsigned short)fahrzeit_tab_fest[pNmp->zeit_index] / 4;	// Check this too !!! XXX
						   MSGTRACE5("SetNextPollInterval agv: %d, mp: %d, nmp: %d fzziel.nDest: %d wegstr: %d",
							   m_id,
							   m_pFzdat->mp,
							   pNmp->nmp,
							   m_pFzdat->fzziel.nDest,
							   m_pFzdat->wegstr);
						   MSGTRACE4("SetNextPollInterval agv: %d, pNmp->zeit_index: %d interval: %d Blocked %s",
							   m_id,
							   pNmp->zeit_index,
							   interval,
							   IsBlocked() ? "TRUE" : "FALSE");

					   }
					   else
					   {
						   MSGWARNING("GetPNmp devuelve 0 en SetNextPollInterval");
						   interval = 2000;
					   }
				   }
				   else
				   {
					   // No hay Nmp
					   // Michael Simulación 2014
					   if (g_Container.m_fSimulateCom) {
						   switch (m_estado) {
						   case CARGARORD:
						   case DESCARGARORD:
							   interval = 15000;	// TODO: Configurable..
							   break;
						   default:
							   interval = 500;

						   }
					   }
					   else {
						   interval = 500;
					   }
				   }
			   }
			   else
			   {
				   // Time outs but may be within polling window
				   if (m_CurTime.QuadPart < m_maxNextPoll.QuadPart)
				   {
					   interval = 500; // Milliseconds, 1 Second...
				   }
				   else
					   interval = 0;
			   }
	}

	} // Switch

	m_minNextPoll = GetTimePlusInterval(interval);
	if (!m_fTimeOutThisPoll)
		// Solo si no hay timeouts...
		m_maxNextPoll = GetTimePlusInterval(interval + 20000);

	return (m_minNextPoll);
}
*/

// Set time interval in milliseconds for the next poll of the AGV
LARGE_INTEGER CAgv::SetNextPollInterval()
{
	int interval;
	bool wasWeg = m_fSendWeg;
	m_fSendWeg = false;

	switch (m_estado)
	{
	case AGVFUERASISTEMA:
		interval = 10000;	// 10 seconds
		break;
	case AGVPERDIDO:
		interval = 20000;	// 20 Seconds
		break;
	case AGVSINDATOS:
		interval = 2000;	// 2 seconds
		break;
	case CARGARBAT:
		interval = 5000;
		break;
	case PUNTOESP:
		interval = 2000;
		break;
	case CARGARBATWAITPLC:
		interval = 5000;	// Esperamos 5 segundos antes de enchufar
		break;

	default:
	{
			   if (!m_fTimeOutThisPoll)
			   {
				   // Michael 16.09.2001 NMP_tabelle * pNmp = GetPNmp (m_pFzdat->mp, m_pFzdat->fzziel.nr);

				   if (0 != m_pFzdat->nmp)
				   {
					   //NMP_tabelle * pNmp = GetPNmpFromNmp(m_pFzdat->mp, m_pFzdat->nmp);
					   CCpDistance* pCpDistance = GetPNmpFromNmp(m_pFzdat->mp, m_pFzdat->nmp);

					   if (pCpDistance != NULL)
					   {
						   if (IsBlocked())
						   {
							   // M2015 interval = 500;	// medio segundo si el AGV está bloqueado
							   interval = 1000;
						   }
						   else
						   {
							   // Michael TODO Mods 2009: If weg telegram already sent, then distance should take the distance travelled into account.
							   // TODO
							   //interval = (fahrzeit_tab_fest[pNmp->zeit_index] * 100) - 2000; // tenths of second	XXX Check this
							   // Michael M2015 12.04.2015 Always negative ?? interval = (pCpDistance->GetTravelTime() * 100) - 2000; // tenths of second	XXX Check this
							   interval = (pCpDistance->GetTravelTime() * 100);
							   if (interval < 0)
								   interval = 0;
							   // Michael 29.04.2009 Mods 2009
							   //if (IsWegSection() )
							   // J.Lachmund: 10.12.2009
							   if (IsDistanceToRequestWegTele())
							   {
								   if (!IsBlockingDistanceTravelled() && !m_lastWegWasSame)
								   {
									   interval = 1000;	// Send a weg telegram every second
									   m_fSendWeg = true;
								   }
								   else
								   {
									   m_fSendWeg = false;
									   //interval = 500;	// Milliseconds
									   //J.Lachmund, 25.11.2010:
									   //int nToDrive = pNmp->str_len - m_pFzdat->wegstr;
									   int nToDrive = pCpDistance->GetLength() - m_pFzdat->wegstr;
									   if (nToDrive < 0) nToDrive = 0;
									   //interval = (fahrzeit_tab_fest[pNmp->zeit_index] * 100) * nToDrive / pNmp->str_len;
									   interval = (pCpDistance->GetTravelTime() * 100) * nToDrive / pCpDistance->GetLength();
									   interval = (interval < 500) ? 500 : (interval - 500);
								   }
							   }
							   // Simulation 2014 Compensate times
							   if (g_Container.m_fSimulateCom)
							   {
								   double nextint = interval * 1.4;
								   interval = (int)nextint;
							   }

						   }
						   // Poner tiempo para BuB
						   //m_pFzdat->fahrzeit = (unsigned short)fahrzeit_tab_fest[pNmp->zeit_index] / 4;	// Check this too !!! XXX
						   unsigned short cp = m_pFzdat->mp;
						   unsigned short ncp = pCpDistance->GetNCp();
						   unsigned short travelTime = pCpDistance->GetTravelTime();	//In 1/2 second
						   m_pFzdat->fahrzeit = (unsigned short)pCpDistance->GetTravelTime();// / 4;	// Check this too !!! XXX
						   MSGTRACE5("SetNextPollInterval agv: %d, mp: %d, nmp: %d fzziel.nDest: %d wegstr: %d",
							   m_id,
							   m_pFzdat->mp,
							   //pNmp->nmp,
							   pCpDistance->GetNCp(),
							   m_pFzdat->fzziel.nDest,
							   m_pFzdat->wegstr);
						   MSGTRACE4("SetNextPollInterval agv: %d, pNmp->zeit_index: %d interval: %d Blocked %s",
							   m_id,
							   //pNmp->zeit_index,
							   pCpDistance->GetDistIndex(),
							   interval,
							   IsBlocked() ? "TRUE" : "FALSE");

					   }
					   else
					   {
						   MSGWARNING("GetPNmp devuelve 0 en SetNextPollInterval");
						   interval = 2000;
					   }
				   }
				   else
				   {
					   // No hay Nmp
					   // Michael Simulación 2014
					   if (g_Container.m_fSimulateCom) {
						   switch (m_estado) {
						   case CARGARORD:
						   case DESCARGARORD:
							   interval = 15000;	// TODO: Configurable..
							   break;
						   default:
							   interval = 500;

						   }
					   }
					   else {
						   interval = 500;
					   }
				   }
			   }
			   else
			   {
				   // Time outs but may be within polling window
				   if (m_CurTime.QuadPart < m_maxNextPoll.QuadPart)
				   {
					   interval = 500; // Milliseconds, 1 Second...
				   }
				   else
					   interval = 0;
			   }
	}

	} // Switch

	m_minNextPoll = GetTimePlusInterval(interval);
	if (!m_fTimeOutThisPoll)
		// Solo si no hay timeouts...
		m_maxNextPoll = GetTimePlusInterval(interval + 20000);

	return (m_minNextPoll);
}

// Limpiar el AGV al haber finalizado una orden
void CAgv::SetDelivered()
{
	m_pOrden->SetEstado (DELIVERED);
	m_pOrden = NULL;	// El Gestor de ordenes borrará
	SetZiel (&m_pFzdat->auftziel);	// Borrar destino
	strcpy ((char *)m_pFzdat->aufident, "");
	m_fWillUpdateBuB = true;
}

// Interpretar un weg telegram: Ver si es necesario XXX
bool CAgv::InterpretarTeleWeg(typ_tele_weg &tele)
{
	unsigned short wegstr = (tele.weg[1] & 0x3f) | ((tele.weg[0] & 0x3f) << 6); // XXX Why ?
	
	char msg[200];

	sprintf (msg, " old wegstr: %d, mp: %d, nmp: %d", 
			m_pFzdat->wegstr  / FZ_WEG_KORREKTUR, 
			m_pFzdat->mp, 
			m_pFzdat->nmp);

	MSGAVISO3 ("InterpretarTeleWeg ()->agv: %d wegstr: %d %s",
			m_id,
			wegstr,
			msg);

	m_pFzdat->wegstr = wegstr * FZ_WEG_KORREKTUR;

	// See if agv has advanced since last weg result
	if (m_lastWegTeleValue == wegstr)
		m_lastWegWasSame = true;

	m_lastWegTeleValue = wegstr;

	return (false);	// don't update BuB for the moment

}

bool CAgv::InterpretarTeleKleineZustand(typ_tele_kz &tele)
{
	// M2015 TODO - if this telegram is used, update status at end of function, which
	// is currently commented out
	MSGAVISO ("TODO - InterpretarTeleKleineZustand is not updated for 2015!!");
	bool fUpdateEstado = false;

	// Convert mp ascii to int
	UINT mp =  GetAtoi ((LPCTSTR)tele.amp, sizeof(tele.amp));
	UCHAR mpPos = tele.amp_pos & 0x0F;    // Fz.-Position

	MSGAVISO3 ("InterpretarTeleKleineZustand ()->agv: %d mp: %d, mppos: %d",
				m_id, mp, mpPos);

	if (AGV_FUERASISTEMA)
	{
		InsertarAGV (mp, mpPos);
		SetEstado (AGVSINDATOS);

	}
	else
	{
		UINT lastMp= m_pFzdat->mp;


		m_pFzdat->mp    = mp;
		m_pFzdat->mppos = mpPos;
		if (lastMp != m_pFzdat->mp)
		{
			m_pFzdat->lmp  = lastMp;
			if (m_pFzdat->fzziel.nDest == m_pFzdat->mp ||
				m_pFzdat->fzziel.nDest == 0)
			{
				// Esta en el destino
				m_pFzdat->nmp  = 0;
				m_pFzdat->uemp = 0;
			}
			else
			{
				m_pFzdat->nmp  = GetNmp (m_pFzdat->mp, m_pFzdat->fzziel.nDest);
				m_pFzdat->uemp = GetNmp (m_pFzdat->nmp,m_pFzdat->fzziel.nDest);
			}
		}
		
		QuizasInsertarBloqueo(tele.status_0); //Xavi, 02/11/05
		// M2015 TODO m_pFzdat->fahr_byte_ist = tele.status_0;  // Statuswort 0
		// M2015 TODO m_pFzdat->akt_byte_ist  = tele.status_1;  // Statuswort 1
		// M2015 TODO m_pFzdat->last_byte_ist = tele.status_2;  // Statuswort 2
		unsigned short wegstr = (tele.wegstrecke[1] & 0x3f) | ((tele.wegstrecke[0] & 0x3f) << 6);
		m_pFzdat->wegstr = wegstr * FZ_WEG_KORREKTUR;
		m_fWillUpdateBuB = true;	// Avisar el BuB de los cambios
		fUpdateEstado = true;
	}
	return (fUpdateEstado);
}




// Interpretar telegrama grande de estado del Agv
bool CAgv::InterpretarTeleGrosseStatus(typ_tele_gs &tele)
{
	bool fUpdateEstado = true;	// Update only if there are changes
	bool fCallMpFolge = false;	// Flag si ha pasado algo raro...
	short lastWegStr = 0;
	unsigned char mappedStatus [5];

	UINT mp     = GetAtoi ((LPCTSTR)tele.amp, sizeof(tele.amp));
	UCHAR mpPos = tele.amp_pos & 0x0F;


	USHORT agvLastNmp = m_pFzdat->nmp;
	
	// Michael MODS 2009 Moved from below to trace

    unsigned short wegstr = (tele.wegstrecke[1] & 0x3f) | ((tele.wegstrecke[0] & 0x3f) << 6);
	lastWegStr = m_pFzdat->wegstr;

    m_pFzdat->wegstr = wegstr * FZ_WEG_KORREKTUR;

	MSGTRACE4 ("InterpretarTeleGrosseStatus()->agv: %d mp: %d, mppos: %d, wegstr: %d",
				m_id, mp, mpPos, m_pFzdat->wegstr );

	MSGTRACE3 ("InterpretarGrosseStatus()->agv: %d fzziel.nDest: %d estado: %s",
		m_id,
		GetAtoi((LPCTSTR)tele.fzziel, sizeof(tele.fzziel)),
		GetStringEstado ());

	if (mp <= 0)
	{
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (2, 12, 1, m_id);
		return (false);
	}


	// M2015 if ( ! FZ_IM_SYSTEM (* m_pFzdat))
	if (! TEST_FZ_IM_SYSTEM(*m_pFzdat))
	{
		InsertarAGV (mp, mpPos);
		SetEstado (AGVSINDATOS);
		// Michael 28.02.2002 para que no intente actualizar el estado así
		fUpdateEstado = false;
	}
	else
	{
		UINT lastMp = m_pFzdat->mp;

        if (lastMp != mp)
        {
			// Michael 17.11.2010 Reset
			m_lastWegTeleValue = 0;
			m_lastWegWasSame = false;
			// Michael 17.11.2010 Reset fin

			// TODO MICHAEL MODS 2009
			m_fWillUpdateBuB = true;
			m_pFzdat->lmp = lastMp;
			// Michael 30.10.2001 Si el MP no es esperado, recalcular
			if (mp != agvLastNmp)
			{
				MSGWARNING3 ("Agv: %d PuntoCom esperado: %d PuntoCom Recibido %d",
					m_id,
					agvLastNmp,
					mp);
				fCallMpFolge = true;
			}

        }
        m_pFzdat->mp    = mp;

		if (m_pFzdat->mppos != mpPos)
			m_fWillUpdateBuB = true;

		// XXX-Lac04: Save destination number
		unsigned short nFzzielOld = m_pFzdat->fzziel.nDest;

        m_pFzdat->mppos = mpPos;
        m_pFzdat->nmp = GetAtoi((LPCTSTR)tele.nmp, sizeof(tele.nmp));
        m_pFzdat->fzziel.nDest = GetAtoi((LPCTSTR)tele.fzziel, sizeof(tele.fzziel));
        if (m_pFzdat->nmp == 0)
        {
			m_pFzdat->uemp = 0;
			if (g_Container.GetSystemState ()  == ANLAUF) // Starting up
			{
				// damit Strecke in Blk-Tabelle belegt wird XXX ???
				// Get an NMP anyway ???
				//m_pFzdat->nmp  = GetNmp (m_pFzdat->mp,statdat[1].mp);
				m_pFzdat->nmp = GetNmp(m_pFzdat->mp, pAGVTCourse->GetPtrDestinationByIndex(0)->GetNumber());
			}
        }
        else
        {
			UINT tMp = GetNmp (m_pFzdat->nmp, m_pFzdat->fzziel.nDest);
			// Calculate with next communications point plus 1
            // Fz. liefert uebernaechsten MP
            m_pFzdat->uemp = GetAtoi ((LPCTSTR)tele.uemp, sizeof(tele.uemp));
            if (m_pFzdat->uemp != tMp)
            {
				MSGWARNING3 ("Mp: %d uemp Agv: %d uemp tpc: %d",
					m_pFzdat->uemp, m_pFzdat->uemp, tMp);
				// Unterschiedliche UEMP

				;
				/* XXX
				sys.mod_nr     = mod_zdv;
				sys.meld_nr    = 12;
				sys.var_anz    = 4;
				sys.kommt_geht = 0;
				sys.var[0]     = fznr;
				sys.var[1]     = m_pFzdat->mp;
				sys.var[2]     = m_pFzdat->uemp;
				sys.var[3]     = uTMp;
				syslog (&sys);
				*/

			}
        }
        m_pFzdat->fzziel.nPos   = tele.fzpos   & 0x0F;	// Destination position
        m_pFzdat->fzziel.nLayer = tele.fzebene & 0x0F;	// Destination level
        // Left / Right of destination
        if (tele.fzlage == '0')
        {
			m_pFzdat->fzziel.ucSide = tele.fzlage & 0x0F;
        }
        else
        {
			m_pFzdat->fzziel.ucSide = tele.fzlage;
        }

		QuizasInsertarBloqueo(tele.status_0);

		MapStatusBytes (tele.status_0, tele.status_1, tele.status_2, 0, 0, mappedStatus);

		if (m_pFzdat->Status0 != mappedStatus[0])
		{
			QuizasInsertarBloqueo(mappedStatus[0]);
			m_pFzdat->Status0 = mappedStatus[0];  // Statuswort 0
			m_fWillUpdateBuB = true;
		}
		if (m_pFzdat->Status1  != mappedStatus[1])
		{
			m_pFzdat->Status1  = mappedStatus[1];  // Statuswort 1
			m_fWillUpdateBuB = true;
		}
		if (m_pFzdat->Status2 != mappedStatus[2])
		{
			m_pFzdat->Status2 = mappedStatus[2];  // Statuswort 2
			m_fWillUpdateBuB = true;
		}

		if (m_pFzdat->Status3 != mappedStatus[3])
		{
			m_pFzdat->Status3 = mappedStatus[3];  // Statuswort 2
			m_fWillUpdateBuB = true;
		}
		if (m_pFzdat->Status4 != mappedStatus[4])
		{
			m_pFzdat->Status4 = mappedStatus[4];  // Statuswort 2
			m_fWillUpdateBuB = true;
		}



		QuizasInsertarBloqueo(tele.status_0);

		 

        // Batteriekapazitaet in % + Batterieoffset
		// TODO Michael : New Battery is array
        m_pFzdat->batterie = (tele.batterie & 0x3F) + BATT_OFFSET;
		m_pFzdat->uSpannung = 0;	// Don't get voltage for old Agvs.



		// XXX-Lac04: Start
		if(nFzzielOld != m_pFzdat->fzziel.nDest || fCallMpFolge)
		{
			if(m_pFzdat->fzziel.nDest != 0)
			{
				USHORT nFz = m_pFzdat->fznr;
				GetMpFolge(m_pFzdat,pMpFolgeVerz[nFz-1],m_pFzdat->fzziel.nDest);
				*(pCrMengeVerz[nFz-1]) = 0;
			}
		}
		// XXX-Lac04: End

		// Michael 28.02.2002 Antes estaba fuera del bucle de FZ_IM_SYSTEM
		if (!m_fHasBeenPolled)
		{
			// Puede ser que esté ya circulando
			if (! IsCargadorCurPos (true))
				SetEnCircuito ();
			g_pGestor->GetAgvConOrden (m_id);
			m_knot = GetKnotCurPos ();	// Ocupar el Knot si está dentro
			m_fHasBeenPolled = true;
		}



	}

	if (!FZ_HAT_FEHLER(*m_pFzdat) && m_numError > 0)
	{
		for (UINT i = 0; i < m_numError; i++)
		{
			CAlarma * ptrAlarma = new CAlarma (true);	// Indicar final de alarma
			ptrAlarma->AvisarAlarma (
				m_pFzdat->errofz[i].modul_nr,
				m_pFzdat->errofz[i].fehler_nr, 
				4,
				m_id,
				m_pFzdat->mp,
				m_pFzdat->nmp,
				m_pFzdat->wegstr);
			g_Container.DeleteAgvError ();
			// Michael 25.01.2002
			CDBEstadistica::LogError (this, i, true);
		}
		// Michael 24.01.2002: Eliminar los errores de la lista del Agv
		for (int i = 0; i < 5; i++)
		{
			m_pFzdat->errofz [i].modul_nr = 0;
			m_pFzdat->errofz [i].fehler_nr = 0;
		}

		m_numError = 0;
	}

	// Michael Mods 2009. Show gálibo error if exists
	if (m_numError > 0)
	{
		for (UINT i = 0; i < m_numError; i++)
		{
			ShowGaliboErrorAtP3 (m_pFzdat->errofz[i].modul_nr, 
				m_pFzdat->errofz[i].fehler_nr,
				m_pFzdat->mp);

		}

	}


	return (fUpdateEstado);

}


unsigned short CAgv::GetWegStr (LPCTSTR pWegstr) 
{
	// Michael 10.04.2015 Compared: Same as OS800 example, so taken as good
	unsigned short wegstr = (*(pWegstr+1) & 0x3f) | ((*pWegstr & 0x3f) << 6);
	wegstr = wegstr * FZ_WEG_KORREKTUR;
	return wegstr;
}



bool CAgv::InterpretarTeleGrosseStatus(typ_tele_gs_wlan &tele)
{
	
	bool fUpdateEstado = true;	// Update only if there are changes
	bool fCallMpFolge = false;	// Flag si ha pasado algo raro...
	short lastWegStr = 0;

	UINT mp     = GetAtoi ((LPCTSTR)tele.amp, sizeof(tele.amp));
	UINT mpPos = GetAtoi ((LPCTSTR)tele.amp_pos, sizeof(tele.amp_pos));


	USHORT agvLastNmp = m_pFzdat->nmp;

	lastWegStr = m_pFzdat->wegstr;


	m_pFzdat->wegstr = GetWegStr((LPCTSTR)tele.wegstrecke);
	
	m_lastWegWasSame = (m_pFzdat->wegstr == lastWegStr);


	MSGTRACE4 ("InterpretarTeleGrosseStatus wlan()->agv:%d mp: %d, mppos: %d, wegstr: %d",
				m_id, mp, mpPos, m_pFzdat->wegstr );

	MSGTRACE3 ("InterpretarTeleGrosseStatus wlan()->agv:%d fzziel.nDest: %d estado: %s",
		m_id,
		GetAtoi((LPCTSTR)tele.fzziel, sizeof(tele.fzziel)),
		GetStringEstado ());

	if (mp <= 0)
	{
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (2, 12, 1, m_id);
		return (false);
	}


	// M2015 if ( ! FZ_IM_SYSTEM (* m_pFzdat))
	if (! TEST_FZ_IM_SYSTEM(*m_pFzdat))
	{
		InsertarAGV (mp, mpPos);
		SetEstado (AGVSINDATOS);
		// Michael 28.02.2002 para que no intente actualizar el estado así
		fUpdateEstado = false;
	}
	else
	{
		UINT lastMp = m_pFzdat->mp;

        if (lastMp != mp)
        {
			// Michael 17.11.2010 Reset
			m_lastWegTeleValue = 0;
			m_lastWegWasSame = false;
			// Michael 17.11.2010 Reset fin

			// TODO MICHAEL MODS 2009
			m_fWillUpdateBuB = true;
			m_pFzdat->lmp = lastMp;
			// Michael 30.10.2001 Si el MP no es esperado, recalcular
			if (mp != agvLastNmp)
			{
				MSGWARNING3 ("Agv: %d PuntoCom esperado: %d PuntoCom Recibido %d",
					m_id,
					agvLastNmp,
					mp);
				fCallMpFolge = true;
			}

        }
        m_pFzdat->mp    = mp;

		if (m_pFzdat->mppos != mpPos)
			m_fWillUpdateBuB = true;

		// XXX-Lac04: Save destination number
		unsigned short nFzzielOld = m_pFzdat->fzziel.nDest;

        m_pFzdat->mppos = mpPos;
        m_pFzdat->nmp = GetAtoi((LPCTSTR)tele.nmp, sizeof(tele.nmp));
        m_pFzdat->fzziel.nDest = GetAtoi((LPCTSTR)tele.fzziel, sizeof(tele.fzziel));
        if (m_pFzdat->nmp == 0)
        {
			m_pFzdat->uemp = 0;
			if (g_Container.GetSystemState ()  == ANLAUF) // Starting up
			{
				// damit Strecke in Blk-Tabelle belegt wird XXX ???
				// Get an NMP anyway ???
				//m_pFzdat->nmp  = GetNmp (m_pFzdat->mp,statdat[1].mp);
				m_pFzdat->nmp = GetNmp(m_pFzdat->mp, pAGVTCourse->GetPtrDestinationByIndex(0)->GetNumber());
			}
        }
        else
        {
			UINT tMp = GetNmp (m_pFzdat->nmp, m_pFzdat->fzziel.nDest);
			// Calculate with next communications point plus 1
            // Fz. liefert uebernaechsten MP
            m_pFzdat->uemp = GetAtoi ((LPCTSTR)tele.uemp, sizeof(tele.uemp));
            if (m_pFzdat->uemp != tMp)
            {
				MSGWARNING3 ("Mp: %d uemp Agv: %d uemp tpc: %d",
					m_pFzdat->uemp, m_pFzdat->uemp, tMp);
				// Unterschiedliche UEMP

				;
				/* XXX
				sys.mod_nr     = mod_zdv;
				sys.meld_nr    = 12;
				sys.var_anz    = 4;
				sys.kommt_geht = 0;
				sys.var[0]     = fznr;
				sys.var[1]     = m_pFzdat->mp;
				sys.var[2]     = m_pFzdat->uemp;
				sys.var[3]     = uTMp;
				syslog (&sys);
				*/

			}
        }
		
        m_pFzdat->fzziel.nPos   = GetAtoi ((LPCTSTR)tele.fzpos, sizeof(tele.fzpos)); // Destination position
        m_pFzdat->fzziel.nLayer = tele.fzebene & 0x0F;	// Destination level M2015 TODO Why 0x0F??
        // Left / Right of destination
        if (tele.fzlage == '0')
        {
			m_pFzdat->fzziel.ucSide = tele.fzlage & 0x0F;
        }
        else
        {
			m_pFzdat->fzziel.ucSide = tele.fzlage;
        }


		// XXX-Lac04: Start
		if(nFzzielOld != m_pFzdat->fzziel.nDest || fCallMpFolge)
		{
			if(m_pFzdat->fzziel.nDest != 0)
			{
				USHORT nFz = m_pFzdat->fznr;
				GetMpFolge(m_pFzdat,pMpFolgeVerz[nFz-1],m_pFzdat->fzziel.nDest);
				*(pCrMengeVerz[nFz-1]) = 0;
			}
		}
		// XXX-Lac04: End




	}
		QuizasInsertarBloqueo(tele.status_0);

		m_pFzdat->Status0 = tele.status_0;
		m_pFzdat->Status1 = tele.status_1;
		m_pFzdat->Status2 = tele.status_2;
		m_pFzdat->Status3 = tele.status_3;
		m_pFzdat->Status4 = tele.status_4;

		

        // Batteriekapazitaet in % + Batterieoffset

		m_pFzdat->batterie = GetAtoi ((LPCTSTR)tele.batterie, sizeof(tele.batterie));
		m_pFzdat->uSpannung= GetAtoi((LPCTSTR)tele.spannung, sizeof(tele.spannung));


	// Michael 28.02.2002 Antes estaba fuera del bucle de FZ_IM_SYSTEM
	if (!m_fHasBeenPolled)
	{
		// Puede ser que esté ya circulando
		if (! IsCargadorCurPos (true))
			SetEnCircuito ();
		g_pGestor->GetAgvConOrden (m_id);
		m_knot = GetKnotCurPos ();	// Ocupar el Knot si está dentro
		m_fHasBeenPolled = true;
	}


	if (!FZ_HAT_FEHLER(*m_pFzdat) && m_numError > 0)
	{
		for (UINT i = 0; i < m_numError; i++)
		{
			CAlarma * ptrAlarma = new CAlarma (true);	// Indicar final de alarma
			ptrAlarma->AvisarAlarma (
				m_pFzdat->errofz[i].modul_nr,
				m_pFzdat->errofz[i].fehler_nr, 
				4,
				m_id,
				m_pFzdat->mp,
				m_pFzdat->nmp,
				m_pFzdat->wegstr);
			g_Container.DeleteAgvError ();
			// Michael 25.01.2002
			CDBEstadistica::LogError (this, i, true);
		}
		// Michael 24.01.2002: Eliminar los errores de la lista del Agv
		for (int i = 0; i < 5; i++)
		{
			m_pFzdat->errofz [i].modul_nr = 0;
			m_pFzdat->errofz [i].fehler_nr = 0;
		}

		m_numError = 0;
	}

	// Michael Mods 2009. Show gálibo error if exists
	if (m_numError > 0)
	{
		for (UINT i = 0; i < m_numError; i++)
		{
			ShowGaliboErrorAtP3 (m_pFzdat->errofz[i].modul_nr, 
				m_pFzdat->errofz[i].fehler_nr,
				m_pFzdat->mp);

		}

	}
	return (fUpdateEstado);

}
// Increment number of errors that AGV may have
// Note: If number exceeded, the next error in will overwrite the last
void CAgv::IncrementErrorNumber() {
	if (m_numError >= 4)
	{
		MSGERROR1 ("Número Errores excedido para Agv: %d", m_id);
	}
	else 
		m_numError++;
}

void CAgv::InterpretarTeleErrorWlan(typ_tele_error_wlan &tele)
{
	unsigned short fehler_nr = GetAtoi ((LPCTSTR)tele.fehler_nr, sizeof(tele.fehler_nr));
	unsigned short modul_nr = FZ_MOD_OFFSET + GetAtoi ((LPCTSTR)tele.modul_nr, sizeof(tele.modul_nr));

	MSGAVISO3 ("InterpretarTeleErrorWlan()->agv: %d fehler_nr: %d , modul_nr: %d", m_id, fehler_nr, modul_nr);

	// M2015: TODO Seems that Wlan Agv sends the same error repeatedly: Look at dealing with this situation

	m_pFzdat->errofz[m_numError].fehler_nr = fehler_nr;
	m_pFzdat->errofz[m_numError].modul_nr = modul_nr;

	m_pFzdat->errofz[m_numError].anParam [0] =  GetAtoi ((LPCTSTR)tele.uParameter [0], sizeof(tele.uParameter [0]));
	m_pFzdat->errofz[m_numError].anParam [1] =  GetAtoi ((LPCTSTR)tele.uParameter [1], sizeof(tele.uParameter [1]));


	// Puede ser que ya tuviera errores... (m_numError)

	CAlarma * ptrAlarma = new CAlarma;
	ptrAlarma->AvisarAlarma (
		m_pFzdat->errofz[m_numError].modul_nr,
		m_pFzdat->errofz[m_numError].fehler_nr, 
		4,
		m_id,
		m_pFzdat->mp,
		m_pFzdat->nmp,
		m_pFzdat->wegstr);
	
	g_Container.AddAgvError ();

	CDBEstadistica::LogError (this, m_numError, false);

	// Michael MODS 2009 - If gálibo error at station P3 (mp 9) then show error to TPC Log
	ShowGaliboErrorAtP3 (m_pFzdat->errofz[m_numError].modul_nr, 
		m_pFzdat->errofz[m_numError].fehler_nr,
		m_pFzdat->mp);


	// Michael 30.09.2001: Check for error in station

	CheckStationError (m_pFzdat->errofz[m_numError].modul_nr, m_pFzdat->errofz[m_numError].fehler_nr);
	
	IncrementErrorNumber();

	SET_FZ_FEHLER(*m_pFzdat);

	// M2015: Error telegram has positional information - update
	UINT mp     = GetAtoi ((LPCTSTR)tele.amp, sizeof(tele.amp));
	UINT mpPos = GetAtoi ((LPCTSTR)tele.amp_pos, sizeof(tele.amp_pos));
	UINT nmp = GetAtoi ((LPCTSTR)tele.nmp, sizeof(tele.nmp));
	USHORT agvLastNmp = m_pFzdat->nmp;

	m_pFzdat->wegstr = GetWegStr((LPCTSTR)tele.wegstrecke);

	MSGTRACE4 ("InterpretarTeleErrorWlan()->agv: %d fz_dat.mp: %d, fz_dat.mppos: %d, fz_dat.wegstr: %d",
		m_id, m_pFzdat->mp, m_pFzdat->mppos, m_pFzdat->wegstr );
	MSGTRACE4 ("InterpretarTeleErrorWlan()->agv: %d mp: %d, mppos: %d, wegstr: %d",
				m_id, mp, mpPos, m_pFzdat->wegstr );


	if (mp <= 0)
	{
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (2, 12, 1, m_id);
		return;
	}

	if (! TEST_FZ_IM_SYSTEM(*m_pFzdat))
	{
		InsertarAGV (mp, mpPos);
		SetEstado (AGVSINDATOS);
	}
	else
	{
		UINT lastMp = m_pFzdat->mp;

        if (lastMp != mp)
        {
			// Michael 17.11.2010 Reset
			m_lastWegTeleValue = 0;
			m_lastWegWasSame = false;
			// Michael 17.11.2010 Reset fin
			m_fWillUpdateBuB = true;
			m_pFzdat->lmp = lastMp;
			// Michael 30.10.2001 Si el MP no es esperado, recalcular
			if (mp != agvLastNmp)
			{
				MSGWARNING3 ("InterpretarTeleErrorWlan(): Agv: %d PuntoCom esperado: %d PuntoCom Recibido %d",
					m_id,
					agvLastNmp,
					mp);
			}
			// M2015 TODO - should be recalculating traffic control if the AGV has changed position.
			//m_pFzdat->mp    = mp;
			//if (m_pFzdat->mppos != mpPos)
			//	m_pFzdat->mppos = mpPos;
			//if (m_pFzdat->nmp != nmp)
			//	m_pFzdat->nmp = nmp;
			MSGWARNING3 ("InterpretarTeleErrorWlan(): Agv: %d PuntoCom esperado: %d PuntoCom Recibido %d NOT UPDATING AGV POS",
					m_id,
					agvLastNmp,
					mp);



		}






	m_fWillUpdateBuB = true;	// Avisar el BuB
	m_fSendQuitum = true;	// Enviar Quitum al BuB



	}
}



bool CAgv::InterpretarTeleError(typ_tele_error &tele)
{
	MSGAVISO1 ("InterpretarTeleError()->agv: %d", m_id);

    // Michael 09.07.2002 memset (m_pFzdat->errofz, 0, sizeof(m_pFzdat->errofz));

	size_t teleLen = m_Tel.m_strRespuesta.GetLength ();

	// Michael 24.01.2002 Puede ser que ya tuviera errores...

    for (int i = m_numError; i < 5; i++)
	{
        if ((teleLen - 1) > 6 +  i * sizeof(tele.error[0]))
        {
			IncrementErrorNumber();
			// sys.meld_nr =
			m_pFzdat->errofz[i].fehler_nr = GetAtoi ((LPCTSTR)tele.error[i].fehler_nr,
				sizeof(tele.error[0].fehler_nr));
			// sys.mod_nr =
			m_pFzdat->errofz[i].modul_nr = FZ_MOD_OFFSET + GetAtoi ((LPCTSTR)tele.error[i].modul_nr,
				sizeof(tele.error[0].modul_nr));

			CAlarma * ptrAlarma = new CAlarma;
			ptrAlarma->AvisarAlarma (
				m_pFzdat->errofz[i].modul_nr,
				m_pFzdat->errofz[i].fehler_nr, 
				4,
				m_id,
				m_pFzdat->mp,
				m_pFzdat->nmp,
				m_pFzdat->wegstr);
			
			g_Container.AddAgvError ();

			// Michael 25.01.2002
			CDBEstadistica::LogError (this, i, false);


			MSGERROR3 ("InterpretarTeleError Agv: %d module: %d error: %d",
				m_id,
				m_pFzdat->errofz[i].fehler_nr,
				m_pFzdat->errofz[i].modul_nr);

			// Michael MODS 2009 - If gálibo error at station P3 (mp 9) then show error to TPC Log
			ShowGaliboErrorAtP3 (m_pFzdat->errofz[i].modul_nr, 
				m_pFzdat->errofz[i].fehler_nr,
				m_pFzdat->mp);


			// Michael 30.09.2001: Check for error in station

			CheckStationError (m_pFzdat->errofz[i].modul_nr, m_pFzdat->errofz[i].fehler_nr);

        }
	}

	SET_FZ_FEHLER(*m_pFzdat);
	m_fWillUpdateBuB = true;	// Avisar el BuB
	m_fSendQuitum = true;	// Enviar Quitum al BuB
	return (false);	// Michael: No actualizamos el estado si el Agv tiene error
}

bool CAgv::InterpretarTeleManualOrder(typ_tele_at &tele)
{

	MSGAVISO1 ("InterpretarTeleManualOrder ()-> Agv: %d", m_id);


	/* XXX Look at treatment
	TYP_CHECKDAT at_neu;

	  memset (&at_neu,0,sizeof(at_neu));
	  at_neu.herkunft = 'S';// manueller Sonderauftrag
	  at_neu.prio_org = 1;
	  at_neu.fznr     = fznr;
	  at_neu.az.nr    = asciibin((char *)tele_at->az, sizeof(tele_at->az));
	  at_neu.az.pos   = (tele_at->az_pos   & 0x0F);
	  at_neu.az.ebene = (tele_at->az_ebene & 0x0F);
	  at_neu.az.lage  = tele_at->az_lage;
	  at_neu.akt_org  = (tele_at->aktion & 0x0F);
	  // pruefen, ob Auftrag formell und (soweit m”glich) inhaltlich OK, -> Funktion mit Return-Status
	  sys.mod_nr = mod_zdv;
	  sys.meld_nr = 15;
	  sys.var_anz = 2;
	  sys.var[0] = fznr;
	  sys.var[1] = at_neu.az.nr;
	  auf_check ('MA', &at_neu.herkunft);
	  syslog (&sys);
	*/

	#pragma message("What to do with a manual order from BuB ?")

	return (true);	// XXX Ver treatment
}

// Actualizar el estado del Agv
void CAgv::SetEstado (enum TEstadoAGV estado)
{
	switch (estado)
	{
	case AGVFUERASISTEMA:
		// XXX See does it have orders associated etc.
		// XXX-Lac01:
		m_pFzdat->lmp    = 0;
		m_pFzdat->mp 	 = 0;
		m_pFzdat->nmp	 = 0;
		m_pFzdat->uemp   = 0;

		// M2015 TODO What is mzdv0 used for ? m_pFzdat->mzdv0 &= OFFBIT1;
		SetEstadoActual(estado);
		m_fWillUpdateBuB = true;

		RESET_FZ_IN_SYSTEM(*m_pFzdat);

		break;
	case AGVSINDATOS:
		switch (m_estado)
		{
		case AGVFUERASISTEMA:
		case AGVPERDIDO:
			SET_FZ_IN_SYSTEM (*m_pFzdat);
			// Michael 14.06.2005
			m_nMantenimiento = 0;	// Por si hubiera estado en mantenimiento
		default:
			//m_estado = estado;//Xavi, 02/11/05
			SetEstadoActual(estado);
			m_fWillUpdateBuB = true;
			// Michael 17.02.2002
			SetCountEsperar ();
			m_cOrdenesOk = 0;	// Para saber si se han mirado de lanzar órdenes
			g_GestorOrden.m_fMirarTodos = true; // Para que se miren todas las órdenes

		}
		break;
	// Don't set this state !!!
	case AGVPERDIDO:
		switch (m_estado)
		{
		case AGVSINDATOS:
			//m_estado = estado;//Xavi, 02/11/05
			SetEstadoActual(estado);
			MSGWARNING1 ("Agv: %d no responde - marcando perdido", m_id);
			m_fWillUpdateBuB = true;
			break;
		default:
			SET_FZ_VERMISST (*m_pFzdat);
			m_fWillUpdateBuB = true;
			break;

		}
		break;
	case CARGARBAT:
		//m_estado = estado;//Xavi, 02/11/05
		SetEstadoActual(estado);
		m_nCargador = m_pFzdat->mp;	// Actualizar donde esté
		m_fWillUpdateBuB = true;
		break;
	case CARGARORD:
		//m_estado = estado;//Xavi, 02/11/05
		SetEstadoActual(estado);
		m_fWillUpdateBuB = true;
		break;
	case IRDESCARGARORD:
		//m_estado = estado;//Xavi, 02/11/05
		SetEstadoActual(estado);
		m_fWillUpdateBuB = true;
		break;
	case MANTENIMIENTO:
		//m_estado = estado;//Xavi, 02/11/05
		SetEstadoActual(estado);
		m_fWillUpdateBuB = true;
		break;
	case CARGARBATWAITPLC:
		//m_estado = estado;//Xavi, 02/11/05
		SetEstadoActual(estado);
		m_fWillUpdateBuB = true;
		break;

	}
}

// Ver si es una orden manual (de Agv) y ya está cargada, o sea finalizada
bool CAgv::IsOrdenAgvCargado()
{
	// Orden Manual y de carga
	return ('S' == m_pOrden->m_at.herkunft && 1 == m_pOrden->m_at.akt_org);
}

/*
// Get Nmp pointer from supplied nmp number
NMP_tabelle* CAgv::GetPNmpFromNmp(UINT mp, UINT nmp)
{
	Meldepunkttabelle  * ptrMp;
	NMP_tabelle * ptrNmp;

	ptrMp  = mptab + (vmptab [mp] - 1); // Get mp data

	ptrNmp = MpNmpTab + ptrMp->str_idx - 1; // First in list

	// Get the requested nmp_tabelle  from the start point
	for (unsigned char i = 0; i < ptrMp->anz_nmp && nmp != ptrNmp->nmp;
			      			ptrNmp++, i++);
	return (ptrNmp);
}
*/

// Get Nmp pointer from supplied nmp number
CCpDistance* CAgv::GetPNmpFromNmp(UINT mp, UINT nmp)
{
	return pAGVTCourse->GetDistPointer(mp, nmp);
}

/* Enviar el Agv a un punto de descanso, o al siguiente punto de descanso
    Devuelve <true> si se ha podido enviar el Agv a un punto
	Devuelve <false> si el destino del Agv no es modificable, o no se ha 
	podido comunicar el punto al Agv */
bool CAgv::EnviarPuntoEspera (UINT curMp)
{
	typ_tele_info teleinfo;
	CString str;
	bool fRes;

	if (IsWlan())
	{
		return (WlanEnviarPuntoEspera(curMp));
	}

	if (IsModifiableDestino ())
	{
		teleinfo.tele_art = 'I';

		str.Format ("%02d", m_id);
		strncpy ((char *)teleinfo.fznr, str, 2);

		if (g_pGestor->GetAgvsEnCircuito () > g_Container.GetTotalAgvsAllowed () 
			|| !IsEnCircuito ())
			// Quitar del circuito
			GetPuntoParking (&m_pFzdat->auftziel, curMp);
		else
			GetPuntoDescanso (&m_pFzdat->auftziel, curMp);
		if (m_pFzdat->mp != m_pFzdat->auftziel.nDest)
		{

			str.Format ("%04d", m_pFzdat->auftziel.nDest);
			strncpy ((char *)teleinfo.auftziel, str, 4);

			teleinfo.auftpos = m_pFzdat->auftziel.nPos + 0x30; // Int to Char
			teleinfo.auftebene = m_pFzdat->auftziel.nLayer + 0x30; // Int to Char
			teleinfo.auftlage = m_pFzdat->auftziel.ucSide;

			teleinfo.kommando_0 = 0;
			teleinfo.kommando_0 |= BIT7;	// Always 1

			// Enviar bloqueado siempre 12.10.2001
			SetBlocked (true);
			SetTeleBlocked (&teleinfo.kommando_0, true);

			teleinfo.kommando_0 |= BIT3;	// Action stopped (Don't load)

			teleinfo.kommando_1 = BIT7;		// Always 1


			MSGAVISO2 ("Enviando Agv: %d a punto de descanso %d",
				m_id,
				m_pFzdat->auftziel.nDest);

			if (!g_Container.m_fSimulateCom)
				fRes = SendInfoTelegram (teleinfo);
			else
				fRes = SimulateInfoTelegram (teleinfo);
		}
		else
			// Es el mismo punto
			fRes = true;
		return (fRes);
	}
	else
		return (false);	// el destino no se puede modificar
}

/* Enviar el Agv a un punto de descanso, o al siguiente punto de descanso
    Devuelve <true> si se ha podido enviar el Agv a un punto
	Devuelve <false> si el destino del Agv no es modificable, o no se ha 
	podido comunicar el punto al Agv */
bool CAgv::WlanEnviarPuntoEspera (UINT curMp)
{
	typ_tele_info_wlan teleinfo;
	CString str;
	bool fRes = true;

	if (IsModifiableDestino ())
	{
		teleinfo.tele_art = 'I';

		str.Format ("%02d", m_id);
		strncpy ((char *)teleinfo.fznr, str, 2);

		if (g_pGestor->GetAgvsEnCircuito () > g_Container.GetTotalAgvsAllowed () 
			|| !IsEnCircuito ())
			// Quitar del circuito
			GetPuntoParking (&m_pFzdat->auftziel, curMp);
		 else
			GetPuntoDescanso (&m_pFzdat->auftziel, curMp);
	
		if (m_pFzdat->mp != m_pFzdat->auftziel.nDest)
		{

			str.Format ("%04d", m_pFzdat->auftziel.nDest);
			strncpy ((char *)teleinfo.auftziel, str, 4);

			sprintf((char *)teleinfo.auftpos, "%02d",m_pFzdat->auftziel.nPos);
			teleinfo.auftebene = m_pFzdat->auftziel.nLayer + 0x30; // Int to Char
			teleinfo.auftlage = m_pFzdat->auftziel.ucSide;

			str.Format ("%04d", m_pFzdat->nmp);
			strncpy ((char *)teleinfo.nmp, str, 4);
			str.Format ("%04d", m_pFzdat->uemp);
			strncpy ((char *)teleinfo.uemp, str, 4);

			strncpy ((char *)teleinfo.EntryPoint, str, 4);

			teleinfo.kommando_0 = 0;
			teleinfo.kommando_0 |= BIT7;	// Always 1

			// Enviar bloqueado siempre 12.10.2001
			// Michael 25.05.2015 don't interfere just incase SetBlocked (true);
			SetTeleBlockedWlan (&teleinfo.kommando_0, true);

			// teleinfo.kommando_0 |= BIT3;	// Action stopped (Don't load)

			teleinfo.kommando_1 = BIT7;		// Always 1

			// TODO What else for commando 2?
			teleinfo.kommando_2 = BIT7;


			MSGAVISO2 ("Enviando Agv: %d a punto  %d",
				m_id,
				m_pFzdat->auftziel.nDest);

			if (!g_Container.m_fSimulateCom)
				m_WlanTel.SetInfoTelegram (teleinfo, m_pWlanMsg);
			else
				; // M2015 TODO fRes = SimulateInfoTelegram (teleinfo);
		}
		else
			// Es el mismo punto
			fRes = true;
		return (fRes);
	}
	else
		return (false);	// el destino no se puede modificar
}


// Buscar un punto de espera para el Agv
void CAgv::GetPuntoDescanso (TYPE_STATION_DEF *ptrZiel, UINT curMp)
{
	static int rangoUrgencia [4] = { 8, 3, 0 };
	ptrZiel->nDest = 0;

	CPuntoDescanso * ptrPuntoElegido = NULL;
	UINT timeToPunto = 0, previousTimeToPunto = 999999;

	CSingleLock lockPuntos (&g_Container.m_csPuntoDescanso);
	lockPuntos.Lock ();

	UINT numPuntos = g_Container.m_PuntoDescansoArray.GetSize ();
	// Loop sobre urgencia del tratamiento del punto de descanso
	for (UINT inxUrg = 0, numAgvsEnPunto = 0; inxUrg < 3 && !ptrPuntoElegido; inxUrg++)
	{
		for (UINT i = 0; i < numPuntos; i++)
		{
			CPuntoDescanso * ptrPuntoDescanso =
				g_Container.m_PuntoDescansoArray.GetAt (i);
			
			if (0 == inxUrg)
				// La primera vez hay que establecerlo
				numAgvsEnPunto = ptrPuntoDescanso->SetNumAgvs ();
			else
				numAgvsEnPunto = ptrPuntoDescanso->GetNumAgvs ();
			if (ptrPuntoDescanso->IsMoreUrgent (rangoUrgencia [inxUrg]))
			{
				
				if (numAgvsEnPunto < ptrPuntoDescanso->GetNumAgvsAllowed () && curMp != ptrPuntoDescanso->GetPtoCom ())
				{
					timeToPunto = GetTimeToPickup (ptrPuntoDescanso->GetPtoCom ());
					if (timeToPunto < previousTimeToPunto)
					{
						ptrPuntoElegido = ptrPuntoDescanso;
						previousTimeToPunto = timeToPunto;
					}
				}
			}
		}
	}

	if (ptrPuntoElegido)
		ptrPuntoElegido->SetZiel (ptrZiel);
	else
	{
		MSGERROR1 ("No encontado punto de espera para Agv: %d", m_id);
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (2, 16, 2, 
			m_id, 
			m_pFzdat->mp);

		// Michael 16.11.2001 Si está en un cargador, deja estar si no está empujado
		if (curMp == 0 && IsCargadorCurPos ())
		{
			SetZiel (ptrZiel, m_pFzdat->mp, 1, 'L');
		}
		else
		{
			/* Michael 30.07.2002 Quitar de momento
			if (g_Container.IsEnFuncionamiento () && g_pGestor->GetAgvsEnCircuitoIdle (m_id) > 2)
			{
				MSGAVISO1 ("Enviando Agv: %d a parking por sobrar", m_id);
				GetPuntoParking (&m_pFzdat->auftziel, m_pFzdat->mp, false);
			}
			else
			*/
			// Michael 30.07.2002 Enviar a P1 si no...
			if (curMp != 7)
				SetZiel (ptrZiel, 7, 2, 'R');
			else
				SetZiel (ptrZiel, 2, 2, 'R');
		}
	}
	lockPuntos.Unlock ();

}

// Ver si el Agv tiene siguiente punto de comunicación
bool CAgv::AgvConDestino()
{
	return (0 != m_pFzdat->fzziel.nDest &&
		m_pFzdat->fzziel.nDest != m_pFzdat->mp);

}

// Cancelar una orden
void CAgv::SetCancelled()
{
	m_pOrden->SetEstado (CANCELLED);
	m_pOrden = NULL;	// El Gestor de ordenes borrará
	SetZiel (&m_pFzdat->auftziel);	// Borrar destino
	strcpy ((char *)m_pFzdat->aufident, "");
}


// Comprobar que el destino del AGV es el mismo que la orden...
bool CAgv::DestinoDiferente(TEstadoAGV estado)
{
	CDestination * pDestOrden;



	if (m_pOrden && 'S' == m_pOrden->m_at.herkunft)
		// Orden Manual
		pDestOrden = new CDestination (&m_pOrden->m_at.az);
	else
	{
		switch (estado)
		{
		case IRCARGARORD:
		case CARGARORD:
			pDestOrden = new CDestination (&m_pOrden->m_at.hz);	// Pickup
			break;
		case IRDESCARGARORD:
		case DESCARGARORD:
			pDestOrden = new CDestination (&m_pOrden->m_at.bz);	// Deliver
			break;
		case IRPUNTOESP:
			// Michael M2015 Seems that an AGV is getting here with auftziel pointing to mp 1
			// Causing a weird order to be sent to the AGV
			pDestOrden = new CDestination (&m_pFzdat->auftziel);
			break;


		default:

			pDestOrden = new CDestination (&m_pFzdat->auftziel);	// All other cases
		}
	}

	// If it is already in station fzziel is 0 ! Mirar comando tambien
// Michael 15.01.2002	bool fRetVal = (((*pDestOrden == m_pFzdat->fzziel) || m_pFzdat->fzziel.nr == 0)
	bool fRetVal = ((*pDestOrden == m_pFzdat->fzziel) || 
		(m_pFzdat->fzziel.nDest == 0 && m_pFzdat->mp == pDestOrden->m_pDest->nDest));

	if (fRetVal == false)
	{
		MSGAVISO3 ("DestinoDiferente Agv: %d DestAgv: %d DestOrden: %d",
			m_id, m_pFzdat->fzziel.nDest, 
			pDestOrden->m_pDest->nDest);
		// M2015 Somehow finding destination of 1 here.
		if (pDestOrden->m_pDest->nDest == 1)
		{
			MSGAVISO3 ("DestinoDiferente Agv: %d DestAgv: %d DestOrden: %d PROBLEM AGV WLAN??",
				m_id, m_pFzdat->fzziel.nDest, 
				pDestOrden->m_pDest->nDest);
		}

		// M2015 Si el AGV tiene error, no comprobar destino diferente

		if (TEST_FZ_FEHLER(*m_pFzdat))
		{
			MSGERROR2 ("DestinoDiferente Agv: %d Estado: %s no tratado por AGV con error", m_id, GetStringEstado());
			return (!fRetVal);
		}

		// Michael 10.11.2004 Si no tiene destino, hay que darle uno
		// if (IsModifiableDestino ())
		if (IsModifiableDestino () || m_pFzdat->fzziel.nDest == 0)
		{
			// Solo cambiamos el destino si el blocking lo permite
			switch (estado)
			{
			case IRCARGARORD:
			case CARGARORD:
				PickupOrden ();
				break;
			case IRDESCARGARORD:
			case DESCARGARORD:
				SetUnloadDestination ();
				break;
			default:
				ModificarDestino (*(pDestOrden->m_pDest));	// Enviar el destino modificado al AGV
			}
		}
	}

	delete pDestOrden;

	return (! fRetVal);
}

// Modificar el destino del agv por haber cambiado en la orden
bool CAgv::ModificarDestino(TYPE_STATION_DEF& destino)
{
	bool fRes = false;
	if (!IsWlan())
	{

		typ_tele_info teleinfo;
		CString str;

		teleinfo.tele_art = 'I';

		str.Format ("%02d", m_id);
		strncpy ((char *)teleinfo.fznr, str, 2);

		SetZiel (&m_pFzdat->auftziel, destino.nDest,
			(unsigned char)destino.nPos,
				destino.ucSide,
				destino.nLayer);

		str.Format ("%04d", destino.nDest);
		strncpy ((char *)teleinfo.auftziel, str, 4);

		teleinfo.auftpos = destino.nPos + 0x30; // Int to Char
		teleinfo.auftebene = destino.nLayer + 0x30; // Int to Char
		teleinfo.auftlage = destino.ucSide;

		// Enviar bloqueado siempre 12.10.2001
		SetBlocked (true);
		SetTeleBlocked (&teleinfo.kommando_0, true);

		// Michael 12.11.2001 - Send what was previously sent
		// M2015 TODO Check that telegram needs mapped
		teleinfo.kommando_0 = MapCommandByte(m_pFzdat->Status0);
		teleinfo.kommando_1 = m_pFzdat->Status1;



		MSGAVISO3 ("ModificarDestino () Agv: %d destino %d pos %d",
					m_id,
					destino.nDest,
					destino.nPos);
		bool fRes;
	if (!g_Container.m_fSimulateCom)
		fRes = SendInfoTelegram (teleinfo);
	else
		fRes = SimulateInfoTelegram (teleinfo);

	}
	else	// Wlan
	{
		// WLan Order
		typ_tele_info_wlan teleinfo;
		CString str;

		teleinfo.tele_art = 'I';
		
		str.Format ("%02d", m_id);
		strncpy ((char *)teleinfo.fznr, str, 2);

		SetZiel (&m_pFzdat->auftziel, destino.nDest,
			(unsigned char)destino.nPos,
				destino.ucSide,
				destino.nLayer);

		str.Format ("%04d", destino.nDest);
		strncpy ((char *)teleinfo.auftziel, str, 4);

	

		sprintf((char *)teleinfo.auftpos, "%02d",destino.nPos);
		// Michael M2015 layer is always 1 for new AGVs
		// TODO - see can this be changed as seems to be 0 for old agvs m_pFzdat->auftziel.nLayer = 1;

		teleinfo.auftebene = destino.nLayer + 0x30; // Int to Char
		teleinfo.auftebene = 1 + 0x30; // Int to Char

		teleinfo.auftlage = destino.ucSide;

		str.Format ("%04d", m_pFzdat->nmp);
		strncpy ((char *)teleinfo.nmp, str, 4);
		str.Format ("%04d", m_pFzdat->uemp);
		strncpy ((char *)teleinfo.uemp, str, 4);
		
		
		strncpy ((char *)teleinfo.EntryPoint, "0000", 4);



		teleinfo.kommando_0 = 0;
		teleinfo.kommando_0 |= BIT7;	// Always 1

		// Michael 30.10.2001 But why always blocked ???
		// Michael 25.05.2015 not here: SetBlocked (true);
		SET_CTRL_STOP(*m_pFzdat);
		SET_AKTION_SPERR(*m_pFzdat);	// Action stopped (Don't load)

		teleinfo.kommando_1 = BIT7;		// Always 1

		teleinfo.kommando_2 = m_pFzdat->Status2;
		


		if (m_pOrden && m_pOrden->m_at.fSimCarga)	// M2015 viene de destino diferente sin orden
			teleinfo.kommando_1 |= BIT6;	// Simulate Pickup

	if (!g_Container.m_fSimulateCom)
	{
		m_WlanTel.SetInfoTelegram (teleinfo, m_pWlanMsg);
		fRes = true;
		
		MSGAVISO3 ("ModificarDestino () Agv: %d destino %d pos %d",
					m_id,
					destino.nDest,
					destino.nPos);

	}
// 	else
//		fRes = SimulateInfoTelegram (teleinfo);
	}	// Wlan



	
	return (fRes);
}


// Poner destino para la descarga manual de una orden
bool CAgv::SetDestinoManual()
{
	typ_tele_info teleinfo;

	if (IsWlan())
	{
		WlanSetDestinoManual();
		return true;
	}

	CString str;
	TYPE_STATION_DEF * ptrZiel;

	// Actualizar actual destination

	ptrZiel = &m_pOrden->m_at.az;

	SetZiel (&m_pFzdat->auftziel, ptrZiel->nDest,
		(unsigned char)ptrZiel->nPos,
			ptrZiel->ucSide,
			ptrZiel->nLayer);
/*
	USHORT nFz = m_pFzdat->fznr;
	GetMpFolge(nFz,pMpFolgeVerz[nFz-1],ptrZiel->nr);
	*(pCrMengeVerz[nFz-1]) = 0;
XXX-Lac04 */

	teleinfo.tele_art = 'I';

	str.Format ("%02d", m_id);
	strncpy ((char *)teleinfo.fznr, str, 2);

	str.Format ("%04d", ptrZiel->nDest);
	strncpy ((char *)teleinfo.auftziel, str, 4);

	teleinfo.auftpos = ptrZiel->nPos + 0x30; // Int to Char
	teleinfo.auftebene = ptrZiel->nLayer + 0x30; // Int to Char
	teleinfo.auftlage = ptrZiel->ucSide;

	teleinfo.kommando_0 = 0;
	// Michael 12.10.2001 Set Blocked
	SetBlocked (true);
	SetTeleBlocked (&teleinfo.kommando_0, true);

	teleinfo.kommando_0 |= BIT7; // Always 1

	teleinfo.kommando_1 = 0;

	// Orden manual
	if (2 == m_pOrden->m_at.akt_org)
		{
		// Deposito
		if (m_pOrden->m_at.fMesa1)
			teleinfo.kommando_1 = BIT2;		// Conveyor 1 delivery

		if (m_pOrden->m_at.fMesa2)
			teleinfo.kommando_1 |= BIT4;	// Conveyor 2 delivery
		}
		// Si no - solo ir a destino

	teleinfo.kommando_1 |= BIT7;	// Always 1

	if (m_pOrden->m_at.fSimCarga)
		teleinfo.kommando_1 |= BIT6;	// Simulate delivery

	bool fRes;
if (!g_Container.m_fSimulateCom)
	fRes = SendInfoTelegram (teleinfo);
else
	fRes = SimulateInfoTelegram (teleinfo);

	if (fRes)
	{
		m_fWillUpdateBuB = true;
		m_pOrden->SetEstado (DELIVER);
	}
	return (fRes);


}


bool CAgv::WlanSetDestinoManual()
{
	typ_tele_info_wlan teleinfo;

	CString str;
	TYPE_STATION_DEF * ptrZiel;

	// Actualizar actual destination

	ptrZiel = &m_pOrden->m_at.az;
	// TODO M2015
	//m_pOrden->m_at.az.nDest = 41;
	//m_pOrden->m_at.az.nPos = 1;
	//m_pOrden->m_at.az.ucSide = 'L';
	//m_pOrden->m_at.az.nLayer = 1;

	// TODO Set to unload
	teleinfo.kommando_1 = 0;

//	teleinfo.kommando_1 = BIT2;
//	teleinfo.kommando_1 |= BIT4;		
	// TODO


	SetZiel (&m_pFzdat->auftziel, ptrZiel->nDest,
		(unsigned char)ptrZiel->nPos,
			ptrZiel->ucSide,
			ptrZiel->nLayer);


	teleinfo.tele_art = 'I';

	str.Format ("%02d", m_id);
	strncpy ((char *)teleinfo.fznr, str, 2);

	str.Format ("%04d", ptrZiel->nDest);
	strncpy ((char *)teleinfo.auftziel, str, 4);



	str.Format("%02d", ptrZiel->nPos);
	strncpy((char *)teleinfo.auftpos, str, 2);
	teleinfo.auftebene = ptrZiel->nLayer + 0x30;
	teleinfo.auftlage = ptrZiel->ucSide;

	str.Format("%04d", 0);
	strncpy((char *)teleinfo.EntryPoint, str, 4);


	teleinfo.kommando_0 = 0;
	// Michael 12.10.2001 Set Blocked
	// Michael 25.05.2015 Don't SetBlocked (true);
	// SetTeleBlocked (&teleinfo.kommando_0, true);
	teleinfo.kommando_0 &= OFFBIT1;

	teleinfo.kommando_0 |= BIT7; // Always 1

	
	// Orden manual TODO Revise M2015
	if (2 == m_pOrden->m_at.akt_org)
		{
		// Deliver
		if (m_pOrden->m_at.fMesa1)
			teleinfo.kommando_1 = BIT2;		// Conveyor 1 deliver

		if (m_pOrden->m_at.fMesa2)
			teleinfo.kommando_1 |= BIT4;	// Conveyor 2 deliver
		}
		// Si no - solo ir a destino

	teleinfo.kommando_1 |= BIT7;	// Always 1

	if (m_pOrden->m_at.fSimCarga)
		teleinfo.kommando_1 |= BIT6;	// Simulate delivery

	teleinfo.kommando_2 = BIT7;	// Always
	
	str.Format ("%04d", m_pFzdat->nmp);
	strncpy ((char *)teleinfo.nmp, str, 4);

	str.Format ("%04d", m_pFzdat->uemp);
	strncpy ((char *)teleinfo.uemp, str, 4);
	
	


	bool fRes;
if (!g_Container.m_fSimulateCom)
{
	m_WlanTel.SetInfoTelegram (teleinfo, m_pWlanMsg);
	fRes = true;
}
else
	; // M2015 TODO fRes = SimulateInfoTelegram (teleinfo);

	if (fRes)
	{
		m_fWillUpdateBuB = true;
		m_pOrden->SetEstado (DELIVER);
	}
	return (fRes);


}

// Enviar blocking telegram al AGV
bool CAgv::WLanSendBlockTelegram(typ_tele_block_wlan tele)
{
	bool fAceptado = true;

	// Guardar los datos del commando por si...
	// M2015 m_pFzdat->msollfz0 = tele.kommando_0;
	m_pFzdat->Kommando0 = tele.kommando_0;

	m_WlanTel.SetBlockTelegram (tele, m_pWlanMsg);

	return (fAceptado);
}





// Enviar blocking telegram al AGV
bool CAgv::SendBlockTelegram(typ_tele_block tele)
{
	int fRetVal;
	int i;
	bool fAceptado;

	// Guardar los datos del commando por si...
	// M2015 m_pFzdat->msollfz0 = tele.kommando_0;
	m_pFzdat->Kommando0 = tele.kommando_0;

	for (i = 0, fAceptado = false; i < 3 && !fAceptado; i++)
	{
	m_Tel.SetBlockTelegram (tele);
	fRetVal = m_pSerCom->SendTelegrama (m_Tel.m_strTelegrama);
	if (fRetVal)
		{
		// El telegrama requiere respuesta
		fRetVal = m_pSerCom->ReceiveTelegrama (m_Tel.m_strRespuesta);

		switch (fRetVal)
		{
		case WAIT_OBJECT_0:
			// se ha recibido el telegrama de respuesta
			m_Tel.SetTeleRecibido ();
			// se debe comprobar el telegrama ???
			if (m_pSerCom->SendTelegrama (m_Tel.StrQuitum (m_id)))
				fAceptado = true;
			else
			{
				MSGERROR1 ("SendBlockTelegram () : Time out enviando StrQuitum Agv: %d", m_id);
			}
			break;
		case WAIT_TIMEOUT:
			m_Tel.SetTeleTout ();
			MSGERROR1 ("SendBlockTelegram () : Timeout waiting for Block reply Agv: %d", m_id);
			break;
		default:
			// XXX mirar posibilidades aquí
			m_Tel.SetTeleTout ();
			MSGERROR2 ("SendBlockTelegram () : ReceiveTelegrama ha devuelto: %d Agv: %d", 
				fRetVal,
				m_id);
			break;
			}
		}
		else
		{
			m_Tel.SetTeleNull ();
		}
	} // for (..)
	return (fAceptado);


}

// Permitir o no una accion del AGV
bool CAgv::SetAction(bool fFree)
{
	bool fRes;
	
	// Michael 28.02.2002 Solo permite acción si tiene drive permission
	if (g_Container.SystemDriveAllowed () && !IsDriveForbiddenByBuB ())
	{
		if (!IsWlan())	// M2015
		{
			typ_tele_block teleblock;
			CString str;
			
			teleblock.tele_art = 'B';
			
			str.Format ("%02d", m_id);
			strncpy ((char *)teleblock.fznr, str, 2);
			
			if (fFree)
			{
				// Drive stopped and action run
				teleblock.kommando_0 = 0;		// Agv (drive) blocked
				teleblock.kommando_0 |= BIT2;	// Agv (drive) blocked
				teleblock.kommando_0 &= OFFBIT3;		// Action run
			}
			
			else
			{
				// Drive stopped and action stopped
				teleblock.kommando_0 = 0;		// Agv (drive) blocked
				teleblock.kommando_0 |= BIT2;	// Agv (drive) blocked
				teleblock.kommando_0 |= BIT3;		// Action stopped
			}
			
			teleblock.kommando_0 |= BIT7;	// Always 1
			
			
			if (!g_Container.m_fSimulateCom)
				fRes = SendBlockTelegram (teleblock);
			else
				fRes = SimulateActionBlockTelegram (teleblock);
		}
		else
		{
			typ_tele_block_wlan teleblock;
			CString str;
		
			teleblock.tele_art = 'B';
		
			str.Format ("%02d", m_id);
			strncpy ((char *)teleblock.fznr, str, 2);
		
			str.Format ("%04d", m_pFzdat->nmp);
			strncpy((char *)teleblock.nmp, str, 4);
		
			str.Format ("%04d", m_pFzdat->uemp);
			strncpy((char *)teleblock.uemp, str, 4);

			if (fFree)
			{
				// Drive stopped and action run
				teleblock.kommando_0 = 0;		// Agv (drive) blocked
				teleblock.kommando_0 &= OFFBIT1;	// Agv (drive) blocked
				teleblock.kommando_0 &= OFFBIT3;		// Action run
			}
		
			else
			{
				// Drive stopped and action stopped
				teleblock.kommando_0 = 0;		// Agv (drive) blocked
				teleblock.kommando_0 &= OFFBIT1;	// Agv (drive) blocked
				teleblock.kommando_0 |= BIT3;		// Action stopped
			}
		
			teleblock.kommando_0 |= BIT7;	// Was Always 1 for old		

			teleblock.kommando_1 = m_pFzdat->Status1;
			teleblock.kommando_2 = m_pFzdat->Status2;

		
		
			if (!g_Container.m_fSimulateCom)
				fRes = WLanSendBlockTelegram (teleblock);
//			else
//				fRes = SimulateActionBlockTelegram (teleblock);
		}	// Wlan

	}
	else
		// No drive permission
		fRes = false;
	
	return (fRes);

}

// Dar orden de descargar...
bool CAgv::DescargarOrden()
{
	// Permitir acción del Agv
	m_fWillUpdateBuB = true;
	if (2 == m_pFzdat->mp)
		// Si se descarga en A2, hay que indicarle destino
		m_estadoPLC.SetDestinoA2 ((LPCTSTR)m_pOrden->m_at.id_nr);


	return (SetAction (true));
}

/* Simular un block telegram */
bool CAgv::SimulateActionBlockTelegram(typ_tele_block tele)
{

	// This only sets action done - doesn't block
	// M2015 if (m_pFzdat->mistfz1 & BIT2)	// Conveyor 1 delivery
	if (FZ_BRINGT(*m_pFzdat))	// Deliver
	{
		// M2015 m_pFzdat->mistfz2 &= OFFBIT2;	// Conveyor 1 unloaded
		SET_FZ_NOT_BELADEN(*m_pFzdat);
		// M2015 m_pFzdat->mistfz2 |= BIT1;	// Order done conveyor 1
		SET_FZ_AUFTRAG_ERLEDIGT(*m_pFzdat);
		// M2015 m_pFzdat->mistfz1 &= OFFBIT2;


	}
	else
	{
		// M2015 if (m_pFzdat->mistfz1 & BIT1)	// Conveyor 1 pickup
		if (FZ_HOLT(*m_pFzdat))	// Pickup
		{
			// M2015 m_pFzdat->mistfz2 |= BIT2;	// Conveyor 1 loaded
			SET_FZ_BELADEN(*m_pFzdat);
			// M2015 m_pFzdat->mistfz2 |= BIT1;	// Order done conveyor 1
			SET_FZ_AUFTRAG_ERLEDIGT(*m_pFzdat);
		}
	}

	// M2015 if	(m_pFzdat->mistfz1 & BIT4)	// Conveyor 2 delivery
	if (FZ_BRINGT_2(*m_pFzdat))
	{
		// M2015 m_pFzdat->mistfz2 &= OFFBIT3;	// Conveyor 2 unloaded
		SET_FZ_NOT_BELADEN_2(*m_pFzdat);	
		// M2015 m_pFzdat->mistfz2 |= BIT6;	// Order done conveyor 2
		SET_FZ_AUFTRAG_ERLEDIGT_2(*m_pFzdat);

		// M2015 m_pFzdat->mistfz1 &= OFFBIT4;
		RESET_FZ_BRINGT_2(*m_pFzdat);

	}
	else
	{
		// M2015 if (m_pFzdat->mistfz1 & BIT3)	// Conveyor 2 pickup
		if (FZ_HOLT_2(*m_pFzdat))
		{
			// M2015 m_pFzdat->mistfz2 |= BIT3;	// Conveyor 2 loaded
			SET_FZ_BELADEN_2(*m_pFzdat);

			// M2015 m_pFzdat->mistfz2 |= BIT6;	// Order done conveyor 2
			SET_FZ_AUFTRAG_ERLEDIGT_2(*m_pFzdat);
		}
	}
	
	// Michael 21.11.2001 -- Set 0 el destino

	m_pFzdat->fzziel.nDest = 0;
	if (m_pFzdat->mp >= 53 && m_pFzdat->mp <= 60)
	{
		//m_pFzdat->wegstr = z_typ_tab[zt_ver_tab[statdat [vtstat[m_pFzdat->mp]].zieltyp_l]].pos_weg;
		TYPE_STATION_DEF station;
		station.nDest = m_pFzdat->mp;
		station.nLayer = 'L';
		station.nPos = 1;
		station.nLayer = 0;
		m_pFzdat->wegstr = pAGVTCourse->GetPosWay(station);
	}

	// Michael 21.11.2001 - Fin
	return (true);
}


// Set the drive blocked or not
void CAgv::SetBlocked(bool fBlock)
{
	//Xavi, 02/11/05: Si es canvia l'estat de bloqueig, insertem un registre a histagv
	if (IsBlocked () != fBlock)
		CDBEstadistica::ArchivarHistAgv(this);
	// Fi xavi
	if (fBlock)
	{
		// M2015
		//m_pFzdat->fahr_byte_ist &= OFFBIT1;	// Agv blocked
		//m_pFzdat->fahr_byte_ist |= BIT2;	// Agv blocked
		SET_AGV_STOP(*m_pFzdat);
	}
	else
	{
		// Ver si estaba bloqueado
		m_fWasBlocked = IsBlocked ();
		// M2015
		// m_pFzdat->fahr_byte_ist |= BIT1;	// Agv not blocked
		// m_pFzdat->fahr_byte_ist &= OFFBIT2;	// Agv blocked
		SET_AGV_DRIVE(*m_pFzdat);
	}
}

// True if the Agv is blocked for drive
bool CAgv::IsBlocked()
{
	// M2015
	//if (! (m_pFzdat->fahr_byte_ist & BIT2))
	//{
	//	// Michael 16.01.2002 Apparently not blocked
	//	if (m_pFzdat->fahr_byte_ist & BIT1)
	//		return (false);
	//	else
	//		return (true);
	//}
	//else
	//	return (true);
	return TEST_AGV_DRIVE(*m_pFzdat) == 1 ? false : true;

}



// Enviar Telegrama de no bloquear drive
bool CAgv::WlanUnblockDrive()
{
	MSGAVISO1 ("UnblockDrive () Agv: %d", m_id);
	typ_tele_block_wlan teleblock;
	CString str;

	teleblock.tele_art = 'B';

	str.Format ("%02d", m_id);
	strncpy ((char *)teleblock.fznr, str, 2);

	// Drive go and action as was
	// M2015 TODO Confirm that it is Status0
	// teleblock.kommando_0 = m_pFzdat->fahr_byte_ist;
	// TODO Should be this for M2015 teleblock.kommando_0 = m_pFzdat->Status0;	
	teleblock.kommando_0 = 0;
	teleblock.kommando_0 |= BIT1;		// Agv (drive) not blocked
	teleblock.kommando_0 &= OFFBIT2;	// Agv (drive) not blocked TODO 2015: No longer necessary

	teleblock.kommando_0 |= BIT7;	// Always 1


	str.Format ("%04d", m_pFzdat->nmp);
	strncpy ((char *)teleblock.nmp, str, 4);
	str.Format ("%04d", m_pFzdat->uemp);
	strncpy ((char *)teleblock.uemp, str, 4);

	// Michael 14.05.2015 Write out what was there: was sending @ for 1 and 2
	teleblock.kommando_1 = m_pFzdat->Status1;

	teleblock.kommando_2 = m_pFzdat->Status2;

	m_pFzdat->Kommando0 = teleblock.kommando_0;

	bool fRes = true;
if (!g_Container.m_fSimulateCom)
{
	m_WlanTel.SetBlockTelegram (teleblock, m_pWlanMsg);
	fRes = true;
}

else
	SetBlocked (false);

	m_fWillUpdateBuB = true;

	return (fRes);

}

// Enviar Telegrama de no bloquear drive
bool CAgv::UnblockDrive()
{
	MSGAVISO1 ("UnblockDrive () Agv: %d", m_id);
	typ_tele_block teleblock;
	CString str;

	teleblock.tele_art = 'B';

	str.Format ("%02d", m_id);
	strncpy ((char *)teleblock.fznr, str, 2);

	// Drive go and action as was
	// M2015 TODO Confirm that it is Status0
	// teleblock.kommando_0 = m_pFzdat->fahr_byte_ist;
	teleblock.kommando_0 = m_pFzdat->Status0;	
	teleblock.kommando_0 |= BIT1;		// Agv (drive) not blocked
	teleblock.kommando_0 &= OFFBIT2;	// Agv (drive) not blocked TODO 2015: No longer necessary

	teleblock.kommando_0 |= BIT7;	// Always 1


	bool fRes = true;
if (!g_Container.m_fSimulateCom)
	fRes = SendBlockTelegram (teleblock);
else
	SetBlocked (false);

	m_fWillUpdateBuB = true;

	return (fRes);

}

// Borrar una orden a petición de Gestor de Órdenes
void CAgv::SetDeleted()
{
	m_pOrden = NULL;	// El Gestor de ordenes borrará
	SetZiel (&m_pFzdat->auftziel);	// Borrar destino
	strcpy ((char *)m_pFzdat->aufident, "");
	SetEstado (AGVSINDATOS);

}

// See if a CP is occupied by an AGV, or an Agv has its current destination this CP
bool CAgv::IsOccupiedCP(UINT cp, bool fRealmenteOcupado)
{
	// Michael 23.01.2002 Flag para indicar que está realmente ocupado
	if (cp == 6)
		return (false);	// Michael 25.10.2001 - esperamos en 6
	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	{
		CAgv * ptrAgv = g_pGestor->GetAgvPtr (i + 1);
		if (ptrAgv)
		{
		if (AGVFUERASISTEMA !=ptrAgv->GetEstado ())
		{
			if (ptrAgv->m_pFzdat->mp == cp
				|| (ptrAgv->m_pFzdat->auftziel.nDest == cp && !fRealmenteOcupado))
				return (true);
			}
		}
	}
	return (false);
}


// True if the Agv is a candidate to receive an order. Returns estado
bool CAgv::IsCandidateForOrder(TEstadoAGV * estado, COrden * ptrOrden)
{
	bool fRetVal = false;
	if (TEST_FZ_IM_SYSTEM(*m_pFzdat) && 
		m_fHasBeenPolled  && 
		!FZ_HAT_FEHLER(*m_pFzdat) && 
		!IsDriveForbiddenByBuB () &&
		m_fEnCircuito)
	{
		switch (m_estado)
		{
/* Michael 31.10.2001
			case ORDENNUEVO:
			case ORDENNUEVOMANUAL:
			case IRCARGARORD:
*/
			case IRPUNTOESP:
			case AGVSINDATOS:
				if (ptrOrden)
				{
					// Michael 17.02.2002 Modificado
					if (ptrOrden->m_at.hz.nDest != m_pFzdat->mp)
						if (! IsModifiableDestino ())
						{
							fRetVal = false;
							break;
						}
				}
				else
					if (! IsModifiableDestino ())
					{
						fRetVal = false;
						break;
					}
			case PUNTOESP:
				*estado = m_estado;
				if (MESA1CARGADA (*m_pFzdat) || MESA2CARGADA (*m_pFzdat))
					fRetVal = false;
				else
					fRetVal = true;
				break;
			default:
				fRetVal = false;
		}
	}
	else
		fRetVal = false;
	return (fRetVal);
}

/*
// Get the time from the current position, or destination, to the pickup CP
UINT CAgv::GetTimeToPickup(UINT mpZiel, UINT mpDest)
{
	UINT uTotalTime = 0;

	if (m_pFzdat->mp == mpZiel)
		uTotalTime = 0;
	else
	{
		NMP_tabelle *pNmp;
		if (mpDest)
			// Interesa el destino
			pNmp = GetPNmp(mpDest,mpZiel);
		else
			// Michael 28.02.2002 Si ya va al nmp, considera este
			if (m_pFzdat->nmp && !IsBlocked ())
			{
				// Michael 10.04.2002 Pero puede ser que el nmp es el destino ya
				if (m_pFzdat->nmp == mpZiel)
				{
					uTotalTime = 0; // Está claro
					pNmp = NULL;
				}
				else
					pNmp = GetPNmp(m_pFzdat->nmp,mpZiel);
			}
			else
			// Michael 28.02.2002 fin
				pNmp = GetPNmp(m_pFzdat->mp,mpZiel);
		if (pNmp)
		{
			uTotalTime = fahrzeit_tab_fest [pNmp->zeit_index] * 100; // Milliseconds
			while (pNmp->nmp != mpZiel)
			{
				pNmp = GetPNmp (pNmp->nmp, mpZiel);
				uTotalTime += fahrzeit_tab_fest [pNmp->zeit_index] * 100;
			}
		}
	}

	//MSGTRACE4 ("GetTimeToPickup () nAgv: %d mp: %d mpZiel: %d time: %d",
	//	m_id,
	//	m_pFzdat->mp,
	//	mpZiel,
	//	uTotalTime); 

	return (uTotalTime);
}
*/

// Get the time from the current position, or destination, to the pickup CP
UINT CAgv::GetTimeToPickup(UINT mpZiel, UINT mpDest)
{
	UINT uTotalTime = 0;

	if (m_pFzdat->mp == mpZiel)
		uTotalTime = 0;
	else
	{
		//NMP_tabelle *pNmp;
		CCpDistance *pCpDistance;
		if (mpDest)
			// Interesa el destino
			pCpDistance = GetPNmp(mpDest, mpZiel);
		else
			// Michael 28.02.2002 Si ya va al nmp, considera este
		if (m_pFzdat->nmp && !IsBlocked())
		{
			// Michael 10.04.2002 Pero puede ser que el nmp es el destino ya
			if (m_pFzdat->nmp == mpZiel)
			{
				uTotalTime = 0; // Está claro
				pCpDistance = NULL;
			}
			else
				pCpDistance = GetPNmp(m_pFzdat->nmp, mpZiel);
		}
		else
			// Michael 28.02.2002 fin
			pCpDistance = GetPNmp(m_pFzdat->mp, mpZiel);
		if (pCpDistance != NULL)
		{
			//uTotalTime = fahrzeit_tab_fest[pNmp->zeit_index] * 100; // Milliseconds
			uTotalTime = pCpDistance->GetTravelTime() * 100; // Milliseconds
			//while (pNmp->nmp != mpZiel)
			while (pCpDistance->GetNCp() != mpZiel)
			{
				//pNmp = GetPNmp(pNmp->nmp, mpZiel);
				//uTotalTime += fahrzeit_tab_fest[pNmp->zeit_index] * 100;
				pCpDistance = GetPNmp(pCpDistance->GetNCp(), mpZiel);
				uTotalTime += pCpDistance->GetTravelTime() * 100;
			}
		}
	}

	//MSGTRACE4 ("GetTimeToPickup () nAgv: %d mp: %d mpZiel: %d time: %d",
	//	m_id,
	//	m_pFzdat->mp,
	//	mpZiel,
	//	uTotalTime); 

	return (uTotalTime);
}

// Disassociate the current order. Normally for a better one
void CAgv::ResetOrden()
{
	if (m_pOrden)
	{
		// The order has already been verified as "resetable"
		// Michael 28.10.2001 m_pOrden->SetEstado (NEWORDER);
		m_pOrden = NULL;	// El Gestor de ordenes borrará
		SetZiel (&m_pFzdat->auftziel);	// Borrar destino
		strcpy ((char *)m_pFzdat->aufident, "");
	}
}


// Comprobar si hay error en la estación o rearme del error de la estación
void CAgv::CheckStationError(UINT module, UINT error)
{
	if (module == 60)	// Mesas del Agv
	{
		if (CARGARORD == m_estado  || DESCARGARORD == m_estado)
		{
			// Agv in station - error
			MSGERROR2 ("Agv: %d Error %d en Estacion", m_id, error);
			m_uErrorEstacion = error;
		}
	}
	else if (module == 59 && error == 13
		&& m_uErrorEstacion != 0)
	{
		// Start button pressed with error in station
		if (CARGARORD == m_estado)
		{
			// send order to load again
			MSGWARNING1 ("Enviando mensaje de carga por error en estación Agv: %d",
				m_id);
			PickupOrden ();	// Give whole order again
			//m_estado = IRCARGARORD;//Xavi, 02/11/05
			SetEstadoActual(IRCARGARORD);
			m_uErrorEstacion = 0;
		}
		else if (DESCARGARORD == m_estado)
		{
			// Send order to unload again
			MSGWARNING1 ("Enviando mensaje de descarga por error en estación Agv: %d",
				m_id);
			SetUnloadDestination ();	// Give whole order again
			//m_estado = IRDESCARGARORD;//Xavi, 02/11/05
			SetEstadoActual(IRDESCARGARORD);
			m_uErrorEstacion = 0;
		}
		else
		{
			MSGERROR1 ("Error en estación con estado Agv: %d erróneo",
				m_id);
		}
	}

}

// Determinar que mesa ordenar pickup si es una orden simple o doble
void CAgv::SetMesaToPickup(UCHAR *kommand)
{
	bool needsPickup1 = false;
	bool needsPickup2 = false;

	int firstMesa = 0;

	if (m_pOrden->m_ptrEstOri->SentidoDelante ())
	{
		firstMesa = 1;
	}
	else
	{
		firstMesa = 2;
	}

	needsPickup1 = m_pOrden->m_at.fMesa1 && ! MESA1CARGADA (*m_pFzdat);
	needsPickup2 = m_pOrden->m_at.fMesa2 && ! MESA2CARGADA (*m_pFzdat);

	if (1 == firstMesa && needsPickup1)
	{
		*kommand |= BIT1;	// Conveyor 1 pickup
	}
	else if (2 == firstMesa && needsPickup2)
		*kommand |= BIT3;	// Conveyor 2 pickup
	else if (needsPickup1)
		*kommand |= BIT1;	// Conveyor 1 pickup
	else
		*kommand |= BIT3;	// Conveyor 2 pickup

}


// Determinar que mesa de descarga usar si es una mesa simple
void CAgv::SetMesaToDeliver(UCHAR *kommand)
{
	bool needsDelivery1 = false;
	bool needsDelivery2 = false;

	int firstMesa = 0;

	if (m_pOrden->m_ptrEstDes->SentidoDelante ())
	{
		firstMesa = 1;
	}
	else
	{
		firstMesa = 2;
	}

	needsDelivery1 = m_pOrden->m_at.fMesa1 && MESA1CARGADA (*m_pFzdat);
	needsDelivery2 = m_pOrden->m_at.fMesa2 && MESA2CARGADA (*m_pFzdat);

	if (1 == firstMesa && needsDelivery1)
	{
		*kommand |= BIT2;	// Conveyor 1 delivery
	}
	else if (2 == firstMesa && needsDelivery2)
		*kommand |= BIT4;	// Conveyor 2 delivery
	else if (needsDelivery1)
		*kommand |= BIT2;	// Conveyor 1 delivery
	else
		*kommand |= BIT4;	// Conveyor 2 delivery

}


// Ver si la orden ha sido cargado parcialmente
bool CAgv::SOrdenCargadaParcial()
{
	// solo si es una mesa simple
	bool cargada = false;
	if ('S' != m_pOrden->m_at.herkunft)
	{
		// Solo ordenes automáticas

		if (1 == m_pOrden->m_ptrEstOri->GetNumeroMesas ())
		{
			cargada = m_pOrden->m_at.fMesa1 && MESA1CARGADA(*m_pFzdat);
			if (!cargada)
			{
				cargada = m_pOrden->m_at.fMesa2 && MESA2CARGADA(*m_pFzdat);
			}
		}
	}
	return (cargada);
}


// Ver si la orden ha sido parcialmente descargada
bool CAgv::SOrderDoneParcial()
{
	bool fOrderDone = false;
	if ('S' != m_pOrden->m_at.herkunft)
	{
		// Solo ordenes automáticas
		if (1 == m_pOrden->m_ptrEstDes->GetNumeroMesas ())
		{
			// Solo mesas individuales
			if (m_pOrden->m_at.fMesa1)
				fOrderDone = ! MESA1CARGADA (*m_pFzdat);		// Conveyor 1 Order done
			if (!fOrderDone)
				fOrderDone = fOrderDone || ! MESA2CARGADA (*m_pFzdat);
		}
	}

	return (fOrderDone);
}


// See whether to block the Agv for entering into a Knot area...
bool CAgv::BlockForKnotNumber()
{
	bool fBlock = false;
	m_nextKnot = 0;

	if (m_pFzdat->nmp != 0)
	{
		UINT knot = IsKnot ();
		m_nextKnot = knot;	// será ocupado por este
		if (0 != knot)
		{
			MSGTRACE2 ("Agv: %d necesita knot: %d", m_id, knot);
			if (knot != m_knot)
				if (m_pFzdat->fz_blk = g_pGestor->IsOccupiedKnot (m_id, knot))
				{
					if (0 != m_pFzdat->fz_blk)
					{
						fBlock = true;
						MSGTRACE2 ("Agv: %d Blocking for knot: %d", m_id, knot);
					}
					else
					{
						m_nextKnot = knot;	// Update when drive permission given
						fBlock = false;
					}
				}
		}
	}
	else
		m_nextKnot = m_knot;

	return (fBlock);


}

/*
// Return knot associated with an mp-nmp
UINT CAgv::IsKnot()
{
	UINT knot = 0;
	Meldepunkttabelle  * ptrMpDest;
	NMP_tabelle * ptrNmp;

	ptrMpDest  = mptab + (vmptab [m_pFzdat->nmp] - 1); // Get mp data for next mp
	ptrNmp = GetPNmpFromNmp (m_pFzdat->mp, m_pFzdat->nmp); // Get nmp data for mp-nmp

	knot = ptrNmp->n_ktn_nr;	// mp-nmp knot takes precedence
	if (0 != knot)
	{
		if (0 != ptrMpDest->ktn_nr && ptrMpDest->ktn_nr != knot)
		{
			// is this an error ?
			MSGERROR2 ("IsKnot mp %d nmp %d knot differente",
				m_pFzdat->mp, m_pFzdat->nmp);
		}
	}
	else
		knot = ptrMpDest->ktn_nr;

	return (knot);
}
*/

// Return knot associated with an mp-nmp
UINT CAgv::IsKnot()
{
	return g_pCourseData->GetManBlockAreaOfDistance(m_pFzdat->mp, m_pFzdat->nmp);
}


void CAgv::SetTeleBlocked(UCHAR *kommand, bool fBlock)
{
	if (fBlock)
	{
		*kommand &= OFFBIT1;	// Agv blocked
		*kommand |= BIT2;	// Agv blocked
	}
	else
	{
		// Ver si estaba bloqueado
		*kommand |= BIT1;	// Agv not blocked
		*kommand &= OFFBIT2;	// Agv blocked
	}


}

void CAgv::SetTeleBlockedWlan(UCHAR *kommand, bool fBlock)
{
	if (fBlock)
	{
		*kommand &= OFFBIT1;	// Agv blocked
	}
	else
	{
		// Ver si estaba bloqueado
		*kommand |= BIT1;	// Agv not blocked
	}


}



// Determinar si se debe bloquear o no el AGV
bool CAgv::ShouldBlock()
{
	bool fStop = true;
	unsigned char  ucFzLastBlk;	// Last Agv for comparison

	if(g_Container.SystemDriveAllowed () && !IsDriveForbiddenByBuB ())
	{
		ucFzLastBlk = m_pFzdat->fz_blk;
		fStop = BlockForKnotNumber ();
		if (!fStop)
			// Michael 19.10.2001
			if (m_pFzdat->nmp != 0)
				fStop = 0 == SetFVAusBlTab(m_pFzdat, FALSE) ? false : true;
			else
				fStop = true;

		if (! fStop)
			if (IsOccupied60 ())
				// Si la posición 60 está ocupada por otro Agv, no hacemos nada
				fStop = true;

		if (! fStop)
		{
			m_knot = m_nextKnot;	// set knot occupied by this Agv as won't be blocked
			m_nAgvEmpujando = false;	// Reset petición de empujar
		}
		else
		{
			if (ucFzLastBlk != m_pFzdat->fz_blk)
				m_fWillUpdateBuB = true;

			EmpujarAgvBloqueo ();	// Intentar empujar el Agv bloqueando este
		}
	}
	else
	{
		fStop = TRUE;
	}

	MSGTRACE2 ("fStop is %s for Agv: %d", fStop == TRUE ? "TRUE" : "FALSE", m_id);


	return (fStop);

}


bool CAgv::WlanEnviarCargador(TYPE_STATION_DEF zCargador) 
{
	CString str;

	typ_tele_info_wlan teleinfo;
	teleinfo.tele_art = 'I';

	str.Format ("%02d", m_id);
	strncpy ((char *)teleinfo.fznr, str, 2);

	str.Format ("%04d", zCargador.nDest);
	strncpy ((char *)teleinfo.auftziel, str, 4);

	str.Format ("%04d", m_pFzdat->nmp);
	strncpy ((char *)teleinfo.nmp, str, 4);
	str.Format ("%04d", m_pFzdat->uemp);
	strncpy ((char *)teleinfo.uemp, str, 4);
	
	strncpy ((char *)teleinfo.EntryPoint, str, 4);


	sprintf((char *)teleinfo.auftpos, "%02d",zCargador.nPos);

	teleinfo.auftebene = zCargador.nLayer + 0x30; // Int to Char
	teleinfo.auftlage = zCargador.ucSide;

	teleinfo.kommando_0 = 0;
	teleinfo.kommando_0 |= BIT7;	// Always 1

	// Enviar bloqueado siempre 12.10.2001
	// Michael 25.05.2015 SetBlocked (true);
	SetTeleBlockedWlan (&teleinfo.kommando_0, true);

	// teleinfo.kommando_0 |= BIT3;	// Action stopped (Don't load)

	teleinfo.kommando_1 = BIT7;		// Always 1
	teleinfo.kommando_2 = BIT7;
	MSGAVISO2 ("Enviando Agv: %d a carga de bateria %d",
				m_id,
				zCargador.nDest);

	if (!g_Container.m_fSimulateCom)
		m_WlanTel.SetInfoTelegram (teleinfo, m_pWlanMsg);
	else
		; // M2015 TODO fRes = SimulateInfoTelegram (teleinfo);

	return true;

}

// Enviar el Agv al cargador de baterias...
bool CAgv::EnviarCargador()
{

	// Michael 23.11.2001: Modificado extensivamente
	TYPE_STATION_DEF zCargador; // Punto destino del cargador
	zCargador.nDest = 0;
	CString str;
	bool fRes;

	// Si ya está en la posición de carga...
	if (m_pFzdat->mp == m_nCargador || (IsCargadorCurPos () && m_nCargador == 0))
	{
		switch (m_pFzdat->mp)
		{
		case 53:
			if (IsOccupiedCP (55, true))
			{
				// Si está ocupado el 55 entonces este esta OK
				zCargador.nDest = m_pFzdat->mp;
			}
			break;
		case 54:
			if (IsOccupiedCP (56, true))
			{
				// Si está ocupado el 56 entonces este esta OK
				zCargador.nDest = m_pFzdat->mp;
			}
			break;
		case 60:
			if (!IsOk60 ())
				break;
			// Si el 60 está bien continua
		default:
			zCargador.nDest = m_pFzdat->mp;
			break;
		}
	}

	if (zCargador.nDest == 0)
	{
		// Si hay otro Agv que va al cargador, esperar
		if (IsIrCargarBat ())
			return (false);

		// No lo enviamos si esta bloqueado
		if (IRPUNTOESP == m_estado && IsBlocked ())
			return (false);

		// No lo enviamos si tiene carga
		if (MESA1CARGADA (*m_pFzdat) || MESA2CARGADA (*m_pFzdat))
			return (false);
		
		
		if (m_nCargador == 0)
		{
			// No tiene cargador asociado
			fRes = CDBEstacion::GetCargadorLibre (m_pFzdat->mp, (UINT *)&zCargador.nDest);
			if (fRes)
			{
				// Michael 23.01.2002 Ver si este cargador es válido
				switch (zCargador.nDest)
				{
				case 53:
					if (! IsOccupiedCP (55, true) || IsOccupiedCP (155))
					{
						fRes = false;
					}
					break;
				case 54:
					if (! IsOccupiedCP (56, true) || IsOccupiedCP (156))
					{
						fRes = false;
					}
					break;
				case 60:
					if (! IsOk60 ())
						fRes = false;
				}
			}
		}
		else
		{
			fRes = true;
			zCargador.nDest = m_nCargador;
		}
	}
	else
		fRes = true;



	if (fRes)
	{
		zCargador.nPos = 1;		// Always
		zCargador.ucSide = 'L';	// Always
		zCargador.nLayer = 0;	// Always


		SetZiel (&m_pFzdat->auftziel,
				zCargador.nDest,
				(unsigned char)zCargador.nPos,
				zCargador.ucSide,
				zCargador.nLayer);

		if (!SLlegadoDestino ())
		{
			if (IsWlan())
			{
				fRes = WlanEnviarCargador(zCargador);
			}
			else
			{
				typ_tele_info teleinfo;

				teleinfo.tele_art = 'I';

				str.Format ("%02d", m_id);
				strncpy ((char *)teleinfo.fznr, str, 2);

		/*
				USHORT nFz = m_pFzdat->fznr;
				GetMpFolge(nFz,pMpFolgeVerz[nFz-1],zCargador.nr);
				*(pCrMengeVerz[nFz-1]) = 0;
		XXX-Lac04 */

				str.Format ("%04d", zCargador.nDest);
				strncpy ((char *)teleinfo.auftziel, str, 4);

				teleinfo.auftpos = zCargador.nPos + 0x30; // Int to Char
				teleinfo.auftebene = zCargador.nLayer + 0x30; // Int to Char
				teleinfo.auftlage = zCargador.ucSide;

				teleinfo.kommando_0 = 0;
				teleinfo.kommando_0 |= BIT7;	// Always 1

				// Enviar bloqueado siempre 12.10.2001
				SetBlocked (true);
				SetTeleBlocked (&teleinfo.kommando_0, true);

				teleinfo.kommando_0 |= BIT3;	// Action stopped (Don't load)

				teleinfo.kommando_1 = BIT7;		// Always 1


				MSGAVISO2 ("Enviando Agv: %d a carga de bateria %d",
							m_id,
							zCargador.nDest);


			if (!g_Container.m_fSimulateCom)
				fRes = SendInfoTelegram (teleinfo);
			else
				fRes = SimulateInfoTelegram (teleinfo);
			}
		} // Llegado destino - no hace falta enviarle telegrama
	}
	else
	{
		MSGERROR1 ("No hay cargador libre para Agv: %d", m_id);
		fRes = false;
	}
	
	if (fRes)
		// Si estuviera en el circuito, quitala
		SetFueraCircuito ();

	// fRes is false when there is no cargador or the comunication failed
	return (fRes);


}

// Enviar Sleep telegram al Agv en la carga de bateria
void CAgv::SleepAgv()
{
	bool fRes;

	if (!IsWlan())
	{
		typ_tele_block teleblock;
		CString str;

		teleblock.tele_art = 'B';

		str.Format ("%02d", m_id);
		strncpy ((char *)teleblock.fznr, str, 2);

		// Drive stopped and action stopped
		teleblock.kommando_0 = 0;		// Agv (drive) blocked
		teleblock.kommando_0 |= BIT2;	// Agv (drive) blocked
		teleblock.kommando_0 |= BIT3;	// Action stopped
		teleblock.kommando_0 |= BIT4;	// Agv switched off

		teleblock.kommando_0 |= BIT7;	// Always 1
		if (!g_Container.m_fSimulateCom)
			fRes = SendBlockTelegram (teleblock);
		else
			fRes = SimulateActionBlockTelegram (teleblock);

	}
	else
	{
		typ_tele_block_wlan teleblock;
		CString str;

		teleblock.tele_art = 'B';

		str.Format ("%02d", m_id);
		strncpy ((char *)teleblock.fznr, str, 2);
	
		str.Format ("%04d", m_pFzdat->nmp);
		strncpy((char *)teleblock.nmp, str, 4);
	
		str.Format ("%04d", m_pFzdat->uemp);
		strncpy((char *)teleblock.uemp, str, 4);



		// Drive stopped and action stopped
		teleblock.kommando_0 = 0;		// Agv (drive) blocked
		// M2015 only for maintenance charge teleblock.kommando_0 |= BIT4;	// Agv switched off
		teleblock.kommando_0 |= BIT5;	// Charge battery


		teleblock.kommando_0 |= BIT7;	// Always 1

		teleblock.kommando_1 = m_pFzdat->Status1;
		teleblock.kommando_2 = m_pFzdat->Status2;

		if (!g_Container.m_fSimulateCom)
			fRes = WLanSendBlockTelegram (teleblock);
//		else
//			fRes = SimulateActionBlockTelegram (teleblock);


	}
}
	

// Avisar el PLC que se debe cargar la bateria
bool CAgv::SwitchOnCargadorPLC()
{
	m_estadoPLC.SwitchCargador (m_id, m_pFzdat->mp, m_pFzdat->wegstr, true);
	return (true);

}



// Comprobar si la carga ya está y se puede sacar el Agv
bool CAgv::SetCargado()
{
	bool todaviaCargando;
	bool yaCargado;
	bool fRetVal;
	// 04.01.2002 No sacar del cargador si la carga ha sido ordenada desde el BuB
	yaCargado = CDBEstacion::IsCargado (m_pFzdat->mp, &todaviaCargando);
	if ((/* 21.11.2001 yaCargado || */ m_nCargador == 0 || !todaviaCargando) 
		/* && 18.04.2002 !g_Container.IsFinDeJornada () */
		&&  g_Container.IsEnFuncionamiento ()	// Solo sacar si se pone en funcionamiento
		/* 04.01.2002 */ && ! IsCargaBateriaBuB ())
	{
		// Ya está cargado
		if (todaviaCargando)
		{
			// Apaga el cargador
			m_estadoPLC.SwitchCargador (m_id, m_pFzdat->mp, m_pFzdat->wegstr, false);
			m_nCargador = 0;
			fRetVal = false;	// Damos otra vuelta hasta que se ve apagado
		}
		else
		{
			// Apaga el cargador por si acaso y para sacar el Agv
			m_estadoPLC.SwitchCargador (m_id, m_pFzdat->mp, m_pFzdat->wegstr, false);
			m_nCargador = 0;
			fRetVal = true;		// Ya está cargado y libre
		}
	}
	else
		fRetVal = false;	// está cargando todavia

	return (fRetVal);
}



bool CAgv::WakeUpAgv()
{
	CSocket mySocket;
	CString strMessage;
	CString	strModemIp;

	
	mySocket.Create();

	if (GetIPForWakeup(strModemIp))
	{
		if (!mySocket.Connect(strModemIp, 9000))
		{
				UINT errorCode = GetLastError();
				if (errorCode != 10035)	// Would block - allow to continue as is normal
				{
					CString str;
					str.Format(
						"DespertarAgv: %d No se ha podido conectar a la dirección %s y puerto %d. LastError: %d", 
						m_id, strModemIp, 9000, errorCode);
					MSGERROR(str);
					mySocket.Close(); 
					return false;
				}
		}


		strMessage = "ON";
		mySocket.Send(strMessage, strMessage.GetLength());
		mySocket.Close();
		MSGAVISO2 ("Despertado el AGV: %d por modem IP:", m_id, strModemIp);
		return true;
	}
	else
	{
		MSGERROR1 ("AGV: %d no encontrado la IP en el registro (HKEY_CURRENT_USER\\Software\\INDUMAT\\tpc\\Settings)",
			m_id);
		return false;	
	}
}

bool CAgv::GetIPForWakeup (CString& wakeupIp)
{
	TCHAR charAgv [20];
	
	printf(charAgv, "AGV%dWAKEUPMODEM", m_id);
		
	wakeupIp = AfxGetApp ()->GetProfileString ("Settings", charAgv);
	if (wakeupIp.GetLength() > 0)
		return true;
	else
		return false;


}


// Ver si se debe cargar bateria
// M2015 TODO For new Agvs, see how to manage daily and weekly battery charging
bool CAgv::SCargarBateria()
{
	bool fRetVal = false;

	if (FZ_BATTERIE_LEER (*m_pFzdat))
	{
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (2, 17, 1, m_id);
	}
	
	return (m_nCargador != 0 || g_Container.IsFinDeJornada () || FZ_BATTERIE_LEER (*m_pFzdat));

}




bool CAgv::CheckTeleE_Check(CString &telegrama) 
{
	CString strECin;
	CString strECcalc;
	int nLen;
	nLen = telegrama.GetLength ();
	strECin = telegrama.Mid (nLen - 3, 2);	// Leave off last ETX
	telegrama.Delete (nLen - 3, 3);	// Telegrama sin Error [2] + ETX
	telegrama.Delete (0);	// Telegrama sin STX
	CTelegrama::GetErrorCheck (strECcalc, telegrama);
	if (strECcalc != strECin)
	{
		MSGERROR2 ("TODO RETURNING TRUE - Telegrama ErrorCheck esperado %s - recibido %s\n", strECcalc, strECin);
		// TODO return (false);
		return (true);
	}
	else
		return true;


}


bool CAgv::CheckTeleAgv(char *idTele)
{
	char pszbuf [4];	// Michael 04.11.2004 was 3

	memcpy (pszbuf, idTele, 2);
	// Michael M2015 pszbuf [3] = '\0';	
	pszbuf [2] = '\0';
	bool fRetVal;
	if (atoi (pszbuf) != (signed)m_id)
	{
		MSGERROR2 ("Recibido telegrama para agv: %s esperando para agv: %d",
			pszbuf,
			m_id);
		// Michael 11.11.2001 SetBlocked por si acaso
		SetBlocked (true);	// TODO - what use would this have? Michael 25.05.2015
		fRetVal = false;
	}
	else
		fRetVal = true;


	return (fRetVal);

}


// Empujar un Agv que está bloquando este
void CAgv::EmpujarAgvBloqueo(bool desdeAtras)
{
	// Solo empuja si tiene algo que hacer
	bool fEmpujar;
	switch (m_estado)
	{
	case IRCARGARORD:	// Aqui debe trasladar la orden al siguiente
	case IRDESCARGARORD:
	case IRCARGARBAT:
	// Michael 14.06.2005 T5K
	case IRMANTENIMIENTO:
		fEmpujar = true;
		break;
		// Michael 30.07.2002: Si quiere salir de 55, 56 - empuja
	case IRPUNTOESP:
		if (m_pFzdat->mp == 55 || m_pFzdat->mp == 56 
			|| m_pFzdat->mp == 155 || m_pFzdat->mp == 156)
			fEmpujar = true;
		else
			fEmpujar = false;
		break;
	default:
		fEmpujar = false;
	}
	if (fEmpujar || m_nAgvEmpujando != 0)
	{
		if (m_pFzdat->fz_blk != 0)
		{
			CAgv * ptrAgvBloqueo = g_pGestor->GetAgvPtr (m_pFzdat->fz_blk);
			if (ptrAgvBloqueo)
			{
			if (ptrAgvBloqueo->m_id != m_id)
				ptrAgvBloqueo->SetAgvEmpujando (m_id);
			else
				MSGERROR1 ("Agv: %d empujando el mismo !!!", m_id);
			}
		}
	}
}

// Llamado desde otro Agv para empujar este
void CAgv::SetAgvEmpujando(UINT nAgv)
{
	switch (m_estado)
	{
	case AGVSINDATOS:
	case PUNTOESP:
	case IRPUNTOESP:
		if (true /* XXX ! IsCargadorCurPos () */)
		{
			// Solo empujar si no está en un cargador

			if (IsBlocked ())
			{
				; // EmpujarAgvBloqueo (true);
			}
			m_nAgvEmpujando = nAgv;	// Poner el Agv que empuja este
			MSGAVISO2 ("Agv: %d empujando Agv: %d",
				nAgv,
				m_id);
		}
	}

}


// Ver si la posicion del Agv es un cargador
bool CAgv::IsCargadorCurPos(bool fAll)
{
	switch (m_pFzdat->mp)
	{
	// Michael 24.05.2007 AGV9
	case 52:
	// Michael 24.05.2007 Fin
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
	// Dejar empujar si esta aquí case 60:
		return (true);
		break;
	default:
		if (fAll && m_pFzdat->mp == 60)
			return (true);
		else
			return (false);
	}


}


// Mirar a ver si se puede modificar el destino del Agv en este momento
bool CAgv::IsModifiableDestino()
{
	if (m_pFzdat->cUmdispoGesperrt == TRUE)
	{
		// Blocking ha puesto este flag para indicar que no se puede modificar el destino
		MSGAVISO3 ("Destino no modificable (cUmdispoGesperrt) para Agv: %d en mp: %d dest: %d",
			m_id,
			m_pFzdat->mp,
			m_pFzdat->auftziel.nDest);
		return (false);
	}
	else
	{
		// Michael 28.08.2002 Si el AGV no tiene destino, dejamos que se pueda modificar
		// para solucionar temas de bloqueos como el que pasó el día 21082002
		if (m_pFzdat->auftziel.nDest != 0)
		{

			// Michael 18.01.2002
			switch (m_pFzdat->mp)
			{
			case 248:
			case 249:
			case 201:
			case 202:
			case 203:
			case 253:
			case 252:
			// Michael 18.02.2002 case 237:
			case 238:
			case 239:
			case 236:	// Michael 29.07.2002 - Angel para evitar bloqueo en 10
				MSGAVISO3 ("Destino no modificable (por Mp) para Agv: %d en mp: %d dest: %d",
					m_id,
					m_pFzdat->mp,
					m_pFzdat->auftziel.nDest);
				return (false);
				break;
			default:
				return (true);
			}
		}
		else
			return (true);	// Michael 28.08.2002 No tiene destino

	}
}


// Mirar a ver si realmente se debe dejar empujar el Agv
bool CAgv::IsOkEmpujar()
{
	bool fRes;
	switch (m_pFzdat->mp)
	{
		case 59:
		case 58:
		case 57:
		case 55:
		case 56:
			// No tiene sentido salir de aquí
			fRes = false;
			break;
		default:
			fRes = true;
			break;
	}
	return (fRes);
}


// Mirar si el Agv está en una situación de deadlock
bool CAgv::IsDeadlocked()
{
	bool fRetVal = false;
	bool fNotBlocked = false;

	//UINT blockingAgvs [KKFZMAX];
	CUIntArray blockingAgvs;
	blockingAgvs.SetSize(g_pCourseData->GetNumberAGVs());
	UINT numBlocked = 0;

	UINT nextAgv = m_id;

	if (m_pFzdat->fz_blk != 0)
	{
		// Vehicle is blocked...
		for (int i = 1; i <= blockingAgvs.GetSize(); i++)
		{
			for (UINT j = 0; j < numBlocked && !fRetVal; j++)
			{
				if (blockingAgvs [j] == nextAgv)
				{
					if (m_uDeadlockCounter >= 3)
					{
						// Realmente está bloqueado
						MSGWARNING2 ("Agv: %d en deadlock con Agv: %d", m_id, blockingAgvs [j]);
						CAlarma * ptrAlarma = new CAlarma;
						ptrAlarma->AvisarAlarma (3, 32, 2, 
							m_id, 
							nextAgv);
						return (true);
					}
					else
					{
						MSGWARNING3 ("Agv: %d posiblemente deadlocked con Agv: %d DeadlockCounter: %d",
							m_id, blockingAgvs [j], m_uDeadlockCounter);
						m_uDeadlockCounter++;
						return (false);
					}
				}
			}
			blockingAgvs [numBlocked] = nextAgv;
			numBlocked ++;
			nextAgv = fzdat[nextAgv].fz_blk;
			CAgv * ptrAgv = g_pGestor->GetAgvPtr (nextAgv);
			if (ptrAgv)
			{
				if (fzdat[nextAgv].fz_blk == 0 || fzdat[nextAgv].FVvonBedienung == 1)
					// No está bloqueado este, o está bloqueado manualmente
				{
					m_uDeadlockCounter = 0;
					return (false);
				}
			}
		}
	}
	return fRetVal;

	
}


// Mirar si el destino donde este el Agv es especial y requiere comprobación especial
bool CAgv::OkDestinoEspecial()
{
	bool fRetVal = true;

	if (m_pOrden->m_at.fSimCarga /* Michael 19.04.02 /* || g_Container.m_fSimulateCom */)
		return (true);


	if (1 == m_pFzdat->mp || 2 == m_pFzdat->mp || 3 == m_pFzdat->mp)
	{
	UINT uRes = m_pOrden->m_ptrEstDes->GetDescargaLleno ((LPCTSTR)m_pOrden->m_at.id_nr);
		switch (uRes)
		{
		case 0:
			// Descargamos aquí
			fRetVal = true;
			break;
		case 1:
			switch (m_pFzdat->mp)
			{
			case 1:
				// Probamos el A2
				SetZiel (&m_pOrden->m_at.bz,
					2,
					(unsigned char)m_pOrden->m_at.bz.nPos,
					m_pOrden->m_at.bz.ucSide,
					0);

				g_GestorOrden.m_dbGestor.ModificarOrden (m_pOrden);	// Actualizar en la base de datos
				m_pOrden->m_ptrEstDes->GetEstAgv (
						m_pOrden->m_at.bz.nDest,
						m_pOrden->m_at.bz.nPos,
						m_pOrden->m_at.bz.ucSide);
				DestinoDiferente(IRDESCARGARORD);
				fRetVal = false;
				break;
			case 2:
				// Vamos a 3
				SetZiel (&m_pOrden->m_at.bz,
					3,
					(unsigned char)m_pOrden->m_at.bz.nPos,
					m_pOrden->m_at.bz.ucSide,
					0);
					m_pOrden->m_ptrEstDes->GetEstAgv (
						m_pOrden->m_at.bz.nDest,
						m_pOrden->m_at.bz.nPos,
						m_pOrden->m_at.bz.ucSide);
				g_GestorOrden.m_dbGestor.ModificarOrden (m_pOrden);	// Actualizar en la base de datos
				DestinoDiferente(IRDESCARGARORD);
				fRetVal = false;
				break;
			case 3:
				// wait
				fRetVal = false;
				break;
			}
			break;
		case 2:
			// Michael 18.04.2002 Faltaba devolver false en estos casos
			MSGERROR1 ("Error del PLC en la descarga en punto: %d", m_pFzdat->mp);
			fRetVal = false;
			break;
		case 3:
			MSGERROR1 ("Mesa %d en manual intentando descargar", m_pFzdat->mp);
			fRetVal = false;
			break;
		case 4:
			MSGAVISO1 ("Esperando descargar en posicion %d", m_pFzdat->mp);
			fRetVal = false;
			break;
		default:
			break;
		}
	}
	else
		// No es especial
		fRetVal = false;

	return (fRetVal);

}

bool CAgv::OkDestinoDescarga()
{
	bool fRetVal = true;
	bool fEnviarRechazo = false;

	if (m_pOrden->m_at.fSimCarga)
		return (true);
	
	if ('S' == m_pOrden->m_at.herkunft)
		// Si es una orden manual, deja descargar... 14.01.2002
		return (true);
		
	if (1 == m_pFzdat->mp || 2 == m_pFzdat->mp || 3 == m_pFzdat->mp)
		return (false);	// No tratado por aquí

	UINT uRes = m_pOrden->m_ptrEstDes->GetDescarga ((LPCTSTR)m_pOrden->m_at.id_nr);
	switch (uRes)
	{
	case 0:
		// Descargamos aquí
		fRetVal = true;
		break;
	case 1:
		// Ocupado - esperamos
		fRetVal = false;
		break;
	case 2:
		// Error PLC
		MSGERROR2 ("Agv: %d Error del PLC en la descarga en punto: %d", 
			m_id,
			m_pFzdat->mp);
		fRetVal = false;
		break;
	case 3:
		// Mesas en manual
		MSGERROR2 ("Agv: %d Mesa %d en manual intentando descargar", 
			m_id,
			m_pFzdat->mp);
		fRetVal = false;
		break;
	case 4:
		// No es de entrada
		MSGERROR2 ("Agv: %d no descarga por mesas de salida en posición %d", 
			m_id,
			m_pFzdat->mp);
		fEnviarRechazo = true;
		fRetVal = false;
		break;
	case 5:
		// Tren / Estación no quiere descarga
		MSGERROR2 ("Agv: %d no descarga en posición %d porque el tren / estación no lo permite",
			m_id,
			m_pFzdat->mp);
		fEnviarRechazo = true;
		fRetVal = false;
		break;
	}


	if (fEnviarRechazo)
	{
		
		SetDestinoRechazo ();
	}

	return (fRetVal);

}


// Modificar el destino a rechazo (descarga) desde una mesa...
bool CAgv::SetDestinoRechazo()
{
	bool fRes;

	if (!IsWlan())
	{
		typ_tele_info teleinfo;
		CString str;

		// Actualizar actual destination

		UCHAR pos;
		if (! MESA1CARGADA (*m_pFzdat)  && MESA2CARGADA (*m_pFzdat))
				// Solo mesa 2
			pos = 1;
		else
			pos = 2;

		SetZiel (&m_pOrden->m_at.bz,
			7,
			pos,	
			'L',
			0);

		g_GestorOrden.m_dbGestor.ModificarOrden (m_pOrden);	// Actualizar en la base de datos
		m_pOrden->m_ptrEstDes->GetEstAgv (
					m_pOrden->m_at.bz.nDest,
					m_pOrden->m_at.bz.nPos,
					m_pOrden->m_at.bz.ucSide);

		SetZiel (&m_pFzdat->auftziel, 
				7,
				pos,
				'L',
				0);


		MSGTRACE1 ("Agv:%d SetDestinoRechazo", m_id);

	/*
		USHORT nFz = m_pFzdat->fznr;
		GetMpFolge(nFz,pMpFolgeVerz[nFz-1],ptrZiel->nr);
		*(pCrMengeVerz[nFz-1]) = 0;
	 XXX-Lac04 */

		teleinfo.tele_art = 'I';

		str.Format ("%02d", m_id);
		strncpy ((char *)teleinfo.fznr, str, 2);

		str.Format ("%04d", m_pFzdat->auftziel.nDest);
		strncpy ((char *)teleinfo.auftziel, str, 4);

		teleinfo.auftpos = m_pFzdat->auftziel.nPos + 0x30; // Int to Char
		teleinfo.auftebene = m_pFzdat->auftziel.nLayer + 0x30; // Int to Char
		teleinfo.auftlage = m_pFzdat->auftziel.ucSide;


		// Michael 01.10.2001: Set Agv blocked initially ?

		teleinfo.kommando_0 = 0;
		SetBlocked (true);
		SetTeleBlocked (&teleinfo.kommando_0, true);

		teleinfo.kommando_0 |= BIT7; // Always 1

		teleinfo.kommando_1 = 0;

		if (m_pOrden->m_at.fMesa1)
			teleinfo.kommando_1 = BIT2;		// Conveyor 1 delivery

		if (m_pOrden->m_at.fMesa2)
			teleinfo.kommando_1 |= BIT4;	// Conveyor 2 delivery


		teleinfo.kommando_1 |= BIT7;	// Always 1

		if (m_pOrden->m_at.fSimCarga)
			teleinfo.kommando_1 |= BIT6;	// Simulate delivery

	if (!g_Container.m_fSimulateCom)
		fRes = SendInfoTelegram (teleinfo);
	else
		fRes = SimulateInfoTelegram (teleinfo);
	} // Wlan
	else
	{
		typ_tele_info_wlan teleinfo;
		CString str;

		// Actualizar actual destination

		UCHAR pos;
		if (! MESA1CARGADA (*m_pFzdat)  && MESA2CARGADA (*m_pFzdat))
				// Solo mesa 2
			pos = 1;
		else
			pos = 2;

		// TODO M2015 - set rechazo layer to 1 for new AGVs?
		SetZiel (&m_pOrden->m_at.bz,
			7,
			pos,	
			'L',
			0); 

		g_GestorOrden.m_dbGestor.ModificarOrden (m_pOrden);	// Actualizar en la base de datos
		m_pOrden->m_ptrEstDes->GetEstAgv (
					m_pOrden->m_at.bz.nDest,
					m_pOrden->m_at.bz.nPos,
					m_pOrden->m_at.bz.ucSide);

		SetZiel (&m_pFzdat->auftziel, 
				7,
				pos,
				'L',
				0);

		MSGTRACE1 ("Agv:%d SetDestinoRechazo", m_id);


		teleinfo.tele_art = 'I';

		str.Format ("%02d", m_id);
		strncpy ((char *)teleinfo.fznr, str, 2);

		str.Format ("%04d", m_pFzdat->auftziel.nDest);
		strncpy ((char *)teleinfo.auftziel, str, 4);


		sprintf((char *)teleinfo.auftpos, "%02d",m_pFzdat->auftziel.nPos);
		// Michael M2015 layer is always 1 for new AGVs
		// TODO - see can this be changed as seems to be 0 for old agvs m_pFzdat->auftziel.nLayer = 1;

		teleinfo.auftebene = m_pFzdat->auftziel.nLayer + 0x30; // Int to Char
		teleinfo.auftebene = 1 + 0x30; // Int to Char

		teleinfo.auftlage = m_pFzdat->auftziel.ucSide;

		str.Format ("%04d", m_pFzdat->nmp);
		strncpy ((char *)teleinfo.nmp, str, 4);
		str.Format ("%04d", m_pFzdat->uemp);
		strncpy ((char *)teleinfo.uemp, str, 4);


		// Michael 01.10.2001: Set Agv blocked initially ?

		teleinfo.kommando_0 = 0;
		SetBlocked (true);
		SetTeleBlocked (&teleinfo.kommando_0, true);

		teleinfo.kommando_0 |= BIT7; // Always 1

		teleinfo.kommando_1 = 0;

		if (m_pOrden->m_at.fMesa1)
			teleinfo.kommando_1 = BIT2;		// Conveyor 1 delivery

		if (m_pOrden->m_at.fMesa2)
			teleinfo.kommando_1 |= BIT4;	// Conveyor 2 delivery


		teleinfo.kommando_1 |= BIT7;	// Always 1

		if (m_pOrden->m_at.fSimCarga)
			teleinfo.kommando_1 |= BIT6;	// Simulate delivery

		teleinfo.kommando_2 = m_pFzdat->Status2;

	if (!g_Container.m_fSimulateCom)
	{
		// TODO - check bits etc...
		m_WlanTel.SetInfoTelegram (teleinfo, m_pWlanMsg);
		fRes = true;
	}
//	else
//		fRes = SimulateInfoTelegram (teleinfo);

	}

	if (fRes)
	{
		m_fWillUpdateBuB = true;
	}
	return (fRes);


}


// Ver si un destino de carga está bien en el momento de tener el Agv delante
bool CAgv::OkDestinoCarga()
{
	if (m_pOrden->m_at.fSimCarga)
		return (true);

	bool fRetVal = true;
	bool fCancelarOrden = false;

	UINT uRes = m_pOrden->m_ptrEstOri->GetCarga ((LPCTSTR)m_pOrden->m_at.id_nr);

	switch (uRes)
	{
	case 0:
		// Descargamos aquí
		fRetVal = true;
		break;
	case 1:
		// No Ocupado - esperamos - o que ???
		fRetVal = false;
		break;
	case 2:
		// Error PLC
		MSGERROR2 ("Agv: %d Error del PLC en la carga en punto: %d", 
			m_id,
			m_pFzdat->mp);
		fCancelarOrden = true;
		fRetVal = false;
		break;
	case 3:
		// Mesas en manual
		MSGERROR2 ("Agv: %d Mesa %d en manual intentando cargar", 
			m_id,
			m_pFzdat->mp);
		fCancelarOrden = true;
		fRetVal = false;
		break;
	case 4:
		// No es de salida
		MSGERROR2 ("Agv: %d no carga por mesas de salida en posición %d", 
			m_id,
			m_pFzdat->mp);
		fCancelarOrden = true;
		fRetVal = false;
		break;
	case 5:
		// Tren / Estación no quiere descarga
		MSGERROR2 ("Agv: %d no carga en posición %d porque el tren / estación no lo permite",
			m_id,
			m_pFzdat->mp);
		fCancelarOrden = true;
		fRetVal = false;
		break;
	}


	if (fCancelarOrden)
	{

		m_pOrden->SetEstado (CANCELLED);
		ResetOrden ();
		SetEstado (AGVSINDATOS);
	}

	return (fRetVal);


}


// Ver si hay otro Agv en camino a un cargador para no enviar este de momento
bool CAgv::IsIrCargarBat()
{
	bool fRetVal = false;
	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	{
		CAgv * ptrAgv = g_pGestor->GetAgvPtr (i + 1);
		if (IRCARGARBAT == ptrAgv->GetEstado ())
		{
			fRetVal = true;
			break;
		}
	}
	return (fRetVal);
}

/*
//------------------------------------------------------------------------------
//   Buscar un nmp alternativo para el caso de un Agv que detecta un deadlock
//   This function is taken from the waker project: ..\steur\blocking\ublock.c
//   Michael 23.02.2002
//	Possibly the vehicle has been sent an nmp which it cannot reach because for the
//	blocking distance it has passed the curve for the nmp.
//	In this case it remains blocked, and will call this function for a second time.
//------------------------------------------------------------------------------
	USHORT CAgv::Umleiten(bool &bNCp)
{
	USHORT nmp = 0, i;
	Meldepunkttabelle *strtab;
	NMP_tabelle       *mp_nmp_tab;
	
	TYP_FZDAT fzDatCopy;

	
	strtab  = mptab + vmptab [m_pFzdat->mp] - 1;
	bNCp = true;
	
	if (strtab->anz_nmp > 1)
	{
		// there are more than one Nmps
		mp_nmp_tab    = MpNmpTab + (strtab->str_idx - 1);
		for (i = 1; (i <= strtab->anz_nmp) && !nmp; i++)
		{
			if (mp_nmp_tab->nmp != m_pFzdat->nmp 
				&& mp_nmp_tab->nmp != m_pFzdat->mp
				// Michael 23.02.2002
				&& mp_nmp_tab->nmp != m_lastUmleitMp)
				// Michael 23.02.2002 fin
			{
				// Note: Michael: Only give a new nmp to a vehicle if the blocking way is the same as in the old nmp
				// This function compares the blocking ways for the Nmps
				if(GetDiffBlkWegPaar(mp_nmp_tab))
				{
					// Copy m_pFzdat structure for the vehicle, set the nmp member to the nmp to be tested...(mp_nmp_tab->nmp)
					//Michael: Call SetFVAusBlTab(m_pFzdat, TRUE); // TRUE to test only...
					//Call GetNmp function to establish the uemp for the nmp... and set this
					//	also in the structure
					
					fzDatCopy = *m_pFzdat;
					fzDatCopy.nmp = mp_nmp_tab->nmp;
					NMP_tabelle * ptrNmp = GetPNmp (mp_nmp_tab->nmp, m_pFzdat->auftziel.nDest);
					fzDatCopy.uemp = ptrNmp->nmp;
					if (// vtstat[mp_nmp_tab->nmp] == 0xFFFF  // XXX why ? 
							mp_nmp_tab->nmp != 60 // 60 is no good !!!
							&& mp_nmp_tab->nmp != 59 // 59 is no good !!!
							&& mp_nmp_tab->nmp != 58 // 58 is no good !!!
							&& mp_nmp_tab->nmp != 57 // 57 is no good !!!
							&& ! SetFVAusBlTab(&fzDatCopy, TRUE))
					{
						MSGAVISO3 ("Umleiten para Agv %d mp %d not blocked for nmp %d",
							m_id,
							m_pFzdat->mp,
							mp_nmp_tab->nmp);

						CAlarma * ptrAlarma = new CAlarma;
						ptrAlarma->AvisarAlarma (2, 18, 3, m_id, m_pFzdat->mp, mp_nmp_tab->nmp);
						nmp = mp_nmp_tab->nmp;
					}
				}
			}
			mp_nmp_tab++;
		}
    }
	if (nmp == 0)
	{
		bNCp = false;
		MSGAVISO1 ("Umleiten para Agv %d - no se ha encontrado nmp alternativo (1)",
			m_id);
		
		// Didnt get an alternative so look at alternatives from the nmp
		strtab  = mptab + vmptab [m_pFzdat->nmp] - 1;
		mp_nmp_tab    = MpNmpTab + (strtab->str_idx - 1);
		
		for (i=1; i <= strtab->anz_nmp; i++)
		{
			if (mp_nmp_tab->nmp == m_pFzdat->mp 
				|| mp_nmp_tab->nmp == m_pFzdat->uemp 
				|| mp_nmp_tab->nmp == m_pFzdat->nmp)
			{
				mp_nmp_tab++;
			}
			else
			{
				// strecke_belegt: Test if the distance is occupied by another vehicle
				// This time file the fzdat copy with this nmp as the mp and the uemp as the nmp !!
				if (!IsOccupiedCP (mp_nmp_tab->nmp))
				{
					fzDatCopy = *m_pFzdat;
					fzDatCopy.uemp = mp_nmp_tab->nmp;

					if (true // vtstat[mp_nmp_tab->nmp] == 0xFFFF  // XXX why ?
						&& mp_nmp_tab->nmp != 60 // 60 is no good !!!
						&& mp_nmp_tab->nmp != 59 // 59 is no good !!!
						&& mp_nmp_tab->nmp != 58 // 58 is no good !!!
						&& mp_nmp_tab->nmp != 57 // 57 is no good !!!
						&& ! SetFVAusBlTab(&fzDatCopy, TRUE))
					{
						MSGAVISO3 ("Umleiten para Agv %d mp %d not blocked for nmp (uemp) %d",
							m_id,
							m_pFzdat->mp,
							mp_nmp_tab->nmp,
							);

						CAlarma * ptrAlarma = new CAlarma;
						ptrAlarma->AvisarAlarma (2, 18, 3, m_id, m_pFzdat->mp, mp_nmp_tab->nmp);
						nmp = mp_nmp_tab->nmp;
						break;
					}
				}
				mp_nmp_tab++;

			}
		}
	}
	
	if (nmp == 0)
	{
		MSGAVISO1 ("Umleiten para Agv %d - no se ha encontrado nmp alternativo (2)",
		m_id);
	}
	else
		m_uDeadlockCounter = 0;	// Resetear mientras se pruebe
	

	// Michael 10.01.2002 Perhaps in the second case it is the UEMP which should be
	// modified for the AGV and not the NMP. Try in the instalation.
	return nmp;
	
	
}
*/

//------------------------------------------------------------------------------
//   Buscar un nmp alternativo para el caso de un Agv que detecta un deadlock
//   This function is taken from the waker project: ..\steur\blocking\ublock.c
//   Michael 23.02.2002
//	Possibly the vehicle has been sent an nmp which it cannot reach because for the
//	blocking distance it has passed the curve for the nmp.
//	In this case it remains blocked, and will call this function for a second time.
//------------------------------------------------------------------------------
USHORT CAgv::Umleiten(bool &bNCp)
{
	USHORT nmp = 0, i;
	//Meldepunkttabelle *strtab;
	//strtab = mptab + vmptab[m_pFzdat->mp] - 1;
	CCommPoint* pCommPoint = pAGVTCourse->GetPtrCommPoint(m_pFzdat->mp);

	//NMP_tabelle       *mp_nmp_tab;
	CCpDistance* pCpDistance;

	TYP_FZDAT fzDatCopy;


	bNCp = true;

	//if (strtab->anz_nmp > 1)
	if (pCommPoint->GetNCps() > 1)
	{
		// there are more than one Nmps
		//mp_nmp_tab = MpNmpTab + (strtab->str_idx - 1);
		int nDistIndex = pCommPoint->GetDistanceIndex();
		for (i = 1; (i <= pCommPoint->GetNCps()) && !nmp; i++)
		{
			pCpDistance = pAGVTCourse->GetPtrDistance(nDistIndex);
			if (pCpDistance->GetNCp() != m_pFzdat->nmp
				&& pCpDistance->GetNCp() != m_pFzdat->mp
				// Michael 23.02.2002
				&& pCpDistance->GetNCp() != m_lastUmleitMp)
				// Michael 23.02.2002 fin
			{
				// Note: Michael: Only give a new nmp to a vehicle if the blocking way is the same as in the old nmp
				// This function compares the blocking ways for the Nmps
				if (GetDiffBlkWegPaar(pCpDistance))
				{
					// Copy m_pFzdat structure for the vehicle, set the nmp member to the nmp to be tested...(mp_nmp_tab->nmp)
					//Michael: Call SetFVAusBlTab(m_pFzdat, TRUE); // TRUE to test only...
					//Call GetNmp function to establish the uemp for the nmp... and set this
					//	also in the structure

					fzDatCopy = *m_pFzdat;
					fzDatCopy.nmp = pCpDistance->GetNCp();
					//NMP_tabelle * ptrNmp = GetPNmp(mp_nmp_tab->nmp, m_pFzdat->auftziel.nDest);
					CCpDistance* pNextCpDistance = GetPNmp(pCpDistance->GetNCp(), m_pFzdat->auftziel.nDest);
					fzDatCopy.uemp = pNextCpDistance->GetNCp();
					if (// vtstat[mp_nmp_tab->nmp] == 0xFFFF  // XXX why ? 
						//mp_nmp_tab->nmp != 60 // 60 is no good !!!
						//&& mp_nmp_tab->nmp != 59 // 59 is no good !!!
						//&& mp_nmp_tab->nmp != 58 // 58 is no good !!!
						//&& mp_nmp_tab->nmp != 57 // 57 is no good !!!
						//&& !SetFVAusBlTab(&fzDatCopy, TRUE))
						pCpDistance->GetNCp() != 60 // 60 is no good !!!
						&& pCpDistance->GetNCp() != 59 // 59 is no good !!!
						&& pCpDistance->GetNCp() != 58 // 58 is no good !!!
						&& pCpDistance->GetNCp() != 57 // 57 is no good !!!
						&& !SetFVAusBlTab(&fzDatCopy, TRUE))
					{
						MSGAVISO3("Umleiten para Agv: %d mp %d not blocked for nmp %d",
							m_id,
							m_pFzdat->mp,
							pCpDistance->GetNCp());

						CAlarma * ptrAlarma = new CAlarma;
						ptrAlarma->AvisarAlarma(2, 18, 3, m_id, m_pFzdat->mp, pCpDistance->GetNCp());
						nmp = pCpDistance->GetNCp();
					}
				}
			}

			//mp_nmp_tab++;
			nDistIndex++;
		}
	}
	if (nmp == 0)
	{
		bNCp = false;
		MSGAVISO1("Umleiten para Agv: %d - no se ha encontrado nmp alternativo (1)",
			m_id);

		// Didnt get an alternative so look at alternatives from the nmp
		//strtab = mptab + vmptab[m_pFzdat->nmp] - 1;
		pCommPoint = pAGVTCourse->GetPtrCommPoint(m_pFzdat->nmp);

		int nDistIndex = pCommPoint->GetDistanceIndex();

		for (i = 1; i <= pCommPoint->GetNCps(); i++)
		{
			pCpDistance = pAGVTCourse->GetPtrDistance(nDistIndex);
			if (pCpDistance->GetNCp() == m_pFzdat->mp
				|| pCpDistance->GetNCp() == m_pFzdat->uemp
				|| pCpDistance->GetNCp() == m_pFzdat->nmp)
			{
				//mp_nmp_tab++;
				nDistIndex++;
			}
			else
			{
				// strecke_belegt: Test if the distance is occupied by another vehicle
				// This time file the fzdat copy with this nmp as the mp and the uemp as the nmp !!
				if (!IsOccupiedCP(pCpDistance->GetNCp()))
				{
					fzDatCopy = *m_pFzdat;
					fzDatCopy.uemp = pCpDistance->GetNCp();

					if (true // vtstat[mp_nmp_tab->nmp] == 0xFFFF  // XXX why ?
						//&& mp_nmp_tab->nmp != 60 // 60 is no good !!!
						//&& mp_nmp_tab->nmp != 59 // 59 is no good !!!
						//&& mp_nmp_tab->nmp != 58 // 58 is no good !!!
						//&& mp_nmp_tab->nmp != 57 // 57 is no good !!!
						//&& !SetFVAusBlTab(&fzDatCopy, TRUE))
						&& pCpDistance->GetNCp() != 60 // 60 is no good !!!
						&& pCpDistance->GetNCp() != 59 // 59 is no good !!!
						&& pCpDistance->GetNCp() != 58 // 58 is no good !!!
						&& pCpDistance->GetNCp() != 57 // 57 is no good !!!
						&& !SetFVAusBlTab(&fzDatCopy, TRUE))
					{
						MSGAVISO3("Umleiten para Agv: %d mp %d not blocked for nmp (uemp) %d",
							m_id,
							m_pFzdat->mp,
							pCpDistance->GetNCp()
							);

						CAlarma * ptrAlarma = new CAlarma;
						ptrAlarma->AvisarAlarma(2, 18, 3, m_id, m_pFzdat->mp, pCpDistance->GetNCp());
						nmp = pCpDistance->GetNCp();
						break;
					}
				}
				//mp_nmp_tab++;
				nDistIndex++;
			}
		}
	}

	if (nmp == 0)
	{
		MSGAVISO1("Umleiten para Agv: %d - no se ha encontrado nmp alternativo (2)",
			m_id);
	}
	else
		m_uDeadlockCounter = 0;	// Resetear mientras se pruebe


	// Michael 10.01.2002 Perhaps in the second case it is the UEMP which should be
	// modified for the AGV and not the NMP. Try in the instalation.
	return nmp;
}






// Enviar umleit telegram al Agv
bool CAgv::SendUmleitTelegram(UINT nmp)
{

	int fRetVal;
	int i;
	bool fAceptado;

	if (IsWlan())
	{
		WlanSendUmleitTelegram(nmp);
		return true;	// TODO
	}
	
	typ_tele_umleit tele;
	CString str;

	tele.tele_art = 'U';
	str.Format ("%02d", m_id);
	strncpy ((char *)tele.fznr, str, 2);

	tele.kommando_0 = 0;

	SetBlocked (true);
	SetTeleBlocked (&tele.kommando_0, true);

	tele.kommando_0 |= BIT7; // Always 1

	str.Format ("%04d", nmp);
	strncpy ((char *)tele.nmp, str, 4);

	// Guardar los datos del commando por si...
	// M2015 	m_pFzdat->msollfz0 = tele.kommando_0;
	m_pFzdat->Kommando0 = tele.kommando_0;

	for (i = 0, fAceptado = false; i < 3 && !fAceptado; i++)
	{
	m_Tel.SetUmleitTelegram (tele);
	fRetVal = m_pSerCom->SendTelegrama (m_Tel.m_strTelegrama);
	if (fRetVal)
		{
		// El telegrama requiere respuesta
		fRetVal = m_pSerCom->ReceiveTelegrama (m_Tel.m_strRespuesta);

		switch (fRetVal)
		{
		case WAIT_OBJECT_0:
			// se ha recibido el telegrama de respuesta
			m_Tel.SetTeleRecibido ();
			// XXX se debe comprobar el telegrama ???
			fRetVal = m_pSerCom->SendTelegrama (m_Tel.StrQuitum (m_id));
			fAceptado = true;
			// Michael 23.02.2002. Guardar MP enviado por si no recibe drive permision
			m_lastUmleitMp = nmp;
			// Michael 23.02.2002 fin
			break;
		case WAIT_TIMEOUT:
			m_Tel.SetTeleTout ();
			MSGTRACE1 ("Timeout waiting for SendUmleitTelegram reply Agv: %d", m_id);
			break;
		default:
			// XXX mirar posibilidades aquí
			m_Tel.SetTeleTout ();
			MSGERROR1 ("ReceiveTelegrama ha devuelto: %d (SendUmleitTelegram)", fRetVal);
			break;
			}
		}
		else
		{
			m_Tel.SetTeleNull ();
		}
	} // for (..)
	return (fAceptado);
}


// Enviar umleit telegram al Agv
bool CAgv::WlanSendUmleitTelegram(UINT nmp)
{
	// M2015 Michael 09.05.2015
	// MSGAVISO ("Saltando WlanSendUmleitTelegram");
	// M2015 Michael 09.05.2015 return true;
	
	
	bool fAceptado = true;
	
	typ_tele_umleit_wlan tele;
	CString str;

	tele.tele_art = 'U';
	str.Format ("%02d", m_id);
	strncpy ((char *)tele.fznr, str, 2);

	SetBlocked (true);
	tele.kommando_0 = 0;
	tele.kommando_0 &= OFFBIT1;	// Blocked

	tele.kommando_0 |= BIT7; // Always 1

	str.Format ("%04d", nmp);
	strncpy ((char *)tele.nmp, str, 4);

	// Michael M2015 09.05.2015 get uemp for mp, nmp
	UINT uemp = GetNmp (nmp, m_pFzdat->fzziel.nDest);
	str.Format ("%04d", uemp);	
	strncpy ((char *)tele.uemp, str, 4);

	
	m_lastUmleitMp = nmp;



	// Guardar los datos del commando por si...
	// M2015 	m_pFzdat->msollfz0 = tele.kommando_0;
	m_pFzdat->Kommando0 = tele.kommando_0;

	m_WlanTel.SetUmleitTelegram (tele, m_pWlanMsg);
	
	return (fAceptado);
}

// Ver si hay diferencia entre distancia de blocking de Agv actual y nmp alternativo
// This function is adapted from ..\wacker\anlag_sw\steuer\allgem\utility.c with same name
bool CAgv::GetDiffBlkWegPaar(CCpDistance* pCpDistance)
{
  bool fRetVal = false;

  
  if (m_pFzdat->mppos)
  {
    // El Agv está en una estación
	  if (m_pFzdat->wegstr > pCpDistance->GetBlockWay())
    {
      fRetVal = true;
    }
  }
  else
  {
    // Ok si la distancia recorrido es inferior a la distancia de blocking del punto propuesto
	  if (m_pFzdat->wegstr <= pCpDistance->GetBlockWay())
    {
      fRetVal = true;
    }
  }

  return (fRetVal);
}

bool CAgv::IsAparcado()
{
	bool fIsAparcado = false;
	if (!IsEnCircuito ())
	{	
		// Michael 05.12.2001: No considerar el punto 60
		for (int i = 0; i < 7 && !fIsAparcado; i++)
		{
			if (m_pFzdat->mp == g_Container.m_uCargadorArray [i])
				fIsAparcado = true;
		}
	}
	return (fIsAparcado);
}

// Buscar punto de parking para un Agv. Opcionalmente quitar del circuito
void CAgv::GetPuntoParking(TYPE_STATION_DEF *ptrZiel, UINT curMp, bool fSetFuera)
{
	// Michael 23.02.2002: No enviar a parking si está el Sistema en FinTurno

	// Michael 05.12.2001: No incluir la estación 60 como punto de parking...
	if (!g_Container.IsFinDeJornada ())
	{
 		for (int i = 0; i < 7; i++)
		{
			if (!IsOccupiedCP (g_Container.m_uCargadorArray [i]) && !IsOccupiedCP (g_Container.m_uCargadorArray [i] + 100))
			{
				switch (g_Container.m_uCargadorArray [i])
				{
				case 53: // Mirar si hay algo en el 55
				case 54: // Mirar si hay algo en el 56
					if (IsDestinoCP (g_Container.m_uCargadorArray [i] + 2)
						// Michael 14.01.2002 Si hay en el 155 o 156 tampoco
						|| IsOccupiedCP (g_Container.m_uCargadorArray [i] + 102))
					{
						// Hay otro que va detras....
						continue;
					}
				}
				ptrZiel->nDest = g_Container.m_uCargadorArray [i];
				ptrZiel->nPos = 1;		// Always
				ptrZiel->ucSide = 'L';	// Always
				ptrZiel->nLayer = 0;		// Always
				if (fSetFuera)
				{
					SetFueraCircuito ();
					g_Container.m_fSendSystemStatusToBuB = true;
				}
				return;
			}
		}
	} // Fin de Jornada


	CAlarma * ptrAlarma = new CAlarma;
	ptrAlarma->AvisarAlarma (2, 19, 1, 
		m_id);

	// Para mandarle a algún sitio
	if (fSetFuera)
		GetPuntoDescanso (ptrZiel, curMp);
}


// Ver si se debe abandonar el punto de descanso
bool CAgv::IsSalirPuntoDescanso()
{
	bool fRetVal = false;
	if (!IsAparcado ())
	{
		if (!IsEnCircuito ())
			fRetVal = true;
		else if (!IsPuntoDescansoCurPos ())
			// Michael 26.07.2002 Si está en un punto de parking y no se necesita, 
			// no salir
			/* Michael 30.07.2002 Quitar de momento
			if (IsCargadorCurPos () && g_pGestor->GetAgvsEnCircuitoIdle (m_id) > 1)
			{
				MSGAVISO2 ("No se saca AGV %d del punto %d por no necesitar", m_id, m_pFzdat->mp);
			}
			else
			*/
				fRetVal = true;

	}
	return (fRetVal);
}


// Determinar si el punto actual es un punto de descanso
bool CAgv::IsPuntoDescansoCurPos()
{
	bool fIsPunto = false;
	/* Michael 23.02.2002 Si estamos en fin de turno, no sacar el Agv del punto 
	   donde esté descansando hasta que le toque ir a cargar bateria
	 */
	if (g_Container.IsEnFuncionamiento ())
	{

		CSingleLock lockPuntos (&g_Container.m_csPuntoDescanso);
		lockPuntos.Lock ();

		UINT numPuntos = g_Container.m_PuntoDescansoArray.GetSize ();
		for (UINT i = 0; i < numPuntos; i++)
		{
			CPuntoDescanso * ptrPuntoDescanso =
				g_Container.m_PuntoDescansoArray.GetAt (i);

			if (m_pFzdat->mp == ptrPuntoDescanso->GetPtoCom ())
			{
				fIsPunto = true;
				break;
			}
		}

		lockPuntos.Unlock ();
	}
	else
		fIsPunto = true;	// Si no está en funcionamiento normal

	return (fIsPunto);


}

// See if another Agv has a destination this CP (but has not arrived yet)
bool CAgv::IsDestinoCP(UINT cp)
{
	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	{
		CAgv * ptrAgv = g_pGestor->GetAgvPtr (i + 1);
		if (AGVFUERASISTEMA !=ptrAgv->GetEstado ())
		{
			if (ptrAgv->m_pFzdat->mp != cp
				&& ptrAgv->m_pFzdat->auftziel.nDest == cp)
				return (true);
		}
	}
	return (false);

}

/*
// Ver si la posición actual es un Knot (en el arranque)
UINT CAgv::GetKnotCurPos()
{
	// Get mp data for  mp
	Meldepunkttabelle  * ptrMp = mptab + (vmptab [m_pFzdat->mp] - 1); 
	return ((UINT)ptrMp->ktn_nr);

}
*/

// Ver si la posición actual es un Knot (en el arranque)

UINT CAgv::GetKnotCurPos()
{
	// Get mp data for  mp
	return g_pCourseData->GetAGVInManBlockArea(m_pFzdat->mp, 0);

}

// Simular una telegrama tipo umleit 
void CAgv::SimulateUmleitTelegram(UINT altNmp,bool bNCp)
{
	if(bNCp)
	{
		m_pFzdat->nmp = altNmp;
		m_pFzdat->uemp = GetNmp (altNmp, m_pFzdat->fzziel.nDest);
	}
	else
	{
		m_pFzdat->uemp = altNmp;
	}
}

/* Inicializar contador de espera dependiendo donde este el AGV
   En estado AGVSINDATOS se esperan unos ciclos a ver si le cae una orden
   antes de proceder a un punto de espera.
   En algunas estaciones no tiene sentido esperar.
 */

void CAgv::SetCountEsperar()
{
	switch (m_pFzdat->mp)
	{
	case 1:
	case 2:
	case 3:
		m_countEsperar = 10;	// para que no espere
		break;
	default:
		m_countEsperar = 0;
	}
}

// Mirar si la posición 60 está ocupada por otro Agv.

bool CAgv::IsOccupied60()
{
	bool fRetVal;

	// Michael 25.05.2007
	// Don't block if fin de jornada
	if (g_Container.IsFinDeJornada ()) 
		return false;

	if (60 == m_pFzdat->mp || 160 == m_pFzdat->mp || 60 == m_pFzdat->fzziel.nDest)
		fRetVal = false;	// Está ocupada por este o destino de este
	else
		if (IsOccupiedCP (60) || IsOccupiedCP (160))
			fRetVal = true;
		else
			fRetVal = false;

	if (fRetVal)
	{
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (2, 20, 1, 
			m_id);
	}
	return (fRetVal);
	

}


// Ver si el punto de carga 60 es válido. Solo cuando todos carros aparcados
// Michael 25.05.2007 Ocupar el 60 con otro agv pendiente de entrar
bool CAgv::IsOk60()
{
	int agvsFuera = 0;
	//for (UINT i = 0; i < KKFZMAX; i++)
	for (UINT i = 0; i < (UINT)g_pCourseData->GetNumberAGVs(); i++)
	{
		if (i + 1 != m_id)
		{
			CAgv * ptrAgv = g_pGestor->GetAgvPtr (i + 1);
			if (ptrAgv)
			{
			if (AGVFUERASISTEMA !=ptrAgv->GetEstado ())
			{
				if (! ptrAgv->IsCargadorCurPos ())
					// Cualquier carro que no esté en un cargador es un peligro !!!
				{	
					// Michael 25.05.2007 AGV7
					agvsFuera ++;
					if (agvsFuera > 1)
						return (false);
					
				}
			}
			}
		}
	}
	return (true);

}

// Ver si un Agv será candidata de una orden ahora o cuando acabe lo que está haciendo
bool CAgv::IsPossibleCandidateForOrder()
{
	bool fRetVal = false;
	if (TEST_FZ_IM_SYSTEM(*m_pFzdat) && 
		m_fHasBeenPolled  && 
		!FZ_HAT_FEHLER(*m_pFzdat) && 
		!IsDriveForbiddenByBuB () &&
		m_fEnCircuito)
	{
		switch (m_estado)
		{
		case AGVPERDIDO:
		case IRCARGARBAT:
		case CARGARBATWAITPLC:
		case CARGARBAT:
			fRetVal = false;
			break;
		default:
			fRetVal = true;
			break;
		}
	}
	else
		fRetVal = false;
	return (fRetVal);
	
}

/*
	Xavi, 02/11/05: Indica si el cami alternatiu esta actiu
 */

bool CAgv::CaminoAltActivado()
{
	return g_Container.m_fCaminoAlternativo;
}


/* Ver si se debe ir por el camino alternativo
	<true> : No se necesita o ha sido embarcado por el camino alternativo
	<false> : Se necesita el camino alternativo y no está disponible
 */

bool CAgv::QuizasCaminoAlt()
{
	bool fRetVal = true;
	USHORT destino;
	
	// Ver que destino tiene
	if (m_pOrden && 'S' == m_pOrden->m_at.herkunft)
		// Orden Manual
		destino = m_pOrden->m_at.az.nDest;
	else
		destino = m_pFzdat->auftziel.nDest;

	if (m_pFzdat->mp == 208)
	{
		MSGAVISO4 ("QuizasCaminoAlt Agv: %d en mp %d con nmp %d y destino %d",
			m_id,
			m_pFzdat->mp,
			m_pFzdat->nmp,
			destino);
	}


	// Michael 07.06.2005
	if (m_pFzdat->mp == 208 && m_pFzdat->nmp == 209
		  && (destino == 41 || destino == 42 || destino == 43))
	{
		// Possiblemente por el camino de arriba
		if (g_Container.m_fCaminoAlternativo)
		{
			TYP_FZDAT fzDatCopy;
			fzDatCopy = *m_pFzdat;
			fzDatCopy.nmp = 210;
			//NMP_tabelle * ptrNmp = GetPNmp (210, destino);
			CCpDistance* pCpDistance = GetPNmp(210, destino);
			//fzDatCopy.uemp = ptrNmp->nmp;
			fzDatCopy.uemp = pCpDistance->GetNCp();
			if (!SetFVAusBlTab(&fzDatCopy, TRUE))
			{
				MSGAVISO1 ("Enviando AGV: %d por camino alternativo", m_id);
				if (!g_Container.m_fSimulateCom)
					SendUmleitTelegram (fzDatCopy.nmp);
				else
					SimulateUmleitTelegram (fzDatCopy.nmp,true);
			}
			else
			{
				// Hay que enviar por arriba, pero está bloqueado
				MSGAVISO1 ("Se debe enviar AGV:%d por camino de arriba pero está bloqueado",
						m_id);
				fRetVal = false;
			}
		}
	}

	return (fRetVal);

}


/* Comprobar si se debe enviar el Agv a la posición de mantenimiento
 * Michael 14.06.2005 T5K
 */

bool CAgv::SEnviarMantenimiento()
{
	if (m_nMantenimiento != 0)
		return true;
	else
		return false;

}

/*
 * Si el AGV debe ir a mantenimiento, intenta encaminarlo
 * Michael 14.06.2005 T5K
 * M2015 TODO: No contempla AGV Wlan
 */
bool CAgv::EnviarMantenimiento()
{
	TYPE_STATION_DEF zMantenimiento; // Punto destino de mantenimiento
	zMantenimiento.nDest = 0;
	typ_tele_info teleinfo;
	CString str;
	bool fRes = true;

	// No lo enviamos si esta bloqueado
	if (IRPUNTOESP == m_estado && IsBlocked ())
		return (false);

	// No lo enviamos si tiene carga
	if (MESA1CARGADA (*m_pFzdat) || MESA2CARGADA (*m_pFzdat))
		return (false);

	// No lo enviamos si el CP destino está ocupado
	if (IsOccupiedCP(m_nMantenimiento))
		return (false);


	zMantenimiento.nDest = m_nMantenimiento;
	zMantenimiento.nPos = 1;		// Always
	zMantenimiento.ucSide = 'L';	// Always
	zMantenimiento.nLayer = 0;	// Always


	SetZiel (&m_pFzdat->auftziel,
			zMantenimiento.nDest,
			(unsigned char)zMantenimiento.nPos,
			zMantenimiento.ucSide,
			zMantenimiento.nLayer);

	if (!SLlegadoDestino ())
	{

		teleinfo.tele_art = 'I';

		str.Format ("%02d", m_id);
		strncpy ((char *)teleinfo.fznr, str, 2);

		str.Format ("%04d", zMantenimiento.nDest);
		strncpy ((char *)teleinfo.auftziel, str, 4);

		teleinfo.auftpos = zMantenimiento.nPos + 0x30; // Int to Char
		teleinfo.auftebene = zMantenimiento.nLayer + 0x30; // Int to Char
		teleinfo.auftlage = zMantenimiento.ucSide;

		teleinfo.kommando_0 = 0;
		teleinfo.kommando_0 |= BIT7;	// Always 1

		// Enviar bloqueado siempre 12.10.2001
		SetBlocked (true);
		SetTeleBlocked (&teleinfo.kommando_0, true);

		teleinfo.kommando_0 |= BIT3;	// Action stopped (Don't load)

		teleinfo.kommando_1 = BIT7;		// Always 1


		MSGAVISO2 ("Enviando Agv: %d a mantenimiento en pos %d",
					m_id,
					zMantenimiento.nDest);


	if (!g_Container.m_fSimulateCom)
		fRes = SendInfoTelegram (teleinfo);
	else
		fRes = SimulateInfoTelegram (teleinfo);
	} // Llegado destino - no hace falta enviarle telegrama
	
	// Si estuviera en el circuito, quitala
	if (fRes)
		SetFueraCircuito ();

	// fRes is false when there is no cargador or the comunication failed
	return (fRes);

}


void CAgv::SetEstadoActual(TEstadoAGV estado)
{
	// TODO 2014 Testing
	if (m_estado == CARGARORD || estado == CARGARORD) {
		int i = 0;
	}
	if (m_estado != estado) 
	{
		//Xavi, 11/10/05: Abans de canviar d'estat, actualitzem l'estadistica
		CDBEstadistica::ArchivarHistAgv(this);
		
		m_estado = estado;
	}
}

void CAgv::QuizasInsertarBloqueo(unsigned char  bloqueo)
{
	if (IsBlocked() != IsBlocked(bloqueo)) 
	{
		//Xavi, 11/10/05: Abans de canviar d'estat, actualitzem l'estadistica
		CDBEstadistica::ArchivarHistAgv(this);
		
	}
}

bool CAgv::IsBlocked(unsigned char bloqueo)
{
	if (! (bloqueo & BIT2))
	{
		// Michael 16.01.2002 Apparently not blocked
		if (bloqueo & BIT1)
			return (false);
		else
			return (true);
	}
	else
		return (true);
}

void CAgv::SetEnCircuito () 
{ 
	if(!m_fEnCircuito)
		CDBEstadistica::ArchivarHistAgv(this); 
	m_fEnCircuito = true; 
	m_fWillUpdateBuB = true; 
};

void CAgv::SetFueraCircuito () 
{ 
	if(m_fEnCircuito) 
		CDBEstadistica::ArchivarHistAgv(this); 
	m_fEnCircuito = false; 
};

/*
//------------------------------------------------------------------------------
// Ver si NO debe ir por 203->236
//	<true> : No se necesita o no esta en esta posición
//	<false> : Se debe ir por el camino alternativo y no está disponible
//------------------------------------------------------------------------------
bool CAgv::QuizasCaminoAlt203()
{

	bool fRetVal = true;
	USHORT destino;
	
	if (!g_Container.m_fCaminoAlternativo203)
		return true;

	if (m_pOrden && 'S' == m_pOrden->m_at.herkunft)
		// Orden Manual
		destino = m_pOrden->m_at.az.nDest;
	else
		destino = m_pFzdat->auftziel.nDest;

	if (m_pFzdat->mp == 203 && m_pFzdat->nmp == 236)		  
	{
		// Intención de pasar por 203->236 cuando desactivado
		MSGAVISO4 ("QuizasCaminoAlt 203 Agv: %d en mp %d con nmp %d y destino %d",
			m_id,
			m_pFzdat->mp,
			m_pFzdat->nmp,
			destino);

		// Michael 15.12.2009 Evitar problemas cuando no tiene destino
		if (destino == 0)
		{
			destino = 7;
			
			if (m_pOrden && 'S' == m_pOrden->m_at.herkunft)
				// Orden Manual
				m_pOrden->m_at.az.nDest = destino;
			else
				m_pFzdat->auftziel.nDest = destino;
		}
		// Michael 15.12.2009 fin





		TYP_FZDAT fzDatCopy;
		fzDatCopy = *m_pFzdat;
		fzDatCopy.nmp = 204;
		MSGTRACE2 ("AGV: %d antes GetPNmp destino: %d", m_id, destino);
		NMP_tabelle * ptrNmp = GetPNmp (204, destino);
		fzDatCopy.uemp = ptrNmp->nmp;
		MSGTRACE1 ("AGV: %d antes SetFVAusBlTab", m_id);
		if (! SetFVAusBlTab (&fzDatCopy, TRUE))
		{
			MSGAVISO1 ("Enviando AGV: %d por camino alternativo (203->204)", m_id);
			if (!g_Container.m_fSimulateCom)
				SendUmleitTelegram (fzDatCopy.nmp);
			else
				SimulateUmleitTelegram (fzDatCopy.nmp,true);
		}
		else
		{
			// Hay que enviar por arriba, pero está bloqueado
			MSGAVISO1 ("Se debe enviar AGV:%d por camino slternativa (203->204) pero está bloqueado",
					m_id);
			fRetVal = false;
		}
	}

	return (fRetVal);

}
*/

//------------------------------------------------------------------------------
// Ver si NO debe ir por 203->236
//	<true> : No se necesita o no esta en esta posición
//	<false> : Se debe ir por el camino alternativo y no está disponible
//------------------------------------------------------------------------------
bool CAgv::QuizasCaminoAlt203()
{

	bool fRetVal = true;
	USHORT destino;

	if (!g_Container.m_fCaminoAlternativo203)
		return true;

	if (m_pOrden && 'S' == m_pOrden->m_at.herkunft)
		// Orden Manual
		destino = m_pOrden->m_at.az.nDest;
	else
		destino = m_pFzdat->auftziel.nDest;

	if (m_pFzdat->mp == 203 && m_pFzdat->nmp == 236)
	{
		// Intención de pasar por 203->236 cuando desactivado
		MSGAVISO4("QuizasCaminoAlt 203 Agv: %d en mp %d con nmp %d y destino %d",
			m_id,
			m_pFzdat->mp,
			m_pFzdat->nmp,
			destino);

		// Michael 15.12.2009 Evitar problemas cuando no tiene destino
		if (destino == 0)
		{
			destino = 7;

			if (m_pOrden && 'S' == m_pOrden->m_at.herkunft)
				// Orden Manual
				m_pOrden->m_at.az.nDest = destino;
			else
				m_pFzdat->auftziel.nDest = destino;
		}
		// Michael 15.12.2009 fin





		TYP_FZDAT fzDatCopy;
		fzDatCopy = *m_pFzdat;
		fzDatCopy.nmp = 204;
		MSGTRACE2("AGV: %d antes GetPNmp destino: %d", m_id, destino);
		//NMP_tabelle * ptrNmp = GetPNmp(204, destino);
		CCpDistance* pCpDistance = GetPNmp(204, destino);
		fzDatCopy.uemp = pCpDistance->GetNCp();
		MSGTRACE1("AGV: %d antes SetFVAusBlTab", m_id);
		if (!SetFVAusBlTab(&fzDatCopy, TRUE))
		{
			MSGAVISO1("Enviando AGV: %d por camino alternativo (203->204)", m_id);
			if (!g_Container.m_fSimulateCom)
				SendUmleitTelegram(fzDatCopy.nmp);
			else
				SimulateUmleitTelegram(fzDatCopy.nmp, true);
		}
		else
		{
			// Hay que enviar por arriba, pero está bloqueado
			MSGAVISO1("Se debe enviar AGV:%d por camino slternativa (203->204) pero está bloqueado",
				m_id);
			fRetVal = false;
		}
	}

	return (fRetVal);

}


// Michael Mods 2009
// 

bool CAgv::SendWegTelegram()
{
	int fRetVal;
	int i;
	bool fAceptado;
	typ_tele_anf teleAnf;
	CString str;

	teleAnf.tele_art = 'w';
	str.Format ("%02d", m_id);
	strncpy ((char *)teleAnf.fznr, str, 2);


	for (i = 0, fAceptado = false; i < 3 && !fAceptado; i++)
	{
	m_Tel.SetAnfTelegram (teleAnf);
	fRetVal = m_pSerCom->SendTelegrama (m_Tel.m_strTelegrama);
	if (fRetVal)
		{
		// El telegrama requiere respuesta
		if (g_Container.m_fSimulateCom){
			SimulateWegTelegram();
			fRetVal = WAIT_OBJECT_0;
		}
		else
		{
			fRetVal = m_pSerCom->ReceiveTelegrama (m_Tel.m_strRespuesta);
		}

		CString strTrace;
		switch (fRetVal)
		{
		case WAIT_OBJECT_0:
			// se ha recibido el telegrama de respuesta
			m_Tel.SetTeleRecibido ();
			m_numTimeout = 0;	// Reset timeouts

			if (CheckRespuestaAnf ())
			{
				fAceptado = true;
			}
			break;
		case WAIT_TIMEOUT:
			m_Tel.SetTeleTout ();
			strTrace.Format ("Timeout waiting for Weg reply Agv: %d", m_id);
			MSGTRACE (strTrace);
			break;
		default:
			// XXX mirar posibilidades aquí
			m_Tel.SetTeleTout ();
			strTrace.Format ("ReceiveTelegrama ha devuelto: %d (Weg)", fRetVal);
			MSGERROR (strTrace);
			break;
			}
		}
		else
		{
			m_Tel.SetTeleNull ();
		}
	} // for (..)
	return (fAceptado);


}



bool CAgv::CheckRespuestaAnf()
{

	bool fRet = false;	// Valor a devolver
	typ_tele_kopf* pTeleHeader; // Telegram header

	pTeleHeader = (typ_tele_kopf*) m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_kopf));

	if (! CheckTeleAgv ((char *)pTeleHeader->fznr))
		return (false);

	switch (pTeleHeader->tele_art)
	{
	case 'w':
		{ // brackets to avoid initialization problem
		typ_tele_weg * tele_weg = (typ_tele_weg*) m_Tel.m_strRespuesta.GetBuffer (sizeof (typ_tele_weg));
		InterpretarTeleWeg (*tele_weg);
		fRet = true;
		}
		break;
	default:
		MSGERROR1 ("CheckRespuestaAnf telegrama tipo: %c", pTeleHeader->tele_art);
		fRet = false;	
	}

	return (fRet);

}

bool CAgv::IsWegSection()
{


	// TODO MODS 2009 - Desactivado 14/05/2009 hasta tener tratamiento weg por parte de J. Lachmund
	//return false;
	// TODO fin

	// compare mp nmp for AGV to see whether weg telegram should be sent
	if (m_pFzdat->mp == 214 && (m_pFzdat->nmp == 232 || m_pFzdat->nmp == 226)) 
	{
		return true;
	}
	if (m_pFzdat->mp == 210 && m_pFzdat->nmp == 298)
	{
		return true;
	}

	if (m_pFzdat->mp == 298 && m_pFzdat->nmp == 299)
	{
		return true;
	}
	if (m_pFzdat->mp == 298 && m_pFzdat->nmp == 211)
	{
		return true;
	}
	if (m_pFzdat->mp == 211 && m_pFzdat->nmp == 212)
	{
		return true;
	}
	if (m_pFzdat->mp == 212 && m_pFzdat->nmp == 31)
	{
		return true;
	}

	if (m_pFzdat->mp == 254)
	{
		return true;
	}
	if (m_pFzdat->mp == 212 && m_pFzdat->nmp == 31)
	{
		return true;
	}
	if (m_pFzdat->mp == 255)
	{
		return true;
	}



	return false;
}





void CAgv::SimulateWegTelegram()
{
	CString strTemp;
	typ_tele_weg tele_weg;

	tele_weg.tele_kopf.tele_art = 'w';
	strTemp.Format ("%02d", m_id);
	strncpy ((char *)tele_weg.tele_kopf.fznr, strTemp, 2);

	// Add 1m to current wegstr
	unsigned short weg = m_pFzdat->wegstr + 500;

	weg = weg / FZ_WEG_KORREKTUR;	// convert from mills to cm
	
	tele_weg.weg [1] = weg & 0x3f;
	tele_weg.weg [0] = (weg >> 6) & 0x3f;


	// Copy telegram to input buffer
	LPTSTR pszbuf = m_Tel.m_strRespuesta.GetBuffer (sizeof (tele_weg));
	memcpy (pszbuf, (const void *)&tele_weg, sizeof (tele_weg));
	pszbuf [sizeof (tele_weg)] = '\0';
	m_Tel.m_strRespuesta.ReleaseBuffer ();

	


}


bool CAgv::IsBlockingDistanceTravelled()
{

	bool fRetVal;

	USHORT nBlockWay = GetBlockWay(m_pFzdat->mp,m_pFzdat->nmp);
	//USHORT nBlockWayCut = (nBlockWay < 2900) ? 0 : (nBlockWay - 2900);	// - 100 por si para antes...
	USHORT nBlockWayCut = (nBlockWay < 500) ? 0 : (nBlockWay - 500);	
	
	MSGTRACE5 ("IsBlockingDistanceTravelled(): Agv: %d, mp = %d, nmp = %d, blockway = %d nBlockWayCut = %d",
		m_id,
		m_pFzdat->mp,
		m_pFzdat->nmp,
		nBlockWay,
		nBlockWayCut);





	if (m_pFzdat->wegstr <= nBlockWayCut)	
    {
		fRetVal = false;
    }
	else
	{
		fRetVal = true;
	}
	return fRetVal;



}

/*
bool CAgv::IsSegmentDistanceTravelled()
{
	bool fRetVal;
	NMP_tabelle * ptrNmp = GetPNmp (m_pFzdat->nmp, m_pFzdat->auftziel.nDest);
		
	
	if (m_pFzdat->wegstr <= ptrNmp->str_len)
    {
      fRetVal = false;
    }
	else
	{
		fRetVal = true;
	}
	return fRetVal;
}
*/

bool CAgv::IsSegmentDistanceTravelled()
{
	bool fRetVal;
	//NMP_tabelle * ptrNmp = GetPNmp(m_pFzdat->nmp, m_pFzdat->auftziel.nDest);
	CCpDistance* pCpDistance = GetPNmp(m_pFzdat->nmp, m_pFzdat->auftziel.nDest);

	if (m_pFzdat->wegstr <= pCpDistance->GetLength())
	{
		fRetVal = false;
	}
	else
	{
		fRetVal = true;
	}

	return fRetVal;
}

void CAgv::ShowGaliboErrorAtP3(int module_nr, int fehler_nr, int mp)
{ 
	if (module_nr == 60 && fehler_nr == 29 && mp == 9)
	{
		MSGERROR1 ("Agv: %d con error de gálibo en estación P3", m_id);
	}

}

bool CAgv::IsDistanceToRequestWegTele()
{
	ULONG lMyKey = m_pFzdat->mp*10000 + m_pFzdat->nmp;
	CDistanceReqWayTele myCDRWT;	//(1,m_pFzdat->mp,m_pFzdat->nmp);
	bool bWegDistance = m_pmapDistancesReqWayTele->Lookup(lMyKey,myCDRWT) == TRUE;
	MSGTRACE4 ("IsDistanceToRequestWegTele() Agv: %d mp: %d nmp: %d bWegDistance: %s", 
		m_id, m_pFzdat->mp, m_pFzdat->nmp, bWegDistance ? "true" : "false");
	return (bWegDistance);
}

/*
USHORT CAgv::GetBlockWay(USHORT nCp, USHORT nNCp)
{
	USHORT nBlockWay = 0;
	if(nNCp != 0)
	{
		// Zeiger auf MP in der MP-DatenTabelle
		Meldepunkttabelle* pCp = mptab + vmptab [nCp] - 1;

		NMP_tabelle* pDist = MpNmpTab + pCp->str_idx - 1;
		for(USHORT nCount = 0;
		(nCount < pCp->anz_nmp) && (nBlockWay == 0);
		nCount++,pDist++)
		{
			if(pDist->nmp == nNCp)
			{
				nBlockWay = pDist->blk_weg;
			}
		}
	}	
	return nBlockWay;
}
*/

USHORT CAgv::GetBlockWay(USHORT nCp, USHORT nNCp)
{
	return pAGVTCourse->GetBlockWay(nCp, nNCp);
}




/* 02.03.2012 Nuevo segmento de salida del pulmon P1 (mp:7)
 * Use umleit telegram to provoke use, and route by cp 245 is shorter
 * 
 */

bool CAgv::QuizasCaminoAlt303()
{
	bool fRetVal = true;
	USHORT destino;
	
	
	if (m_pOrden && 'S' == m_pOrden->m_at.herkunft)
		// Orden Manual
		destino = m_pOrden->m_at.az.nDest;
	else
		destino = m_pFzdat->auftziel.nDest;


	if (IsCaminoAlt303())
	{
		// Send by 303
		MSGAVISO4 ("QuizasCaminoAlt 303 Agv: %d en mp %d con nmp %d y destino %d",
			m_id,
			m_pFzdat->mp,
			m_pFzdat->nmp,
			destino);

		// Michael 15.12.2009 Evitar problemas cuando no tiene destino
		if (destino == 0)
		{
			destino = 39; // TODO - Surely not
			
			if (m_pOrden && 'S' == m_pOrden->m_at.herkunft)
				// Orden Manual
				m_pOrden->m_at.az.nDest = destino;
			else
				m_pFzdat->auftziel.nDest = destino;
		}
		// Michael 15.12.2009 fin





		TYP_FZDAT fzDatCopy;
		fzDatCopy = *m_pFzdat;
		fzDatCopy.nmp = 303;
		MSGTRACE2 ("AGV: %d antes GetPNmp destino: %d", m_id, destino);
		CCpDistance *pDist = pAGVTCourse->GetNextDistPointer(303, destino);
		fzDatCopy.uemp = pDist->GetNCp();

		MSGTRACE1 ("AGV: %d antes SetFVAusBlTab", m_id);
		if (! SetFVAusBlTab (&fzDatCopy, TRUE))
		{
			MSGAVISO1 ("Enviando AGV: %d por camino alternativo (107->303)", m_id);
			if (!g_Container.m_fSimulateCom)
			{
				SendUmleitTelegram (fzDatCopy.nmp);
				// Michael 15.03.2012 set these for bub
			}
			else
				SimulateUmleitTelegram (fzDatCopy.nmp,true);
			m_pFzdat->nmp = fzDatCopy.nmp;
			m_pFzdat->uemp = fzDatCopy.uemp;
				// fin for bub
			m_fWillUpdateBuB = true; 

		}
		else
		{
			// Hay que enviar por arriba, pero está bloqueado
			MSGAVISO1 ("Se debe enviar AGV:%d por camino slternativa (107->303) pero está bloqueado",
					m_id);
			fRetVal = false;
		}
	}

	return (fRetVal);


}



bool CAgv::IsCaminoAlt303()
{
	USHORT destino;
	
	if (!g_Container.m_fCaminoAlternativo303)
		return false;

	if (m_pOrden && 'S' == m_pOrden->m_at.herkunft)
		// Orden Manual
		destino = m_pOrden->m_at.az.nDest;
	else
		destino = m_pFzdat->auftziel.nDest;

	// Do not redirect if destino is 8 or 9 (P2, P3)
	return (m_pFzdat->mp == 107 && m_pFzdat->nmp == 245 && destino != 9 && destino != 8);


}

// set the bits to indicate the mesa cargada del AGV
void CAgv::SimulateCargada(bool mesa1, bool mesa2)
{

	if (mesa1) 
		// M2015 m_pFzdat->mistfz2 |= BIT2;	// Conveyor 1
		SET_FZ_BELADEN(*m_pFzdat);
	if (mesa2) 
		SET_FZ_BELADEN_2(*m_pFzdat);	// Conveyor 2
}

void CAgv::SetAGVTCourse(CCourseAGVType* pCourse)
{
	pAGVTCourse = pCourse;
}
/**
* Map the status bytes from the incoming telegram to the m_pFzdat variable
* @return true if the status has changed
*/
void CAgv::MapStatusBytes(unsigned char s1, unsigned char s2, unsigned char s3, unsigned char s4, unsigned char s5, unsigned char (&mappedStatus)[5] )
{
	

	if (IsWlan()) {
		mappedStatus [0]  = s1;
		mappedStatus [1] = s2;
		mappedStatus [2] = s3;
		mappedStatus [3]= s4;
		mappedStatus [4] = s5;
	}
	else {
		// Mapping old status to new status..
		// Copy bytes which are the same
		mappedStatus [0] = s1;
		mappedStatus [1] = s2;
		mappedStatus [2] = BIT7;

		mappedStatus [4] = 0;
		mappedStatus [4] |= BIT7;		
		mappedStatus [4] |= (s3 & (0x20)) >> 2; // Bit 6 to bit 4 : Order done conveyor 2
		mappedStatus[4] |= (s3 & (0x4)) << 2; // Bit 3 to bit 5 : Conveyor loaded conveyor 2
	
		mappedStatus [3] = 0;
		mappedStatus [3] |= BIT7;		
		mappedStatus [3] |= (s3 & (0x1)); // Bit 1 to bit 1 : Order done conveyor 1
		mappedStatus [3] |= (s3 & (0x2)) << 1; // Bit 2 to bit 3 : Conveyor loaded 1

		mappedStatus [3] |= (s3 & (0x8)) << 1; // Bit 4 to bit 5 : Battery

		mappedStatus [3] |= (s3 & (0x10)) << 1; // Bit 5 to bit 6 : Vehicle Error

	}
}


unsigned char CAgv::MapCommandByte (unsigned char k1)
{
	unsigned char mappedKommand;
	mappedKommand = (k1 & 0x1) ? 1 : 2;
	mappedKommand |= (k1 & 0xFA);

	return mappedKommand;
}

WlanMsg * CAgv::SetClientConnection (void) {
	if (m_pWlanMsg == NULL)
	{
		m_pWlanMsg = new WlanMsg();
	}
	m_WlanTel.SetAgv (m_id);
	return m_pWlanMsg;
}

	
	



void CAgv::MaybeSendQuittum() 
{
	// M2015 TODO See whether necessary to send quittum when not blocked if (m_fSendQuitum || ! IsBlocked ())
	if (m_fSendQuitum)
	{
		// Enviar quitum
		MSGAVISO1 ("Enviando Quitum al Agv: %d", m_id);
		m_WlanTel.SendQuitum(m_pWlanMsg);
		m_fSendQuitum = false;
	}

}

void CAgv::MaybeUnblockDrive()
{
	if (m_fWasBlocked && ! IsBlocked ())
	{
		//	// M2015 TODO
		//if (m_pFzdat->mp == 41 && m_pFzdat->auftziel.nDest == 44 && !IsBlocked()) 
		//{
		//	TestWlanSendUmleitTelegram();
		//}
		
		WlanUnblockDrive ();
		m_fWasBlocked = false;
	}
}



void CAgv::ProcessMessages() {

	CString strMsg;
	bool fUpdateBuB = false;
	bool fComReceived = false;
	MSGTRACE1 ("ProcessMessages Agv: %d", m_id);
	if (m_pWlanMsg)
	{
	while (m_pWlanMsg->GetNextMessageIn(&strMsg))
	{
		RESET_FZ_VERMISST(*m_pFzdat);
		TCHAR art = strMsg.GetAt(1);
		fComReceived = true;
			
		// m_art = m_tele_kopf.tele_art;
		switch (art) {
			case 'L':	// Layout
			case 'V':	// Version
				{
					// TODO: Note both are variable length, so should read to STX
					// But may be better not to bother with these
					int readLen = sizeof(typ_tele_kopf_wlan);
				}
				break;
			case 'S':	
				{
				int readLen = sizeof(typ_tele_gs_wlan);
				typ_tele_gs_wlan tele_gs;
				// TODO - check that corresponds with length read
				LPTSTR pszbuf = strMsg.GetBuffer(readLen);
				memcpy ((void *)&tele_gs, (const void *)pszbuf, readLen);
				strMsg.ReleaseBuffer();
				if (CheckTeleAgv((char*)tele_gs.tele_kopf.fznr) && CheckTeleE_Check(strMsg))
				{
					m_WlanTel.SendQuitum(m_pWlanMsg);
					m_fWlanUpdateEstado = InterpretarTeleGrosseStatus (tele_gs);
					if (IsBlocked())
					{
						// Update status only if Agv is blocked - otherwise will be sending messages
						// when not required
						updateEstadoGrossStatus (m_fWlanUpdateEstado);
						UpdateEstado();
						MaybeUnblockDrive();
					}


					fUpdateBuB = true;

				}	
				}
				break;


			case 'E':
				{
				int readLen = sizeof(typ_tele_error_wlan);
				typ_tele_error_wlan tele_error;
				// TODO - check that corresponds with length read
				LPTSTR pszbuf = strMsg.GetBuffer(readLen);
				memcpy ((void *)&tele_error, (const void *)pszbuf, readLen);
				strMsg.ReleaseBuffer();
				if (CheckTeleAgv((char*)tele_error.tele_kopf.fznr) && CheckTeleE_Check(strMsg))
				{
					// For the moment not putting in a critical section,
					// although may be necessary as may update AGV Position etc..
					InterpretarTeleErrorWlan(tele_error);
					m_WlanTel.SendQuitum(m_pWlanMsg);
					fUpdateBuB = true;
					//bool fUpdateEstado = InterpretarTeleGrosseStatus (tele_gs);
					//updateEstadoGrossStatus (fUpdateEstado);
					//if (fUpdateEstado)
					//	UpdateEstado();
					//if (m_fWasBlocked && ! IsBlocked ())
					//	WlanUnblockDrive ();

				}


					

				}
				break;

			case 'A':	
				{
				// This is a vehicle generated transport telegram
					m_WlanTel.SendQuitum(m_pWlanMsg);
				break;
				}

			case 'Q':
				{
					int readLen = 3; // Error (2) + ETX
				}
				break;


			default:
//				ar.Read(buffer, sizeof(tele_version) - sizeof(tele_kopf));
				int i = 0;


				
		}
		
	}
	if (fUpdateBuB)
	{
		g_Container.m_pMainWnd->PostMessage (WM_USER_AGV_UPDATE_BUB, 0, (UINT)m_id);
	}
	if (!fComReceived)
	{
		if (HasBeenPolled())
		{
			if (IsBlocked())
			{
				UpdateEstadoGrossStatusWlan();
				if (IsWlanUpdateEstado())
				{
					MaybeSendQuittum();
					ResetWlanUpdateEstado();
				}
				UpdateEstado();
				MaybeUnblockDrive();
			}
			else
				WlanMaybeRequestWegDistance();

		}

	}
}
}








// Poll a Wlan AGV in order to get an Info telegram response (Ostensibly to get the WEG distance travelled)
void CAgv::WlanPoll(void)
{
	MSGAVISO1 ("WlanPoll () Agv: %d", m_id);
	typ_tele_anf_wlan telepoll;
	CString str;

	telepoll.tele_art = 'P';

	str.Format ("%02d", m_id);
	strncpy ((char *)telepoll.fznr, str, 2);

	m_WlanTel.SetPollTelegram(telepoll, m_pWlanMsg);
}



// Ver si es un AGV Wlan
bool CAgv::SetWlanProperties(void)
{
	// See if it is a WLan Agv()
	TCHAR charAgv [6];

	sprintf(charAgv, "AGV%d", m_id);
		

	m_sWlanIP = AfxGetApp ()->GetProfileString ("Settings", charAgv);
	if (!m_sWlanIP.IsEmpty())
	{
		// Initialize elements
		m_fWlanAgv = true;
		return true;
	}
	else
		return false;
}

void CAgv::SetSocketClosed()
{
	MSGWARNING1 ("** Agv: %d comunicación cerrada - marcando perdido", m_id);
	SET_FZ_VERMISST (*m_pFzdat);
	g_Container.m_pMainWnd->PostMessage (WM_USER_AGV_UPDATE_BUB, 0, (UINT)m_id);
	if (m_pWlanMsg)
		m_pWlanMsg->SetClosed();
	// Quit the thread then
	// TODO ::PostQuitMessage(0);
	// TODO - Abort the sending connection
}

void CAgv::UpdateEstadoGrossStatusWlan(void)
{
	updateEstadoGrossStatus(true);
}

void CAgv::WlanMaybeRequestWegDistance (void)
{
	m_CurTime = GetTimePlusInterval (0);
	if (m_CurTime.QuadPart >= m_minNextPoll.QuadPart)
	{
	if (IsDistanceToRequestWegTele())
	{
		if (!IsBlockingDistanceTravelled() && !m_lastWegWasSame)
		{
			// Send a weg telegram every second
			m_minNextPoll = GetTimePlusInterval(1000);
			WlanPoll();
		}
	}
	}

}
