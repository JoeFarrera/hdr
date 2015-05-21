// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "tpc.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "tpcView.h"
#include "testSRDlg.h"
#include "DBEstacion.h"
#include "TestEstacionDLG.h"
#include "AGVSelectDlg.h"
#include "SituationSet.h"
//#include "v_block.h"
//#include "Tabsetfv.h"
#include "AutomaticTrafficControl.h"
#include "InsertarAgvDlg.h"
#include "Agv.h"
#include "GestorAgv.h"
#include "DBEstadoPLC.h"
#include "SolicitarCargaBat.h"
#include "TestGenOrdDlg.h"
#include "TiempoOrdenDlg.h"
#include "GestorOrden.h"
#include "Alarma.h"
#include "FTFHistoryDlg.h"
#include "tpcdoc.h"
#include "DialogStopPlc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDD_TESTSR, OnTestsr)
	ON_COMMAND(IDD_TESTESTACION, OnTestEstacion)
	ON_COMMAND(ID_WRITEAGVHISTORY, OnWriteAGVHistory)
	ON_UPDATE_COMMAND_UI(ID_SIMULATECOM, OnUpdateSimulatecom)
	ON_COMMAND(ID_SIMULATECOM, OnSimulatecom)
	ON_UPDATE_COMMAND_UI(ID_GENERATESIMORDERS, OnUpdateGeneratesimorders)
	ON_COMMAND(ID_GENERATESIMORDERS, OnGeneratesimorders)
	ON_COMMAND(IDS_INSERTARAGV, OnInsertarAgv)
	ON_COMMAND(IDS_CARGABAT, OnCargabat)
	ON_COMMAND(IDD_TESTSR_Q, OnTestsrQ)
	ON_COMMAND(ID_HERRAMIENTAS_TESTGENORDENES, OnHerramientasTestgenordenes)
	ON_COMMAND(ID_TIEMPOORDEN, OnTiempoOrden)
	ON_WM_CLOSE()
	ON_COMMAND(ID_SCROLL_LIST, OnScrollList)
	ON_UPDATE_COMMAND_UI(ID_SCROLL_LIST, OnUpdateScrollList)
	ON_COMMAND(IDS_READSITSTOP, OnReadsitstop)
	ON_COMMAND(IDD_TEST_STOP_PLC_COMS, OnTestStopPlcComs)
	ON_COMMAND(IDD_AUTOTRAFFICCONTROL, OnAutotrafficcontrol)
	ON_UPDATE_COMMAND_UI(IDD_AUTOTRAFFICCONTROL, OnUpdateAutotrafficcontrol)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
	ON_MESSAGE (WM_USER_MESSAGE, OnUserMessage)
	ON_MESSAGE (WM_USER_MESSAGE_GLOBAL, OnUserMessageGlobal)
	ON_MESSAGE (WM_USER_SIEMENS_SR, OnUserMessageSiemens)
	ON_MESSAGE (WM_USER_SIEMENS_SEND, OnUserMessageSiemensSend)
	ON_MESSAGE (WM_USER_TESTUITHREAD, OnUserMessageTestUIThread)
	ON_MESSAGE (WM_USER_GETESTADOPLC, OnUserMessageGetEstadoPLC)
	ON_MESSAGE (WM_USER_AGV_UPDATE_BUB, OnUserMessageAgvUpdateBuB)
	ON_MESSAGE (WM_USER_SOCKET_READ, OnUserMessageSocketRead)
	ON_MESSAGE (WM_USER_SOCKET_WRITE, OnUserMessageSocketWrite)

	ON_WM_TIMER()
	ON_COMMAND(ID_HERRAMIENTAS_GUARDARPOSICIONESAGVS, &CMainFrame::OnHerramientasGuardarposicionesagvs)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CAPS, &CMainFrame::OnUpdateSocketRead)	
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_NUM, &CMainFrame::OnUpdateSocketWrite)	
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_pView = NULL;
	m_sRead.Format("%s","NONE");
	m_sWrite.Format("%s","NONE");

	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, 
		CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}
/*
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndDlgBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
	*/
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	SetTimer(1, 2000, NULL); // Timer per l'onIdle
	SetTimer(2, 15000, NULL); //Timer que fa que cada quinze segons s'actualitzin totes les estacions
	return 0;
}

/*
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT, // lpcs
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(0, 0), pContext) || 
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CTpcView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}
*/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

CTpcView* CMainFrame::GetRightPane()
{
	// Michael 09.01.2002 Por haber quitado el Splitter Window ...
	return ((CTpcView *)GetActiveView ());

	/* En vez de...
	CWnd* pWnd = m_wndSplitter.GetPane(0, 1);
	CTpcView* pView = DYNAMIC_DOWNCAST(CTpcView, pWnd);
	return pView;
	*/
}

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	CTpcView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
		pCmdUI->Enable(FALSE);
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetRadio(bChecked ? 1 : 0);
		}
	}
}


void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.
	CTpcView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CTpcView,
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;

		// other commands change the style on the list control
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;

		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;

		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;

		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
	}
}


// Recibir mensajes de otros threads y enviarlos al view principal
afx_msg LRESULT CMainFrame::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	if (NULL == m_pView)
		m_pView = GetRightPane ();

	MESSAGEPARMS * pMsg = (MESSAGEPARMS *)lParam;
	// Michael 14.06.2005 T5K
	if (pMsg)
	{
	// Michael 14.06.2005 T5K
	if (m_pView){
				
		m_pView->Message (pMsg->tipo, 
			pMsg->file,
			pMsg->linea,
			pMsg->modulo, 
			pMsg->strMensaje);

		delete pMsg;	// Borrar mensaje. Very Important
		}
	}

	return (0);

}

// Recibir mensajes de otros procesos y enviarlos al view principal
afx_msg LRESULT CMainFrame::OnUserMessageGlobal(WPARAM wParam, LPARAM lParam)
{
//	CString strMessage = (LPCTSTR)lParam;
	if (NULL == m_pView)
		m_pView = GetRightPane ();

	UINT uVal = (UINT)lParam;

	MSGAVISO1 ("Recibido de OnUserMessageGlobal: %d", uVal);

	return (0);

}


// Enviar mensaje a la ventana principal del thread principal. Estática...
void CMainFrame::Mensaje(MESSAGEPARMS *pMsg, const char *format, ...)
{
	va_list vl;
	va_start (vl, format);
	vsprintf (pMsg->strMensaje, format, vl);
	
	if (g_Container.m_pMainWnd)
		g_Container.m_pMainWnd->PostMessage (WM_USER_MESSAGE, 0, (long)pMsg);



}


// Respuesta de Siemens S/R
afx_msg LRESULT CMainFrame::OnUserMessageSiemens(WPARAM wParam, LPARAM lParam)
{
#if ! defined (FOREXPORTTOINDUMAT)

/* Michael 09.01.2002 Mover tratamiento siemens al thread PLCThread
	RB1 * ptrRB = (RB1 *) lParam;
	g_Container.m_pSR->Response(ptrRB);
*/

	if (g_Container.m_wPLCThreadId)
	{
		
		if (!PostThreadMessage (g_Container.m_wPLCThreadId,
			WM_USER_SIEMENS_SR, wParam, lParam))
		{
			MSGERROR1 ("PostThreadMessage failure: GetLastError ():", GetLastError ());
		}
		else
			MSGAVISO ("PostThreadMessage Completed");
	}
	else
	{
		MSGERROR ("Thread de Siemens no existe");
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (6, 8, 0);
	}
	

	return 0;

#endif
	return 0;

}

// Enviar el estado al PLC
afx_msg LRESULT CMainFrame::OnUserMessageSiemensSend(WPARAM wParam, LPARAM lParam)
{
	if (g_Container.m_wPLCThreadId)
	{
	if (!PostThreadMessage (g_Container.m_wPLCThreadId,
				WM_USER_SIEMENS_SEND, wParam, lParam))
		{
			MSGERROR1 ("PostThreadMessage failure: GetLastError ():", GetLastError ());
		}
		else
			MSGAVISO ("PostThreadMessage Completed");
	}
	else
	{
		MSGERROR ("Thread de Siemens no existe");
		CAlarma * ptrAlarma = new CAlarma;
		ptrAlarma->AvisarAlarma (6, 8, 0);
	}

	return 0;
}

// Enviar telegrama de test al PLC por Siemens SR
void CMainFrame::OnTestsr() 
{
#if ! defined (FOREXPORTTOINDUMAT)
	/* Michael 25.09.2001
	   Adjust pulmón A2 to be 3 bytes in what is sent here
	   Tested MCF 25.09.2001
	 */
	/*
	CTestSRDlg testSR;
	BYTE lenTele;
	if (IDOK == testSR.DoModal ())
	{
		switch (testSR.m_tipoTel [0])
		{
		case 'B':
			// Bloque de datos
			lenTele = 19; // Número de carga baterias
			break;
		case 'Q':
			// Petición de telegramas
			lenTele = 1;
			break;
		default:
			{
				CString strTemp;
				strTemp.Format ("Tipo de telegrama %c no reconocido", testSR.m_tipoTel [0]);
				AfxMessageBox(strTemp, MB_OK);
				lenTele = 0;
			}
			break;
		}

		if (lenTele > 0)
			g_Container.m_pSR->SendTelegrama (testSR.m_tipoTel [0], testSR.m_Edit, lenTele);
	}
	*/
	CDBEstadoPLC estadoPLC;
	if (IDYES == AfxMessageBox ("Enviar el estado al PLC ?", MB_YESNOCANCEL))
		estadoPLC.SendEstadoPLC ();

#endif

}

// XXX-Lac02: This function handles the new menu point 'Deadlock/Write AGV history'
void CMainFrame::OnWriteAGVHistory() 
{
	CFTFHistoryDlg dlgHist;

	if (IDOK ==	dlgHist.DoModal())
	{
		g_Container.m_listSelectAgvHist.AddTail (&dlgHist.m_listSelect);
		g_Container.m_histFileType = (enum T_Hist)dlgHist.m_nHistAnlass; // For file extension
		g_Container.m_fWriteAgvHistoryFile = true;
		// List will be written in Agv thread....

	}
}


void CMainFrame::OnTestEstacion() 
{
#if !defined (FOREXPORTTOINDUMAT)

	CTestEstacionDLG test;

	while (IDOK == test.DoModal ())
	{
		CDBEstacion estacion;
		if (estacion.GetEstAgv (test.m_nPtoCom, test.m_nPosicion, test.m_nLado))
		{
			test.m_CodClIND = estacion.GetClIND ();
		}
		else
			test.m_CodClIND = "????";
	}
#endif
}

void CMainFrame::OnUpdateSimulatecom(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(g_Container.m_fSimulateCom);
	
}

void CMainFrame::OnSimulatecom() 
{
	g_Container.m_fSimulateCom = !g_Container.m_fSimulateCom;
	AfxGetApp ()->WriteProfileInt ("Settings", 
		"SimulateCom",
		g_Container.m_fSimulateCom ? 1 : 0);
}

void CMainFrame::OnUpdateGeneratesimorders(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(g_Container.m_fAutoOrders);
	
}

void CMainFrame::OnGeneratesimorders() 
{
	g_Container.m_fAutoOrders = !g_Container.m_fAutoOrders;
	AfxGetApp ()->WriteProfileInt ("Settings", 
		"AutoOrders",
		g_Container.m_fAutoOrders ? 1 : 0);
}


// Insert an Agv in the System at a specified CP: For testing only
void CMainFrame::OnInsertarAgv() 
{

	CInsertarAgvDlg insAgv;
	if (IDOK == insAgv.DoModal ())
	{
		CAgv * ptrAgv = g_pGestor->GetAgvPtr (insAgv.m_nAgv);
		if (ptrAgv)
		{
			// Insert Agv independent of whether it is already in system or not
			ptrAgv->InsertarAGV (insAgv.m_Mp, insAgv.m_Pos);
			ptrAgv->SetEstado (AGVSINDATOS);
		}
	}

}


// Probar carga de bateria
void CMainFrame::OnCargabat() 
{
	CSolicitarCargaBat solCarga;
	if (IDOK == solCarga.DoModal ())
	{
		CAgv * ptrAgv = g_pGestor->GetAgvPtr (solCarga.m_nAgv);
		if (ptrAgv)
		{
			if (solCarga.m_fCargar)
			{
				if (solCarga.m_cargador != -1)
				{
					solCarga.m_cargador += 53;	// Convertir en punto de comunicación

					if (ptrAgv->m_pFzdat->mp != solCarga.m_cargador && CAgv::IsOccupiedCP (solCarga.m_cargador))
					{			
						CString strMsg;
						strMsg.Format(IDS_CARGADOROCUPADO,solCarga.m_nAgv);
						AfxMessageBox(strMsg);
					}
					else
					{
						ptrAgv->SetPeticionCarga (solCarga.m_cargador);
					}
				}
				else
					AfxMessageBox(IDS_NOCARGADOR);
			}
			else
				ptrAgv->SetPeticionCarga (0);	// Anular petición anterior
		}
		else
			AfxMessageBox (IDS_AGVNOTFOUND);
	}
}

void CMainFrame::OnTestsrQ() 
{
#if ! defined (FOREXPORTTOINDUMAT)
	BYTE ptrDatos [2];
	if (IDYES == AfxMessageBox ("Solicitar datos al PLC (Q) ?", MB_YESNOCANCEL))
		g_Container.m_pSR->SendTelegrama ('Q', ptrDatos, 1);
#endif
		
}

// Stub para probar generación de órdenes
void CMainFrame::OnHerramientasTestgenordenes() 
{
	CTestGenOrdDlg testGen;
	if (IDOK == testGen.DoModal ())
	{
		CDBEstacion dbEst;
		dbEst.TestGeneracionOrden (testGen.m_codest, testGen.m_tipest);
	}

}

void CMainFrame::OnTiempoOrden() 
{
	CTiempoOrdenDlg tiempoDlg;
	tiempoDlg.m_augmentoText = g_GestorOrden.GetAugmentoTiempo () / 1000;
	tiempoDlg.m_augmento = tiempoDlg.m_augmentoText * 10;
	tiempoDlg.m_frecuenciaText = g_GestorOrden.GetFrecuenciaOrden ();
	tiempoDlg.m_frecuencia = tiempoDlg.m_frecuenciaText / 10;
	tiempoDlg.m_partida = g_GestorOrden.GetPartidaOrden ();
	if (IDOK == tiempoDlg.DoModal ())
	{
		g_GestorOrden.SetAugmentoTiempo (tiempoDlg.m_augmentoText * 1000);
		g_GestorOrden.SetFrecuenciaOrden (tiempoDlg.m_frecuenciaText);
		g_GestorOrden.SetTiempoPartidaOrden (tiempoDlg.m_partida);
	}
	
}


void CMainFrame::OnClose() 
{
	// Michael 28.12.2001: Avisar del cierre
	if (IDYES == AfxMessageBox(IDS_CLOSE_WARNING, MB_YESNO))
	{
			// Michael 09.06.2008 Disconectar Siemens
			if (g_Container.m_pSR)
			{
				g_Container.m_pSR->Process(DO_CLOSE);
				MSGAVISO ("Closing Siemens S/R");
				delete g_Container.m_pSR;
			}
		// Michael 09.06.2008 Fin

		
	

	}

		CFrameWnd::OnClose();
}

// Solicitar el estado del PLC
afx_msg LRESULT CMainFrame::OnUserMessageGetEstadoPLC(WPARAM wParam, LPARAM lParam)
{
	BYTE ptrDatos [2];
	MSGAVISO ("Solicitando el estado del PLC");
	if (!(g_Container.m_pSR == NULL))
		g_Container.m_pSR->SendTelegrama ('Q', ptrDatos, 1);
	return 0;
}


afx_msg LRESULT CMainFrame::OnUserMessageAgvUpdateBuB(WPARAM wParam, LPARAM lParam)
{
	if (!PostThreadMessage (g_Container.m_wBuBThreadId,
				WM_USER_AGV_UPDATE_BUB, wParam, lParam))
		{
			MSGAVISO1 ("PostThreadMessage failure: GetLastError ():", GetLastError ());
		}
		else
			; // MSGAVISO ("PostThreadMessage Completed");
		return 0;
}


afx_msg LRESULT CMainFrame::OnUserMessageTestUIThread(WPARAM wParam, LPARAM lParam)
{
	    if (!PostThreadMessage (g_Container.m_wPLCThreadId,
				WM_USER_TESTUITHREAD, 0, 0))
		{
			MSGAVISO1 ("PostThreadMessage failure: GetLastError ():", GetLastError ());
		}
		else
			MSGAVISO ("PostThreadMessage Completed");
		return 0;
}



void CMainFrame::OnScrollList() 
{
	g_Container.m_fScrollList = !g_Container.m_fScrollList;
	
}

void CMainFrame::OnUpdateScrollList(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(g_Container.m_fScrollList);
}

void CMainFrame::OnReadsitstop() 
{
	g_Container.m_fReadSitStopGoFiles = true;	// Serán leidos en Agv thread
	
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent){
		case 1:
			{
				CTpcApp* pTpcApp = (CTpcApp*)AfxGetApp();
				pTpcApp->OnIdle(0);
				break;
			}

		case 2: // Actualitza totes les estacions cada 15 segons
			{
				//g_Container.m_pDoc->SendAllStations(NULL, true);
				g_Container.m_GestorEstBuB.UpdateEstacion();
				break;
			}
	}

}

void CMainFrame::OnTestStopPlcComs() 
{
	// TODO: Add your command handler code here	
	
	CDialogStopPlc plcDialog = new CDialogStopPlc();

	if (IDOK == plcDialog.DoModal ())
	{
		g_Container.m_pSR->SendClose();

	}

	
}

void CMainFrame::OnAutotrafficcontrol() 
{
	theAutoTrafficControl.SetActivated(theAutoTrafficControl.IsActivated() ? false : true);
	
}

void CMainFrame::OnUpdateAutotrafficcontrol(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(theAutoTrafficControl.IsActivated() ?  1 : 0);
	
}

void CMainFrame::OnHerramientasGuardarposicionesagvs()
{
	// TODO: Add your command handler code here
	g_Container.m_fWriteAGVPosition = true;
}

void CMainFrame::OnUpdateSocketRead(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(); 
	pCmdUI->SetText(m_sRead); 
}
void CMainFrame::OnUpdateSocketWrite(CCmdUI *pCmdUI)
{
    pCmdUI->Enable(); 
	pCmdUI->SetText(m_sWrite); 
}

afx_msg LRESULT CMainFrame::OnUserMessageSocketRead(WPARAM wParam, LPARAM lParam)
{
	m_sRead.Format("%s", (LPCTSTR)lParam);
	return 0;
}

afx_msg LRESULT CMainFrame::OnUserMessageSocketWrite(WPARAM wParam, LPARAM lParam)
{
	m_sWrite.Format("%s", (LPCTSTR)lParam);
	return 0;
}