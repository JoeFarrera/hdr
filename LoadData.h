#if !defined(AFX_LOADDATA_H__B267D1E3_65E8_4951_9216_5DE796EA873F__INCLUDED_)
#define AFX_LOADDATA_H__B267D1E3_65E8_4951_9216_5DE796EA873F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadData.h : header file
//

#include <fstream.h>
/////////////////////////////////////////////////////////////////////////////
// CLoadData dialog

class CTpcView;
#include "Meldepunkt.h"
#include "NMeldepunkt.h"
#include "Container.h"



class CLoadData : public CDialog
{
// Construction
public:
	DECLARE_SERIAL (CLoadData)
	void LoadNMP (fstream fS, CNMeldepunkt * ptrNMp);
	void SetFicheroNMP(LPCTSTR pszFichero);
	BOOL LoadMP (fstream fS, fstream fSNMP, CMeldepunkt * ptrMp);
	void SetFichero (LPCTSTR pszFichero);
	CLoadData(CWnd* pParent = NULL, CTpcView* pView = NULL);   // standard constructor
	virtual ~CLoadData();

	
	void SetMPArrayPtr (CTypedPtrArray <CObArray, CMeldepunkt*> * ptrMPArray)
	{
		m_mpArray = ptrMPArray;
	};
	void SetAreaMatrixPtr (tAreaFinder * ptrArea)
	{
		m_ptrAreaMatrix = ptrArea;
	};
	
// Dialog Data
	//{{AFX_DATA(CLoadData)
	enum { IDD = IDD_LOADDATA };
	CString	m_strFichero;
	CString	m_strCurLine;
	CString	m_strFicheroNMP;
	CString	m_strFicheroArea;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadData)
	public:
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoadData)
	afx_msg void OnLoadfile();
	afx_msg void OnBrowsearea();
	afx_msg void OnLoadarea();
	afx_msg void OnBrowsefile();
	afx_msg void OnBrowsefnmp();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void GetNMPList (fstream fSNMP, CMeldepunkt * ptrMp);
	unsigned int GetValue (CString * pStr);
	CTpcView * m_pView;
	CTypedPtrArray <CObArray, CMeldepunkt*> * m_mpArray; // MP array - passed in
	tAreaFinder * m_ptrAreaMatrix;	// Area matrix pointer - passed in


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADDATA_H__B267D1E3_65E8_4951_9216_5DE796EA873F__INCLUDED_)
