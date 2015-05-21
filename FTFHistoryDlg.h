#if !defined(AFX_FTFHISTORYDLG_H__551B8CC0_1401_11D6_8460_0000F4C47446__INCLUDED_)
#define AFX_FTFHISTORYDLG_H__551B8CC0_1401_11D6_8460_0000F4C47446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FTFHistoryDlg.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CFTFHistoryDlg 

class CFTFHistoryDlg : public CDialog
{
// Konstruktion
public:
	CStringList m_listSelect;
	int m_nHistAnlass;
	CFTFHistoryDlg(CWnd* pParent = NULL);   // Standardkonstruktor

// Dialogfelddaten
	//{{AFX_DATA(CFTFHistoryDlg)
	enum { IDD = IDD_FTF_HISTORY };
	CListBox	m_ctrlListBoxFTFOhneHist;
	CListBox	m_ctrlListBoxFTFMitHist;
	//}}AFX_DATA


// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CFTFHistoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:

	// Generierte Nachrichtenzuordnungsfunktionen
	//{{AFX_MSG(CFTFHistoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHistKollision();
	afx_msg void OnHistDeadlock();
	afx_msg void OnHistAllgem();
	afx_msg void OnFtfZuHist();
	afx_msg void OnFtfNoHist();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ChangeListe(CListBox &tSrc,CListBox &tDest);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // AFX_FTFHISTORYDLG_H__551B8CC0_1401_11D6_8460_0000F4C47446__INCLUDED_
