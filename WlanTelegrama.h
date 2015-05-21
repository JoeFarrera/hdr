// WlanTelegrama.h: interface for the CWlanTelegrama class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WLAN_TELEGRAMA_H__DEEFBF4C_21F9_11D5_9929_0010B547EBE6__INCLUDED_)
#define AFX_WLAN_TELEGRAMA_H__DEEFBF4C_21F9_11D5_9929_0010B547EBE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fz_tele.h"
#include "WLAN_FZ_Tele.h"

class WlanMsg;




enum WlanTeleEstado {
	WLAN_TELNULL = 0,	// No hay telegrama en curso
	WLAN_TELPEND,		// Pendiente de enviar
	WLAN_TELRECV,		// Respuesta recibida
	WLAN_TELTOUT			// Time out esperando respuesta
};



class CWlanTelegrama 
{
public:
	void CWlanTelegrama::SetPollTelegram (typ_tele_anf_wlan telePoll, WlanMsg *ptrMsg);
	void CWlanTelegrama::SetAgv (UINT nAgv) {m_nAgv = nAgv;};
	void CWlanTelegrama::SendQuitum (WlanMsg * ptrMsg);
	void SendTelegrama(LPCTSTR telegrama, WlanMsg * ptrMsg);
	void SetUmleitTelegram (typ_tele_umleit_wlan teleUmleit, WlanMsg * ptrMsg);
	void SetBlockTelegram (typ_tele_block_wlan teleBlock, WlanMsg * ptrMsg);
	void SetAnfTelegram (typ_tele_anf anfTele);
	void SetInfoTelegram (typ_tele_info_wlan infoTele, WlanMsg * ptrMsg);
	void SetRespuesta (bool respuesta) { m_fRespuesta = respuesta;}; // Indicar respuesta requerida
	bool EsperarRespuesta () { return (m_fRespuesta); }; // Ver si respuesta esperada
	bool TelePendiente () { return (m_nEstado == WLAN_TELPEND);};	// TRUE si hay telegrama por enviar
	void SetTeleRecibido () { m_nEstado = WLAN_TELRECV; };	// Indicar que hay un telegrama entrante
	bool TeleRecibido () { return (m_nEstado == WLAN_TELRECV); };	// TRUE si hay un telegrama entrante
	void SetTeleNull () { m_nEstado = WLAN_TELNULL; };	// Indicar que no hay telegrama pendiente
	void SetTeleTout () { m_nEstado = WLAN_TELTOUT; }; // Indicar que ha habido time out esperando respuesta
	bool TeleTout () { return (m_nEstado == WLAN_TELTOUT); };	// TRUE si ha habido time out esperando respuesta
	const CString StrQuitum (UINT nAgv);

	
	static void GetErrorCheck (CString& errCheck, LPCTSTR telegrama);
	CWlanTelegrama();	// Constructor
	virtual ~CWlanTelegrama();
	CString m_strTelegrama;	// Telegrama a enviar
	CString m_strRespuesta;	// Telegrama recibido

private:
	WlanTeleEstado m_nEstado;	// Estado del la comunicacíón de los telegramas
	bool m_fRespuesta;		// Flag; Indica si hay que esperar respuesta para el telegrama enviado
	CString m_strQuitum;	// Quitum telegram
	UINT m_nAgv;
};

#endif // !defined(AFX_TELEGRAMA_H__DEEFBF4C_21F9_11D5_9929_0010B547EBE6__INCLUDED_)
