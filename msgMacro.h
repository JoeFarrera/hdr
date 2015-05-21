// msgMacro.h: Messages...
#include "MainFrm.h"

#define MSGAVISO(f) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MAVISO; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T("%s"), f);}

#define MSGAVISO1(f, p1) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MAVISO; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1);}

#define MSGAVISO2(f, p1, p2) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MAVISO; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2);}

#define MSGAVISO3(f, p1, p2, p3) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MAVISO; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2, p3);}

#define MSGAVISO4(f, p1, p2, p3, p4) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MAVISO; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2, p3, p4);}

#define MSGTRACE(f) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MTRACE; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T("%s"), f);}

#define MSGTRACE1(f, p1) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MTRACE; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1);}

#define MSGTRACE2(f, p1, p2) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MTRACE; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2);}

#define MSGTRACE3(f, p1, p2, p3) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MTRACE; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2, p3);}

#define MSGTRACE4(f, p1, p2, p3, p4) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MTRACE; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2, p3, p4);}

#define MSGTRACE5(f, p1, p2, p3, p4, p5) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MTRACE; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2, p3, p4, p5);}

#define MSGWARNING(f) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MWARNING; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T("%s"), f);}

#define MSGWARNING1(f, p1) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MWARNING; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1);}

#define MSGWARNING2(f, p1, p2) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MWARNING; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2);}

#define MSGWARNING3(f, p1, p2, p3) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MWARNING; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2, p3);}

#define MSGERROR(f) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MERROR; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T("%s"), f);}

#define MSGERROR1(f, p1) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MERROR; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1);}

#define MSGERROR2(f, p1, p2) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MERROR; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2);}

#define MSGERROR3(f, p1, p2, p3) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MERROR; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2, p3);}

#define MSGERROR4(f, p1, p2, p3, p4) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MERROR; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2, p3, p4);}

#define MSGFATAL(f) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MFATAL; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T("%s"), f);}

#define MSGFATAL1(f, p1) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MFATAL; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1);}

#define MSGFATAL2(f, p1, p2) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MFATAL; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2);}

#define MSGFATAL3(f, p1, p2, p3) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MFATAL; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2, p3);}

#define MSGFATAL4(f, p1, p2, p3, p4) { MESSAGEPARMS * pMsg = new MESSAGEPARMS; pMsg->linea = __LINE__;\
		pMsg->tipo = MFATAL; pMsg->file = __FILE__; pMsg->modulo = "TPC"; \
		CMainFrame::Mensaje (pMsg, _T(f), p1, p2, p3, p4);}

