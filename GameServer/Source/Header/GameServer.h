#pragma once

#include <Listener.h>
#include <Logger.h>
#include <GameSocket.h>
#include <Singleton.h>

class GameServer
{
public:
	int Run();
	
private:
	bool ConnectToDatabase();
	bool loadDataTable();
};
#define sGameServer AKCore::Singleton<GameServer>::Instance()