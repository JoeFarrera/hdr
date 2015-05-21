// Telegrama.h: interface for the CTelegrama class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TELEGRAMA_H__DEEFBF4C_21F9_11D5_9929_0010B547EBE6__INCLUDED_)
#define AFX_TELEGRAMA_H__DEEFBF4C_21F9_11D5_9929_0010B547EBE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fz_tele.h"

enum TeleEstado {
	TELNULL = 0,	// No hay telegrama en curso
	TELPEND,		// Pendiente de enviar
	TELRECV,		// Respuesta recibida
	TELTOUT			// Time out esperando respuesta
};



class CTelegrama 
{
public:
	void SetUmleitTelegram (typ_tele_umleit teleUmleit);
	void SetBlockTelegram (typ_tele_block teleBlock);
	void SetAnfTelegram (typ_tele_anf anfTele);
	void SetInfoTelegram (typ_tele_info infoTele);
	void SetRespuesta (bool respuesta) { m_fRespuesta = respuesta;}; // Indicar respuesta requerida
	bool EsperarRespuesta () { return (m_fRespuesta); }; // Ver si respuesta esperada
	void SetPollingTelegram (UINT nAgv); // Activar un telegrama de polling
	bool TelePendiente () { return (m_nEstado == TELPEND);};	// TRUE si hay telegrama por enviar
	void SetTeleRecibido () { m_nEstado = TELRECV; };	// Indicar que hay un telegrama entrante
	bool TeleRecibido () { return (m_nEstado == TELRECV); };	// TRUE si hay un telegrama entrante
	void SetTeleNull () { m_nEstado = TELNULL; };	// Indicar que no hay telegrama pendiente
	void SetTeleTout () { m_nEstado = TELTOUT; }; // Indicar que ha habido time out esperando respuesta
	bool TeleTout () { return (m_nEstado == TELTOUT); };	// TRUE si ha habido time out esperando respuesta
	const CString StrQuitum (UINT nAgv);

	
	static void GetErrorCheck (CString& errCheck, LPCTSTR telegrama);
	CTelegrama();	// Constructor
	virtual ~CTelegrama();
	CString m_strTelegrama;	// Telegrama a enviar
	CString m_strRespuesta;	// Telegrama recibido

private:
	TeleEstado m_nEstado;	// Estado del la comunicacíón de los telegramas
	bool m_fRespuesta;		// Flag; Indica si hay que esperar respuesta para el telegrama enviado
	CString m_strQuitum;	// Quitum telegram
};

#endif // !defined(AFX_TELEGRAMA_H__DEEFBF4C_21F9_11D5_9929_0010B547EBE6__INCLUDED_)
