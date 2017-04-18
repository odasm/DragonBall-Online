#pragma once

#include <Define.h>
#include <Opcodes.h>

//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_AU(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)
struct sSERVER_INFO
{
	char		szCharacterServerIP[16 + 1];
	WORD		wCharacterServerPortForClient;
	DWORD		dwLoad;
};
//------------------------------------------------------------------
BEGIN_PROTOCOL(AU_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(AU_LOGIN_RES)
WORD		wResultCode;
WCHAR		awchUserId[16 + 1];
BYTE		abyAuthKey[16];
UINT		AccountID;
BYTE		lastServerID;
BYTE		lastChannelID;
UINT		dev;
BYTE		byServerInfoCount;
BYTE		CharServerCount;
END_PROTOCOL()
BEGIN_PROTOCOL(AU_COMMERCIAL_SETTING_NFY)
END_PROTOCOL()
#pragma pack()
