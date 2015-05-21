#include "stdafx.h"
#include "tpc.h"
#include "ClientSocket.h"
#include "WlanMsg.h"
#include "Agv.h"
#include "AGVSockThread.h"
#include "AgvSockType.h"

CAgvSockType::CAgvSockType(void)
: m_pAgvSockThread(NULL)
{
}

CAgvSockType::CAgvSockType(CAGVSockThread * ptrThread, int idAgv)
{
	m_pAgvSockThread = ptrThread;
	m_id = idAgv;
}




CAgvSockType::~CAgvSockType(void)
{
}
