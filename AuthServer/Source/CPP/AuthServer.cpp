#include <iostream>
#include <AuthServer.h>
#include <mysqlconn_wrapper.h>
#include <XmlParser.h>

AuthServer::AuthServer(int _port, int _workerThread) : port(_port), worker(_workerThread)
{
	network = NULL;
}
AuthServer::~AuthServer()
{
	if (network != nullptr)
		delete network;
}
bool AuthServer::ConnectToDatabase()
{
	std::string user, password, host, db;

	user = sXmlParser->GetStr("MySQL", "User");
	password = sXmlParser->GetStr("MySQL", "Password");
	host = sXmlParser->GetStr("MySQL", "Host");
	db = sXmlParser->GetStr("MySQL", "Database");

	sDB->setInfos(user, password, host, db);
	if (sDB->connect() == false)
	{
		sLog->outError("Connecting to database failed...");
		system("PAUSE");
		return false;
	}
	sDB->switchDb(db);
	sDB->prepare("SELECT * FROM account");
	sDB->execute();
	while (sDB->fetch())
	{
		sLog->outDetail("Database: account [%d]", sDB->rowsCount());
	}
	sLog->outDetail("Database connection established.");
}
bool AuthServer::Start()
{
	if (sXmlParser->loadFile("AuthServer") == false)
		return false;
	if (ConnectToDatabase() == false)
		return false;

	network = new Listener<AuthSocket>(sXmlParser->GetInt("Server", "Port"), worker);
	sLog->outString("AuthServer: Listener started, awaiting for connection...");
	return true;
}