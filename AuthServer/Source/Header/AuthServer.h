#pragma once

#include <Listener.h>
#include <AuthSocket.h>
#include <Logger.h>

class AuthServer
{
public:
	AuthServer(int _port = 50200, int _workerThread = 1);
	~AuthServer();
	bool ConnectToDatabase();
	bool Start();
private:
	Listener<AuthSocket>	*network;
	int port;
	int worker;
};