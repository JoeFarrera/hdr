// msg.cpp : implementation of the CMsg class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "tpc.h"
#include "konstant.h"
#include "komdat.h"
#include "zs_bed.h"


#include "msg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsg

IMPLEMENT_DYNCREATE(CMsg, CObject)

/////////////////////////////////////////////////////////////////////////////
// CMsg construction/destruction

CMsg::CMsg()
{
	Init();
}

CMsg::~CMsg()
{

}

/////////////////////////////////////////////////////////////////////////////
// CMsg Operations

void CMsg::Init()
{
	m_bClose = FALSE;
	m_strText = _T("");
	m_msgList.RemoveAll();
	m_ptrCliSock = NULL;
}


void CMsg::SetClientSocket(CClientSocket * pSocket) {
	m_ptrCliSock = pSocket;
}
/////////////////////////////////////////////////////////////////////////////
// CMsg serialization

void CMsg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// Telegrammlaenge aus Sicherheitsgründen begrenzen
		if (m_Tele.nTeleLaenge > sizeof(m_Tele.nData))
			m_Tele.nTeleLaenge = sizeof(m_Tele.nData);


		ar << m_Tele.nTeleLaenge;
		ar << m_Tele.nRetStatus;
		ar << m_Tele.nOnOff;
		ar << m_Tele.nPcNr;
		ar.Write(m_Tele.nData,sizeof(m_Tele.nData));

		// Michael 19.02.2015 Don't seem to require this
		LPTSTR p = m_strText.GetBuffer(m_Tele.nTeleLaenge + 3);
		for (unsigned int i= 0; i<m_Tele.nTeleLaenge; i++)
		{
			if (m_Tele.nData[i] <= 9)
				p[i] = m_Tele.nData[i] +0x30;
			else p[i] = m_Tele.nData[i];
		}

		p[m_Tele.nTeleLaenge] = 0;
		m_strText.ReleaseBuffer();
		// MSGTRACE1 ("CMsg >> %s", m_strText);
		//m_msgList.AddTail(m_strText);

	}
	else
	{
		// M2015 Michael 05.05.2015
		//ar >> m_Tele.nTeleLaenge;
		//ar >> m_Tele.nRetStatus;
		//ar >> m_Tele.nOnOff;
		//ar >> m_Tele.nPcNr;
		//ar.Read(m_Tele.nData,sizeof(m_Tele.nData));
		ar.Read(&m_Tele,sizeof(m_Tele));

		// Telegrammlaenge aus Sicherheitsgründen begrenzen
		if (m_Tele.nTeleLaenge > sizeof(m_Tele.nData))
			m_Tele.nTeleLaenge = sizeof(m_Tele.nData);

		LPTSTR p = m_strText.GetBuffer(m_Tele.nTeleLaenge+3);

		for (unsigned int i= 0; i<m_Tele.nTeleLaenge; i++)
		{
			if (m_Tele.nData[i] <= 0x09)
				p[i] = m_Tele.nData[i] +0x30;
			else p[i] = m_Tele.nData[i];

		}


		p[m_Tele.nTeleLaenge] = 0;
		m_strText.ReleaseBuffer();
		m_msgList.AddTail(m_strText);

		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMsg diagnostics

#ifdef _DEBUG
void CMsg::AssertValid() const
{
	CObject::AssertValid();
}

void CMsg::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG


