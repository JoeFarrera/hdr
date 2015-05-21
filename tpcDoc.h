// tpcDoc.h : interface of the CTpcDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TPCDOC_H__B8A177BA_E09C_4620_A9C5_80FD1791EF52__INCLUDED_)
#define AFX_TPCDOC_H__B8A177BA_E09C_4620_A9C5_80FD1791EF52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "konstant.h"
#include "komdat.h"
#include "TRAFFICMANAGER\DistanceReqWayTele.h"	// Added by ClassView
#include "ISocketSink.h"
#include "ISocketServer.h"


class CTCPServerSocket;
class CMsg;
class CClientSocket;
class ISocketServer;
class ISocketSink;

class CTpcDoc : public CDocument, ISocketServer, ISocketSink
{
protected: // create from serialization only
	CTpcDoc();
	DECLARE_DYNCREATE(CTpcDoc)

// Attributes
private:
	CDistanceReqWayTeleMap	m_mapDistancesReqWayTele;
	void * m_ptrContext;	// Contexto para Oracle


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpcDoc)
	public:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTpcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTpcDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPCDOC_H__B8A177BA_E09C_4620_A9C5_80FD1791EF52__INCLUDED_)
