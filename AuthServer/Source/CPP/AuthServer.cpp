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
	/*sDB->setInfos("root", "", "tcp://127.0.0.1:3306", "dragonball");
	if (sDB->connect() == false)
	{
		sLog->outError("Connecting to database failed...");
		system("PAUSE");
		return false;
	}
	sDB->switchDb("dragonball");
	sDB->prepare("SELECT * FROM accounts");
	sDB->execute();
	while (sDB->fetch())
	{
		sLog->outDetail("Database account contain: %s", sDB->getString("UserName"));
	}
	sLog->outDetail("Database connection established.");*/

	network = new Listener<AuthSocket>(port, worker);
	sLog->outString("AuthServer: Listener started, awaiting for connection...");
	return true;
}