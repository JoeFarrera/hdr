// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "tpc.h"

#include "tpcDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here

}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |=  TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	cs.cx = 200;
	cs.lpszName = "Sistema TPC";

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CTpcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	CTreeCtrl & treeCtrl = GetTreeCtrl ();
	m_IList.Create (IDB_BITMAP1,16, 1, RGB (255, 255, 255));
	// m_IList.Create (IDB_BITMAP2,32, 1, RGB (255, 255, 255));
	GetTreeCtrl ().SetImageList (&m_IList, TVSIL_NORMAL);

	
	HTREEITEM hRoot = treeCtrl.InsertItem (_T("Gestor TPC"),0,0,
			TVI_ROOT, TVI_SORT);

	HTREEITEM hMesa = treeCtrl.InsertItem (_T("Mesas"),1,1,
			hRoot, TVI_SORT);
	
	HTREEITEM hAgv = treeCtrl.InsertItem (_T("Agvs"),0,0,
			hRoot, TVI_SORT);
	
	treeCtrl.InsertItem (_T("Agv 1"), m_hAgv);
	treeCtrl.InsertItem (_T("Agv 2"), m_hAgv);
	
	treeCtrl.InsertItem (_T("Mesa P1"), 1,1, hMesa);
	treeCtrl.InsertItem (_T("Mesa P2"), 1,1, hMesa);
	
	
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CTpcDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTpcDoc)));
	return (CTpcDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers


