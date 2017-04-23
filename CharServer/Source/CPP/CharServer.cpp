#include <iostream>
#include <CharServer.h>
#include <mysqlconn_wrapper.h>
#include <XmlParser.h>

CharServer::CharServer(int _port, int _workerThread) : port(_port), worker(_workerThread)
{
	network = NULL;
}
CharServer::~CharServer()
{
	if (network != nullptr)
		delete network;
}
bool CharServer::loadDataTable()
{
	return true;
}
bool CharServer::ConnectToDatabase()
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
	sDB->prepare("SELECT * FROM characters");
	sDB->execute();
	while (sDB->fetch())
	{
		sLog->outDetail("Database: characters [%d]", sDB->rowsCount());
	}
	sLog->outDetail("Database connection established.");
}
bool CharServer::Start()
{
	if (sXmlParser->loadFile("CharServer") == false)
		return false;
	if (loadDataTable() == false)
		return false;
	sLog->outDetail("Table data loaded: [%d]", 0);
	if (ConnectToDatabase() == false)
		return false;

	network = new Listener<CharSocket>(sXmlParser->GetInt("Server", "Port"), worker);
	sLog->outString("CharServer: Listener started, awaiting for connection...");
	return true;
}