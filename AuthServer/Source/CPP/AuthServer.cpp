#include <iostream>
#include <AuthServer.h>

AuthServer::AuthServer(int _port, int _workerThread) : port(_port), worker(_workerThread)
{
}
AuthServer::~AuthServer()
{
	delete network;
}
void AuthServer::Start()
{
	sLog->outDetail("Connecting to database...");

	network = new Listener<AuthSocket>(port, worker);
	sLog->outString("AuthServer: Listener started, awaiting for connection...");
}