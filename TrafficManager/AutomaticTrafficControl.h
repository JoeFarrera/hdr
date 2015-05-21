// AutomaticTrafficControl.h: interface for the CAutomaticTrafficControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOMATICTRAFFICCONTROL_H__AAB6A44F_D6BE_4EE0_B235_C9D50E3F5827__INCLUDED_)
#define AFX_AUTOMATICTRAFFICCONTROL_H__AAB6A44F_D6BE_4EE0_B235_C9D50E3F5827__INCLUDED_

#include "BlockTableAGVTypePair.h"	// Added by ClassView
#include "DistanceReqWayTele.h"
#include "..\TabSetFV.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAutomaticTrafficControl  
{
public:
	void FillBlockTable(DEF_BLOCK_TABLE* pBlockTables);
	void SetActivated(const bool bActivated);
	bool IsActivated();
	void GenerateListDistancesReqWayTele_Old(CDistanceReqWayTeleMap &mapDistancesReqWayTele);
	void GenerateListDistancesReqWayTele(CDistanceReqWayTeleMap &mapDistancesReqWayTele);
	//bool GetNoCrashDriveStop(int nTopAGV, int nCompAGV, int &nDrivestopReason);
	bool GetNoCrashDriveStop(TYP_FZDAT *pTopData, TypEABFzStrukt &tAFz,
							 int nCompAGV, TypEABFzStrukt &tVFz,
							 int &nDrivestopReason);
	//bool ReadBlockTable();
	CAutomaticTrafficControl();
	virtual ~CAutomaticTrafficControl();

private:
	bool m_bActivated;
	bool IsToRequestWayTele(int nDIndex,int nMyBlockway);
	bool BlockWayAlreadyFree(CCrashDistance*	pCD,
							 int				nAGVWay,
							 bool				bPosWayLaengerBlkWay);
	bool CheckBlkPosWay(CCrashDistance *pCD, CCrashDistance *pReversedCD, int nBlkPosWay, int nDrivenWay, bool bStopedAGV);
	bool ProcessTop_Comp(	CCrashDistance*	pCrT_C,
							CCrashDistance*	pCrC_T,
							CCrashDistance*	pCrT_NC,
							CCrashDistance*	pCrNT_C,
							CCrashDistance*	pCrC_NT,
							CCrashDistance*	pCrNT_NC,
							USHORT			nCompWay,
							int				nBlkPosWayComp,
							bool			bCompPosWayLongerBlkWay,
							bool			bDrivePermCompAGV,
							USHORT			nTopWay,
							bool			&bRequestWayCompAGV,
							bool			bStopedCompAGV);
	bool ProsessTop_NextComp(	CCrashDistance*	pCrNC_T,
								CCrashDistance*	pCrNC_NT,
								CCrashDistance*	pCrC_NT,
								CCrashDistance*	pCrNT_C,
								USHORT			nCompWay,
								int				nBlkPosWayComp,
								int				nNextBlkPosWayComp,
								int				nNextBlkPosWayTop,
								bool			bDrivePermCompAGV,
								USHORT			nTopWay,
								bool			bStopedCompAGV);
	bool ProcessNextTop_Comp(	CCrashDistance*	pCrNT_C,
								CCrashDistance*	pCrC_NT,
								CCrashDistance*	pCrNT_NC,
								CCrashDistance*	pCrNC_NT,
								USHORT			nCompWay,
								int				nBlkPosWayComp,
								int				nNextBlkPosWayComp,
								bool			bDrivePermCompAGV,
								int				nNextBlkPosWayTop,
								bool			bNTopPosWayLongerBlkWay,
								bool			&bRequestWayCompAGV,
								bool			bStopedCompAGV);
	bool ProcessNextTop_NextComp(	CCrashDistance*	pCrNC_NT,
									int				nNextBlkPosWayComp,
									bool			bDrivePermCompAGV,
									CCrashDistance*	pCrNT_NC,
									int				nNextBlkPosWayTop);
	CCrashDistance* GetPCrashDistances(CBlockTableAGVTypePair *pBlkTable, int nTDistIndex, int nCDistIndex);
	bool CompAGVOrdersNoCrashDriveStop(	USHORT	nTopAGVType,
										USHORT	nCompAGVType,
										int		nDistIndexTop,
										int		nDistIndexComp,
										int		nNextDistIndexTop,
										int		nNextDistIndexComp,
										USHORT	nCompWay,
										int		nBlkPosWayComp,
										bool	bCompPosWayLongerBlkWay,
										int		nNextBlkPosWayComp,
										bool	bDrivePermCompAGV,
										USHORT	nTopWay,
										int		nNextBlkPosWayTop,
										bool	bNTopPosWayLongerBlkWay,
										bool&	bRequestWayCompAGV,
										bool	bStopedCompAGV);
	bool PosWayLongerBlkWay(TYP_FZDAT *pAGVData, int nDistIndex, bool bNCp);
	bool IsAGVOnStation(TYP_FZDAT *pAGVData);
	bool GetAnyStation(TYPE_STATION_DEF &myStation);
	int GetBlockWay(int nCp, int nNCp);
	int GetBlockWay(int nDistIndex);
	UINT GetPosWay(TYPE_STATION_DEF& myStation);
	UINT GetBlkPosWay(TYP_FZDAT *pAGVData, int nDistIndex, bool bNCp);
	//(NMP_tabelle* GetDistancePointer(int nCp, int nDestination);	JLac
	//UINT GetConnectionLength(int nCp, int nDestination, long& lLength);	JLac
	int GetAGVTypeDistanceIndex(int nCp, int nNCp);
	bool GetNoCrashDriveStop(TYP_FZDAT *pTopData, int nNCpTopAGV, int nCompAGV, int nNCpCompAGV);
	int m_nDriveStopReasonNCB;
	CBlockTableAGVTypePair m_BlockTable;
public:
	CBlockTableAGVTypePair* GetPtrBlockTable();
};


extern CAutomaticTrafficControl theAutoTrafficControl;

#endif // !defined(AFX_AUTOMATICTRAFFICCONTROL_H__AAB6A44F_D6BE_4EE0_B235_C9D50E3F5827__INCLUDED_)
