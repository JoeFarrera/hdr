#pragma once

class CClientSocket;

class ISocketSink
{
public:

	ISocketSink(void)
	{
	}

	virtual ~ISocketSink(void)
	{
	}
	virtual void ProcessPendingRead (CClientSocket *pClientSocket)
	{
	}

	virtual void CloseSocket(CClientSocket *pClientSocket)
	{
	}
};
