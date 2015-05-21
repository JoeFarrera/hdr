// Container.h: interface for the CContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTAINER_H__9C63167B_63C0_4AAC_80F0_30C2400EF123__INCLUDED_)
#define AFX_CONTAINER_H__9C63167B_63C0_4AAC_80F0_30C2400EF123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "konstant.h"
#include "zs_bed.h"		// Mensajes al BuB
#include "DBUtil.h"	// Added by ClassView
#include "SiemensSR.h"	// Added by ClassView
#include "GestorEstacionBuB.h"
#include "Alarma.h"

class CTpcView;
class CTpcDoc;
class CPuntoDescanso;
class CPLCThread;
class CBuBThread;

/*	JLac
typedef	struct {
		int nep;	// Decision point number (index)
		int mp;		// Meldepunkt
		unsigned char nMpIndex [KKMAX_BER_P1]; // Matrix of nmp index pointers (Max areas)
	} tAreaFinder;
*/

#define MAX_CARGADORES	9	// Michael 24.05.2007 AGV9



class CContainer : public CObject  
{
public:
	void InitStations();
	void SetCaminoAlt203(bool fCaminoAlt203);
	void SetCaminoAlt303(bool fCaminoAlt303);
	bool IsSimulateEscribirPLC();
	void SetCaminoAlt (bool fCaminoAlt);
	bool m_fCaminoAlternativo;	// Camino alternativo para 41
	bool m_fCaminoAlternativo203;	// No Permitir 203->236
	bool m_fCaminoAlternativo303;	// Alternativa de salida del pulmón P1
	CString m_strTren;	// String a visualizar del tren
	CString m_strRadio;	// String a visualizar de la radio
	CStringList m_listSelectAgvHist; // List of Agvs for writing history
	enum T_Hist m_histFileType;	// Type of file extension for the history file
	bool m_fWriteAgvHistoryFile;

	bool m_fReadSitStopGoFiles;	// Activar cuando se debe leer ficheros Sit
	bool m_fSiemensFatalError;	// Cuando el error de Siemens es fatal
	bool m_fWriteAGVPosition;	// Escribir la posición de cada AGV
	void GetStartTime();
	void SetStartTime (CTime newTime, BOOL fAutoStart);
	CTime m_StartTime;
	bool m_fAutoStart;
	bool m_fScrollList;	// Scroll lista de mensajes en el view principal
	bool m_fQuizasAnularDelivery;	// Flag para comprobar las órdenes en curso
	bool m_fSendSystemStatusToBuB;	// True si hay que enviar el estado del sistema al BuB
	UINT m_uCargadorArray [MAX_CARGADORES];	// Array de los cargadores ordenados
	void SetTotalAgvsAllowed (UINT numAgvs) { m_totalAgvsAllowed = numAgvs; };
	UINT GetTotalAgvsAllowed () { return (m_totalAgvsAllowed);};
	// Michael 24.05.2007 AGV9 bool IsActualizadoTodosCargadores () { return (m_bEstadoCargador & 0xFF == 0xFF);};
	bool IsActualizadoTodosCargadores () { return ((m_EstadoCargador & 0x1FF) == 0x1FF);};
	void SetEstadoCargador (UINT nCargador);
	CCriticalSection m_csAlarma;	// Critical Section para controlar aceso
	CTypedPtrArray <CObArray, CAlarma*> m_AlarmaArray; // Array de alarmas
	CGestorEstacionBuB m_GestorEstBuB;
	bool m_fRequestUpdateSiemens;
	bool IsEnFuncionamiento();
	bool m_fAutoOrders;		// Generate orders automatically when simulating
	bool m_fSimulateCom;	// Simulate Agv communications
	CPLCThread * m_pPLCThread;		// Thread para coms con el PLC
#if ! defined (FOREXPORTTOINDUMAT)
	CSiemensSR * m_pSR;		// Puntero s Send Receive class de Siemens
#endif
	CDBUtil m_dbutil;		// Utilidades de base de datos
	CWnd * m_pMainWnd;
	CWnd * m_pPLCWnd;		// Ventana del thread del PLC
	DWORD m_wPLCThreadId;	// Identificación del thread del PLC
	CTpcDoc * m_pDoc;
	CBuBThread * m_pBuBThread;
	DWORD m_wBuBThreadId;	// BuB thread id
	void SetPtrDoc (CTpcDoc * pDoc);
	void InitSystemState();
	void SetArrancadoAgvThread() {m_fAgvThreadArrancado = true;};
	bool IsArrancadoAgvThread() {return (m_fAgvThreadArrancado);};
	UCHAR GetSystemState() {return (m_SystemStatus.anla_zust);}; // Estado del sistema
	UCHAR GetSystemDrive() {return (m_SystemStatus.anla_fahrt);}; // Estado de los drives
	bool SystemDriveAllowed () {return (FAHRERLAUBNIS == m_SystemStatus.anla_fahrt);};
	bool IsFinDeJornada () {return (FEIERABEND == m_SystemStatus.anla_zust);};
	bool IsSincronizacion () {return (SYNCHRONISATION == m_SystemStatus.anla_zust); };
	UCHAR GetSystemAuto() {return (m_SystemStatus.betr_vaha);}; // System en Semi o Automático
	UCHAR GetSystemHostConnection () {return (m_SystemStatus.betr_art_host);}; // Host on-line / off-line
	UCHAR GetNumAgvError () {return (m_SystemStatus.FzSammelStoerung);};
	UCHAR GetNumStationError () {return (m_SystemStatus.StationsSammelStoerung);};

	void SetSystemState(UCHAR state) {m_SystemStatus.anla_zust = state;}; // Estado del sistema
	void SetSystemDrive(UCHAR state) {m_SystemStatus.anla_fahrt = state;}; // Estado de los drives
	void SetSystemAuto(UCHAR state) {m_SystemStatus.betr_vaha = state;}; // System en Semi o Automático
	void SetSystemHostConnection (UCHAR state) {m_SystemStatus.betr_art_host = state;
		m_fSendSystemStatusToBuB = true; }; // Host on-line / off-line
	void AddAgvError () {m_SystemStatus.FzSammelStoerung ++;};
	void AddStationError () {m_SystemStatus.StationsSammelStoerung ++;};
	void DeleteAgvError () {m_SystemStatus.FzSammelStoerung --;};
	void DeleteStationError () {m_SystemStatus.StationsSammelStoerung --;};

	void SetPtrView (CTpcView * pView);
	CContainer(CWnd* pParent = NULL, CTpcView* pView = NULL);
	virtual ~CContainer();
	CTpcView * m_pView;	// Principal view for interaction
	void GetRegisterVariables();
	CTypedPtrArray <CObArray, CPuntoDescanso*> m_PuntoDescansoArray; // Array of puntos de descanso
	CCriticalSection m_csPuntoDescanso;	// Critical Section para controlar aceso
	CCriticalSection m_csForzarOrden;	// Critical Section para la generación de Órdenes
	CCriticalSection m_csAsignarOrden;	// M2015 
	SHORT m_defArray [3][100];	// Array de defectos
	void SetBlockingTableCopied () { m_fBlockingTableCopied = true; }
	bool IsBlockingTableCopied () { return (m_fBlockingTableCopied); }


private:
	UINT m_totalAgvsAllowed;	// Número total de Agvs permitidos en circulación
	// Michael 24.05.2007 AGV9
	// BYTE m_bEstadoCargador;	// Indicador de que se ha recibido el estado de todos los cargadores
	int m_EstadoCargador;	// Indicador de que se ha recibido el estado de todos los cargadores
	bool m_fAgvThreadArrancado;	// Estado del thread de los Agv
	TYP_SYSTEMDATEN m_SystemStatus;
	//	CTypedPtrArray <CObArray, CMeldepunkt*> m_mpArray;	// Communications points
	//tAreaFinder m_AreaMatrix [KKMAX_EP_P1];	// Area Finder matrix (MAX Decision Points)		//JLac
	// TYP_STATDAT m_StatDat [MAX_STATIONEN+1];
	bool m_fSimulateEscribirPLC;	// 2009 Simulate write to PLC
	bool m_fBlockingTableCopied;




};

#endif // !defined(AFX_CONTAINER_H__9C63167B_63C0_4AAC_80F0_30C2400EF123__INCLUDED_)
