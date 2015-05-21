// Telegrama.cpp: implementation of the CTelegrama class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "Telegrama.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTelegrama::CTelegrama() 
{
	m_nEstado = TELNULL;	// Inicializar sin telegrama

}

CTelegrama::~CTelegrama()
{

}

// Calcular y devolver los bytes de error check para un telegrama
void CTelegrama::GetErrorCheck(CString& errCheck, LPCTSTR telegrama)
{
	int nLenTele, i;
	BYTE bSumBytes, bComp;

	nLenTele = strlen (telegrama);

	for (i = 0, bSumBytes = 0; i < nLenTele; i ++)
		bSumBytes = bSumBytes + (BYTE)telegrama [i];

	bComp = ~(bSumBytes) + 0x1;
	// bComp = bComp << 1;
	errCheck.Insert (1, 0x30 | ((bComp & 0XF0) >> 4));
	errCheck.Insert (0, 0x30 | (bComp & 0x0F));
}


// Confeccionar y activar un telegrama de polling
void CTelegrama::SetPollingTelegram(UINT nAgv)
{
	m_strTelegrama.Format ("%02d", nAgv);
	m_nEstado = TELPEND;
	SetRespuesta (true);	// esperar respuesta para este telegrama
}

// Confeccionar y activar un telegrama de información
void CTelegrama::SetInfoTelegram(typ_tele_info teleInfo)
{
	// copiar todo menos el error check
	LPTSTR pszbuf = m_strTelegrama.GetBuffer (sizeof (typ_tele_info) - 2);
	memcpy (pszbuf, (const void *)&teleInfo, sizeof (typ_tele_info) - 2);
	pszbuf [sizeof (typ_tele_info) - 2] = '\0';
	m_strTelegrama.ReleaseBuffer ();
	m_nEstado = TELPEND;
	SetRespuesta (true);	// Requiere respuesta...

}

// Confeccionar y enviar el Quitum...
const CString CTelegrama::StrQuitum(UINT nAgv)
{
	m_strQuitum.Format (_T("Q%02d"), nAgv);
	return (m_strQuitum);
}

// Confeccionar y activar un telegrama de anf...
void CTelegrama::SetAnfTelegram(typ_tele_anf teleAnf)
{
	// copiar todo menos el error check
	LPTSTR pszbuf = m_strTelegrama.GetBuffer (sizeof (typ_tele_anf) - 2);
	memcpy (pszbuf, (const void *)&teleAnf, sizeof (typ_tele_anf) - 2);
	m_strTelegrama.ReleaseBuffer ();
	m_nEstado = TELPEND;
	SetRespuesta (true);	// Requiere respuesta...

}

// Confeccionar y activar un telegrama de blocking
void CTelegrama::SetBlockTelegram(typ_tele_block teleBlock)
{
	// copiar todo menos el error check
	LPTSTR pszbuf = m_strTelegrama.GetBuffer (sizeof (typ_tele_block) - 2);
	memcpy (pszbuf, (const void *)&teleBlock, sizeof (typ_tele_block) - 2);
	pszbuf [sizeof (typ_tele_block) - 2] = '\0';
	m_strTelegrama.ReleaseBuffer ();
	m_nEstado = TELPEND;
	SetRespuesta (true);	// Requiere respuesta...

}

void CTelegrama::SetUmleitTelegram(typ_tele_umleit teleUmleit)
{
	// copiar todo menos el error check
	LPTSTR pszbuf = m_strTelegrama.GetBuffer (sizeof (typ_tele_umleit) - 2);
	memcpy (pszbuf, (const void *)&teleUmleit, sizeof (typ_tele_umleit) - 2);
	pszbuf [sizeof (typ_tele_umleit) - 2] = '\0';
	m_strTelegrama.ReleaseBuffer ();
	m_nEstado = TELPEND;
	SetRespuesta (true);	// Requiere respuesta...
}
