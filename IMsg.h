#pragma once

#include "ClientSocket.h"
class CClientSocket;

class IMsg
{
public:

	IMsg(void)
	{
	}

	virtual ~IMsg(void)
	{
	}

	virtual void Serialize(CArchive& ar)
	{
	}

	virtual void SetClientSocket (CClientSocket * pSocket) {
	}
};
