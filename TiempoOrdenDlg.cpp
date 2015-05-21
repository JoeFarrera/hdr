// TiempoOrdenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tpc.h"
#include "TiempoOrdenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTiempoOrdenDlg dialog


CTiempoOrdenDlg::CTiempoOrdenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTiempoOrdenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTiempoOrdenDlg)
	m_augmento = 0;
	m_augmentoText = 0;
	m_frecuencia = 0;
	m_partida = 0;
	m_frecuenciaText = 0;
	//}}AFX_DATA_INIT
}


void CTiempoOrdenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTiempoOrdenDlg)
	DDX_Slider(pDX, IDC_SLIDER1, m_augmento);
	DDX_Text(pDX, IDC_AUGMENTOTIEMPO, m_augmentoText);
	DDX_Slider(pDX, IDC_SLIDER2, m_frecuencia);
	DDX_Text(pDX, IDC_PARTIDAORDEN, m_partida);
	DDV_MinMaxUInt(pDX, m_partida, 0, 100);
	DDX_Text(pDX, IDC_FRECUENCIAORDEN, m_frecuenciaText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTiempoOrdenDlg, CDialog)
	//{{AFX_MSG_MAP(CTiempoOrdenDlg)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, OnReleasedcaptureSlider2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTiempoOrdenDlg message handlers

void CTiempoOrdenDlg::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData (TRUE);
	if (m_augmento > 0)
		m_augmentoText = m_augmento / 10;
	else
		m_augmentoText = 1;
	UpdateData (FALSE);
	*pResult = 0;
}

void CTiempoOrdenDlg::OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData (TRUE);
	if (m_frecuencia > 0)
		m_frecuenciaText = m_frecuencia * 10;
	else
		m_frecuenciaText = 1;
	UpdateData (FALSE);
	*pResult = 0;
}
