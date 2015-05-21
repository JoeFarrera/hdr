// tpcDoc.cpp : implementation of the CTpcDoc class
//

#include "stdafx.h"
#include "tpc.h"

#include "tpcDoc.h"
#include "tpcView.h"
#include "Msg.h"
#include "TCPServerSocket.h"
#include "ClientSocket.h"
#include "Agv.h"
#include "GestorAgv.h"
#include "Orden.h"
#include "GestorOrden.h"
#include "dbutil.h"
#include "dbestacion.h"
#include "BackgroundTasks.h"
#include "GestorEstacionBuB.h"
#include "PLCThread.h"

//#include "Block.h"	JLac
//#include "TabSetFV.h"	//JLac
#include "AutomaticTrafficControl.h"

#include "Bed_zs.h"
#include "Macro.h"
#include "DBEstadistica.h"

#include "MsgMacro.h"
#include "A1_Z_TYP.C86"	// ziel type table
#include "a1_e_typ.c86" // Level table 


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTpcDoc

IMPLEMENT_DYNCREATE(CTpcDoc, CDocument)

BEGIN_MESSAGE_MAP(CTpcDoc, CDocument)
	//{{AFX_MSG_MAP(CTpcDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTpcDoc construction/destruction





CTpcDoc::CTpcDoc()
{
	// TODO: add one-time construction code here

}

CTpcDoc::~CTpcDoc()
{

	EABDeInit();
}

BOOL CTpcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;





	CString strFile = AfxGetApp ()->GetProfileString ("Settings", "ControlDataFile");
	TRACE ("Autoblock file is: %s", strFile);
	
	CString strPath = _T(".\\");
	CCourseLoader * pCL = new CCourseLoader(strPath + strFile);	// TODO Decide path for controldata.bin file
	g_pCourseData = pCL->LoadCourse();

	// TODO - Throw error if file has not been read!! (g_pCourseData is null)

	delete pCL;	// No longer needed

	// TODO controldata.bin 2014
	//UINT i = g_Block.ReadBlockTab();
	EABInit();
	//Ver_BlStre_Root = g_Block.GetVerBlStreRoot();
	//BlStre_Root     = g_Block.GetBlStreRoot();
	//BlStreExt_Root  = g_Block.GetBlStreExtBlock();

	
	// TODO controldata.bin 2014 JL: Seems to be reading the blocking table a second time
	theAutoTrafficControl.SetActivated(true);
	if(theAutoTrafficControl.IsActivated())
	{
		//bool bTrafficManagerOK = theAutoTrafficControl.ReadBlockTable();
		theAutoTrafficControl.FillBlockTable(g_pCourseData->GetPtrBlockTables());

		g_Container.SetBlockingTableCopied();

		//Generate list with distances to request Weg Telegram:
		theAutoTrafficControl.GenerateListDistancesReqWayTele(m_mapDistancesReqWayTele);
		//int nDistancesReqWayTele = m_mapDistancesReqWayTele.GetCount();
	}

	g_Container.SetPtrDoc (this);

	//Set the AGVType course:
	g_pGestor = new CGestorAGV(&m_mapDistancesReqWayTele);	// Crear Gestion de AGVs

	g_Container.InitStations();	


	// StartSocketServer ();

	

	// Arrancar thread de comunicaciones
	//g_pGestor->ArrancarComThread ();
	g_pGestor->ArrancarComThread (&m_mapDistancesReqWayTele);


	m_ptrContext = g_Container.m_dbutil.ConectarOracle ("TPCDocThread");

	g_GestorOrden.InitTiempoEspera ();

	CBackgroundTasks::ArrancarThread ();

	CAlarma * ptrAlarm = new CAlarma;
	ptrAlarm->AvisarAlarma (2, 1, 2, 4, 5, 0,0,0,0,"This is the OnNewDocument", "from the tpc");

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTpcDoc serialization

/////////////////////////////////////////////////////////////////////////////
// CTpcDoc diagnostics

#ifdef _DEBUG
void CTpcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTpcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTpcDoc commands

