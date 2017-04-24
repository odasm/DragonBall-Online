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

bool CharSocket::GetLoginRequest(Packet &packet)
{
	sUC_LOGIN_REQ* req = (sUC_LOGIN_REQ*)packet.GetPacketData();
	sCU_LOGIN_RES res;
	memset(&res, 0, sizeof(sCU_LOGIN_RES));

	res.bEncrypt = 0;
	res.wPacketSize = sizeof(sCU_LOGIN_RES) - 2;
	res.wOpCode = Opcodes::CU_LOGIN_RES;
	//res.byChecksum = sizeof(sCU_LOGIN_RES);

	res.dwRaceAllowedFlag = 255;
	res.lastServerFarmId = req->serverID;
	res.wResultCode = ResultCodes::CHARACTER_SUCCESS;
	res.LastChannelID = 255;
	
	// storing data
	AccountID = req->accountId;
	ServerID = req->serverID;

	Write((char*)&res, sizeof(sCU_LOGIN_RES));
	return true;
}
bool CharSocket::GetCharacterServerList(Packet &packet, bool one)
{
	sCU_SERVER_FARM_INFO sinfo;
	memset(&sinfo, 0, sizeof(sCU_SERVER_FARM_INFO));

	sinfo.bEncrypt = 0;
	sinfo.wPacketSize = sizeof(sCU_SERVER_FARM_INFO) - 2;
	sinfo.wOpCode = CU_SERVER_FARM_INFO;
	for (int i = 0; i < sXmlParser->GetInt("ServerCount", "value"); i++)
	{
		char snode[20];
		sprintf_s(snode, "Server%d", i + 1);
		sinfo.serverFarmInfo.serverFarmId = i;
		
		memcpy(sinfo.serverFarmInfo.wszGameServerFarmName, charToWChar(sXmlParser->GetStr(snode, "Name")), MAX_SIZE_SERVER_FARM_NAME_UNICODE);
		
		sinfo.serverFarmInfo.dwLoad = 0;
		sinfo.serverFarmInfo.dwMaxLoad = 100;

		if (sXmlParser->GetInt(snode, "Private") == 1)
		{
			if (sDB.GetIsGameMaster(AccountID) != 1)
			{
				sinfo.serverFarmInfo.byServerStatus = DBO_SERVER_STATUS_LOCKED;
			}
			else
			{
				sinfo.serverFarmInfo.byServerStatus = DBO_SERVER_STATUS_UP;
			}
		}
		else
		{
			sinfo.serverFarmInfo.byServerStatus = DBO_SERVER_STATUS_UP;
		}

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
void CharSocket::SendCharacterList()
{
	sCU_CHARACTER_INFO cinfo;
	memset(&cinfo, 0, sizeof(sCU_CHARACTER_INFO));

	cinfo.bEncrypt = 0;
	cinfo.wPacketSize = sizeof(sCU_CHARACTER_INFO) - 2;
	sDB.GetDBAccCharListData(&cinfo, AccountID, ServerID);
	cinfo.wOpCode = CU_CHARACTER_INFO;

	sDB.prepare("SELECT * FROM characters WHERE AccountID = ? AND ServerID = ?");
	sDB.setInt(1, AccountID);
	sDB.setInt(2, ServerID);
	sDB.execute();
	for (int i = 0; i < cinfo.byCount; i++)
	{
		sDB.fetch();
		wcscpy_s(cinfo.sPcData[i].awchName, MAX_SIZE_CHAR_NAME_UNICODE, charToWChar(sDB.getString("Name").c_str()));
		cinfo.sPcData[i].charId = sDB.getInt("CharacterID");
		cinfo.sPcData[i].byRace = sDB.getInt("RaceID");
		cinfo.sPcData[i].byClass = sDB.getInt("ClassID");
		cinfo.sPcData[i].bIsAdult = sDB.getBoolean("IsAdult");
		cinfo.sPcData[i].byGender = sDB.getInt("GenderID");
		cinfo.sPcData[i].byFace = sDB.getInt("FaceID");
		cinfo.sPcData[i].byHair = sDB.getInt("HairID");
		cinfo.sPcData[i].byHairColor = sDB.getInt("HairColorID");
		cinfo.sPcData[i].bySkinColor = sDB.getInt("SkinColorID");
		cinfo.sPcData[i].byLevel = sDB.getInt("CurrentLevel");
		cinfo.sPcData[i].bTutorialFlag = sDB.getInt("TutorialFlag");
		cinfo.sPcData[i].bNeedNameChange = sDB.getBoolean("NameChange");
		cinfo.sPcData[i].dwMoney = sDB.getInt("ZennyInventory");
		cinfo.sPcData[i].dwMoneyBank = sDB.getInt("ZennyBank");
		cinfo.sPcData[i].worldId = sDB.getInt("WorldID");
		cinfo.sPcData[i].worldTblidx = sDB.getInt("WorldTableID");
		cinfo.sPcData[i].dwMapInfoIndex = sDB.getInt("MapInfoID");
		cinfo.asDelData[i].charId = INVALID_CHARACTERID;
		sLog.outDebug("Getting information for characters: %d", cinfo.sPcData[i].charId);
	}
	for (int i = 0; i < cinfo.byCount; i++)
	{
		for (int x = 0; x < EQUIP_SLOT_TYPE_COUNT; x++)
		{
			sDB.prepare("select * from items WHERE place=7 AND pos=? AND owner_id=?");
			sDB.setInt(1, x);
			sDB.setInt(2, cinfo.sPcData[i].charId);
			sDB.execute();
			sDB.fetch();
			if (sDB.rowsCount() == 0)
			{
				cinfo.sPcData[i].sItem[x].tblidx = INVALID_TBLIDX;
			}
			else
			{
				cinfo.sPcData[i].sItem[x].tblidx = sDB.getInt("tblidx");
				cinfo.sPcData[i].sItem[x].byBattleAttribute = 0;
				cinfo.sPcData[i].sItem[x].byGrade = 0;
				cinfo.sPcData[i].sItem[x].byRank = 0;
			}
		}
	}
	Write((char*)&cinfo, sizeof(sCU_CHARACTER_INFO));
}
bool CharSocket::GetCharacterLoad(Packet &packet)
{
	sUC_CHARACTER_LOAD_REQ* req = (sUC_CHARACTER_LOAD_REQ*)packet.GetPacketData();
	sDB.DBUpdateLastServer(req->accountId, req->serverFarmId);
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
		cninfo.serverChannelInfo[i].bIsVisible = true;
		cninfo.serverChannelInfo[i].byServerStatus = eDBO_SERVER_STATUS::DBO_SERVER_STATUS_UP;
		cninfo.serverChannelInfo[i].byServerChannelIndex = i;
		cninfo.serverChannelInfo[i].dwLoad = 0;
		cninfo.serverChannelInfo[i].dwMaxLoad = 100;
		cninfo.serverChannelInfo[i].serverFarmId = req->serverFarmId;
		cninfo.serverChannelInfo[i].is_pvpdb = false;
	}
	Write((char*)&cninfo, sizeof(sCU_SERVER_CHANNEL_INFO));

	SendCharacterList(); // SEND THE CURRENT CHARACTER LIST

	sCU_CHARACTER_LOAD_RES clres;
	memset(&clres, 0, sizeof(sCU_CHARACTER_LOAD_RES));

	clres.bEncrypt = 0;
	clres.wPacketSize = sizeof(sCU_CHARACTER_LOAD_RES) - 2;
	clres.wOpCode = CU_CHARACTER_LOAD_RES;
	clres.wResultCode = CHARACTER_SUCCESS;
	clres.ServerID = req->serverFarmId;
	clres.OpenCharSlots = MAX_COUNT_USER_CHAR_SLOT;
	clres.VipCharSlot = 0;

	Write((char*)&clres, sizeof(sCU_CHARACTER_LOAD_RES));
	return true;
}
bool CharSocket::GetCreateCharactersRequest(Packet &packet)
{
	sUC_CHARACTER_ADD_REQ* data = (sUC_CHARACTER_ADD_REQ*)packet.GetPacketData();
	sCU_CHARACTER_ADD_RES res;
	memset(&res, 0, sizeof(res));

	wcstombs(username, data->awchCharName, MAX_SIZE_USERID_UNICODE + 1);
	res.wResultCode = sDB.checkUsedName(username);
	res.bEncrypt = 0;
	res.wOpCode = CU_CHARACTER_ADD_RES;
	res.wPacketSize = sizeof(sCU_CHARACTER_ADD_RES) - 2;

	if (res.wResultCode == CHARACTER_SUCCESS)
	{
		wcscpy_s(res.sPcDataSummary.awchName, MAX_SIZE_CHAR_NAME_UNICODE, charToWChar(username));
		res.sPcDataSummary.byRace = data->byRace;
		res.sPcDataSummary.byClass = data->byClass;
		res.sPcDataSummary.bIsAdult = false;
		res.sPcDataSummary.byGender = data->byGender;
		res.sPcDataSummary.byFace = data->byFace;
		res.sPcDataSummary.byHair = data->byHair;
		res.sPcDataSummary.byHairColor = data->byHairColor;
		res.sPcDataSummary.bySkinColor = data->bySkinColor;
		res.sPcDataSummary.byLevel = 1;
		res.sPcDataSummary.dwMoney = 0;
		res.sPcDataSummary.dwMoneyBank = 0;
		res.sPcDataSummary.bTutorialFlag = true;
		res.sPcDataSummary.bNeedNameChange = false;

		res.unknown = 1;

		res.sPcDataSummary.sDogi.byDojoColor = 255;
		res.sPcDataSummary.sDogi.byGuildColor = 255;
		res.sPcDataSummary.sDogi.byType = 255;
		res.sPcDataSummary.sDogi.guildId = INVALID_TBLIDX;
		res.sPcDataSummary.sMarking.dwCode = 255;
		res.sPcDataSummary.dwMapInfoIndex = INVALID_TBLIDX;
		res.sPcDataSummary.worldId = 1;
		res.sPcDataSummary.worldTblidx = INVALID_TBLIDX;
		res.sPcDataSummary.fPositionX = 0;
		res.sPcDataSummary.fPositionY = 0;
		res.sPcDataSummary.fPositionZ = 0;

		res.sPcDataSummary.wUnknow1 = 65535;
		for (int x = 0; x < 6; x++)
		{
			res.sPcDataSummary.abyUnknow2[x] = 255;
		}

		res.sPcDataSummary.charId = AddCharacters(res.sPcDataSummary);
		if (res.sPcDataSummary.charId == -1)
			res.wResultCode = CHARACTER_FAIL;
		else
		{
			for (int x = 0; x < EQUIP_SLOT_TYPE_COUNT; x++)
			{
				sDB.prepare("select * from items WHERE place=7 AND pos=? AND owner_id=?");
				sDB.setInt(1, x);
				sDB.setInt(2, res.sPcDataSummary.charId);
				sDB.execute();
				sDB.fetch();
				if (sDB.rowsCount() == 0)
				{
					res.sPcDataSummary.sItem[x].tblidx = INVALID_TBLIDX;
				}
				else
				{
					res.sPcDataSummary.sItem[x].tblidx = sDB.getInt("tblidx");
					res.sPcDataSummary.sItem[x].byBattleAttribute = 0;
					res.sPcDataSummary.sItem[x].byGrade = 0;
					res.sPcDataSummary.sItem[x].byRank = 0;
				}
			}
		}
	}
	Write((char*)&res, sizeof(sCU_CHARACTER_ADD_RES));
	SendCharacterList(); // FORCE REFRESHING ???
	return true;
}
bool CharSocket::GetCharactersDeleteRequest(Packet &packet)
{
	sUC_CHARACTER_DEL_REQ *req = (sUC_CHARACTER_DEL_REQ*)packet.GetPacketData();
	sCU_CHARACTER_DEL_RES res;
	memset(&res, 0, sizeof(sCU_CHARACTER_DEL_RES));

	res.bEncrypt = 0;
	res.wOpCode = CU_CHARACTER_DEL_RES;
	res.wPacketSize = sizeof(sCU_CHARACTER_DEL_RES) - 2;
	
	res.charId = req->charId;
	res.wResultCode = sDB.DeleteCharacter(AccountID, req->charId);

	Write((char*)&res, sizeof(sCU_CHARACTER_DEL_RES));
	
	/*sCU_CHARACTER_DEL_NFY nfy; // notify to reload the GUI

	nfy.bEncrypt = 0;
	nfy.charId = req->charId;
	nfy.wOpCode = CU_CHARACTER_DEL_NFY;
	nfy.wPacketSize = sizeof(sCU_CHARACTER_DEL_NFY) - 2;

	Write((char*)&nfy, sizeof(sCU_CHARACTER_DEL_NFY));*/
	return true;
}
bool CharSocket::SendCharacterExit(Packet &packet)
{
	sCU_CHARACTER_EXIT_RES res;
	memset(&res, 0, sizeof(sCU_CHARACTER_EXIT_RES));

	res.bEncrypt = 0;
	res.wOpCode = CU_CHARACTER_EXIT_RES;
	res.wPacketSize = sizeof(sCU_CHARACTER_EXIT_RES) - 2;
	res.wResultCode = GAME_SUCCESS;

	Write((char*)&res, sizeof(sCU_CHARACTER_EXIT_RES));
}
bool CharSocket::_ProcessCharPacket(Packet& packet, WORD wOpCode)
{
	switch (wOpCode)
	{
		case UC_LOGIN_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_LOGIN_REQ ~~~~~~~~~");
			return GetLoginRequest(packet);
		}
		case UC_CHARACTER_SERVERLIST_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CHARACTER_SERVERLIST_REQ ~~~~~~~~~");
			return GetCharacterServerList(packet, false);
		}
		case UC_CHARACTER_SERVERLIST_ONE_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CHARACTER_SERVERLIST_ONE_REQ ~~~~~~~~~");
			return GetCharacterServerList(packet, true);
		}
		case UC_CHARACTER_ADD_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CHARACTER_ADD_REQ ~~~~~~~~~");
			return GetCreateCharactersRequest(packet);
		}
		case UC_CHARACTER_DEL_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CHARACTER_DEL_REQ ~~~~~~~~~");
			return GetCharactersDeleteRequest(packet);
		}
		case UC_CHARACTER_SELECT_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CHARACTER_SELECT_REQ ~~~~~~~~~");
			break;
		}
		case UC_CHARACTER_EXIT_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CHARACTER_EXIT_REQ ~~~~~~~~~");
			break;
		}
		case UC_CHARACTER_LOAD_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CHARACTER_LOAD_REQ ~~~~~~~~~");
			return GetCharacterLoad(packet);
		}
		case UC_CHARACTER_DEL_CANCEL_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CHARACTER_DEL_CANCEL_REQ ~~~~~~~~~");
			break;
		}
		case UC_CONNECT_WAIT_CHECK_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CONNECT_WAIT_CHECK_REQ ~~~~~~~~~");
			break;
		}
		case UC_CONNECT_WAIT_CANCEL_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CONNECT_WAIT_CANCEL_REQ ~~~~~~~~~");
			break;
		}
		case UC_CHARACTER_RENAME_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CHARACTER_RENAME_REQ ~~~~~~~~~");
			break;
		}
		case UC_CASHITEM_HLSHOP_REFRESH_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CASHITEM_HLSHOP_REFRESH_REQ ~~~~~~~~~");
			return true;
			break;
		}
		case UC_CASHITEM_BUY_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CASHITEM_BUY_REQ ~~~~~~~~~");
			break;
		}
		case UC_CHAR_SERVERLIST_REQ:
		{
			sLog.outDebug("~~~~~~~~~ UC_CHAR_SERVERLIST_REQ ~~~~~~~~~");
			return GetCharacterServerList(packet, false); // will send the all list and the client will connect to the last connected server
		}
		default:
		{
			sLog.outError("Packet_[%u] Unknow", wOpCode);
			return false;
		}
	}
	return false;
}
int CharSocket::AddCharacters(sPC_SUMMARY data)
{
	int charid = -1;
	//sITEM_TBLDAT sItem;
	//sPC_TBLDAT pcdata = *(sPC_TBLDAT*)sTBM.GetPcTable()->GetPcTbldat(data.byRace, data.byClass, data.byGender);
	sDB.prepare("INSERT INTO characters (AccountID, Name, ServerID, RaceID, ClassID, GenderID, FaceID, HairID, HairColorID, SkinColorID, GuildID) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	sDB.setInt(1, AccountID);
	sDB.setString(2, username);
	sDB.setInt(3, ServerID);
	sDB.setInt(4, data.byRace);
	sDB.setInt(5, data.byClass);
	sDB.setInt(6, data.byGender);
	sDB.setInt(7, data.byFace);
	sDB.setInt(8, data.byHair);
	sDB.setInt(9, data.byHairColor);
	sDB.setInt(10, data.bySkinColor);
	sDB.setInt(11, 0);
	sDB.execute();

	sDB.prepare("SELECT `CharacterID` from `characters` WHERE `Name` = ? AND `AccountID` = ?;");
	sDB.setString(1, username);
	sDB.setInt(2, AccountID);
	sDB.execute();
	sDB.fetch();
	sLog.outDebug("RowCount = %d", sDB.rowsCount());
	if (sDB.rowsCount() == 0)
		return -1;
	charid = sDB.getInt("CharacterID");

	// QUICK SLOT
	sDB.prepare("INSERT INTO quickslot (`charId`) VALUES ( ? );");
	sDB.setInt(1, charid);
	sDB.execute();
	// BASE ITEM
	sDB.prepare("INSERT INTO items (`owner_id`, `tblidx`, `place`, `pos`, `count`, `rank`, `durability`) VALUES (?, '19901', '0', '0', '0', '1', '10'), (?, '532001', '7', '5', '1', '1', '100');");
	sDB.setInt(1, charid);
	sDB.setInt(2, charid);
	sDB.execute();
	if (RACE_HUMAN == data.byRace)
	{
		if (data.byClass == PC_CLASS_HUMAN_FIGHTER)
		{
			sDB.prepare("INSERT INTO skills (`skill_id`, `owner_id`, `RpBonusAuto`, `RpBonusType`, `SlotID`, `TimeRemaining`, `Exp`) VALUES ('10111',?,?,'0','1','0','0');");
			sDB.setInt(1, charid);
			sDB.setBoolean(2, 0);
			sDB.execute();

			sDB.prepare("INSERT INTO items (`owner_id`, `tblidx`, `place`, `pos`, `count`, `rank`, `durability`)"
			"VALUES (?,  '500001', '7', '0', '1', '1', '100'), "
			"(?,  '522001', '7', '2', '1', '1', '100'), "
			"(?,  '522002', '7', '3', '1', '1', '100'), "
			"(?,  '522003', '7', '4', '1', '1', '100');");
			sDB.setInt(1, charid);
			sDB.setInt(2, charid);
			sDB.setInt(3, charid);
			sDB.setInt(4, charid);
			sDB.execute();
		}
		else if (data.byClass == PC_CLASS_HUMAN_MYSTIC)
		{
			sDB.prepare("INSERT INTO skills (`skill_id`, `owner_id`, `RpBonusAuto`, `RpBonusType`, `SlotID`, `TimeRemaining`, `Exp`) VALUES ('10111',?,?,'0','1','0','0');");
			sDB.setInt(1, charid);
			sDB.setBoolean(2, 0);
			sDB.execute();

			sDB.prepare("INSERT INTO items (`owner_id`, `tblidx`, `place`, `pos`, `count`, `rank`, `durability`)"
				"VALUES (?,  '500001', '7', '0', '1', '1', '100'), "
				"(?,  '523001', '7', '2', '1', '1', '100'), "
				"(?,  '523002', '7', '3', '1', '1', '100'), "
				"(?,  '523003', '7', '4', '1', '1', '100');");
			sDB.setInt(1, charid);
			sDB.setInt(2, charid);
			sDB.setInt(3, charid);
			sDB.setInt(4, charid);
			sDB.execute();
			sDB.prepare("INSERT INTO skills (`skill_id`, `owner_id`, `SlotID`, `TimeRemaining`) VALUES('110111',?,'1','0');");
			sDB.setInt(1, charid);
			sDB.execute();
		}
		sDB.prepare("UPDATE characters SET `Position_X` = '4467.0', `Position_Y` = '-40.0', `Position_Z` = '3951.0' WHERE `CharacterID` = ? LIMIT 1;");
		sDB.setInt(1, charid);
		sDB.execute();
	}
	else if (RACE_NAMEK == data.byRace)
	{
		if (data.byClass == PC_CLASS_NAMEK_FIGHTER)
		{

		}
		else if (data.byClass == PC_CLASS_NAMEK_MYSTIC)
		{

		}
	}
	else if (RACE_MAJIN == data.byRace)
	{
		if (data.byClass == PC_CLASS_MIGHTY_MAJIN)
		{

		}
		else if (data.byClass == PC_CLASS_WONDER_MAJIN)
		{

		}
	}
	else
	{
		sLog.outError("Error, Account: %d try to create invalid class / race [%d][%d] from ip: %s", AccountID, data.byClass, data.byRace, m_address);
		return -1;
	}
	return charid;
}