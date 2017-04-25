#include <iostream>
#include <GameServer.h>
#include <mysqlconn_wrapper.h>
#include <XmlParser.h>

void signalHandler(int signum)
{
	sLog.outDebug("Interrupt signal (%d) received.", signum);
	switch (signum)
	{
		case SIGINT:
		{
			//World::StopNow(SHUTDOWN_EXIT_CODE);
			break;
		}
		case SIGTERM:
		{
			//World::StopNow(SHUTDOWN_EXIT_CODE);
			break;
		}
		case SIGBREAK:
		{
			//World::StopNow(SHUTDOWN_EXIT_CODE);
			break;
		}
	}
}
bool GameServer::loadDataTable()
{
	return true;
}
bool GameServer::ConnectToDatabase()
{
	std::string user, password, host, db;

	user = sXmlParser.GetStr("MySQL", "User");
	password = sXmlParser.GetStr("MySQL", "Password");
	host = sXmlParser.GetStr("MySQL", "Host");
	db = sXmlParser.GetStr("MySQL", "Database");

	sDB.setInfos(user, password, host, db);
	if (sDB.connect() == false)
		return false;
	sDB.switchDb(db);
	sLog.outDetail("Database connection established.");
	return true;
}
int GameServer::Run()
{
	signal(SIGINT, signalHandler);
	signal(SIGABRT, signalHandler);
	signal(SIGFPE, signalHandler);
	signal(SIGILL, signalHandler);
	signal(SIGSEGV, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGBREAK, signalHandler);

	std::cout << blue << "\n\n" << std::endl;
	std::cout << "\t     ____                              ____        _ _ " << std::endl;
	std::cout << "\t    |  _ \\ _ __ __ _  __ _  ___  _ __ | __ )  __ _| | |" << std::endl;
	std::cout << "\t    | | | | '__/ _` |/ _` |/ _ \\| '_ \\|  _ \\ / _` | | |" << std::endl;
	std::cout << "\t    | |_| | | | (_| | (_| | (_) | | | | |_) | (_| | | |" << std::endl;
	std::cout << "\t    |____/|_|  \\__,_|\\__, |\\___/|_| |_|____/ \\__,_|_|_|" << std::endl;
	std::cout << "\t                     |___/                             " << std::endl;
	std::cout << yellow << "\t   	           AKCore 2017					\n\n" << white << std::endl;
	sLog.outString("Using configuration file 'GameServer.xml'.");
	sLog.outString("Using Boost: %s", BOOST_LIB_VERSION);

	if (sXmlParser.loadFile("GameServer") == false)
		return 1;
	sLog.SetLogLevel((LogLevel)sXmlParser.GetInt("LogLevel", "Value"));
	if (loadDataTable() == false)
	{
		sLog.outError("Table data unsucessfully loaded, exiting...");
		system("PAUSE");
		return 1;
	}
	if (ConnectToDatabase() == false)
	{
		sLog.outError("Database connection failed, exiting...");
		return 1;
	}

	{
		//network = new Listener<GameSocket>(sXmlParser.GetInt("Server", "Port"), worker);
		sLog.outString("GameServer: Listener started on port: [%d]", sXmlParser.GetInt("Server", "Port"));
		//while (!World::IsStopped())
		//{
			//std::this_thread::sleep_for(std::chrono::seconds(1));
		//}
	}
	sLog.outString("Halting process...");
	return 0;
}