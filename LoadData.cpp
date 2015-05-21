// LoadData.cpp : implementation file
//



#include "stdafx.h"
#include "tpc.h"

#include "tpcDoc.h"
#include "tpcView.h"
#include "LoadData.h"

#include "msgMacro.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoadData dialog

IMPLEMENT_SERIAL (CLoadData, CDialog, 1)

CLoadData::CLoadData(CWnd* pParent /*=NULL*/, CTpcView* pView)
	: CDialog(CLoadData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadData)
	m_strFichero = _T("");
	m_strCurLine = _T("");
	m_strFicheroNMP = _T("");
	m_strFicheroArea = _T("");
	//}}AFX_DATA_INIT
	m_pView = pView;
	m_strFicheroArea.Empty ();
}

CLoadData::~CLoadData ()
{
	int i = m_mpArray->GetSize ();
	for (; i > 0; i-- )
	{
		CMeldepunkt * ptrMp = m_mpArray->GetAt (i - 1);
		m_mpArray->RemoveAt (i - 1);
		delete ptrMp;
	}
}



void CLoadData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadData)
	DDX_Text(pDX, IDC_FILENAME, m_strFichero);
	DDX_Text(pDX, IDC_CURLINE, m_strCurLine);
	DDX_Text(pDX, IDC_FICHERONMP, m_strFicheroNMP);
	DDX_Text(pDX, IDC_FICHEROAREA, m_strFicheroArea);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoadData, CDialog)
	//{{AFX_MSG_MAP(CLoadData)
	ON_BN_CLICKED(IDC_LOADFILE, OnLoadfile)
	ON_BN_CLICKED(IDC_BROWSEAREA, OnBrowsearea)
	ON_BN_CLICKED(IDC_LOADAREA, OnLoadarea)
	ON_BN_CLICKED(IDC_BROWSEFILE, OnBrowsefile)
	ON_BN_CLICKED(IDC_BROWSEFNMP, OnBrowsefnmp)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoadData message handlers

void CLoadData::SetFichero(LPCTSTR pszFichero)
{
	m_strFichero = pszFichero;

}

// Load data from the file specified
void CLoadData::OnLoadfile() 
{
	fstream fS, fSNMP;
	CString strRead;
	LPTSTR pStr;


	
	fS.open (m_strFichero, 
				ios::in || ios::nocreate, 
				filebuf::sh_read);

	if (!fS.is_open ())
		AfxMessageBox ("File not open");
	else
	{
		fSNMP.open (m_strFicheroNMP,
			ios::in || ios::nocreate, 
				filebuf::sh_read);


		pStr = strRead.GetBuffer (500);
		while (fS.getline (pStr, 100))
		{			
			m_strCurLine = pStr;
			UpdateData (FALSE);
			if (-1 != strRead.Find ("<MP"))
			{
				CMeldepunkt * ptrMp = new CMeldepunkt;
				if (LoadMP (fS, fSNMP, ptrMp))
					m_mpArray->Add (ptrMp);
				else
				{
					AfxMessageBox ("Not liked...");
					break;
				}


			//	if (IDCANCEL == AfxMessageBox (pStr, MB_OKCANCEL))
			//		break;;
			}
			/*
			if (-1 != strRead.Find ("MP\\>"))
			{
				if (IDCANCEL == AfxMessageBox (pStr, MB_OKCANCEL))
					break;;
			}
			*/
			

		}
		fS.close ();
		fSNMP.close ();
		int nCount = m_mpArray->GetSize ();
		for (int i = 0; i < nCount; i++)
		{
			CMeldepunkt *mp = m_mpArray->GetAt(i);
			TRACE (_T("lf_nr: %d strNMP: %s anz_nmp: %d\n"), 
					mp->m_Mp.lf_nr,
					mp->m_strNMP,
					mp->m_Mp.anz_nmp);
			if (mp->m_Mp.anz_nmp > 0)
			{
				int i = mp->m_NmpArray.GetSize ();
				TRACE (_T("mp.m_NmpArray->GetSize (): %d\n"), i);
				for (int j = 0; j < i; j++)
				{
					CNMeldepunkt * ptrNMP;
					ptrNMP = mp->m_NmpArray.GetAt (j);
					TRACE (_T("%d next nmp: %d\n"), j, ptrNMP->m_NMp.nmp);
				}
			}


		}
	}
	
}

BOOL CLoadData::LoadMP(fstream fS, fstream fSNMP, CMeldepunkt * ptrMp)
{
	CString strRead;
	LPTSTR pStr;

	for (int i = 0; i < 8; i++)
	{
		pStr = strRead.GetBuffer (100);
		fS.getline (pStr, 100);
		strRead.ReleaseBuffer ();
		if (-1 != strRead.Find (_T("anz_nmp")))
		{
			ptrMp->m_Mp.anz_nmp = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("lf_nr")))
		{
			ptrMp->m_Mp.lf_nr = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("typ")))
		{
			ptrMp->m_Mp.typ = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("gs_nr")))
		{
			ptrMp->m_Mp.gs_nr = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("zdv_nr")))
		{
			ptrMp->m_Mp.zdv_nr = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("ktn_nr")))
		{
			ptrMp->m_Mp.ktn_nr = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("ep_nr")))
		{
			ptrMp->m_Mp.ep_nr = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("nmp_strPtr")))
		{
			ptrMp->m_strNMP = strRead.Mid (strRead.Find ("=") + 1, 8);
			ptrMp->m_strNMP.TrimRight ();
			ptrMp->m_strNMP.TrimLeft ();

			// sacar número del MP
			int pos = ptrMp->m_strNMP.Find ("MP_");
			CString strValue =  ptrMp->m_strNMP.Mid (pos + 3, 4);	// Read 4 positions
			ptrMp->m_numMP = atoi (strValue);

			ptrMp->GetStationPtr ();


			CString strMessage;
			strMessage.Format (_T("MP: %s lf_nr: %d anz_nmp: %d"), 
				ptrMp->m_strNMP,
				ptrMp->m_Mp.lf_nr,
				ptrMp->m_Mp.anz_nmp);
			MSGTRACE (strMessage);


			if (ptrMp->m_Mp.typ & 0x02)
			{
				strMessage.Format (_T(" Destino: %d Cust Name: %s anz_nmp: %d"), 
					ptrMp->m_ptrStat->zieltyp_l,
					ptrMp->m_strCustName);

				MSGTRACE (strMessage);

			}


			GetNMPList (fSNMP, ptrMp);
			continue;
		}

		// for each attribute of MP (total of 8)
	}
	return (true);

}

// devolver el valor de un variable leido
unsigned int CLoadData::GetValue(CString *pStr)
{
	int pos;
	pos = pStr->Find ("=");
	CString strValue = pStr->Mid (pos + 1, 8);	// Read 8 positions
	return (atoi (strValue));
}

void CLoadData::SetFicheroNMP(LPCTSTR pszFichero)
{
	m_strFicheroNMP = pszFichero;

}

// Get list of next points for a point
void CLoadData::GetNMPList(fstream fSNMP, CMeldepunkt *ptrMp)
{
	CString strRead;
	LPTSTR pStr;

	pStr = strRead.GetBuffer (500);
	fSNMP.seekg (0);
	while (fSNMP.getline (pStr, 100))
	{		
		if (-1 != strRead.Find (ptrMp->m_strNMP))
		{
			// Read list of next points
			for (int i = 0; i < ptrMp->m_Mp.anz_nmp; i++)
			{
			
				CNMeldepunkt * ptrNMp = new CNMeldepunkt;

			
				LoadNMP (fSNMP, ptrNMp);
				CString strMessage;
				strMessage.Format (_T(" NMP : %d blk_weg: %d str_len: %d nep: %d"), 
					ptrNMp->m_NMp.nmp,
					ptrNMp->m_NMp.blk_weg,
					ptrNMp->m_NMp.str_len,
					ptrNMp->m_NMp.nep);
				MSGTRACE (strMessage);

			
				ptrMp->m_NmpArray.Add (ptrNMp);
			}
			break;	// list was found and processed
		}
	}




}

// load array of next points
void CLoadData::LoadNMP(fstream fS, CNMeldepunkt *ptrNMp)
{
	CString strRead;
	LPTSTR pStr;
	BOOL fLastVal;
	int i;

	// Read all values, or a maximum of 10 lines
	for (fLastVal = false, i = 0; (i < 10 && !fLastVal); i++)
	{
		pStr = strRead.GetBuffer (100);
		fS.getline (pStr, 100);
		strRead.ReleaseBuffer ();
		if (-1 != strRead.Find (_T("nmp")))
		{
			ptrNMp->m_NMp.nmp = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("zeit_index")))
		{
			ptrNMp->m_NMp.zeit_index = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("blk_weg")))
		{
			ptrNMp->m_NMp.blk_weg = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("str_len")))
		{
			ptrNMp->m_NMp.str_len = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("n_ktn_nr")))
		{
			ptrNMp->m_NMp.n_ktn_nr = GetValue (&strRead);
			continue;
		}
		if (-1 != strRead.Find (_T("nep")))
		{
			ptrNMp->m_NMp.nep = GetValue (&strRead);
			fLastVal = true;	// this is the last value to read
		}
		/*
		if (-1 != strRead.Find (_T("zus_tab")))
		{
			ptrNMp->n_NMp.zus_tab = GetValue (&strRead);
			continue;
		} 
		* Ver si implementar esto */

		// for each attribute of NMP (total of 6)
	}



}

void CLoadData::OnBrowsearea() 
{
	CFileDialog areaFileDialog (TRUE);
	LPTSTR szFilemp [500];
	wsprintf ((char *)szFilemp, _T("C:\\hdr_es\\KONTRAST.2\\Prj\\HDR_ES\\ANL1\\SRC\\A1_EP_FB.INC"));

	areaFileDialog.m_ofn.lpstrTitle = "Fichero EP-FB a abrir";
	areaFileDialog.m_ofn.lpstrFile = (char *)szFilemp;

	if (areaFileDialog.DoModal () == IDOK)
	{
		m_strFicheroArea = areaFileDialog.GetPathName ();
		UpdateData (FALSE);

	}
	
}

void CLoadData::OnLoadarea() 
{
	if (m_strFicheroArea.IsEmpty ())
		AfxMessageBox ("Hay que especificar el fichero fuente primero");
	else
	{
		fstream fS;
		fS.open (m_strFicheroArea, 
				ios::in || ios::nocreate, 
				filebuf::sh_read);

		if (!fS.is_open ())
			AfxMessageBox ("File not open");
		else
		{
			CString strRead;
			LPTSTR pStr = strRead.GetBuffer (500);
			// for each decision point
			for (int nCurEP = 0; nCurEP < KKMAX_EP && fS.getline (pStr, 100); )
			{		
				strRead.ReleaseBuffer ();
				m_strCurLine = pStr;
				UpdateData (FALSE);
				int pos = strRead.Find ("<EP");
				if (-1 != pos)
				{

					CString strValue = strRead.Mid (pos + 3, 3);	// Read 3 positions
					nCurEP = atoi (strValue);
					m_ptrAreaMatrix [nCurEP].nep = nCurEP;
					pos = strRead.Find ("MP");
					strValue = strRead.Mid (pos + 2, 3);
					m_ptrAreaMatrix [nCurEP].mp = atoi (strValue);
					pStr = strRead.GetBuffer (500);
					fS.getline (pStr, 100);
					strRead.ReleaseBuffer ();
					// for each area
					for (int i = 0, curPos = 0; i < KKMAX_BER; i ++)
					{
						int nPos = strRead.Find (",", curPos);
						if (nPos == -1) {
							// read last one
							if (curPos > 0)
							{
								nPos = strRead.GetLength ();
								CString strMPValue = strRead.Mid (curPos, nPos - curPos);
								strMPValue.TrimLeft ();
								m_ptrAreaMatrix [nCurEP].nMpIndex [i + 1] = atoi (strMPValue);
								// if there are more expected
								if (i < (KKMAX_BER - 1))
								{
									pStr = strRead.GetBuffer (500);
									fS.getline (pStr, 100); // read next line
	 								strRead.ReleaseBuffer ();
									nPos = strRead.Find (",");
									curPos = 0;
								}
								continue;
							}
						}
						CString strMPValue = strRead.Mid (curPos, nPos - curPos);
						strMPValue.TrimLeft ();
						m_ptrAreaMatrix [nCurEP].nMpIndex [i + 1] = atoi (strMPValue);
						curPos = nPos + 1;
					}


					pStr = strRead.GetBuffer (500);

				}
			}
			fS.close ();
		}
	}

}

void CLoadData::OnBrowsefile() 
{
	CFileDialog mpFileDialog (TRUE);
	LPTSTR szFilemp [500];


	wsprintf ((char *)szFilemp, m_strFichero);
	mpFileDialog.m_ofn.lpstrTitle = "Fichero MP a abrir";
	mpFileDialog.m_ofn.lpstrFile = (char *)szFilemp;
	
	if (mpFileDialog.DoModal () == IDOK)
		SetFichero (mpFileDialog.GetPathName ());

	
}

void CLoadData::OnBrowsefnmp() 
{
	CFileDialog mpFileDialog (TRUE);
	LPTSTR szFilemp [500];


	wsprintf ((char *)szFilemp, m_strFicheroNMP);
	mpFileDialog.m_ofn.lpstrTitle = "Fichero NMP a abrir";
	mpFileDialog.m_ofn.lpstrFile = (char *)szFilemp;
	
	if (mpFileDialog.DoModal () == IDOK)
		SetFicheroNMP (mpFileDialog.GetPathName ());

	
	
}

void CLoadData::Serialize(CArchive& ar) 
{
	m_mpArray->Serialize (ar);
	if (ar.IsStoring())
	{
		for (int i = 0; i < KKMAX_EP_P1; i++)
			ar.Write (&m_ptrAreaMatrix [i], sizeof (tAreaFinder));
	}
	else
	{	// loading code
			for (int i = 0; i < KKMAX_EP_P1; i++)
				ar.Read (&m_ptrAreaMatrix [i], sizeof (tAreaFinder));

	}
}

void CLoadData::OnSave() 
{
	CFileDialog mpFileDialog (TRUE);
	LPTSTR szFilemp [500];


	if (m_mpArray->GetSize () > 0)
	{
	wsprintf ((char *)szFilemp, 
		_T("D:\\hdr_es\\KONTRAST.2\\Prj\\HDR_ES\\ANL1\\SRC\\MPDATA.TPC"));

	mpFileDialog.m_ofn.lpstrTitle = "Fichero a salvar";
	mpFileDialog.m_ofn.lpstrFile = (char *)szFilemp;
	
	if (mpFileDialog.DoModal () == IDOK)
	{
		CFile theFile;
		theFile.Open(mpFileDialog.GetFileName (), CFile::modeCreate | CFile::modeWrite);
		try {
		CArchive archive (&theFile, CArchive::store);

		Serialize (archive);
		archive.Close();
		theFile.Close();
		} catch (CFileException* ex ) {
           ex->ReportError ();
		}
		catch (CArchiveException* exAr) {
			exAr->ReportError ();
		}

	}
	}
	else
	{
		AfxMessageBox ("No hay datos para guardar");
	}
		
}

void CLoadData::OnLoad() 
{
	CFileDialog mpFileDialog (TRUE);
	LPTSTR szFilemp [500];


	wsprintf ((char *)szFilemp, 
		_T("D:\\hdr_es\\KONTRAST.2\\Prj\\HDR_ES\\ANL1\\SRC\\MPDATA.TPC"));

	mpFileDialog.m_ofn.lpstrTitle = "Fichero a salvar";
	mpFileDialog.m_ofn.lpstrFile = (char *)szFilemp;
	
	if (mpFileDialog.DoModal () == IDOK)
	{
		CFile theFile;
		theFile.Open(mpFileDialog.GetFileName (), CFile::modeRead | CFile::shareDenyRead);
		try {
		CArchive archive (&theFile, CArchive::load);
		
		m_mpArray->RemoveAll ();	// Borrar contenido actual

		Serialize (archive);
		archive.Close();
		theFile.Close();
		MSGTRACE ("Heres the file version");

		for (int i = 0; i < m_mpArray->GetSize (); i ++)
		{
			CMeldepunkt * ptrMp = m_mpArray->GetAt (i);
			CString strMessage;
			strMessage.Format (_T("MP: %s lf_nr: %d anz_nmp: %d"), 
				ptrMp->m_strNMP,
				ptrMp->m_Mp.lf_nr,
				ptrMp->m_Mp.anz_nmp);
					
			MSGTRACE (strMessage);


			if (ptrMp->m_Mp.typ & 0x02)
			{
				strMessage.Format (_T("--> Destino: %d Cust Name: %s"), 
					ptrMp->m_ptrStat->zieltyp_l,
					ptrMp->m_strCustName);

				MSGTRACE (strMessage);

			}

			for (int j = 0; j < ptrMp->m_NmpArray.GetSize (); j++)
			{
				CNMeldepunkt * ptrNMp = ptrMp->m_NmpArray.GetAt (j);
				strMessage.Format (_T("NMP : %d blk_weg: %d str_len: %d nep: %d"), 
					ptrNMp->m_NMp.nmp,
					ptrNMp->m_NMp.blk_weg,
					ptrNMp->m_NMp.str_len,
					ptrNMp->m_NMp.nep);
				MSGTRACE (strMessage);

			}



		}
		} catch (CFileException* ex ) {
           ex->ReportError ();
		}
		catch (CArchiveException* exAr) {
			exAr->ReportError ();
		}

	}
	
}
