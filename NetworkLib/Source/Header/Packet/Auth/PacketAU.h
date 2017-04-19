#pragma once

#include <Define.h>
#include <Opcodes.h>

//------------------------------------------------------------------
//
//------------------------------------------------------------------
typedef unsigned int ACCOUNTID;
typedef BYTE SERVERFARMID;
//------------------------------------------------------------------

#pragma pack(1)
struct sSERVER_INFO
{
	char		szCharacterServerIP[64 + 1];
	WORD		wCharacterServerPortForClient;
	DWORD		dwLoad;
	WORD		unknow;
};
//------------------------------------------------------------------
BEGIN_PROTOCOL(AU_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AU_LOGIN_RES)
WORD				wResultCode;
WCHAR				awchUserId[16 + 1];
BYTE				abyAuthKey[16];
ACCOUNTID			AccountID;
BYTE				lastServerID;
SERVERFARMID		lastChannelID;
DWORD				dev;
BYTE				byServerInfoCount;

sSERVER_INFO		CharServerInfo;
END_PROTOCOL()
BEGIN_PROTOCOL(AU_COMMERCIAL_SETTING_NFY)
END_PROTOCOL()
#pragma pack()