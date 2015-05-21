// DBGestorOrden.h: interface for the CDBGestorOrden class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBGESTORORDEN_H__D4008287_7DAC_4A4E_B3D0_F5B33E600B40__INCLUDED_)
#define AFX_DBGESTORORDEN_H__D4008287_7DAC_4A4E_B3D0_F5B33E600B40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Orden.h"

class COrden;
typedef struct
{
 int codord;
 int agv;
 char feccre [8 + 1];
 char fecasig [8 + 1];
 char estado [1 + 1];
 char fecfin [8 + 1];
 int priini;
 int priact;
 int prifin;
 char codart [10 + 1];
 char estdesagv [9 + 1];
 char estoriagv [9 + 1];
 int numpal;
 char estdestpc [7 + 1];
 char estoritpc [7 + 1];
 char tipord [1 + 1];
} TOrden;


class CDBGestorOrden  
{
public:
	static UCHAR GetPrioridadOrden(int codord);
	static void ForzarOrden();
	static void IniciarSistema();
	static void ModificarOrden (COrden * ptrOrden);
	static COrden * GetOrden(int estado = NOTHING, UINT nAgv = 0);
	void InsertarOrden (COrden * ptrOrden);
	CDBGestorOrden();
	virtual ~CDBGestorOrden();
	void UpdateEstadoOrden (COrden * ptrOrden, TEstadoOrden estadoNuevo);
	void DeleteOrden (COrden * ptrOrden);

protected:

private:
	T_sqlca sqlca;
	T_oraca oraca;

};

#endif // !defined(AFX_DBGESTORORDEN_H__D4008287_7DAC_4A4E_B3D0_F5B33E600B40__INCLUDED_)
