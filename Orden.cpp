// Orden.cpp: implementation of the COrden class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "Destination.h"
#include "Orden.h"
#include "DBEstacion.h"
#include "GestorOrden.h"
#include "Agv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrden::COrden()
{
	m_estado = NEWORDER;	// No cambiar. Ver GetOrden de DBGestorOrden
	SetOrderInsertTime ();
	m_To.SetDestino (&m_at.bz);
	m_ptrEstOri = NULL;
	m_ptrEstDes = NULL;
	m_uBuBIndex = 0;

	m_insertTime = CAgv::GetTimePlusInterval (0);
	m_lastTime.QuadPart = 0;
	m_tiempoEspera = 0;
	m_at.prio_akt = 1;
}

COrden::~COrden()
{
	if (m_ptrEstOri)
		delete m_ptrEstOri;
	if (m_ptrEstDes)
		delete m_ptrEstDes;

}


// Comparar dos ordenes
bool COrden::operator ==(COrden &orden)
{
	int i = strcmp ((const char *)m_at.id_nr, (const char *)orden.m_at.id_nr);
	return (i == 0 ? true : false);

}

// Asignar la orden a un AGV
void COrden::SetAssigned(UINT fznr)
{
	g_GestorOrden.m_dbGestor.UpdateEstadoOrden (this, PENDINGASSIGN);	// Actualizar en la base de datos
	m_estado = PENDINGASSIGN;
	m_at.af_status = STATUS_HOLEN;
	m_at.fznr = fznr;
	m_fBuB = true;
}

void COrden::SetOrderInsertTime()
{
	CTime time = CTime::GetCurrentTime ();
	int year = time.GetYear ();

	m_at.datum.jahr  = (unsigned short) year;
	m_at.datum.monat = (unsigned char)time.GetMonth ();
	m_at.datum.tag = (unsigned char)time.GetDay ();
	m_at.datum.stunde = (unsigned char)time.GetHour ();
	m_at.datum.minute  = (unsigned char)time.GetMinute ();
	m_at.datum.sekunde = (unsigned char)time.GetSecond ();
}

void COrden::SetEstado (enum TEstadoOrden estado)
{
	switch (estado)
	{
	case (DELIVERED):
		m_at.af_status = STATUS_AE;	// Finished
		break;
	case (COLLECT):
		m_at.af_status = STATUS_HOLEN;
		break;
	case (DELIVER):
		m_at.af_status = STATUS_BRINGEN;
		break;
	}

#pragma message("COrden::SetEstado: What to do when an Agv resets an order previously assigned ?")

	g_GestorOrden.m_dbGestor.UpdateEstadoOrden (this, estado);	// Actualizar en la base de datos
	m_estado = estado;

	m_fBuB = true;

};

void COrden::GetDatosEstacion()
{
	if (IsManual ())
	{
		m_ptrEstOri = new CDBEstacion ();
		m_ptrEstDes = new CDBEstacion ();
		// M2015 Puede ser que se ha comunicado el código de la estación
		if (m_at.az.nDest == 0)
		{
			CEstacionBuB * ptrEstBuB = g_Container.m_GestorEstBuB.FindEstacionBuB ((LPCTSTR)m_at.BzString);
			if (ptrEstBuB != NULL) 
			{
				m_at.az.nDest = ptrEstBuB->m_mp;
				m_at.az.nPos = ptrEstBuB->m_pos;
				m_at.az.ucSide = ptrEstBuB->m_lage;

			}
		}
		else {
			// TODO Get the customer name 
			// CEstacionBuB * ptrEstBuB = g_Container.m_GestorEstBuB. (m_at.BzString);
		}
		m_ptrEstOri->GetEstAgv (m_at.az.nDest, m_at.az.nPos, m_at.az.ucSide);
		m_ptrEstDes->GetEstAgv (m_at.az.nDest, m_at.az.nPos, m_at.az.ucSide);
	}
	else
	{
		m_ptrEstOri->GetEstAgv (m_at.hz.nDest, m_at.hz.nPos, m_at.hz.ucSide);

		m_ptrEstDes = new CDBEstacion ();
		m_ptrEstDes->GetEstAgv (m_at.bz.nDest, m_at.bz.nPos, m_at.bz.ucSide);
	}


}


// Calcular el tiempo máximo que se debe esperar para asignar un Agv...
UINT COrden::GetMaxTiempoEspera(UINT frecuencia, UINT augmento, UINT partida)
{
	m_lastTime = CAgv::GetTimePlusInterval (0);
	UCHAR prio;

	// Michael 28.02.2002 Convertir partida a millisegundos. Añadir a elapsed.
	partida = partida * 1000;

	// Tiempo transcurrido en ms
	UINT elapsedTime = (UINT)(m_lastTime.QuadPart - m_insertTime.QuadPart) / 100000;

	// Michael 26.02.2002 Tratamiento prioridad
	prio = m_at.prio_akt;

	if (1 == prio)
	{
		// El TPC solo actualiza prioridad de 1->2. Más prioritario es por el BuB
		prio = CDBGestorOrden::GetPrioridadOrden (atoi ((char *)m_at.id_nr));
		if (prio != m_at.prio_akt)
		{
			m_at.prio_akt = prio;
			m_fBuB = true;
		}
	}

	if ((prio - 1) >= 0)
		prio --;



	// Michael 25.06.2002 Augmenta orden sin prioridad, poc a poc

	if (0 == prio)
		m_tiempoEspera = partida + ((elapsedTime / frecuencia) * 10);
	else
		m_tiempoEspera = partida + ((elapsedTime / frecuencia) * (augmento * prio));

	if (m_tiempoEspera < partida)
		m_tiempoEspera = partida;

	// to be sure !!!
	if (m_tiempoEspera == 0)
		m_tiempoEspera = augmento;

	return (m_tiempoEspera);
}


// Ver si ha transcurrido el tiempo de espera para volver a intentar asignar una orden
bool COrden::IsOkMirar(UINT frecuencia)
{
	LARGE_INTEGER curTime = CAgv::GetTimePlusInterval (0);
	UINT elapsedTime = (UINT)(curTime.QuadPart - m_lastTime.QuadPart)/100000;

	if (elapsedTime >= frecuencia)
		return (true);
	else
		return (false);
}
