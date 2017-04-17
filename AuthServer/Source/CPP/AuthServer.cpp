#include <iostream>
#include <AuthServer.h>
#include <mysqlconn_wrapper.h>

AuthServer::AuthServer(int _port, int _workerThread) : port(_port), worker(_workerThread)
{
	network = NULL;
}
AuthServer::~AuthServer()
{
	if (network != nullptr)
		delete network;
}
bool AuthServer::Start()
{
	MySQLConnWrapper* con = new MySQLConnWrapper();
	con->setInfos("root", "", "tcp://127.0.0.1:3306", "world");
	if (con->connect() == false)
	{
		sLog->outError("Connecting to database failed...");
		system("PAUSE");
		delete con;
		return false;
	}
	sLog->outDetail("Database connection established.");

	network = new Listener<AuthSocket>(port, worker);
	sLog->outString("AuthServer: Listener started, awaiting for connection...");
	delete con;
	return true;
}