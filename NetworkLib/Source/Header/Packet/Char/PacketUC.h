#pragma once

#include <Define.h>
#include <Opcodes.h>

struct sUC_LOGIN_REQ
{
	ACCOUNTID		accountId;
	BYTE			abyAuthKey[16];
	SERVERFARMID	serverID;
};
struct sUC_CHARACTER_LOAD_REQ
{
	ACCOUNTID		accountId;
	SERVERFARMID	serverFarmId;
};
struct sUC_CHARACTER_ADD_REQ
{
	WCHAR			awchCharName[16 + 1];
	DWORD			dwCodePage;
	BYTE			byRace;
	BYTE			byClass;
	BYTE			byGender;
	BYTE			byFace;
	BYTE			byHair;
	BYTE			byHairColor;
	BYTE			bySkinColor;
	BYTE			byBlood;
};