#include <Define.h>
#include <Opcodes.h>

#pragma once

#pragma pack(1)
struct sDBO_GAME_SERVER_FARM_INFO
{
	sDBO_GAME_SERVER_FARM_INFO() :
		serverFarmId(INVALID_SERVERFARMID),
		byServerStatus(DBO_SERVER_STATUS_DOWN),
		dwMaxLoad(0),
		dwLoad(0)
	{
		::ZeroMemory(wszGameServerFarmName, sizeof(wszGameServerFarmName));
	};
	SERVERFARMID serverFarmId;
	WCHAR wszGameServerFarmName[32 + 1];
	BYTE byServerStatus;
	DWORD dwMaxLoad;
	DWORD dwLoad;
};
struct sDBO_GAME_SERVER_CHANNEL_INFO
{
	sDBO_GAME_SERVER_CHANNEL_INFO() :
		serverFarmId(INVALID_SERVERFARMID),
		byServerChannelIndex(INVALID_SLOTID),
		bIsVisible(true),
		byServerStatus(DBO_SERVER_STATUS_DOWN),
		dwMaxLoad(0),
		dwLoad(0)
	{
	};
	SERVERFARMID serverFarmId;
	BYTE byServerChannelIndex;
	bool bIsVisible;
	BYTE byServerStatus;
	DWORD dwMaxLoad;
	DWORD dwLoad;
	//BYTE bScramble;
};
struct sDELETE_WAIT_CHARACTER_INFO
{
	CHARACTERID		charId;
	DWORD			dwPastTick;
};
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_LOGIN_RES)
WORD			wResultCode;
SERVERFARMID	lastServerFarmId;
DWORD			dwRaceAllowedFlag;
BYTE			LastChannelID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_SERVERLIST_ONE_RES)
WORD wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_SERVERLIST_RES)
WORD wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_SERVER_FARM_INFO)
sDBO_GAME_SERVER_FARM_INFO		serverFarmInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_SERVER_CHANNEL_INFO)
BYTE	byCount;
sDBO_GAME_SERVER_CHANNEL_INFO		serverChannelInfo[10];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_INFO)
sDELETE_WAIT_CHARACTER_INFO asDelData[8];
BYTE			byCount;
//sPC_SUMMARY		sPcData[NTL_MAX_COUNT_USER_CHAR_SLOT]; // Maximum of Character is 8 (Temporary)	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_LOAD_RES)
WORD	wResultCode;
BYTE	ServerID;
BYTE	OpenCharSlots;
BYTE	VipCharSlot;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(CU_CHARACTER_ADD_RES)
WORD			wResultCode;
//sPC_SUMMARY		sPcDataSummary;
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()