// Orden.h: interface for the COrden class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDEN_H__5B0E8687_5134_475E_A56A_8E7D56C0B426__INCLUDED_)
#define AFX_ORDEN_H__5B0E8687_5134_475E_A56A_8E7D56C0B426__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KONSTANT.H"
#include "KOMDAT.H"
// #include "a1_konst.c86"
#include "typedef.h"
#include "Destination.h"	// Added by ClassView

class CDBEstacion;


enum TEstadoOrden {
	NOTHING,	// Orden nuevo sin tocar
	NEWORDER,	//	STATUS_NEU            1     /* Auftrag ist neu                  */	// New
	COLLECT,	// STATUS_HOLEN          2     /* Auftrag ist im Holenstatus       */	// Collect
	DELIVER,	// STATUS_BRINGEN        3     /* Auftrag ist im Bringenstatus     */	// Deliver
	PENDINGASSIGN,	// Pending acceptance from AGV
	PENDINGCANCEL,	// Pending cancel confirmation from AGV
	DELIVERED,		// Orden entregada - Finalizar...
	CANCELLED		// Anulado - Finalizar...
};

class COrden : public CObject  
{
public:
	bool IsOkMirar(UINT frecuencia);
	UINT GetMaxTiempoEspera(UINT frecuencia, UINT augmento, UINT partida = 0);
	UINT m_uBuBIndex;
	bool IsManual() { return ('S' == m_at.herkunft);}; // Agv Transport orden o no
	void GetDatosEstacion();
	CDestination m_To;	// Destino de la orden
	void SetOrderInsertTime();	// Poner la fecha de inserción de la orden
	void ResetBuB () { m_fBuB = false; }; // Reset flag de avisar BuB
	void SetAssigned (UINT fznr);
	COrden();
	virtual ~COrden();
	bool operator==(COrden& orden);
	TYP_AT_DAT m_at;
	TEstadoOrden GetEstado () {return (m_estado);};
	void SetEstado (TEstadoOrden estado);
	bool m_fBuB;	// Avisar el BuB cuando hay cambios
	CDBEstacion * m_ptrEstDes;	// Datos estación Destino
	CDBEstacion * m_ptrEstOri;	// Datos estación Origen
	void SetEstadoWithoutDB (TEstadoOrden estado) { m_estado = estado; };



private:
	UINT m_tiempoEspera;
	LARGE_INTEGER m_insertTime;	// Hora en que se ha creado la orden
	LARGE_INTEGER m_lastTime;	// Hora última en que se ha mirado de lanzar la orden
	TEstadoOrden m_estado;	// Estado de la orden
};

#endif // !defined(AFX_ORDEN_H__5B0E8687_5134_475E_A56A_8E7D56C0B426__INCLUDED_)
