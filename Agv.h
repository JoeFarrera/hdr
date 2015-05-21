// Agv.h: interface for the CAgv class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AGV_H__EBD7D77A_227E_11D5_992A_0010B547EBE6__INCLUDED_)
#define AFX_AGV_H__EBD7D77A_227E_11D5_992A_0010B547EBE6__INCLUDED_

#if _MSC_VER > 1000   
#pragma once
#endif // _MSC_VER > 1000

#include "FZ_TELE.H"
#include "Telegrama.h"
#include "WlanTelegrama.h"
#include "DBEstadoPLC.h"	// Added by ClassView

#include "DistanceReqWayTele.h"
#include "CCourseAGVType.h"
#include "ISocketSink.h"


class CTelegrama;
class CWlanTelegrama;
class CSerialCom;
class COrden;
class WlanMsg;
class ISocketSink;



#define AGV_FUERASISTEMA	(m_estado == AGVFUERASISTEMA)


class CAgv : public ISocketSink
{
public:
	void ProcessMessages();		
	bool WakeUpAgv();
	void MaybeSendQuittum();
	void MaybeUnblockDrive();
	void ResetWlanUpdateEstado() { m_fWlanUpdateEstado = false; };
	void SetWlanUpdateEstado() { m_fWlanUpdateEstado = true; };
	bool IsWlanUpdateEstado() { return (m_fWlanUpdateEstado); };
	bool IsWlan() { return (m_fWlanAgv == true); };
	bool IsCaminoAlt303();
	bool SendWegTelegram();
	//Xavi, 02/11/05
	bool CaminoAltActivado();
	//Fi Xavi, 02/11/05
	bool QuizasCaminoAlt();
	bool IsModifiableDestino();
	UINT m_cOrdenesOk; // Flag set por el Gestor de Órdenes al intentar con el Agv
	bool IsPossibleCandidateForOrder();
	bool IsCargadorCurPos(bool fAll = false);
	bool IsEnCircuito () { return (m_fEnCircuito); };
	//Xavi, 02/11/05
	//void SetEnCircuito () { m_fEnCircuito = true; m_fWillUpdateBuB = true; };
	//void SetFueraCircuito () { m_fEnCircuito = false; };
	void SetEnCircuito (); 
	void SetEnCircuitoInit (bool enCircuito) { m_fEnCircuito = enCircuito; m_fWillUpdateBuB = true; };
	void SetFueraCircuito ();
	//Fi Xavi, 02/11/05
	void SetAgvEmpujando (UINT nAgv);
	void SetPeticionCarga (UINT nCargador) { m_nCargador = nCargador;}
	static bool IsOccupiedCP (UINT cp, bool fRealmenteOcupado = false);
	UINT m_nextKnot;	// Next knot if Agv gets drive permission
	UINT m_knot;	// Knot ocupado por el AGV
	bool GetStationError () { return (0 != m_uErrorEstacion);}
	void ResetOrden();
	UINT GetTimeToPickup (UINT mpZiel, UINT mpDest = 0);
	bool IsCandidateForOrder(TEstadoAGV * estado, COrden * ptrOrden = NULL);
	void SetDeleted();
	void InsertarAGV (UINT mp, UCHAR mpPos);	// Registrar Agv como presente
	CCriticalSection m_cs;	// Critical Section para controlar aceso
	LARGE_INTEGER SetNextPollInterval();
	LPCSTR GetStringEstado();
	COrden * m_pOrden;	// Orden de transporte
	void SetOrden (COrden * orden);
	static CCpDistance* GetPNmp (UINT mp, UINT ziel);
	static unsigned int GetNmpWeg  (unsigned int  mp, unsigned int ziel, long *weg);
	unsigned short GetPosWeg (TYPE_STATION_DEF Ziel);
	static unsigned short GetWeg (TYP_FZDAT * fzdat);
	UINT GetNmp(unsigned short mp, unsigned short ziel);
	bool m_fBuB;	// Avisar el BuB de los telegramas recibidos
	static void SetZiel (TYPE_STATION_DEF * ptrZiel,
        unsigned short nr = 0,
        unsigned char  pos = 0,
	    unsigned char  lage = 'L',
        unsigned char ebene = 0);
	TYP_FZDAT * GetFzdatPtr ();
	int GetAtoi (LPCTSTR pStr, int len);
	static LARGE_INTEGER GetTimePlusInterval (UINT interval);
	LARGE_INTEGER m_minNextPoll;	// System min. time to next poll
	LARGE_INTEGER m_maxNextPoll;	// System max. time to next poll
	LARGE_INTEGER m_CurTime;		// Cur Time

	TEstadoAGV GetEstado () {return (m_estado);};
	void SetEstado (TEstadoAGV estado);
	void ResetBuB () { m_fBuB = false; };
	LARGE_INTEGER Poll ();
	UINT m_id;
	CAgv(UINT nId, CSerialCom * pSerCom,CDistanceReqWayTeleMap* pmapDistancesReqWayTele);
	virtual ~CAgv();
	CTelegrama m_Tel;	// Telegramas en curso
	CWlanTelegrama m_WlanTel;	
	CTpcView * m_pView;	// View
	void Init();	// Inicializar datos del AGV
	TYP_FZDAT * m_pFzdat; // Datos del AGV
	bool HasBeenPolled () { return (m_fHasBeenPolled);};
	UINT m_nCargador;	// CP del cargador en caso de peticion de carga de bateria
	UINT m_nMantenimiento;	// Michael 14.06.2005 T5K CP de mantenimiento si es que se desea ir a mantenimiento
	void SetCargaBateriaBuB (bool fOnOff) { m_fCargaBateriaBuB = fOnOff; };
	bool IsCargaBateriaBuB () { return (m_fCargaBateriaBuB); };
	bool SOrdenCargada();
	bool SetCargado();
	//Xavi, 02/11/05
	bool IsBlocked();
	bool IsDriveForbiddenByBuB () { return (1 == m_pFzdat->FVvonBedienung ? true : false);};
	CTime tiempo;
	//Fi Xavi, 02/11/05
	void SetBlocked(bool fBlock);
	void SetBubBlocked (int blockVal) { m_pFzdat->FVvonBedienung = blockVal; };
	// set the bits to indicate the mesa cargada del AGV
	void SimulateCargada(bool mesa1, bool mesa2);
	void CAgv::GetOrden(int orden); 
	static void SetAGVTCourse(CCourseAGVType* pCourse);
	void SetClientConnection (WlanMsg * pWlanMsg);
	void UpdateEstado();	


private:
	bool GetIPForWakeup (CString& wakeupIp);
	bool WlanEnviarCargador(TYPE_STATION_DEF zCargador);
	bool SMaybeInStationWlan ();
	bool m_fWlanAgv;
	CString m_sWlanIP;
	bool m_fWlanUpdateEstado;
	unsigned short GetWegStr (LPCTSTR pWegstr);
	void InterpretarTeleErrorWlan(typ_tele_error_wlan &tele);
	bool CheckTeleE_Check(CString &telegrama);
	void updateEstadoGrossStatus (bool fUpdateEstado);
	static CCourseAGVType *pAGVTCourse;
	bool QuizasCaminoAlt303();
	USHORT m_lastWegTeleValue;	
	bool m_lastWegWasSame;
	USHORT GetBlockWay(USHORT nCp, USHORT nNCp);
	bool IsDistanceToRequestWegTele();
	void ShowGaliboErrorAtP3 (int module_nr, int fehler_nr, int mp);
	bool IsSegmentDistanceTravelled();
	bool IsBlockingDistanceTravelled();
	void SimulateWegTelegram();
	bool IsWegSection();
	bool CheckRespuestaAnf();
	bool QuizasCaminoAlt203();
	bool EnviarMantenimiento();
	bool SEnviarMantenimiento();
	CDBEstadoPLC m_estadoPLC;
	UINT m_lastUmleitMp;	// Almacenar último MP enviado por Umleit...
	bool IsOk60();
	UINT m_uDeadlockCounter;	// Contador para detectar deadlocks
	bool m_fCargaBateriaBuB;	// Carga de bateria solicitado por BuB
	bool IsOccupied60();
	void SetCountEsperar();
	void SimulateUmleitTelegram (UINT altNmp,bool bNCp);
	UINT GetKnotCurPos();
	bool IsDestinoCP (UINT cp);
	bool IsPuntoDescansoCurPos();
	bool IsSalirPuntoDescanso();
	void GetPuntoParking (TYPE_STATION_DEF *ptrZiel, UINT curMp = 0, bool fSetFuera = true);
	bool IsAparcado();
	bool m_fEnCircuito;	// El Agv se considera en el circuito
	UINT m_numError;
	bool GetDiffBlkWegPaar (CCpDistance* pCpDistance);
	bool SendUmleitTelegram(UINT nmp);
	USHORT Umleiten(bool &bNCp);
	bool IsIrCargarBat();
	bool OkDestinoCarga();
	bool m_fHasBeenPolled;	// Indicar que el Agv ha sido "polled" por lo menos una vez
	bool SetDestinoRechazo ();
	bool OkDestinoDescarga();
	UINT m_countEsperar;	// Contador antes de ir a punto de espera
	bool OkDestinoEspecial();
	bool IsDeadlocked();
	bool IsOkEmpujar();
	void ResetEmpujado () { m_nAgvEmpujando = 0; };
	bool IsEmpujado () { return (m_nAgvEmpujando != 0); };
	void EmpujarAgvBloqueo(bool desdeAtras = false);
	UINT m_nAgvEmpujando;	// Agv que quiere empujar este
	bool CheckTeleAgv (char * idTele);
	bool SCargarBateria();
	bool SwitchOnCargadorPLC();
	void SleepAgv();
	bool EnviarCargador();
	bool ShouldBlock();
	void SetTeleBlocked (UCHAR * kommand, bool fBlock);
	UINT IsKnot();
	//bool IsDriveForbiddenByBuB () { return (1 == m_pFzdat->FVvonBedienung ? true : false);}; //Xavi, 02/11/05
	bool BlockForKnotNumber();
	bool SOrderDoneParcial();
	bool SOrdenCargadaParcial();
	void SetMesaToDeliver (UCHAR * kommand);
	void SetMesaToPickup (UCHAR * kommand);
	void CheckStationError (UINT module, UINT error);
	UINT m_uErrorEstacion;	// Record error in the station
	bool m_fWillUpdateBuB;	// Intention of updating the BuB
	bool m_fWasBlocked; // Indicar si el AGV está bloqueado o no
	bool UnblockDrive();
	//bool IsBlocked(); //Xavi, 02/11/05
	bool SimulateActionBlockTelegram (typ_tele_block tele);
	bool DescargarOrden ();
	bool SetAction (bool fFree);
	bool SendBlockTelegram (typ_tele_block tele);
	bool SetDestinoManual ();
	bool WlanSetDestinoManual ();
	bool ModificarDestino(TYPE_STATION_DEF& destino);
	bool DestinoDiferente(TEstadoAGV estado);
	void SetCancelled();
	bool AgvConDestino ();
	bool m_fSendQuitum;	// Indicar si enviar un quitum o no
	bool m_fTimeOutThisPoll;	// Se ha dado timeout en el ciclo de poll
	void GetPuntoDescanso (TYPE_STATION_DEF * ptrZiel, UINT curMp = 0);
	bool EnviarPuntoEspera(UINT curMp = 0);
	bool WlanUnblockDrive();
	bool WlanEnviarPuntoEspera (UINT curMp);
	bool WLanSendBlockTelegram(typ_tele_block_wlan tele);
	bool WlanSendUmleitTelegram(UINT nmp);
	CCpDistance* GetPNmpFromNmp(UINT mp, UINT nmp);
	bool IsOrdenAgvCargado();
	bool InterpretarTeleManualOrder (typ_tele_at& tele);
	bool InterpretarTeleError (typ_tele_error& tele);
	bool InterpretarTeleGrosseStatus (typ_tele_gs& tele);
	bool InterpretarTeleGrosseStatus(typ_tele_gs_wlan &tele);
	bool InterpretarTeleKleineZustand (typ_tele_kz& tele);
	bool InterpretarTeleWeg (typ_tele_weg& tele);
	void SetDelivered();
	int	 SimulatePoll();
	bool SimulateInfoTelegram(typ_tele_info tele);
	bool SOrderDone();
	bool SetUnloadDestination();
	bool CargarOrden();
	bool SLlegadoDestino();
	bool ComprobarRespuestaInfo (typ_tele_info tele);
	bool SendInfoTelegram (typ_tele_info tele);
	bool PickupOrden();
	UINT m_numTimeout;	// Numero de timeouts que ha dado el AGV
	void ActionTimeoutPoll();
	void InterpretarTelegramaRecibido ();
	TEstadoAGV m_estado;	// Estado del AGV
	CSerialCom* m_pSerCom;	// Comunicaciones Port Serie
	//Xavi, 02/11/05: Afegit per que sempre que es canvia d'estat passi per aqui
	void SetEstadoActual (TEstadoAGV estado);
	bool IsBlocked(unsigned char bloqueo);
	void QuizasInsertarBloqueo(unsigned char  bloqueo);
	bool m_fSendWeg;	// Si se debe enviar un telegrama WEG en el next poll
	CDistanceReqWayTeleMap*	 m_pmapDistancesReqWayTele;
	void MapStatusBytes(unsigned char s1, unsigned char s2, unsigned char s3, unsigned char s4, unsigned char s5, unsigned char (&mappedStatus)[5] );
	unsigned char MapCommandByte (unsigned char k1);
	bool m_newAgv;
	// M2015
	WlanMsg * m_pWlanMsg;	// Wlan communication
	void WlanMaybeRequestWegDistance (void);
public:
	// Poll a Wlan AGV in order to get an Info telegram response (Ostensibly to get the WEG distance travelled)
	void WlanPoll(void);
	// Ver si es un AGV Wlan
	bool SetWlanProperties(void);
	void SetSocketClosed ();
	void UpdateEstadoGrossStatusWlan(void);
};

#endif // !defined(AFX_AGV_H__EBD7D77A_227E_11D5_992A_0010B547EBE6__INCLUDED_)
