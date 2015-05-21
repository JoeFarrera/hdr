// GestorOrden.cpp: implementation of the CGestorOrden class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "tpcView.h"
#include "Orden.h"
#include "bed_zs.h"
#include "DBGestorOrden.h"
#include "DBEstacion.h"
#include "GestorOrden.h"
#include "Agv.h"
#include "GestorAgv.h"
#include "MACRO.H"
#include "BuBThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGestorOrden::CGestorOrden()
{

	m_OrdenIdCtr = 0;	// XXX Michael: Initialize this as Oracle Sequence
	m_fUpdateRequired = true;	// Para el BuB
	m_NextSimulationStation = 1;
	m_fGetOrdenesDB = true;

	m_frecuenciaOrden = 0;
	m_augmentoTiempoOrden = 0;
	m_tiempoPartidaOrden = 0;
	m_fMirarTodos = false;
	m_fMirarTodosPrivate = false;


}

CGestorOrden::~CGestorOrden()
{

}

// Insertar una orden manual en la lista de órdenes
void CGestorOrden::InsertarOrdenPtrManual(COrden *pOrden)
{
	GetBuBIndex (pOrden);
	m_OrdenPtrArray.Add (pOrden);
}

// Si existe una orden, devolverla. Si no devolver null
COrden * CGestorOrden::ExisteOrden(COrden *pOrden)
{
	int numOrden = m_OrdenPtrArray.GetSize ();
	
	for (int i = 0; i < numOrden; i++ )
	{
		COrden * ptrOrden = m_OrdenPtrArray.GetAt (i);
		if (&ptrOrden == &pOrden)
			return ptrOrden;
	}
	return (NULL);
}


// Asignar orden a un AGV en condiciones
bool CGestorOrden::AsignarOrden(COrden * ptrOrden)
{
	bool fAsigned = false;
	int nAgvSelected = 0;
	TEstadoAGV estado;
	UINT timeToPickup = 0, previousTimeToPickup = 999999;
	CAgv * ptrAgv;
	
	// Solo lanzamos órdenes nuevos si está el sistema en funcionamiento
	if (g_Container.IsEnFuncionamiento () || ptrOrden->IsManual ())
	{
		// Mirar la orden si un Agv solicita que se miren todos o por cumplir el tiempo desde la última mirada
		if (m_fMirarTodosPrivate || ptrOrden->IsOkMirar (m_frecuenciaOrden))
		{	

			UINT maxTimeToPickup = ptrOrden->GetMaxTiempoEspera (m_frecuenciaOrden,
				m_augmentoTiempoOrden,
				m_tiempoPartidaOrden);
			
			if (! ptrOrden->IsManual ())
			{
				// Michael 02.01.2002 Mirar si la orden puede ser asignada
				UINT uRes = ptrOrden->m_ptrEstOri->GetCarga ((LPCTSTR)ptrOrden->m_at.id_nr);
				if (5 == uRes)
				{
					// Tren / Estación no permite Carga o Descarga...
					CAlarma * ptrAlarma = new CAlarma;
					ptrAlarma->AvisarAlarma (13, 1, 1,0,0,0,0,0,0,
						(char *)ptrOrden->m_at.id_nr); 

					ptrOrden->SetEstado (CANCELLED);
				}
				else
				{
					// Ver si habrá un Agv cerca por su destino.
					// Si no, no tiene sentido esperar para asignar la orden
					/* Michael 16.02.2002 Revisar....
					for (int nAgv = 1; nAgv <= KKFZMAX; nAgv ++) 
					{
						ptrAgv = g_pGestor->GetAgvPtr (nAgv);
						if (ptrAgv->IsPossibleCandidateForOrder ())
						{
							timeToPickup = ptrAgv->GetTimeToPickup (ptrOrden->m_at.hz.nr,
								ptrAgv->m_pFzdat->auftziel.nr);
							if (timeToPickup < previousTimeToPickup)
							{
								previousTimeToPickup = timeToPickup;
							}
						}
					}
					maxTimeToPickup = maxTimeToPickup < previousTimeToPickup ? 
						previousTimeToPickup : maxTimeToPickup;

					previousTimeToPickup = 999999; // Reseteamos para la busqueda siguiente...
					*/


					//for (int nAgv = 1; nAgv <= KKFZMAX && !fAsigned; nAgv ++) 
					for (int nAgv = 1; nAgv <= g_pCourseData->GetNumberAGVs() && !fAsigned; nAgv++)
					{
						ptrAgv = g_pGestor->GetAgvPtr (nAgv);
						if (ptrAgv->IsCandidateForOrder (&estado, ptrOrden))
						{
							timeToPickup = ptrAgv->GetTimeToPickup (ptrOrden->m_at.hz.nDest);
							if (timeToPickup <= maxTimeToPickup)
							{
								
								if (timeToPickup < previousTimeToPickup)
								{
									// This Agv may already have an easier order
									if (ptrAgv->m_pOrden)
									{
										if (timeToPickup < 
											ptrAgv->GetTimeToPickup (ptrAgv->m_pOrden->m_at.hz.nDest))
										{
											// This order is closer than the current Agv order
											previousTimeToPickup = timeToPickup;
											nAgvSelected = nAgv;
										}
									}
									else
									{
										// Ok - el Agv no tiene order
										previousTimeToPickup = timeToPickup;
										nAgvSelected = nAgv;
									}
									
								}
							}
							else
								MSGTRACE5 ("Agv:%d mp:%d desestimado por tiempoPickup %d ; maxTiempoPickup %d Orden %s",
								nAgv, ptrAgv->m_pFzdat->mp, timeToPickup, maxTimeToPickup, (LPCTSTR)ptrOrden->m_at.id_nr);
						}
					}
				}

				//if (KKFZMAX >= nAgvSelected && nAgvSelected != 0)
				if (g_pCourseData->GetNumberAGVs() >= nAgvSelected && nAgvSelected != 0)
				{
					ptrAgv = g_pGestor->GetAgvPtr (nAgvSelected);
					
					MSGTRACE3 ("Selected AGV: %d for order con destation: %d and time: %d",
						nAgvSelected, 
						ptrOrden->m_at.hz.nDest, 
						previousTimeToPickup);
					CSingleLock lockAgv (&ptrAgv->m_cs);
					// Block access to the Agv to verify availability
					TRACE1 ("Esperando Aceso AGV %d\n", nAgvSelected);
					lockAgv.Lock ();
					TRACE1 ("Aceso Agv %d granted...\n", nAgvSelected);
					if (ptrAgv->IsCandidateForOrder (&estado, ptrOrden))
					{
						switch (estado)
						{
						case ORDENNUEVO:		
						case ORDENNUEVOMANUAL:	
						case IRCARGARORD:
							ptrAgv->m_pOrden->SetEstado (NEWORDER);
							ptrAgv->ResetOrden ();
							ptrAgv->SetEstado (AGVSINDATOS);
							
							// Continue...
						case AGVSINDATOS:
						case IRPUNTOESP:
						case PUNTOESP:
							ptrAgv->SetOrden (ptrOrden);
							ptrOrden->SetAssigned (ptrAgv->m_id);
							
							MSGAVISO2 ("Orden %s asignado al AGV %d", 
								ptrOrden->m_at.id_nr,
								ptrAgv->m_id);
							
							fAsigned = true;
							break;
						default:
							MSGTRACE2 ("CGestorOrden::AsignarOrden(): Agv %d estado: %s",
								ptrAgv->m_id,
								ptrAgv->GetStringEstado ());
							break;
						}
					}
					else
						MSGTRACE1 ("CGestorOrden::AsignarOrden() - Agv %d no longer available !!",
						ptrAgv->m_id);
					
					lockAgv.Unlock ();
				}
			}
			else
			{
				// Manual order
				CAgv * ptrAgv = g_pGestor->GetAgvPtr (ptrOrden->m_at.fznr);
				TRACE1 ("Esperando Aceso AGV %d\n", ptrOrden->m_at.fznr);
				CSingleLock lockAgv (&ptrAgv->m_cs);	// Bloquear acceso al AGV...
				lockAgv.Lock ();
				TRACE1 ("Aceso Agv %d granted...\n", ptrOrden->m_at.fznr);
				int estado = ptrAgv->GetEstado ();
				switch (estado)
				{
				case AGVSINDATOS:
				case IRPUNTOESP:
				case PUNTOESP:
					ptrAgv->SetOrden (ptrOrden);
					ptrOrden->SetAssigned (ptrAgv->m_id);
					MSGAVISO2 ("Orden MANUAL %s asignado al AGV %d", 
						ptrOrden->m_at.id_nr,
						ptrAgv->m_id);
					
					fAsigned = true;
					break;
				default:
					MSGERROR2 ("CGestorOrden::AsignarOrden()(MANUAL): Agv %d estado: %s",
						ptrAgv->m_id,
						ptrAgv->GetStringEstado ());
					break;
					
				}
				lockAgv.Unlock ();
			}
	} // IsOkMirar
	} // IsEnFuncionamiento
	
	
	return (fAsigned);
	
}
// Intentar actualizar el estado de las órdenes
void CGestorOrden::UpdateEstado()
{
	bool fRet = false;

	if (! g_Container.IsArrancadoAgvThread ())
		return;
	if (m_fGetOrdenesDB)
	{
		m_fGetOrdenesDB = false;
		GetAutomaticOrden ();
	}
	
	m_fMirarTodosPrivate = m_fMirarTodos;
	m_fMirarTodos = false;

	int numOrden = m_OrdenPtrArray.GetSize ();
	for (int i = 0; i < numOrden; i++ )
	{
		COrden * ptrOrden = m_OrdenPtrArray.GetAt (i);
		switch (ptrOrden->GetEstado ())
		{
		case NEWORDER:
			fRet = AsignarOrden (ptrOrden);
			break;
		case DELIVERED:
			// Michael: Delivered / Cancelled will be different in database
			if (false == ptrOrden->m_fBuB)
			{
				// dejar que se haga la visualización primero
				DeleteOrden (i);
				i--;
				numOrden = m_OrdenPtrArray.GetSize ();
			}
			break;
		case CANCELLED:
			if (false == ptrOrden->m_fBuB)
			{
				DeleteOrden (i);
				i--;
				numOrden = m_OrdenPtrArray.GetSize ();
			}
			break;
		case PENDINGCANCEL:
			if (QuitarAgvOrden (*ptrOrden))
			{
				DeleteOrden (i);
				i--;
				numOrden = m_OrdenPtrArray.GetSize ();
			}

			break;

		}
		
	}

	// Michael 17.02.2002 Poner flag que hemos mirado Agvs.
	if (m_fMirarTodosPrivate)
	{
		//for (int nAgv = 1; nAgv <= KKFZMAX; nAgv++)
		for (int nAgv = 1; nAgv <= g_pCourseData->GetNumberAGVs(); nAgv++)
		{
			CAgv * ptrAgv = g_pGestor->GetAgvPtr(nAgv);
			if (ptrAgv->m_cOrdenesOk < 3)
				ptrAgv->m_cOrdenesOk++;

		}
	}



}

// Devolver identificador de orden nueva. Incrementa el contador
void CGestorOrden::GetNewOrdenId(unsigned char* szId)
{
	sprintf ((char *)szId, "%*d", KMAXID_AT - 1, ++m_OrdenIdCtr);
}

// Actualizar (modificar) una orden desde el BuB: Puede modificar prioridad o destino
bool CGestorOrden::BuBUpdateOrden(send_at_tele& tele)
{
	COrden * ptrOrden;
	int index;
	ptrOrden =  GetOrdenPtr (tele.id_nr, &index);
	bool fSuccess = true;

	if (NULL != ptrOrden)
	{
		if (! (ptrOrden->m_To == tele.bz))
		{
			// Destino diferente
			switch (ptrOrden->GetEstado ())
			{
			case PENDINGCANCEL:
			case DELIVERED:
				// No se puede modificar en en este estado
				fSuccess = false;
				break;
			default:
				ptrOrden->m_at.bz = tele.bz;
				fSuccess = true;
				break;
			}
		}
		// Actualizar prioridad
		ptrOrden->m_at.prio_akt = tele.prioritaet;
		g_GestorOrden.m_dbGestor.ModificarOrden (ptrOrden);	// Actualizar en la base de datos

		ptrOrden->m_fBuB = true;	// avisar al BuB
	}
	else
		// no se ha encontrado la orden
	{
		CString strTemp;
		AfxFormatString1 (strTemp, IDS_NOORDMODIFICAR, (const char *)tele.id_nr);
		MSGERROR (strTemp);
		fSuccess = false;
	}
	return (fSuccess);
}


COrden * CGestorOrden::GetOrdenPtr(unsigned char *szId, int *index)
{
	COrden * ptrOrden = NULL;
	bool fFound = false;
	int numOrden = m_OrdenPtrArray.GetSize ();
	for (int i = 0; i < numOrden; i++ )
	{
		ptrOrden = m_OrdenPtrArray.GetAt (i);
		if (0 == strcmp ((char *)ptrOrden->m_at.id_nr, (char *)szId))
		{
			// found
			*index = i;
			fFound = true;
			break;
		}
	}
	if (fFound)
		return (ptrOrden);
	else
		return (NULL);
	// XXX Michael: This should be locked
}


// Cancelar (o Finalizar) una orden desde el BuB. Puede ser AT o MA
bool CGestorOrden::BuBDeleteOrden(unsigned char * id_nr, bool finalizar)
{
	COrden * ptrOrden;
	int index;
	ptrOrden =  GetOrdenPtr (id_nr, &index);
	bool fSuccess = true;

	// Michael 02.09.2001: See how to treat order confirmation (TK_TA_QUITTIEREN)
	if (NULL != ptrOrden)
	{
			switch (ptrOrden->GetEstado ())
			{
			case DELIVERED:
			case DELIVER:
				if (!(ptrOrden->m_at.herkunft == 'S' && ptrOrden->m_at.akt_org == 0))
				{
					// Comprobar que no es borrado de una orden en error
					fSuccess = TestDeleteStationError (ptrOrden, finalizar);
					// Michael 11.10.2001: XXX Dejar borrar mientras pruebas...
					if (!fSuccess)
						if (QuitarAgvOrden (*ptrOrden))
							// Michael 12.07.2002 DeleteOrden (index);
							ptrOrden->SetEstado (CANCELLED);
						else
							ptrOrden->SetEstado (PENDINGCANCEL);

					// Si no; no se puede borrar ya que tiene carga
				}
				else
				{
					// Deja borrar orden manual sin tarea
					if (QuitarAgvOrden (*ptrOrden))
						DeleteOrden (index);
					else
						ptrOrden->SetEstado (PENDINGCANCEL);
					fSuccess = true;
					// Es una orden manual sin nada...
				}

				break;
			case NEWORDER:
				DeleteOrden (index);
				break;
			case COLLECT:
				if (TestDeleteStationError (ptrOrden, finalizar))
					break;
			default:
				if (QuitarAgvOrden (*ptrOrden))
					// Michael 12.07.2002 DeleteOrden (index);
					ptrOrden->SetEstado (CANCELLED);
				else
					ptrOrden->SetEstado (PENDINGCANCEL);
				fSuccess = true;
				break;
			}
	}
	else
		// no se ha encontrado la orden
	{
		CString strTemp;
		AfxFormatString1 (strTemp, IDS_NOORDBORRAR, (const char *)id_nr);
		MSGERROR (strTemp);
		m_fUpdateRequired = true;	// Update BuB anyway !!!
		fSuccess = false;
	}
	return (fSuccess);
}



// Borrar una orden por su index en el array
void CGestorOrden::DeleteOrden(int index)
{
	COrden * ptrOrden = m_OrdenPtrArray.GetAt (index);
	// Borrar del BuB
	// M2015 Give to BuBThread to communicate deletion
	g_Container.m_pBuBThread->AddOrderToDelete(ptrOrden->m_at.herkunft, 'D', ptrOrden->m_uBuBIndex);

	m_listBuB.AddTail (ptrOrden->m_uBuBIndex); // Liberar BuB index

	UINT nAgv = ptrOrden->m_at.fznr;
	
	g_GestorOrden.m_dbGestor.DeleteOrden (ptrOrden);	// Actualizar en la base de datos

	delete ptrOrden;
	m_OrdenPtrArray.RemoveAt (index);
	if (nAgv)
		// If running simulation will generate a new order for the Agv
		SimulateNewOrden (nAgv);
}


// Suspender o liberar una orden a peticion del BuB
bool CGestorOrden::BuBSuspendReleaseOrden(send_at_tele &tele)
{
	COrden * ptrOrden;
	int index;
	ptrOrden =  GetOrdenPtr (tele.id_nr, &index);
	enum TEstadoOrden estado;
	bool fSuccess = true;
	if (NULL != ptrOrden)
	{
		estado = ptrOrden->GetEstado ();
		if (TK_TA_SPERREN == tele.auftr_aktion)
		{
			// Suspender orden
			switch (ptrOrden->GetEstado ())
			{
			case NEWORDER:
				// Order suspended
				ptrOrden->m_at.gesp_at = 1;
				break;
			case COLLECT:
			case PENDINGASSIGN:
				// Collection suspended
				ptrOrden->m_at.gesp_bz = 1;
				break;
			case DELIVER:
				// Delivery suspended
				ptrOrden->m_at.gesp_hz = 1;
				break;
			default:
				fSuccess = false;
				break;
			}
		}

		else if (TK_TA_SPERREN == tele.auftr_aktion)
		{
			// Suspendido por motivo que sea !
				ptrOrden->m_at.gesp_at = 0;
				ptrOrden->m_at.gesp_bz = 0;
				ptrOrden->m_at.gesp_hz = 0;
		}
		ptrOrden->m_fBuB = true;	// Avisar al BuB
	}
	else
		fSuccess = false;

	return (fSuccess);
				
}

// Actualizar (modificar) una orden (de AGV:Manual) desde el BuB: Puede modificar prioridad o destino
bool CGestorOrden::BuBUpdateOrden(send_ma_tele &tele)
{
	COrden * ptrOrden;
	int index;
	ptrOrden =  GetOrdenPtr (tele.id_nr, &index);
	bool fSuccess = true;

	if (NULL != ptrOrden)
	{
		if (! (ptrOrden->m_To == tele.az))
		{
			// Destino diferente
			switch (ptrOrden->GetEstado ())
			{
			case PENDINGCANCEL:
			case DELIVERED:
				// No se puede modificar en este estado
				fSuccess = false;
				break;
			default:
				ptrOrden->m_at.az = tele.az;
				fSuccess = true;
				break;
			}
		}
		// Actualizar prioridad
		ptrOrden->m_fBuB = true;	// avisar al BuB
	}
	else
		// no se ha encontrado la orden
	{
		CString strTemp;
		AfxFormatString1 (strTemp, IDS_NOORDMODIFICAR, (const char *)tele.id_nr);
		MSGERROR (strTemp);
		fSuccess = false;
	}
	return (fSuccess);

}


// Si se está simulando, añadir una orden nueva...
void CGestorOrden::SimulateNewOrden(UINT nAgv)
{
if (g_Container.m_fSimulateCom && g_Container.m_fAutoOrders)
{

	TYP_AT_DAT at_neu;

	memset (&at_neu,0,sizeof(at_neu));

	at_neu.af_status = 0;
	at_neu.akt_z.nLayer = 0;
	at_neu.akt_z.ucSide = 0;
	at_neu.akt_z.nDest = 0;
	at_neu.akt_z.nPos = 0;
	at_neu.aktion = 1;
	at_neu.art = 2; // Not used
	at_neu.at_lauf_nr = 1;
	at_neu.bnr = 1;


	at_neu.herkunft = 'S';  // Manual AGV order !!!
	at_neu.prio_org = 1;
	at_neu.fznr     = nAgv;
	at_neu.akt_org  = 0;	// No action

	CAGVTDestination* pDest = g_pCourseData->GetPtrCourseAGVType(1)->GetPtrDestinationByIndex(m_NextSimulationStation);
	//at_neu.az.nDest = statdat [m_NextSimulationStation].mp;
	//at_neu.az.ucSide = statdat [m_NextSimulationStation].pos_anz_r > 0 ? 'R' : 'L';
	at_neu.az.nDest = pDest->GetNumber();
	at_neu.az.ucSide = pDest->GetRightPositions() > 0 ? 'R' : 'L';
	at_neu.az.nPos = 1;
	at_neu.az.nLayer = 0;

	m_NextSimulationStation += nAgv;
	//if (m_NextSimulationStation >= KMAX_STATIONEN)
	//	m_NextSimulationStation -= KMAX_STATIONEN - 1;
	if (m_NextSimulationStation >= g_pCourseData->GetPtrCourseAGVType(1)->GetNumberDestinations())
		m_NextSimulationStation -= g_pCourseData->GetPtrCourseAGVType(1)->GetNumberDestinations() - 1;


	at_neu.fMesa1 = false;
	at_neu.fMesa2 = false;
	at_neu.fSimCarga = false;
	
	COrden * pOrden = new COrden;	// la insertamos
			
	memcpy ((void *)&(pOrden->m_at), &at_neu, sizeof (at_neu));
	g_GestorOrden.GetNewOrdenId (pOrden->m_at.id_nr);
	pOrden->SetOrderInsertTime ();
	g_GestorOrden.InsertarOrdenPtrManual (pOrden);

	m_fUpdateRequired = true;
}	// Simulate

}



bool CGestorOrden::QuitarAgvOrden(COrden& orden)
{
	bool fQuitado = false;

	CAgv * ptrAgv = g_pGestor->GetAgvPtr (orden.m_at.fznr);
	if (ptrAgv)
	{
		CSingleLock lockAgv (&ptrAgv->m_cs);	// Bloquear acceso al AGV...
		MSGTRACE1 ("QuitarAgvOrden () Esperando Aceso AGV %d\n", ptrAgv->m_id);
		lockAgv.Lock ();
		MSGTRACE1 ("QuitarAgvOrden () Aceso Agv %d granted...\n", ptrAgv->m_id);
		
/* Michael 12.11.2001 quitar en todos los casos		
		int estado = ptrAgv->GetEstado ();

		switch (estado)
		{
		case IRDESCARGARORD:
			if (!(orden.m_at.herkunft == 'S' && orden.m_at.akt_org == 0))
				break;
			// Solo si es una orden manual sin nada que hacer
		case ORDENNUEVO:
		case ORDENNUEVOMANUAL:
		case IRCARGARORD:
			// Michael 11.10.2001: XXX Estos dos para pruebas...
		case CARGARORD:
		case DESCARGARORD:

			ptrAgv->SetDeleted ();
			fQuitado = true;
			break;
		default:
			MSGWARNING2 ("QuitarAgvOrden::: Agv %d estado: %s",
				ptrAgv->m_id,
				ptrAgv->GetStringEstado ());
			break;
			
		}
	Michael 12.11.2001		*/

		ptrAgv->SetDeleted ();
		fQuitado = true;
		lockAgv.Unlock ();
	}
	else
	{
		MSGERROR2 ("QuitarAgvOrden () Orden: %s no encontrado Agv: %d",
			orden.m_at.id_nr,
			orden.m_at.fznr);
		fQuitado = true;
	}
	return (fQuitado);

}

// Comprobar peticion de borrar una orden que está en la estación
bool CGestorOrden::TestDeleteStationError(COrden *ptrOrden, bool finalizar)
{		
	bool fQuitado = false;
	CAgv * ptrAgv = g_pGestor->GetAgvPtr (ptrOrden->m_at.fznr);

	if (ptrAgv->GetStationError ()) // See if the Agv has an error at the station
	{
		// Operator requests deletion of an order which cannot be performed
		CSingleLock lockAgv (&ptrAgv->m_cs);	// Bloquear acceso al AGV...
		TRACE1 ("TestStationError () Esperando Aceso AGV %d\n", ptrAgv->m_id);
		lockAgv.Lock ();
		TRACE1 ("TestStationError () Aceso Agv %d granted...\n", ptrAgv->m_id);
		int estado = ptrAgv->GetEstado ();
		switch (estado)
		{
		case CARGARORD:
			// was loading
			ptrAgv->SetDeleted ();
			// Michael 13.09.2002 Aunque indique finalizar, cancela la orden
			// ptrOrden->SetEstado (finalizar ? DELIVERED : CANCELLED);
			ptrOrden->SetEstado (CANCELLED);
			// Michael 13.09.2002 Fin
			fQuitado = true;
			break;
		case DESCARGARORD:
			// was unloading
			ptrAgv->SetDeleted ();
			ptrOrden->SetEstado (finalizar ? DELIVERED : CANCELLED);
			fQuitado = true;
			break;
		default:
			break;
			
		}
		lockAgv.Unlock ();
	}
	return (fQuitado);
}

// Insertar una orden nueva
void CGestorOrden::InsertarOrden(COrden *ptrOrden)
{
	m_dbGestor.InsertarOrden (ptrOrden);	// insertar en la base de datos
	ptrOrden->SetOrderInsertTime ();		// Get this from the insert time...
	GetBuBIndex (ptrOrden);
	m_OrdenPtrArray.Add (ptrOrden);			// Añadir a la lista...	
}

void CGestorOrden::GetAutomaticOrden()
{
	// M2015 Michael: Added g_Container.IsEnFuncionamiento () || ! g_Container.SystemDriveAllowed ()
	if (! g_Container.IsArrancadoAgvThread () || ! g_Container.IsEnFuncionamiento () || ! g_Container.SystemDriveAllowed ())
		return;

	COrden * ptrOrden;
	// M2015 por errores de concurrencia observados el 27.04.2015
	// 28.04.2015 doesn't seem to work, as previously assigned orders are being retrieved as new orders, and assigned to the AGV that has the 
	// order (to deliver), and then to a new AGV.
	CSingleLock lockOrden (&g_Container.m_csAsignarOrden);
	MSGTRACE ("GetAutomaticOrden () Esperando lock m_csAsignarOrden...");
	lockOrden.Lock ();
	MSGTRACE ("GetAutomaticOrden () Got lock m_csAsignarOrden...");


	while (ptrOrden = m_dbGestor.GetOrden ())
	{
		GetBuBIndex (ptrOrden);
		m_OrdenPtrArray.Add (ptrOrden);
	}
	
	lockOrden.Unlock();
	MSGTRACE ("GetAutomaticOrden () Unlocked lock m_csAsignarOrden...");

}

// Asignar el siguiente index a usar para el BuB
void CGestorOrden::GetBuBIndex(COrden * ptrOrden)
{
	if (m_listBuB.GetCount () > 0)
	{
		ptrOrden->m_uBuBIndex = m_listBuB.GetHead ();
		m_listBuB.RemoveHead ();
	}
	else
		ptrOrden->m_uBuBIndex = m_OrdenPtrArray.GetSize () + 1;

}

void CGestorOrden::InitTiempoEspera()
{
	m_frecuenciaOrden = AfxGetApp ()->GetProfileInt ("Settings", "FrecuenciaOrden", 0);
    if (0 == m_frecuenciaOrden)
	   {
			m_frecuenciaOrden = 500;
			AfxGetApp ()->WriteProfileInt ("Settings", "FrecuenciaOrden", m_frecuenciaOrden);
	   }
	m_augmentoTiempoOrden = AfxGetApp ()->GetProfileInt ("Settings", "AugmentoTiempoOrden", 0);
    if (0 == m_augmentoTiempoOrden)
	   {
			m_augmentoTiempoOrden = 10000;
			AfxGetApp ()->WriteProfileInt ("Settings", "AugmentoTiempoOrden", m_augmentoTiempoOrden);
	   }

	m_tiempoPartidaOrden = AfxGetApp ()->GetProfileInt ("Settings", "TiempoPartidaOrden", 0);
    if (0 == m_tiempoPartidaOrden)
	   {
			m_tiempoPartidaOrden = 5;	// segundos
			AfxGetApp ()->WriteProfileInt ("Settings", "TiempoPartidaOrden", m_tiempoPartidaOrden);
	   }
}

void CGestorOrden::SetAugmentoTiempo(UINT augmento)
{
	m_augmentoTiempoOrden = augmento;
	AfxGetApp ()->WriteProfileInt ("Settings", "AugmentoTiempoOrden", m_augmentoTiempoOrden);
}

void CGestorOrden::SetFrecuenciaOrden(UINT frecuencia)
{
	m_frecuenciaOrden = frecuencia;
	AfxGetApp ()->WriteProfileInt ("Settings", "FrecuenciaOrden", m_frecuenciaOrden);
}

void CGestorOrden::SetTiempoPartidaOrden(UINT tiempoPartida)
{
	m_tiempoPartidaOrden = tiempoPartida;
	AfxGetApp ()->WriteProfileInt ("Settings", "TiempoPartidaOrden", m_tiempoPartidaOrden);
}


// Añadir una orden a afectos del BuB
void CGestorOrden::AddOrden(COrden *ptrOrden)
{
	GetBuBIndex (ptrOrden);
	m_OrdenPtrArray.Add (ptrOrden);
}

/* Mirar las órdenes en curso (con carga) para enviar a rechazo los que ya no
	son necesarias por cambio en el tren / estación */
void CGestorOrden::QuizasEnviarRechazo()
{
	int numOrden = m_OrdenPtrArray.GetSize ();
	for (int i = 0; i < numOrden; i++ )
	{
		COrden * ptrOrden = m_OrdenPtrArray.GetAt (i);
		if (DELIVER == ptrOrden->GetEstado () && 'S' != ptrOrden->m_at.herkunft
				&& 7 != ptrOrden->m_at.bz.nDest 
				&& 1 != ptrOrden->m_at.bz.nDest
				&& 2 != ptrOrden->m_at.bz.nDest
				&& 3 != ptrOrden->m_at.bz.nDest)
		{
			// Solo los ordenes completos que tienen destino trenes...
			MSGTRACE1 ("QuizasEnviarRechazo () : Mirando orden: %s", ptrOrden->m_at.id_nr);
			// Mirar a ver si el tren / estación todavía quiere la orden
			// Se mira desde el punto de vista de la estación de carga (EstOri)
			UINT uRes = ptrOrden->m_ptrEstOri->GetCarga ((LPCTSTR)ptrOrden->m_at.id_nr);
			if (5 == uRes)
				{
				// Tren / Estación no permite Carga o Descarga
				CAlarma * ptrAlarma = new CAlarma;
				ptrAlarma->AvisarAlarma (13, 2, 1,0,0,0,0,0,0,
					(char *)ptrOrden->m_at.id_nr);
				CAgv * ptrAgv = g_pGestor->GetAgvPtr (ptrOrden->m_at.fznr);
				if (ptrAgv)
				{
					// No hace falta bloquear el Agv ?
					UCHAR pos;
					if (! MESA1CARGADA (*ptrAgv->m_pFzdat)  && MESA2CARGADA (*ptrAgv->m_pFzdat))
						// Solo mesa 2
						pos = 1;
					else
						pos = 2;
				
					CAgv::SetZiel (&ptrOrden->m_at.bz,
						7,
						pos,	
						'L',
						0);

					m_dbGestor.ModificarOrden (ptrOrden);	// Actualizar en la base de datos

					ptrOrden->m_ptrEstDes->GetEstAgv (
							ptrOrden->m_at.bz.nDest,
							ptrOrden->m_at.bz.nPos,
							ptrOrden->m_at.bz.ucSide);
				}
				else
					MSGERROR1 ("QuizasEnviarRechazo (): Orden: %s Agv no encontrado",
						ptrOrden->m_at.id_nr);
			}
		}
	}
}

// Insertar una orden manual de rechazo para un Agv.
void CGestorOrden::InsertarOrdenRechazo(CAgv * ptrAgv)
{
	COrden * pOrden = new COrden;	// la insertamos

	memset (&pOrden->m_at,0,sizeof(pOrden->m_at));

	pOrden->m_at.af_status = 0;
	pOrden->m_at.akt_org = 0;	// Original Action
	pOrden->m_at.akt_z.nLayer = 0;
	pOrden->m_at.akt_z.ucSide = 0;
	pOrden->m_at.akt_z.nDest = 0;
	pOrden->m_at.akt_z.nPos = 0;
	pOrden->m_at.aktion = 1;
	pOrden->m_at.art = 2; // Not used
	pOrden->m_at.at_lauf_nr = 1;
	pOrden->m_at.bnr = 1;

	pOrden->m_at.herkunft = 'S';  // Manual AGV order !!!
	pOrden->m_at.prio_org = 1;
	pOrden->m_at.fznr     = (unsigned char)ptrAgv->m_id;
	pOrden->m_at.akt_org  = 2;	// Entregar
	
	int pos;
	if (MESA2CARGADA (*ptrAgv->m_pFzdat) && ! MESA1CARGADA (*ptrAgv->m_pFzdat))
		// Hay que descargar en la posición 1
		pos = 1;
	else
		pos = 2;

	// M2015 TODO
	//CAgv::SetZiel (&pOrden->m_at.az, 
	//	289,
	//	pos,
	//	'R',
	//	1);

	CAgv::SetZiel (&pOrden->m_at.az, 
	7,
	pos,
	'L');


	pOrden->m_at.fMesa1 = MESA1CARGADA (*ptrAgv->m_pFzdat);
	pOrden->m_at.fMesa2 = MESA2CARGADA (*ptrAgv->m_pFzdat);
	pOrden->m_at.fSimCarga = false;
	
			
	memcpy ((void *)&(pOrden->m_at), &pOrden->m_at, sizeof (pOrden->m_at));
	GetNewOrdenId (pOrden->m_at.id_nr);
	pOrden->SetOrderInsertTime ();

	pOrden->GetDatosEstacion ();	// Recuper datos de la estación
	pOrden->SetAssigned (ptrAgv->m_id);	// Ya la asignamos al Agv
	ptrAgv->SetOrden (pOrden);

	InsertarOrdenPtrManual (pOrden);
}
