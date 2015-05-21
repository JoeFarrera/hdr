// Alarma.cpp: implementation of the CAlarma class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "Alarma.h"
#include "Container.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlarma::CAlarma()
{
	m_fUpdateBuB = true;
	m_fFin = false;

}

CAlarma::~CAlarma()
{

}

void CAlarma::AvisarAlarma (UINT modNr, UINT alarmNr, UINT numVar, UINT var1, UINT var2, UINT var3, UINT var4, UINT var5, UINT var6, LPCTSTR strvar1, LPCTSTR strvar2)
{
	m_sysLog.mod_nr = modNr;
	m_sysLog.meld_nr = alarmNr;
	m_sysLog.var_anz = numVar;
	m_sysLog.var [0] = var1;
	m_sysLog.var [1] = var2;
	m_sysLog.var [2] = var3;
	m_sysLog.var [3] = var4;
	m_sysLog.var [4] = var5;
	m_sysLog.var [5] = var6;
	if (strvar1)
	{
		strncpy (m_sysLog.strvar [0].str, strvar1, 39);
		m_sysLog.strvar [0].len = strlen (strvar1);
	}
	else
		m_sysLog.strvar [0].len = 0;
	if (strvar2)
	{
		strncpy (m_sysLog.strvar [1].str, strvar2, 39);
		m_sysLog.strvar [1].len = strlen (strvar2);
	}
	else
		m_sysLog.strvar [1].len = 0;
	
	m_sysLog.kommt_geht = 0;	// El error llega


	// Añadir al array para que sea enviado al BuB
	CSingleLock lockAlarma (&g_Container.m_csAlarma);
	lockAlarma.Lock ();

	g_Container.m_AlarmaArray.Add (this);

	lockAlarma.Unlock ();



}

bool CAlarma::SetTelegramaBuB(empf_dt_tele *tele)
{
	bool fRetVal;
	if (m_fUpdateBuB)
	{
		if (m_fFin)
		{
			m_sysLog.kommt_geht = 1;	// Finalizado
		}
		else
			m_sysLog.kommt_geht = 0;

		memcpy (&tele->TSys, &m_sysLog, sizeof (TYP_SYSLOG)); 
		m_fUpdateBuB = false;
		fRetVal = true;
		// Delete here si no hace falta más ?
	}
	else
		fRetVal = false;

	return (fRetVal);

}

CAlarma::CAlarma(bool fFin)
{
	CAlarma ();
	m_fFin = true;
}
