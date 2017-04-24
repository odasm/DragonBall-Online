#pragma once

#include <Define.h>
#include <Opcodes.h>
#include <SharedDef.h>
#include <SharedType.h>

struct sUC_LOGIN_REQ
{
	ACCOUNTID		accountId;
	BYTE			abyAuthKey[MAX_SIZE_USERID_UNICODE];
	SERVERFARMID	serverID;
};
struct sUC_CHARACTER_LOAD_REQ
{
	ACCOUNTID		accountId;
	SERVERFARMID	serverFarmId;
};
struct sUC_CHARACTER_ADD_REQ
{
	WCHAR			awchCharName[MAX_SIZE_CHAR_NAME_UNICODE + 1];
	//DWORD			dwCodePage; //bug
	BYTE			byRace;
	BYTE			byClass;
	BYTE			byGender;
	BYTE			byFace;
	BYTE			byHair;
	BYTE			byHairColor;
	BYTE			bySkinColor;
	BYTE			byBlood;
	DWORD			unknown;
};
struct sUC_CHARACTER_DEL_REQ
{
	CHARACTERID		charId;
};