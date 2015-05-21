// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__2636F2CD_B23B_41DC_8425_A42F5469AB54__INCLUDED_)
#define AFX_STDAFX_H__2636F2CD_B23B_41DC_8425_A42F5469AB54__INCLUDED_


#define WINVER 0x0502


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcview.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>		// Michael: MFC Template Support
#include <afxmt.h>			// Michael: MFC Critical Section
#include <afxsock.h>		// Michael: Sockets
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define WM_USER_MESSAGE		WM_USER+0x100
#define WM_USER_SIEMENS_SR	WM_USER+0x101
#define WM_USER_SIEMENS_SEND WM_USER+0x102
#define WM_USER_TESTUITHREAD WM_USER+0x103
#define WM_USER_GETESTADOPLC WM_USER+0x104	
#define WM_USER_MESSAGE_GLOBAL WM_USER+0x110
#define WM_USER_AGV_UPDATE_BUB WM_USER+0x120
#define WM_USER_SOCKET_READ WM_USER+0x130
#define WM_USER_SOCKET_WRITE WM_USER+0x140





typedef struct tagMESSAGEPARMS {
    UINT tipo;	// Tipo de mensaje (Gravedad)
    LPCSTR file;	// Fichero fuente del mensaje
	UINT linea;		// Línea dentro del fichero
	LPCSTR modulo;	// Módulo de la aplicación (no implementado)
	char strMensaje [500];	// Texto del mensaje
} MESSAGEPARMS;


#define GNU_PACKED

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__2636F2CD_B23B_41DC_8425_A42F5469AB54__INCLUDED_)
