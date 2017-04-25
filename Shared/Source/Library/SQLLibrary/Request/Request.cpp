#include <mysqlconn_wrapper.h>
#include <Logger.h>
#include "../../../NetworkLib/Source/Header/Packet/Char/PacketCU.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <list>

void MySQLConnWrapper::UpdateAccountOnline(int AccountID, bool isLogging)
{
	prepare("UPDATE `account` SET `AccountStatus` = ? WHERE `AccountID` = ?");
	setInt(1, isLogging);
	setInt(2, AccountID);
	execute();
}
ResultCodes MySQLConnWrapper::ValidateLoginRequest(char *username, char* password, int accid)
{
	prepare("SELECT `AccountID` FROM `account` WHERE `userName` = ?");
	setString(1, username);
	execute();
	fetch();
	if (rowsCount() == 0)
	{
		return AUTH_USER_NOT_FOUND;
	}
	else
	{
		prepare("SELECT `AccountID`, `AccountStatus` FROM `account` WHERE `userName` = ? AND `UserPassword` = ?");
		setString(1, username);
		setString(2, password);
		execute();
		fetch();
		if (rowsCount() == 0)
		{
			return AUTH_WRONG_PASSWORD;
		}
		if (getInt("AccountStatus") == 1)
		{
			return AUTH_USER_EXIST_IN_CHARACTER_SERVER;
		}
		else
		{
			prepare("SELECT `AccountID` FROM `account` WHERE `userName`= ? AND `UserPassword`= ? AND `IsBanned`='0'");
			setString(1, username);
			setString(2, password);
			execute();
			fetch();
			if (rowsCount() > 0)
			{
				return AUTH_SUCCESS;
			}
			else
			{
				return AUTH_USER_TEMPORARY_BLOCK;
			}
		}
	}
	return AUTH_DB_FAIL;
}
int MySQLConnWrapper::GetAccountID(char* username, char* password)
{
	prepare("SELECT `AccountID` FROM `account` WHERE `userName` = ? AND `UserPassword` = ?");
	setString(1, username);
	setString(2, password);
	execute();
	while (fetch())
	{
		return getInt("AccountID");
	}
	return -1;
}
int MySQLConnWrapper::GetLastServerID(int accid)
{
	prepare("SELECT `LastServerID` FROM `account` WHERE `AccountID` = ?");
	setInt(1, accid);
	execute();
	while (fetch())
	{
		return getInt("LastServerID");
	}
	return 0xFF;
}
int MySQLConnWrapper::GetIsGameMaster(int accid)
{
	prepare("SELECT `IsGM` FROM `account` WHERE `AccountID` = ?");
	setInt(1, accid);
	execute();
	while (fetch())
	{
		return getInt("IsGM");
	}
	return 0xFF;
}
void MySQLConnWrapper::DBUpdateLastServer(int accid, int servid)
{
	prepare("UPDATE `account` SET `LastServerID` = ? WHERE `AccountID` = ?;");
	setInt(1, servid);
	setInt(2, accid);
	execute();
	fetch();
}
void MySQLConnWrapper::GetDBAccCharListData(sCU_CHARACTER_INFO* outdata, int accid, int serverid)
{
	outdata->bEncrypt = 0;
	outdata->wOpCode = CU_CHARACTER_INFO;
	outdata->wPacketSize = sizeof(sCU_CHARACTER_INFO) - 2;

	prepare("SELECT * FROM `characters` WHERE `AccountID`= ? AND `ServerID`= ? LIMIT 8;");
	setInt(1, accid);
	setInt(2, serverid);
	execute();
	outdata->byCount = rowsCount();
	sLog.outDebug("Account: %d, num characters: %d", accid, outdata->byCount);
}
ResultCodes MySQLConnWrapper::checkUsedName(char* Name)
{
	prepare("SELECT * FROM `characters` WHERE `Name` = ?;");
	setString(1, Name);
	execute();
	fetch();
	if (rowsCount() == 0)
		return CHARACTER_SUCCESS;
	else
		return CHARACTER_SAMENAME_EXIST;
	return CHARACTER_DB_QUERY_FAIL;
}
ResultCodes MySQLConnWrapper::DeleteCharacter(int accId, int charId)
{
	// CHECK IF GUILD OWNER !!!!! /\

	// Char db
	prepare("DELETE FROM `characters` WHERE `AccountID`= ? AND `CharacterID`= ?;");
	setInt(1, accId);
	setInt(2, charId);
	execute();
	if (rowsCount() != 0)
	{
		sLog.outError("Error while removing character, Account: %d, CharacterID: %d", accId, charId);
		return ResultCodes::CHARACTER_DELETE_CHAR_FAIL;
	}
	prepare("DELETE FROM `quickslot` WHERE `charId` = ?;");
	setInt(1, charId);
	execute();
	if (rowsCount() != 0)
	{
		sLog.outError("Error while removing character quickslot, Account: %d, CharacterID: %d", accId, charId);
		return ResultCodes::CHARACTER_DELETE_CHAR_FAIL;
	}
	prepare("DELETE FROM `skills` WHERE `owner_id` = ?;");
	setInt(1, charId);
	execute();
	if (rowsCount() != 0)
	{
		sLog.outError("Error while removing character skills, Account: %d, CharacterID: %d", accId, charId);
		return ResultCodes::CHARACTER_DELETE_CHAR_FAIL;
	}
	prepare("DELETE FROM `items` WHERE `owner_id` = ?;");
	setInt(1, charId);
	execute();
	if (rowsCount() != 0)
	{
		sLog.outError("Error while removing character items, Account: %d, CharacterID: %d", accId, charId);
		return ResultCodes::CHARACTER_DELETE_CHAR_FAIL;
	}
	sLog.outDetail("Character have been deleted: Account: %d, CharacterID: %d", accId, charId);
	return ResultCodes::CHARACTER_SUCCESS;
}
void MySQLConnWrapper::VerifyCharacterToDelete(int accid)
{
	std::list<int> characterToDelete;
	time_t startedAt;
	time(&startedAt);
	prepare("SELECT * FROM `characters` WHERE `AccountID` = ?");
	setInt(1, accid);
	execute();
	while (fetch())
	{
		std::string deletedAt = getString("deletionStartedAt");
		if (deletedAt != "" && deletedAt != "NULL")
		{
			boost::posix_time::ptime t(boost::posix_time::time_from_string(deletedAt));
			tm pt_tm = to_tm(t); // cast struct to time_t
			if (t.is_not_a_date_time() == false)
			{
				time_t deletedAtDate = mktime(&pt_tm);
				time_t timeNow;
				time(&timeNow);
				double diff = difftime(timeNow, deletedAtDate) / 60 / 60; // in hour

				if (diff >= 1) // more than a hour past so delete the character
				{
					characterToDelete.push_back(getInt("CharacterID"));
				}
			}
		}
	}
	for (std::list<int>::iterator it = characterToDelete.begin(); it != characterToDelete.end(); ++it)
	{
		DeleteCharacter(accid, *it);
	}
}
ResultCodes MySQLConnWrapper::CancelDeleteCharacterPending(CHARACTERID charid)
{
	prepare("SELECT * FROM `characters` WHERE `CharacterID` = ?;");
	setInt(1, charid);
	execute();
	if (fetch())
	{
		std::string deletedAt = getString("deletionStartedAt");
		if (deletedAt != "" && deletedAt != "NULL")
		{
			boost::posix_time::ptime t(boost::posix_time::time_from_string(deletedAt));
			tm pt_tm = to_tm(t); // cast struct to time_t
			if (t.is_not_a_date_time() == false)
			{
				time_t deletedAtDate = mktime(&pt_tm);
				time_t timeNow;
				time(&timeNow);
				double diff = difftime(timeNow, deletedAtDate) / 60 / 60; // in hour

				if (diff < 1) // less than a hour past so cancel the deletion
				{
					prepare("UPDATE `characters` SET `deletionStartedAt` = ? WHERE `CharacterID` = ? LIMIT 1;");
					setString(1, "NULL");
					setInt(2, charid);
					execute();
					sLog.outDetail("Character have been restored: CharacterID: %d", charid);
					return ResultCodes::CHARACTER_SUCCESS;
				}
			}
			return ResultCodes::CHARACTER_FAIL;
		}
	}
	return ResultCodes::CHARACTER_DB_QUERY_FAIL;
}
int MySQLConnWrapper::GetAmountOfCharacter(int accid, int servid)
{
	prepare("SELECT * FROM `characters` WHERE `AccountID`= ? AND `ServerID`= ? LIMIT 8;");
	setInt(1, accid);
	setInt(2, servid);
	execute();
	return rowsCount();
}