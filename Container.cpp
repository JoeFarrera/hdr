// Container.cpp: implementation of the CContainer class.
// 
// Principal data container for the TPC
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "Container.h"
#include "PuntoDescanso.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContainer::CContainer(CWnd* pParent /* = NULL */, CTpcView* pView /*= NULL */)
{
	m_pView = pView;
	m_pDoc = NULL;
	m_pMainWnd = pParent;
	m_fSimulateCom = false;	// Serán actualizados 
	m_fSimulateEscribirPLC = false;
	m_fAutoOrders = false;
	m_fAgvThreadArrancado = false;
	InitSystemState ();
	m_fRequestUpdateSiemens = false;
	m_EstadoCargador = 0;
	m_totalAgvsAllowed = 0;
	m_fSendSystemStatusToBuB = false;
	m_fQuizasAnularDelivery = false;
	m_fScrollList = true;
	m_StartTime = 0;
	m_wPLCThreadId = 0;
	m_pSR = NULL;
	m_fSiemensFatalError = false;
	m_fReadSitStopGoFiles = false;
	m_fWriteAgvHistoryFile = false;
	m_strRadio.Empty ();
	m_strTren.Empty ();
	// m_uCargadorArray es inicializado por llamada a CDBEstacion::InitArrayCargadores
}

CContainer::~CContainer()
{
}

void CContainer::InitStations() {
	m_GestorEstBuB.InitStations();
}



void CContainer::SetPtrView(CTpcView *pView)
{
	m_pView = pView;

}

// Inicializar estado del sistema al arrancar
void CContainer::InitSystemState()
{
	m_SystemStatus.anla_zust = SYNCHRONISATION;	// Inicializando

	// XXX-Lac01:
	//m_SystemStatus.anla_fahrt = FAHRERLAUBNIS;	// Drive permitted
	m_SystemStatus.anla_fahrt = FAHRVERBOT;	// Drive forbidden

	m_SystemStatus.betr_vaha = VOLL_AUTO;		// Fully automatic (no semi automatic)
	m_SystemStatus.betr_art_host = HOST_OFFLINE;
	m_SystemStatus.FzSammelStoerung = 0;	// Agvs en error
	m_SystemStatus.StationsSammelStoerung = 0;	// Estaciones en error
	m_SystemStatus.numAgvAllowed = 0;
	m_SystemStatus.numAgvEnCircuito = 0;
	m_fBlockingTableCopied = false;
}

void CContainer::SetPtrDoc(CTpcDoc *pDoc)
{
	m_pDoc = pDoc;
}



void CContainer::GetRegisterVariables()
{
   int nVal = AfxGetApp ()->GetProfileInt ("Settings", "SimulateCom", -1);
   if (-1 == nVal)
   {
		AfxGetApp ()->WriteProfileInt ("Settings", "SimulateCom", 0);
   }
   else
	   m_fSimulateCom = nVal == 0 ? false : true;
   
   nVal = AfxGetApp ()->GetProfileInt ("Settings", "AutoOrders", -1);
   if (-1 == nVal)
   {
		AfxGetApp ()->WriteProfileInt ("Settings", "AutoOrders", 0);
   }
   else
	   m_fAutoOrders = nVal == 0 ? false : true;

   nVal = AfxGetApp ()->GetProfileInt ("Settings", "CaminoAlternativo", -1);
   if (-1 == nVal)
   {
		AfxGetApp ()->WriteProfileInt ("Settings", "CaminoAlternativo", 0);
   }
   else
	   m_fCaminoAlternativo = nVal == 0 ? false : true;

   nVal = AfxGetApp ()->GetProfileInt ("Settings", "SimulateEscribirPLC", -1);

   if (-1 == nVal)
   {
		AfxGetApp ()->WriteProfileInt ("Settings", "SimulateEscribirPLC", 0);
   }
   else
	   m_fSimulateEscribirPLC = nVal == 0 ? false : true;

   nVal = AfxGetApp ()->GetProfileInt ("Settings", "CaminoAlternativo203", -1);
   if (-1 == nVal)
   {
		AfxGetApp ()->WriteProfileInt ("Settings", "CaminoAlternativo203", 0);
		m_fCaminoAlternativo203 = false;
   }
   else
	   m_fCaminoAlternativo203 = nVal == 0 ? false : true;


   nVal = AfxGetApp ()->GetProfileInt ("Settings", "CaminoAlternativo303", -1);

   if (-1 == nVal)
   {
		AfxGetApp ()->WriteProfileInt ("Settings", "CaminoAlternativo303", 0);
		m_fCaminoAlternativo303 = false;
   }
   else
	   m_fCaminoAlternativo303 = nVal == 0 ? false : true;



   GetStartTime();

}

bool CContainer::IsEnFuncionamiento()
{
	return (BETRIEBSBEREIT == m_SystemStatus.anla_zust);

}

// Indicar que se ha recibido el estado del cargador pasado en parámetro
void CContainer::SetEstadoCargador(UINT nCargador)
{
	m_EstadoCargador |= 0x1 << (nCargador - 1);

}

// Devolver la hora programada de inicio...
void CContainer::GetStartTime()
{
	CTime curTime = CTime::GetCurrentTime ();

	int nVal = AfxGetApp ()->GetProfileInt ("Settings", "ProgAutoStart", -1);
	if (-1 == nVal)
		m_fAutoStart = false;
	else
		m_fAutoStart = nVal ? true : false;

	nVal = AfxGetApp ()->GetProfileInt ("Settings", "ProgTimeDay", -1);
	if (-1 == nVal)
		nVal = curTime.GetDay ();
	int nDay = nVal;

	nVal = AfxGetApp ()->GetProfileInt ("Settings", "ProgTimeMonth", -1);
	if (-1 == nVal)
		nVal = curTime.GetMonth ();
	int nMonth = nVal;

	nVal = AfxGetApp ()->GetProfileInt ("Settings", "ProgTimeYear", -1);
	if (-1 == nVal)
		nVal = curTime.GetMonth ();
	int nYear = nVal;

	nVal = AfxGetApp ()->GetProfileInt ("Settings", "ProgTimeHour", -1);
	if (-1 == nVal)
		nVal = curTime.GetHour ();
	int nHour = nVal;

	nVal = AfxGetApp ()->GetProfileInt ("Settings", "ProgTimeMinute", -1);
	if (-1 == nVal)
		nVal = curTime.GetMinute ();
	int nMin = nVal;

	nVal = AfxGetApp ()->GetProfileInt ("Settings", "ProgTimeSecond", -1);
	if (-1 == nVal)
		nVal = curTime.GetSecond ();
	int nSec = nVal;

	if (nYear < 2000)
		nYear += 2001;
	CTime CurTimeSet = CTime(nYear, nMonth, nDay, nHour, nMin, nSec, -1 );


	m_StartTime = CurTimeSet;
}

void CContainer::SetStartTime(CTime newTime, BOOL fAutoStart)
{
	m_StartTime = newTime;
	m_fAutoStart = fAutoStart ? true : false;
	AfxGetApp ()->WriteProfileInt ("Settings", "ProgAutoStart", m_fAutoStart);
	AfxGetApp ()->WriteProfileInt ("Settings", "ProgTimeYear", m_StartTime.GetYear ());
	AfxGetApp ()->WriteProfileInt ("Settings", "ProgTimeMonth", m_StartTime.GetMonth ());
	AfxGetApp ()->WriteProfileInt ("Settings", "ProgTimeDay", m_StartTime.GetDay ());
	AfxGetApp ()->WriteProfileInt ("Settings", "ProgTimeHour", m_StartTime.GetHour ());
	AfxGetApp ()->WriteProfileInt ("Settings", "ProgTimeMinute", m_StartTime.GetMinute ());
	AfxGetApp ()->WriteProfileInt ("Settings", "ProgTimeSecond", m_StartTime.GetSecond ());

}

void CContainer::SetCaminoAlt(bool fCaminoAlt)
{
	m_fCaminoAlternativo = fCaminoAlt;
	AfxGetApp ()->WriteProfileInt ("Settings", "CaminoAlternativo", m_fCaminoAlternativo ? 1 : 0);
}

bool CContainer::IsSimulateEscribirPLC()
{
	return m_fSimulateEscribirPLC;
}

void CContainer::SetCaminoAlt203(bool fCaminoAlt203)
{
	m_fCaminoAlternativo203 = fCaminoAlt203;
	AfxGetApp ()->WriteProfileInt ("Settings", "CaminoAlternativo203", m_fCaminoAlternativo203 ? 1 : 0);

}

void CContainer::SetCaminoAlt303(bool fCaminoAlt303)
{
	m_fCaminoAlternativo303 = fCaminoAlt303;
	AfxGetApp ()->WriteProfileInt ("Settings", "CaminoAlternativo303", m_fCaminoAlternativo303 ? 1 : 0);

}