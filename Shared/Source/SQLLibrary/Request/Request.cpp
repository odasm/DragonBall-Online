#include <mysqlconn_wrapper.h>
#include <Logger.h>

ResultCodes MySQLConnWrapper::ValidateLoginRequest(char *username, char* password, int accid)
{
	sDB->prepare("SELECT `ID` FROM account WHERE userName = ?");
	sDB->setString(1, username);
	sDB->execute();
	sDB->fetch();
	if (sDB->rowsCount() == 0)
	{
		return AUTH_USER_NOT_FOUND;
	}
	else
	{
		sDB->prepare("SELECT `ID` FROM account WHERE userName = ? AND passWord = ?");
		sDB->setString(1, username);
		sDB->setString(2, password);
		sDB->execute();
		sDB->fetch();
		if (sDB->rowsCount() == 0)
		{
			return AUTH_WRONG_PASSWORD;
		}
		else
		{
			sDB->prepare("SELECT `ID` FROM `account` WHERE `userName`= ? AND `passWord`= ? AND `State`='0'");
			sDB->setString(1, username);
			sDB->setString(2, password);
			sDB->execute();
			sDB->fetch();
			if (sDB->rowsCount() == 0)
			{
				bool reconnect = true;
				if (reconnect)
				{
					sDB->prepare("UPDATE `account` SET `State` = '1' WHERE `ID` = ?;");
					sDB->setInt(1, accid);
					sDB->execute();
					return AUTH_SUCCESS;
				}
			}
			else
			{
				sDB->prepare("UPDATE `account` SET `State` = '1' WHERE `ID` = ?;");
				sDB->setInt(1, accid);
				sDB->execute();
				return AUTH_SUCCESS;
			}
		}
	}
	return AUTH_DB_FAIL;
}
int MySQLConnWrapper::GetAccountID(char* username, char* password)
{
	sDB->prepare("SELECT `ID` FROM account WHERE userName = ? AND passWord = ?");
	sDB->setString(1, username);
	sDB->setString(2, password);
	sDB->execute();
	while (sDB->fetch())
	{
		return sDB->getInt("ID");
	}
	return -1;
}
int MySQLConnWrapper::GetLastServerID(int accid)
{
	sDB->prepare("SELECT `LastServerID` FROM account WHERE ID = ?");
	sDB->setInt(1, accid);
	sDB->execute();
	while (sDB->fetch())
	{
		return sDB->getInt("LastServerID");
	}
	return 0xFF;
}