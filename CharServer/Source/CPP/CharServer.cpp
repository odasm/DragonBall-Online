#include <iostream>
#include <CharServer.h>
#include <mysqlconn_wrapper.h>
#include <XmlParser.h>

CharServer::CharServer(int _port, int _workerThread) : port(_port), worker(_workerThread)
{
}
CharServer::~CharServer()
{
}
bool CharServer::loadDataTable()
{
	CNtlBitFlagManager flagManager;
	if (false == flagManager.Create(TableContainer::TABLE_COUNT))
		return false;
	TableFileNameList fileNameList;
	if (false == fileNameList.Create())
		return false;
	flagManager.Set(TableContainer::TABLE_WORLD);
	flagManager.Set(TableContainer::TABLE_PC);
	flagManager.Set(TableContainer::TABLE_MOB);
	flagManager.Set(TableContainer::TABLE_NPC);
	flagManager.Set(TableContainer::TABLE_ITEM);
	flagManager.Set(TableContainer::TABLE_ITEM_OPTION);
	flagManager.Set(TableContainer::TABLE_SKILL);
	flagManager.Set(TableContainer::TABLE_NEWBIE);
	flagManager.Set(TableContainer::TABLE_WORLD_MAP);
	flagManager.Set(TableContainer::TABLE_WORLD_ZONE);
	flagManager.Set(TableContainer::TABLE_FORMULA);
	flagManager.Set(TableContainer::TABLE_EXP);

	fileNameList.SetFileName(TableContainer::TABLE_WORLD, "table_world_data");
	fileNameList.SetFileName(TableContainer::TABLE_PC, "table_pc_data");
	fileNameList.SetFileName(TableContainer::TABLE_ITEM, "table_item_data");
	fileNameList.SetFileName(TableContainer::TABLE_ITEM_OPTION, "table_item_option_data");
	fileNameList.SetFileName(TableContainer::TABLE_SKILL, "table_skill_data");
	fileNameList.SetFileName(TableContainer::TABLE_NEWBIE, "table_newbie_data");
	fileNameList.SetFileName(TableContainer::TABLE_WORLD_MAP, "table_worldmap_data");
	fileNameList.SetFileName(TableContainer::TABLE_WORLD_ZONE, "table_world_zone_data");
	fileNameList.SetFileName(TableContainer::TABLE_FORMULA, "TD_Formula");
	fileNameList.SetFileName(TableContainer::TABLE_GAME_MANIA_TIME, "table_gamemaniatime_data");
	fileNameList.SetFileName(TableContainer::TABLE_EXP, "table_exp_data");

	std::string path = sXmlParser.GetStr("GameData", "Path");
	sTBM.Create(flagManager, (char*)path.c_str(), &fileNameList, eLOADING_METHOD::LOADING_METHOD_BINARY, GetACP(), NULL);

	sNEWBIE_TBLDAT *newbi = (sNEWBIE_TBLDAT*)sTBM.GetNewbieTable()->GetNewbieTbldat(0, 0);
	if (newbi == NULL)
	{
		sLog.outError("ERROR TABLE NULL");
		return false;
	}
	else
	{
		sLog.outDetail("TABLE Loaded !");
	}
	return true;
}
bool CharServer::ConnectToDatabase()
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
bool CharServer::Start()
{
	if (sXmlParser.loadFile("CharServer") == false)
		return false;
	sLog.SetLogLevel((LogLevel)sXmlParser.GetInt("LogLevel", "Value"));
	if (loadDataTable() == false)
	{
		sLog.outError("Table data unsucessfully loaded, exiting...");
		system("PAUSE");
		return false;
	}
	if (ConnectToDatabase() == false)
	{
		sLog.outError("Database connection failed, exiting...");
		return false;
	}
	Listener<CharSocket>(sXmlParser.GetInt("Server", "Port"), worker);
	sLog.outString("CharServer: Listener started on port: [%d]", sXmlParser.GetInt("Server", "Port"));

	return true;
}