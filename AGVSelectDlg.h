#if !defined(AFX_AGVSELECTDLG_H__59A6B7E1_B28D_11D5_965E_0050DAE18928__INCLUDED_)
#define AFX_AGVSELECTDLG_H__59A6B7E1_B28D_11D5_965E_0050DAE18928__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AGVSelectDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CAGVSelectDlg 

class CAGVSelectDlg : public CDialog
{
// Konstruktion
public:
	CAGVSelectDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CAGVSelectDlg)
	enum { IDD = IDD_SELECT_AGV_HISTORY };
	UINT	m_nFz;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CAGVSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CAGVSelectDlg)
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_AGVSELECTDLG_H__59A6B7E1_B28D_11D5_965E_0050DAE18928__INCLUDED_
