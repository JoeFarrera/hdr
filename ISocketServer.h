#pragma once

class CClientSocket;

class ISocketServer
{
public:

	ISocketServer(void)
	{
	}
	
	virtual void ProcessPendingAccept (CSocket * pServerSocket)
	{
	}


	virtual ~ISocketServer(void)
	{
	}
};
