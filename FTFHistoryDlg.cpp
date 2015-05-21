// FTFHistoryDlg.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "tpc.h"
#include "FTFHistoryDlg.h"
#include "SituationSet.h"
//#include "v_block.h"
#include "Tabsetfv.h"

#include "MACRO.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CFTFHistoryDlg 


CFTFHistoryDlg::CFTFHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFTFHistoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFTFHistoryDlg)
	//}}AFX_DATA_INIT
}


void CFTFHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFTFHistoryDlg)
	DDX_Control(pDX, IDC_LIST_FTFOHNEHIST, m_ctrlListBoxFTFOhneHist);
	DDX_Control(pDX, IDC_LIST_FTFMITHIST, m_ctrlListBoxFTFMitHist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFTFHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CFTFHistoryDlg)
	ON_BN_CLICKED(IDC_HIST_KOLLISION, OnHistKollision)
	ON_BN_CLICKED(IDC_HIST_DEADLOCK, OnHistDeadlock)
	ON_BN_CLICKED(IDC_HIST_ALLGEM, OnHistAllgem)
	ON_BN_CLICKED(IDC_FTF_ZU_HIST, OnFtfZuHist)
	ON_BN_CLICKED(IDC_FTF_NO_HIST, OnFtfNoHist)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CFTFHistoryDlg 

BOOL CFTFHistoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	// TODO: Zusätzliche Initialisierung hier einfügen
	int iLBIndex;
	//for (int i = 1; i <= KKFZMAX; i++)
	for (int i = 1; i <= g_pCourseData->GetNumberAGVs(); i++)
	{
		// alle Fahrzeuge durchscannen und FZ-Nummer in Listbox übernehmen
		if (TEST_FZ_IM_SYSTEM(fzdat[i]))
		{
			CString strFZ;
			strFZ.Format("%3d", fzdat[i].fznr);
			// Einfügeindex merken
			iLBIndex = m_ctrlListBoxFTFOhneHist.AddString(strFZ);
			// index in g_FZ mit der FZ Nummer speichern
			//pLB->SetItemData(iLBIndex, i);
		}
	}
	m_nHistAnlass = HIST_ALLGEMEIN;
	CButton *pButton = (CButton*)GetDlgItem(IDC_HIST_ALLGEM);
	pButton->SetCheck(BST_CHECKED);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void CFTFHistoryDlg::OnHistKollision() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	m_nHistAnlass = HIST_CRASH;
}

void CFTFHistoryDlg::OnHistDeadlock() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	m_nHistAnlass = HIST_DEADLOCK;	
}

void CFTFHistoryDlg::OnHistAllgem() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	m_nHistAnlass = HIST_ALLGEMEIN;	
}

void CFTFHistoryDlg::OnFtfZuHist() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	ChangeListe(m_ctrlListBoxFTFOhneHist,m_ctrlListBoxFTFMitHist);
	UpdateData(false);
}

void CFTFHistoryDlg::OnFtfNoHist() 
{
	// TODO: Code für die Behandlungsroutine der Steuerelement-Benachrichtigung hier einfügen
	ChangeListe(m_ctrlListBoxFTFMitHist, m_ctrlListBoxFTFOhneHist);	
	UpdateData(false);
}

void CFTFHistoryDlg::ChangeListe(CListBox &tSrc, CListBox &tDest)
{
	CString str;
	CStringList listChange;
	int nIndex ;
	int nAnzSel = tSrc.GetSelCount();
	if(nAnzSel > 0)
	{
		int *piSelItems = new int[nAnzSel];

		tSrc.GetSelItems(nAnzSel,piSelItems);
		for(int i = 0;
		i < nAnzSel;
		i++)
		{
			tSrc.GetText(*(piSelItems + i),str);
			listChange.AddTail(str);
		}
		POSITION posChange = listChange.GetHeadPosition();
		while(posChange != NULL)
		{
			str = listChange.GetNext(posChange);
			nIndex = tSrc.FindString(-1,str);
			tSrc.DeleteString(nIndex);
			tDest.AddString(str);
		}

		delete piSelItems;
	}
}

void CFTFHistoryDlg::OnOK() 
{
	// TODO: Zusätzliche Prüfung hier einfügen
	if(m_ctrlListBoxFTFMitHist.GetCount() > 0)
	{
		for(int nFz = 1;
		//nFz <= KKFZMAX;
		nFz <= g_pCourseData->GetNumberAGVs();
		nFz++)
		{
			CString str;
			str.Format("%3d",nFz);
			if(m_ctrlListBoxFTFMitHist.FindString(-1,str) != LB_ERR)
			{
				// gefunden
				m_listSelect.AddTail(str);
			}
		}
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox("No se ha seleccionado Agvs para la historia !");
	}
}
