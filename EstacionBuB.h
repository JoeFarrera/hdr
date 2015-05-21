// EstacionBuB.h: interface for the CEstacionBuB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ESTACIONBUB_H__9AD1D04B_ED26_4DB0_9109_41FC39C772C7__INCLUDED_)
#define AFX_ESTACIONBUB_H__9AD1D04B_ED26_4DB0_9109_41FC39C772C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAlarma;

class CEstacionBuB : public CObject  
{

private:
	CAIDStation * m_pAIDStation;
public:
	void UpdateEstacion (LPCTSTR estDes, UINT nAgvDes);
	CAlarma * m_pAlarma;
	void UpdateEstacion (UCHAR error, bool fCargado);
	void UpdateEstacion(BYTE ocuPul, BYTE capPul, LPCSTR codEstPul, UCHAR error, bool fCargado);
	bool IsThis (UINT mp, UINT pos, UCHAR lage) 
		{ return (mp == m_mp && pos == m_pos && lage == m_lage); };
	bool IsThis (LPCTSTR psClientName) 
	{ return (strcmp(psClientName, m_KundenNr) == 0);};
	bool SetTelegramBuB (empf_s0_tele * tele, bool fUpdateAll);
	void Init(UINT index, UINT pos, UCHAR lage);
	CEstacionBuB();
	virtual ~CEstacionBuB();
	bool IsCargaDescarga () { return (m_typ == 1 || m_typ == 2 || m_typ == 7); }
	

public:
	bool m_fUpdateBuB;
	UINT m_uBuBIndex;
	UINT m_mp;	
	UINT m_pos;
	UCHAR m_lage;
	UCHAR m_typ;
	UCHAR m_closed;
	UCHAR m_error;
	UCHAR m_prio;
	bool m_fCargado;
	LPTSTR m_KundenNr;
	LPTSTR m_codEstDes;	// Estaci�n de descanso asociado
	UINT	m_numAgvDes; // N�mero de Agvs permitido descansando
	// Michael 11.12.2004 m�s campos, m�s campos
	LPTSTR m_codpulvac;	// Pulm�n de vac�o asociado
	int m_ocuPul;	// Ocupaci�n del pulm�n
	int m_capPul;	// Ocupaci�n del pulm�n
	UCHAR m_automan;	// Autom�tico o manual ('A' | 'M')
	UCHAR m_senact;		// Sentido Actual ('E' | 'S' | 'N')
	UCHAR m_prepal;		// Presencia palet ('S' | 'N')
	UCHAR m_mesaop;		// Mesa operativa  ('S' | 'N')
	int m_errmesa;		// C�digo de error del PLC
	UCHAR m_finprod;	// Final producci�n pedido ('S' | 'N')
	int m_resagv;		// Palets ya reservadas para la estaci�n
	UCHAR m_tipest;		// Tipo de estaci�n



};

#endif // !defined(AFX_ESTACIONBUB_H__9AD1D04B_ED26_4DB0_9109_41FC39C772C7__INCLUDED_)
