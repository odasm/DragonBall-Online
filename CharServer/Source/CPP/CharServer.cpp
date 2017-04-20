#include <iostream>
#include <CharServer.h>
#include <mysqlconn_wrapper.h>

CharServer::CharServer(int _port, int _workerThread) : port(_port), worker(_workerThread)
{
	network = NULL;
}
CharServer::~CharServer()
{
	if (network != nullptr)
		delete network;
}
bool CharServer::Start()
{
	network = new Listener<CharSocket>(port, worker);
	sLog->outString("CharServer: Listener started, awaiting for connection...");
	return true;
}