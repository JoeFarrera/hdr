// PuntoDescanso.h: interface for the CPuntoDescanso class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUNTODESCANSO_H__6BFDDA3B_4465_4355_8F9D_499D3EC6DBEF__INCLUDED_)
#define AFX_PUNTODESCANSO_H__6BFDDA3B_4465_4355_8F9D_499D3EC6DBEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPuntoDescanso : public CObject  
  
{
public:
	bool IsMoreUrgent (UINT urgency) { 	return (urgency <= m_urgency);};
	void SetZiel (TYPE_STATION_DEF *ptrZiel);
	CPuntoDescanso(UINT ptoCom, UINT pos, UCHAR lado, UINT numAgv, UINT urgency);
	UINT GetNumAgvsAllowed () { return (m_numAgvsAllowed);};
	UINT SetNumAgvs();
	CPuntoDescanso();
	virtual ~CPuntoDescanso();
	UINT GetNumAgvs () { return (m_numAgvs); };
	UINT GetPtoCom () { return (m_ptoCom); };

private:
	UINT m_urgency;			// Medida de la urgencia de realizar una orden para la estación
	UINT m_numAgvsAllowed;	// Número de Agvs permitido en el punto de descanso
	UINT m_numAgvs;
	UCHAR m_lado;
	UINT m_pos;
	UINT m_ptoCom;
};

#endif // !defined(AFX_PUNTODESCANSO_H__6BFDDA3B_4465_4355_8F9D_499D3EC6DBEF__INCLUDED_)
