// GestorOrden.h: interface for the CGestorOrden class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GESTORORDEN_H__CB03353E_2C8D_4BD2_99C8_DE3A9906E40F__INCLUDED_)
#define AFX_GESTORORDEN_H__CB03353E_2C8D_4BD2_99C8_DE3A9906E40F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bed_zs.h"
#include "DBGestorOrden.h"

class COrden;
class CDBGestorOrden;
class CAgv;

class CGestorOrden  
{
public:
	bool m_fMirarTodos;	// Flag para indicar que se deben mirar todas las órdenes
	void InsertarOrdenRechazo (CAgv * ptrAgv);
	void QuizasEnviarRechazo();
	void AddOrden (COrden * ptrOrden);
	void SetTiempoPartidaOrden (UINT tiempoPartida);
	void SetFrecuenciaOrden (UINT frecuencia);
	void SetAugmentoTiempo(UINT augmento);
	UINT GetAugmentoTiempo () { return (m_augmentoTiempoOrden);};
	UINT GetFrecuenciaOrden () { return (m_frecuenciaOrden);};
	UINT GetPartidaOrden () { return (m_tiempoPartidaOrden);};
	void InitTiempoEspera();
	void SetGetOrdenesDB () { m_fGetOrdenesDB = true; };
	void GetAutomaticOrden();
	void InsertarOrden (COrden * ptrOrden);
	CDBGestorOrden m_dbGestor;
	bool BuBUpdateOrden (send_ma_tele& tele);
	bool m_fUpdateRequired;	// Indicar al BuB que han habido cambios
	bool BuBSuspendReleaseOrden (send_at_tele& tele);
	bool BuBDeleteOrden (unsigned char * id_nr, bool finalizar);
	bool BuBUpdateOrden (send_at_tele& tele);
	void GetNewOrdenId(unsigned char* szId);
	void UpdateEstado();
	bool AsignarOrden(COrden * ptrOrden);
	COrden * ExisteOrden (COrden * pOrden);
	void InsertarOrdenPtrManual(COrden * pOrden);
	CGestorOrden();
	virtual ~CGestorOrden();
	CTypedPtrArray <CObArray, COrden*> m_OrdenPtrArray; // Array of orden pointers
	COrden * GetOrdenPtr (unsigned char * szId, int *index);
	void DeleteOrden(int index);
private:
	bool m_fMirarTodosPrivate;	// Cópia privada del flag para indicar que se deben mirar todas las órdenes
	UINT m_augmentoTiempoOrden;	// Augmento de tiempo a sumar al tiempo de espera de un Agv
	UINT m_tiempoPartidaOrden;	// Tiempo al iniciar la busqueda de una orden. Segundos
	UINT m_frecuenciaOrden;	// Frecuencia con la cual se debe augmentar radio de busqueda de la orden
	void GetBuBIndex(COrden * ptrOrden);
	CList <int, int> m_listBuB;	// BuB index list
	bool m_fGetOrdenesDB;		// Recuperar órdenes de la base de datos
	bool TestDeleteStationError (COrden * ptrOrden, bool finalizar);
	bool QuitarAgvOrden(COrden& orden);
	UINT m_NextSimulationStation;	// Para generar órdenes por simulación
	void SimulateNewOrden(UINT nAgv);
	UINT m_OrdenIdCtr;
 
};

extern CGestorOrden g_GestorOrden;

#endif // !defined(AFX_GESTORORDEN_H__CB03353E_2C8D_4BD2_99C8_DE3A9906E40F__INCLUDED_)
