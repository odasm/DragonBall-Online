#include <CharSocket.h>
#include <Logger.h>
#include <stdio.h>
#include <cstdio>
#include <Opcodes.h>
#include <Encryptation.h>
#include <mysqlconn_wrapper.h>
#include <XmlParser.h>
#include <Packet\Char\PacketUC.h>
#include <Packet\Char\PacketCU.h>

static wchar_t* charToWChar(const char* text)
{
	size_t conv, size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(&conv, wa, size, text, size);
	return wa;
}

bool CharSocket::GetLoginRequest(Packet &packet)
{
	sUC_LOGIN_REQ* req = (sUC_LOGIN_REQ*)packet.GetPacketData();

	sLog->outDebug("ACC ID: %i Server: %i auth key: %s", req->accountId, req->serverID, req->abyAuthKey);

	sCU_LOGIN_RES res;
	memset(&res, 0, sizeof(sCU_LOGIN_RES));

	res.bEncrypt = 0;
	res.wPacketSize = sizeof(sCU_LOGIN_RES) - 2;
	res.wOpCode = Opcodes::CU_LOGIN_RES;
	//res.byChecksum = sizeof(sCU_LOGIN_RES);

	res.dwRaceAllowedFlag = 0xFF;
	res.lastServerFarmId = req->serverID;
	res.wResultCode = ResultCodes::CHARACTER_SUCCESS;
	res.LastChannelID = 255;

	Write((char*)&res, sizeof(sCU_LOGIN_RES));
	res = {};
	return true;
}
bool CharSocket::GetCharacterServerList(Packet &packet, bool one)
{
	for (int i = 0; i < 0; i++)
	{
		sLog->outDebug("need to read the futur config file and get the all server list from");
	}
	sCU_SERVER_FARM_INFO sinfo;
	memset(&sinfo, 0, sizeof(sCU_SERVER_FARM_INFO));

	sinfo.bEncrypt = 0;
	sinfo.wPacketSize = sizeof(sCU_SERVER_FARM_INFO) - 2;
	sinfo.wOpCode = CU_SERVER_FARM_INFO;

	sLog->outDebug("Loading %d server", sXmlParser->GetInt("ServerCount", "value"));

	for (int i = 0; i < sXmlParser->GetInt("ServerCount", "value"); i++)
	{
		char snode[20];
		sprintf_s(snode, "Server%d", i + 1);
		sinfo.serverFarmInfo.serverFarmId = i;
		
		memcpy(sinfo.serverFarmInfo.wszGameServerFarmName, charToWChar(sXmlParser->GetStr(snode, "Name")), 32);
		sinfo.serverFarmInfo.byServerStatus = DBO_SERVER_STATUS_UP;
		sinfo.serverFarmInfo.dwLoad = 0;
		sinfo.serverFarmInfo.dwMaxLoad = 100;
		Write((char*)&sinfo, sizeof(sCU_SERVER_FARM_INFO));
	}

	if (one)
	{
		sCU_CHARACTER_SERVERLIST_ONE_RES slone;
		memset(&slone, 0, sizeof(sCU_CHARACTER_SERVERLIST_ONE_RES));

		slone.bEncrypt = 0;
		slone.wPacketSize = sizeof(sCU_CHARACTER_SERVERLIST_ONE_RES) - 2;
		slone.wOpCode = CU_CHARACTER_SERVERLIST_ONE_RES;
		slone.wResultCode = CHARACTER_SUCCESS;

		Write((char*)&slone, sizeof(sCU_CHARACTER_SERVERLIST_ONE_RES));
	}
	else
	{
		sCU_CHARACTER_SERVERLIST_RES slres;
		memset(&slres, 0, sizeof(slres));

		slres.bEncrypt = 0;
		slres.wPacketSize = sizeof(sCU_CHARACTER_SERVERLIST_RES) - 2;
		slres.wOpCode = CU_CHARACTER_SERVERLIST_RES;
		slres.wResultCode = CHARACTER_SUCCESS;

		Write((char*)&slres, sizeof(sCU_CHARACTER_SERVERLIST_RES));
	}
	return true;
}
bool CharSocket::GetCharacterLoad(Packet &packet)
{
	sUC_CHARACTER_LOAD_REQ* req = (sUC_CHARACTER_LOAD_REQ*)packet.GetPacketData();
	//DBUpdateLastServer(); // update server farm etc
	sCU_SERVER_CHANNEL_INFO cninfo;
	memset(&cninfo, 0, sizeof(sCU_SERVER_CHANNEL_INFO));

	cninfo.bEncrypt = 0;
	cninfo.wPacketSize = sizeof(sCU_SERVER_CHANNEL_INFO) - 2;
	cninfo.wOpCode = CU_SERVER_CHANNEL_INFO;

	char snode[20];
	sprintf_s(snode, "Server%d", req->serverFarmId + 1);
	cninfo.byCount = sXmlParser->GetInt(snode, "Count");
	for (int i = 0; i < cninfo.byCount; ++i)
	{
		// check if developper for dev server status from xml and database
		cninfo.serverChannelInfo[i].bIsVisible = true;
		cninfo.serverChannelInfo[i].byServerChannelIndex = i;
		cninfo.serverChannelInfo[i].byServerStatus = eDBO_SERVER_STATUS::DBO_SERVER_STATUS_UP;
		cninfo.serverChannelInfo[i].dwLoad = 0;
		cninfo.serverChannelInfo[i].dwMaxLoad = 100;
		cninfo.serverChannelInfo[i].serverFarmId = req->serverFarmId;
		//cninfo.serverChannelInfo[i].bScramble = 0;
	}
	Write((char*)&cninfo, sizeof(sCU_SERVER_CHANNEL_INFO));

	sCU_CHARACTER_INFO cinfo;
	memset(&cinfo, 0, sizeof(sCU_CHARACTER_INFO));

	cinfo.bEncrypt = 0;
	cinfo.wPacketSize = sizeof(sCU_CHARACTER_INFO) - 2;
	for (int i = 0; i < 8; i++)
	{
		cinfo.asDelData[i].charId = 0xFFFFFFFF;
		cinfo.asDelData[i].dwPastTick = 0xFFFFFFFF;
	}
	cinfo.byCount = 0;//GetDBAccCharListData(&cinfo);
	cinfo.wOpCode = CU_CHARACTER_INFO;

	Write((char*)&cinfo, sizeof(sCU_CHARACTER_INFO));

	sCU_CHARACTER_LOAD_RES clres;
	memset(&clres, 0, sizeof(sCU_CHARACTER_LOAD_RES));

	clres.bEncrypt = 0;
	clres.wPacketSize = sizeof(sCU_CHARACTER_LOAD_RES) - 2;
	clres.wOpCode = CU_CHARACTER_LOAD_RES;
	clres.wResultCode = CHARACTER_SUCCESS;
	clres.ServerID = req->serverFarmId;
	clres.OpenCharSlots = 8;
	clres.VipCharSlot = 0;

	Write((char*)&clres, sizeof(sCU_CHARACTER_LOAD_RES));
	return true;
}
bool CharSocket::_ProcessCharPacket(Packet& packet, WORD wOpCode)
{
	switch (wOpCode)
	{
		case UC_LOGIN_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_LOGIN_REQ ~~~~~~~~~");
			return GetLoginRequest(packet);
		}
		case UC_CHARACTER_SERVERLIST_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CHARACTER_SERVERLIST_REQ ~~~~~~~~~");
			return GetCharacterServerList(packet, false);
		}
		case UC_CHARACTER_SERVERLIST_ONE_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CHARACTER_SERVERLIST_ONE_REQ ~~~~~~~~~");
			return GetCharacterServerList(packet, true);
		}
		case UC_CHARACTER_ADD_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CHARACTER_ADD_REQ ~~~~~~~~~");
			sUC_CHARACTER_ADD_REQ* data = (sUC_CHARACTER_ADD_REQ*)packet.GetPacketData();
			sCU_CHARACTER_ADD_RES res;
			memset(&res, 0, sizeof(res));
			res.bEncrypt = 0;
			res.wOpCode = CU_CHARACTER_ADD_RES;
			res.wPacketSize = sizeof(sCU_CHARACTER_ADD_RES) - 2;
			res.wResultCode = 200;
			Write((char*)&res, sizeof(sCU_CHARACTER_ADD_RES));
			return true;
		}
		case UC_CHARACTER_DEL_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CHARACTER_DEL_REQ ~~~~~~~~~");
			break;
		}
		case UC_CHARACTER_SELECT_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CHARACTER_SELECT_REQ ~~~~~~~~~");
			break;
		}
		case UC_CHARACTER_EXIT_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CHARACTER_EXIT_REQ ~~~~~~~~~");
			break;
		}
		case UC_CHARACTER_LOAD_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CHARACTER_LOAD_REQ ~~~~~~~~~");
			return GetCharacterLoad(packet);
		}
		case UC_CHARACTER_DEL_CANCEL_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CHARACTER_DEL_CANCEL_REQ ~~~~~~~~~");
			break;
		}
		case UC_CONNECT_WAIT_CHECK_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CONNECT_WAIT_CHECK_REQ ~~~~~~~~~");
			break;
		}
		case UC_CONNECT_WAIT_CANCEL_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CONNECT_WAIT_CANCEL_REQ ~~~~~~~~~");
			break;
		}
		case UC_CHARACTER_RENAME_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CHARACTER_RENAME_REQ ~~~~~~~~~");
			break;
		}
		case UC_CASHITEM_HLSHOP_REFRESH_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CASHITEM_HLSHOP_REFRESH_REQ ~~~~~~~~~");
			return true;
			break;
		}
		case UC_CASHITEM_BUY_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CASHITEM_BUY_REQ ~~~~~~~~~");
			break;
		}
		case UC_CHAR_SERVERLIST_REQ:
		{
			sLog->outDebug("~~~~~~~~~ UC_CHAR_SERVERLIST_REQ ~~~~~~~~~");
			break;
		}
		default:
		{
			sLog->outError("Packet_[%u] Unknow", wOpCode);
			return false;
		}
	}
	return false;
}	