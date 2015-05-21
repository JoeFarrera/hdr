// GestorEstacionBuB.cpp: implementation of the CGestorEstacionBuB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "GestorEstacionBuB.h"
#include "dbestacion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
CGestorEstacionBuB::CGestorEstacionBuB()
{
	int AnzahlPosLage = 0;	// Number of positions ?
	for (int i = 1; i <= KMAX_STATIONEN; i++)
	{
		AnzahlPosLage += statdat[i].pos_anz_r;
		AnzahlPosLage += statdat[i].pos_anz_l;
	}

	
	for (int i = 1; i <= KMAX_STATIONEN ; i++)
	{
		// Lefthand side
		for (int pos = 0; pos < statdat [i].pos_anz_l; pos++)
		{
			CEstacionBuB * ptrEst = new CEstacionBuB;
			m_EstacionPtrArray.Add (ptrEst);
			ptrEst->Init (i, pos, 'L');
		}

		// right side
		for (int pos = 0; pos < statdat [i].pos_anz_r; pos++)
		{
			CEstacionBuB * ptrEst = new CEstacionBuB;
			m_EstacionPtrArray.Add (ptrEst);
			ptrEst->Init (i, pos, 'R');
		}
	} // for
}
*/

CGestorEstacionBuB::CGestorEstacionBuB()
{
// TODO	this->InitStations();	
}

CGestorEstacionBuB::~CGestorEstacionBuB()
{
}

void CGestorEstacionBuB::InitStations()
{
	this->m_pCourse = g_pCourseData->GetPtrCourseAGVType(1);
	int nStations = this->m_pCourse->GetNumberDestinations();

	int AnzahlPosLage = 0;	// Number of positions ?
	//for (int i = 1; i <= KMAX_STATIONEN; i++)
	for (int i = 0; i < nStations; i++)
	{
		AnzahlPosLage += this->m_pCourse->GetPtrDestinationByIndex(i)->GetRightPositions();
		//AnzahlPosLage += statdat[i].pos_anz_l;
		AnzahlPosLage += this->m_pCourse->GetPtrDestinationByIndex(i)->GetLeftPositions();
	}


	//for (int i = 1; i <= KMAX_STATIONEN; i++)
	for (int i = 0; i < nStations; i++)
	{
		// Lefthand side
		//for (int pos = 0; pos < statdat[i].pos_anz_l; pos++)
		CAGVTDestination* pDestination = this->m_pCourse->GetPtrDestinationByIndex(i);
		for (int pos = 1; pos <= (int)pDestination->GetLeftPositions(); pos++)
		{
			
			
			if (g_pCourseData->IsAIDStation(pDestination->GetNumber(), 'L', (int)pos, (int)1))
			{
				
				CEstacionBuB * ptrEst = new CEstacionBuB;
				m_EstacionPtrArray.Add(ptrEst);
				ptrEst->Init(i, pos, 'L');
			}
		}

		// right side
		//for (int pos = 0; pos < statdat[i].pos_anz_r; pos++)
		for (UINT pos = 1; pos <= pDestination->GetRightPositions(); pos++)
		{
			if (g_pCourseData->IsAIDStation(pDestination->GetNumber(), 'R', (int)pos, (int)1))
			{
				CEstacionBuB * ptrEst = new CEstacionBuB;
				m_EstacionPtrArray.Add(ptrEst);
				ptrEst->Init(i, pos, 'R');
			}
		}
	} // for
}

void CGestorEstacionBuB::SetPtrCourse(CCourseAGVType* pCourse)
{
	this->m_pCourse = pCourse;
}

CEstacionBuB * CGestorEstacionBuB::SetEstacionBuB(UINT mp, UINT pos, UCHAR lage, bool fError, bool fCargado)
{
	CEstacionBuB * ptrEstBuB = NULL;

	UINT numStations = m_EstacionPtrArray.GetSize ();
	for (UINT i = 0; i < numStations; i++)
	{
		ptrEstBuB = m_EstacionPtrArray.GetAt (i);
		if (ptrEstBuB->IsThis (mp, pos, lage))
		{
			ptrEstBuB->UpdateEstacion (fError, fCargado);
			break;
		}
	}
	return (ptrEstBuB);

}

// Actualizar datos de los descansos
void CGestorEstacionBuB::SetEstacionBuB(UINT mp, UINT pos, UCHAR lage, LPCTSTR estDes, UINT nAgvDes)
{
	CEstacionBuB * ptrEstBuB = NULL;

	UINT numStations = m_EstacionPtrArray.GetSize ();
	for (UINT i = 0; i < numStations; i++)
	{
		ptrEstBuB = m_EstacionPtrArray.GetAt (i);
		if (ptrEstBuB->IsThis (mp, pos, lage))
		{
			ptrEstBuB->UpdateEstacion (estDes, nAgvDes);
			break;
		}
	}
}


//Actualitza el BuB en cas de simulació
void CGestorEstacionBuB::UpdateEstacion()
{
	CEstacionBuB * ptrEstBuB = NULL;
	UINT numStations = m_EstacionPtrArray.GetSize ();
	for (UINT i = 0; i < numStations; i++)
	{
		ptrEstBuB = m_EstacionPtrArray.GetAt (i);
		// Solo interesan estaciones de carga/descarga
		if (ptrEstBuB->IsCargaDescarga())
		{
			CDBEstacion dbEstacion;
			if (dbEstacion.GetEstAgv(ptrEstBuB->m_mp, ptrEstBuB->m_pos, ptrEstBuB->m_lage))
			{
				if (dbEstacion.GetNumeroMesas() > 0)
				{
					ptrEstBuB->UpdateEstacion(
						dbEstacion.m_OcuPulmon,
						dbEstacion.m_CapPulmon,
						dbEstacion.GetCodEstPul(),	// Michael 12.12.2004 Esto no es el pulmón de vacio asociado
						dbEstacion.m_pEstAgv1->errmesa != 0, 
						dbEstacion.m_pEstAgv1->prepal[0] == 'S');

					if (dbEstacion.GetNumeroMesas() > 1)
						ptrEstBuB->UpdateEstacion(
							dbEstacion.m_OcuPulmon,
							dbEstacion.m_CapPulmon,
							dbEstacion.GetCodEstPul(),
							dbEstacion.m_pEstAgv2->errmesa != 0, 
							dbEstacion.m_pEstAgv2->prepal[0] == 'S');
				}
			}
		}
	}
}


CEstacionBuB * CGestorEstacionBuB::FindEstacionBuB(LPCTSTR psClientName)
{
	CEstacionBuB * ptrEstBuB = NULL;
	UINT numStations = m_EstacionPtrArray.GetSize ();
	for (UINT i = 0; i < numStations; i++)
	{
		ptrEstBuB = m_EstacionPtrArray.GetAt (i);
		if (ptrEstBuB->IsThis(psClientName))
		{
			return ptrEstBuB;
		}
	}

	return NULL;
}
