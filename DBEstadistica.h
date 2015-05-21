// DBEstadistica.h: interface for the CDBEstadistica class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBESTADISTICA_H__4EFF3953_A65B_4C92_8358_C61330B7F574__INCLUDED_)
#define AFX_DBESTADISTICA_H__4EFF3953_A65B_4C92_8358_C61330B7F574__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAgv;

class CDBEstadistica  
{
public:
	static bool GetStringsDefecto (UINT nZona, UINT nIndMesa, UINT nBit, LPTSTR strCodMes, LPTSTR strCodClInd, LPTSTR strDefecto);
	static void LogDefecto (UINT nZona, UINT nIndMesa, UINT nBit, bool fFin, bool fAbort = false);
	static void ArchivarOrdTran();
	static void BorrarHistoricos();
	static void LogError (CAgv * ptrAgv, UINT nErr, bool fFin, bool fAbort = false);
	static void ArchivarHistAgv(CAgv * ptrAgv);
	static void GetLastPositionAGV(CAgv * ptrAgv);
	CDBEstadistica();
	virtual ~CDBEstadistica();


};

#endif // !defined(AFX_DBESTADISTICA_H__4EFF3953_A65B_4C92_8358_C61330B7F574__INCLUDED_)
