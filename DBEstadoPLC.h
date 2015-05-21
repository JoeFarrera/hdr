// DBEstadoPLC.h: interface for the CDBEstadoPLC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBESTADOPLC_H__49533BD2_36E7_40CF_B262_8A67B6B7FA35__INCLUDED_)
#define AFX_DBESTADOPLC_H__49533BD2_36E7_40CF_B262_8A67B6B7FA35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CDBEstadoPLC  
{
public:
	void UpdateInstanceTimestamp();
	void SetEstadoSemaforo(int s1, int s2);
	void GetEstadoSemaforo (int * s1, int * s2);
	void SetDestinoA2 (LPCTSTR codord);
	void LimpiarEstadoPLC();
	void SwitchCargador (UINT nAgv, UINT ptoCom, UINT wegstr, bool setOn);
	void SendEstadoPLC();
	CDBEstadoPLC();
	virtual ~CDBEstadoPLC();

private:
	T_sqlca sqlca;
	T_oraca oraca;
};

#endif // !defined(AFX_DBESTADOPLC_H__49533BD2_36E7_40CF_B262_8A67B6B7FA35__INCLUDED_)
