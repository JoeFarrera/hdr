// msg.h : interface of the CMsg class
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

#ifndef __MSG_H
#define __MSG_H

#include "tele.h"
#include "IMsg.h"

class CClientSocket;
class IMsg;

class CMsg : public CObject, IMsg
{
protected:
	void Init();
	DECLARE_DYNCREATE(CMsg);
public:
	CTele m_Tele;
	CClientSocket * m_ptrCliSock;
	CMsg();

// Attributes
public:
	CString m_strText;
	BOOL m_bClose;
	CStringList m_msgList;

// Operations
public:

// Implementation
public:
	virtual ~CMsg();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	void SetClientSocket(CClientSocket * pSocket);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
#endif // __MSG_H