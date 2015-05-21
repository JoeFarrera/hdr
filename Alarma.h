// Alarma.h: interface for the CAlarma class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALARMA_H__00EAC096_1584_4588_A6AF_B1CB70EB5452__INCLUDED_)
#define AFX_ALARMA_H__00EAC096_1584_4588_A6AF_B1CB70EB5452__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAlarma : public CObject  
{
public:
	void SetFin () { m_fFin = true; m_fUpdateBuB = true; };
	CAlarma (bool fEsperarFin);
	bool SetTelegramaBuB (empf_dt_tele *tele);
	void AvisarAlarma (UINT modNr, UINT alarmNr, UINT numVar, UINT var1 = 0, UINT var2 = 0, UINT var3 = 0, UINT var4 = 0, UINT var5 = 0, UINT var6 = 0, LPCTSTR strvar1 = NULL, LPCTSTR strvar2 = NULL);
	CAlarma();
	virtual ~CAlarma();

private:
	bool m_fFin;	// Indicar que el alarma ha sido subsanado
	bool m_fUpdateBuB;	// Actualizar el BuB
	TYP_SYSLOG m_sysLog;
};

#endif // !defined(AFX_ALARMA_H__00EAC096_1584_4588_A6AF_B1CB70EB5452__INCLUDED_)
