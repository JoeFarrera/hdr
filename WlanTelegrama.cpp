// WlanTelegrama.cpp: implementation of the CWlanTelegrama class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tpc.h"
#include "WlanMsg.h"
#include "WlanTelegrama.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWlanTelegrama::CWlanTelegrama() 
{
	m_nEstado = WLAN_TELNULL;	// Inicializar sin telegrama
	m_nAgv = 0;

}

CWlanTelegrama::~CWlanTelegrama()
{

}

// Calcular y devolver los bytes de error check para un telegrama
void CWlanTelegrama::GetErrorCheck(CString& errCheck, LPCTSTR telegrama)
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


// Confeccionar y activar un telegrama de información
void CWlanTelegrama::SetInfoTelegram(typ_tele_info_wlan teleInfo, WlanMsg *ptrMsg)
{
	// copiar todo menos el error check
	LPTSTR pszbuf = m_strTelegrama.GetBuffer (sizeof (teleInfo) - 2);
	memcpy (pszbuf, (const void *)(&teleInfo.tele_art), sizeof (teleInfo) - 4);
	pszbuf [sizeof (teleInfo) - 4] = '\0';
	m_strTelegrama.ReleaseBuffer ();
	m_nEstado = WLAN_TELPEND;
	SendTelegrama(m_strTelegrama, ptrMsg);

}

void CWlanTelegrama::SetPollTelegram (typ_tele_anf_wlan telePoll, WlanMsg *ptrMsg)
{
	// copiar todo menos el error check
	LPTSTR pszbuf = m_strTelegrama.GetBuffer (sizeof (telePoll) - 2);
	memcpy (pszbuf, (const void *)(&telePoll.tele_art), sizeof (telePoll) - 4);
	pszbuf [sizeof (telePoll) - 4] = '\0';
	m_strTelegrama.ReleaseBuffer ();
	m_nEstado = WLAN_TELPEND;
	SendTelegrama(m_strTelegrama, ptrMsg);

}

void CWlanTelegrama::SendQuitum (WlanMsg *ptrMsg) {
	CString strErrorCheck;
	CString strQuitum;
	strQuitum.Format (_T("Q%02d"), m_nAgv);
	SendTelegrama(strQuitum, ptrMsg);
}
// Confeccionar y enviar el Quitum...
const CString CWlanTelegrama::StrQuitum(UINT nAgv)
{
	m_strQuitum.Format (_T("Q%02d"), nAgv);
	return (m_strQuitum);
}

// Confeccionar y activar un telegrama de anf...
void CWlanTelegrama::SetAnfTelegram(typ_tele_anf teleAnf)
{
	// copiar todo menos el error check
	LPTSTR pszbuf = m_strTelegrama.GetBuffer (sizeof (typ_tele_anf) - 2);
	memcpy (pszbuf, (const void *)&teleAnf, sizeof (typ_tele_anf) - 2);
	m_strTelegrama.ReleaseBuffer ();
	m_nEstado = WLAN_TELPEND;
	SetRespuesta (true);	// Requiere respuesta...

}

// Confeccionar y activar un telegrama de blocking
void CWlanTelegrama::SetBlockTelegram(typ_tele_block_wlan teleBlock, WlanMsg * ptrMsg)
{
	// copiar todo menos el error check
	LPTSTR pszbuf = m_strTelegrama.GetBuffer (sizeof (teleBlock) - 2);
	memcpy (pszbuf, (const void *)&teleBlock.tele_art, sizeof (teleBlock) - 4);
	pszbuf [sizeof (teleBlock) - 4] = '\0';
	m_strTelegrama.ReleaseBuffer ();
	m_nEstado = WLAN_TELPEND;
	SendTelegrama (m_strTelegrama, ptrMsg);

}

void CWlanTelegrama::SetUmleitTelegram(typ_tele_umleit_wlan teleUmleit, WlanMsg * ptrMsg)
{
	// copiar todo menos el error check
	LPTSTR pszbuf = m_strTelegrama.GetBuffer (sizeof (teleUmleit) - 2);
	memcpy (pszbuf, (const void *)&teleUmleit.tele_art, sizeof (teleUmleit) - 4);
	pszbuf [sizeof (teleUmleit) - 4] = '\0';
	m_strTelegrama.ReleaseBuffer ();
	m_nEstado = WLAN_TELPEND;
	SendTelegrama (m_strTelegrama, ptrMsg);

}

void CWlanTelegrama::SendTelegrama(LPCTSTR telegrama, WlanMsg * ptrMsg)
{

	CString strErrorCheck;
	GetErrorCheck (strErrorCheck, telegrama);
	
	if (ptrMsg && !ptrMsg->IsClosed())
	{
		CString strTelegram;
		strTelegram.Format (_T("%c%s%s%c"), STX, telegrama, strErrorCheck, ETX);
		// Queue Message for sending
		ptrMsg->QueueMessage(strTelegram);
		if (ptrMsg->IsClosed())
		{
			MSGERROR ("CWlanTelegrama::SendTelegrama No enviada");
		}
	}
	else
	{
		MSGERROR ("CWlanTelegrama::SendTelegrama - Conexión cerrada antes de enviar");
	}

}




