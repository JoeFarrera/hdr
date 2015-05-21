// PuntoDescanso.cpp: implementation of the CPuntoDescanso class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "PuntoDescanso.h"
#include "Agv.h"
#include "GestorAgv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPuntoDescanso::CPuntoDescanso()
{
	m_ptoCom = 0;
	m_pos = 0;
	m_lado = 0;
	m_numAgvsAllowed = 0;


}

CPuntoDescanso::~CPuntoDescanso()
{

}

CPuntoDescanso::CPuntoDescanso(UINT ptoCom, UINT pos, UCHAR lado, UINT numAgv, UINT urgency)
{
	m_ptoCom = ptoCom;
	m_pos = pos;
	m_lado = lado;
	m_numAgvsAllowed = numAgv;
	m_urgency = urgency;

}


// Determinar número de Agvs en el punto
UINT CPuntoDescanso::SetNumAgvs()
{
	m_numAgvs = 0;
	//for (int i = 0; i < KKFZMAX; i++)
	for (int i = 0; i < g_pCourseData->GetNumberAGVs(); i++)
	{
		CAgv * ptrAgv = g_pGestor->GetAgvPtr (i + 1);
		UINT estado = ptrAgv->GetEstado ();
		switch (ptrAgv->GetEstado ())
		{
		case IRPUNTOESP:
		case PUNTOESP:
		case IRCARGARORD:	// Michael 18.02.2002 - Los que van a cargar también
							// Pero los CARGARORD no (Ya se va)
			if (ptrAgv->m_pFzdat->mp == m_ptoCom 
				|| ptrAgv->m_pFzdat->auftziel.nDest== m_ptoCom)
				m_numAgvs ++;
		}
	}
	return (m_numAgvs);
}

void CPuntoDescanso::SetZiel(TYPE_STATION_DEF *ptrZiel)
{
	CAgv::SetZiel (ptrZiel,
		m_ptoCom,
		m_pos,
		m_lado);
}

