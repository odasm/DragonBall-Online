#include <AuthSocket.h>
#include <Logger.h>
#include <stdio.h>
#include <cstdio>
#include <Opcodes.h>
#include <Encryptation.h>
#include <mysqlconn_wrapper.h>
#include <Packet\Auth\PacketAU.h>

enum eStatus
{
	STATUS_CONNECTED = 0,
	STATUS_AUTHED
};

struct UA_LOGIN_REQ
{
	WCHAR		awchUserId[16 + 1];
	WCHAR		awchPasswd[16 + 1];
	DWORD		dwCodePage;
	WORD		wLVersion;
	WORD		wRVersion;
	BYTE		abyMacAddress[5 + 1];
};

AuthSocket::AuthSocket(boost::asio::io_service &service, std::function<void(Socket *)> closeHandler)
	: Socket(service, closeHandler)
{
}
void AuthSocket::OnConnectionDone()
{
	uint8 rawData[] = { 0x06, 0x00, 0x03, 0x00, 0x30, 0x2C, 0x67, 0x4C };

	Write((char*)rawData, sizeof(rawData));
	memset(&rawData, 0, sizeof(rawData));
}
void AuthSocket::Send(BYTE* pData, int size, int opcode)
{
	Packet packet(size);
	packet.SetPacket(pData, size);

	packet.GetPacketHeader()->bySequence = opcode;
	packet.GetPacketHeader()->byChecksum = 3;

	sLog->outPacketDebugger(&packet);
	Write((char*)&packet, size);
}
bool AuthSocket::_ProcessLoginPacket(Packet& packet)
{
	if (packet.GetPacketHeader()->bySequence == Opcodes::UA_LOGIN_TW_REQ) // Get the login request data
	{
		bool val = _HandleOnLogin(packet);

		return val;
	}
	else if (packet.GetPacketHeader()->bySequence == Opcodes::UA_LOGIN_DISCONNECT_REQ)
	{
		sAU_LOGIN_DISCONNECT_RES res;
		memset(&res, 0, sizeof(sAU_LOGIN_DISCONNECT_RES));

		res.bEncrypt = 2;
		res.wPacketLen = 0;
		res.wOpCode = Opcodes::AU_LOGIN_DISCONNECT_RES;
		res.byChecksum = 3;

		Write((char*)&res, sizeof(sAU_LOGIN_DISCONNECT_RES));
		return true;
	}
}
bool AuthSocket::_HandleOnLogin(Packet& packet)
{
	UA_LOGIN_REQ *req = (UA_LOGIN_REQ*)packet.GetPacketData();
	char username[16];
	char password[16];
	wcstombs(username, req->awchUserId, 16);
	wcstombs(password, req->awchPasswd, 16);

	/*Packet commercial_setting;
	commercial_setting.SetPacket(Opcodes::AU_COMMERCIAL_SETTING_NFY);
	Write((char*)commercial_setting.GetPacketBuffer(), 4);
	commercial_setting.Destroy();*/

	sAU_COMMERCIAL_SETTING_NFY commercial;
	memset(&commercial, 0, sizeof(sAU_COMMERCIAL_SETTING_NFY));

	commercial.bEncrypt = 2;
	commercial.wPacketLen = 0;
	commercial.wOpCode = Opcodes::AU_COMMERCIAL_SETTING_NFY;
	commercial.byChecksum = 3;

	Write((char*)&commercial, sizeof(sAU_COMMERCIAL_SETTING_NFY));
	//com.Destroy();

	sAU_LOGIN_RES res;
	memset(&res, 0, sizeof(sAU_LOGIN_RES));

	res.bEncrypt = sizeof(sAU_LOGIN_RES) - 4;
	res.wPacketLen = 0;
	res.wOpCode = Opcodes::AU_LOGIN_RES;
	res.byChecksum = 3;

	res.wResultCode = 100;
	memcpy(res.awchUserId, req->awchUserId, 16);
	memcpy(res.abyAuthKey, "SE@WASDE#$RFWD@D", 16);
	res.AccountID = 0;
	res.lastChannelID = 255;
	res.lastServerID = 255;
	res.dev = 65535;

	res.byServerInfoCount = 1;
	res.CharServerInfo.dwLoad = 0;
	memcpy(res.CharServerInfo.szCharacterServerIP, "127.0.0.1", strlen("127.0.0.1"));
	res.CharServerInfo.wCharacterServerPortForClient = 50300;
	res.CharServerInfo.unknow = 65535;

	Write((char*)&res, sizeof(sAU_LOGIN_RES));

	return true;
	/*sAU_LOGIN_RES res;
	memset(&res, 0, sizeof(sAU_LOGIN_RES));
	
	int AccountID = sDB->GetAccountID(username, password);

	res.wResultCode = sDB->ValidateLoginRequest(username, password, AccountID);
	memcpy(res.awchUserId, req->awchUserId, 16);
	memcpy(res.abyAuthKey, "SE@WASDE#$RFWD@D", 16);
	res.AccountID = AccountID;
	res.lastChannelID = 255;
	res.lastServerID = sDB->GetLastServerID(AccountID);
	res.dev = 65535;

	res.byServerInfoCount = 1;
	res.CharServerInfo.dwLoad = 0;
	memcpy(res.CharServerInfo.szCharacterServerIP, "127.0.0.1", strlen("127.0.0.1"));
	res.CharServerInfo.wCharacterServerPortForClient = 50300; // try to use AuthServer for Auth & Char ?
	res.CharServerInfo.unknow = 65535;

	Packet result;
	//result.SetPacket((BYTE*)&res, sizeof(sAU_LOGIN_RES), Opcodes::AU_LOGIN_RES);
	Write((char*)result.GetPacketBuffer(), result.GetUsedSize());

	req = NULL;
	res = {};
	result.Destroy();
	return true;*/
}
bool AuthSocket::ProcessIncomingData()
{
	while (ReadLengthRemaining() > 0)
	{
		size_t sizeInc = ReadLengthRemaining();
		Packet *pk = new Packet();
		pk->AttachData((BYTE*)InPeak(), sizeInc);
		PACKETDATA *header = (PACKETDATA*)InPeak();
		sLog->outDebug("~~~~~~~ opcode %u ~~~~~~~", header->wOpCode);

		/*
			///		 DECRYPT PACKET HERE ????		\\\
		*/
		//sLog->outPacketDebugger(&packet);
		bool process = false;
		if (pk->GetPacketHeader()->bySequence == 4)
		{
			uint8 rawData2[] = { 0x22, 0x00, 0x10, 0x00, 0x49, 0xD1, 0xF1, 0x1C, 0x6D, 0x58, 0xF9, 0xC5, 0x30, 0x26, 0xA4, 0x7B,
				0xB2, 0xD8, 0x2C, 0x86, 0x58, 0x60, 0x7B, 0xDD, 0xF0, 0x77, 0xCF, 0x25, 0x48, 0xB3, 0x65, 0x45,
				0x38, 0x80, 0x14, 0x72 };
			Write((char*)rawData2, sizeof(rawData2));
			memset(&rawData2, 0, sizeof(rawData2));
			process = true;
		}
		else if (pk->GetPacketHeader()->bySequence == Opcodes::SYS_ALIVE)
		{
			process = true;
		}
		
		else if (pk->GetPacketHeader()->bySequence >= Opcodes::UA_OPCODE_BEGIN && pk->GetPacketHeader()->bySequence <= Opcodes::UA_LOGIN_DISCONNECT_REQ)
		{
			process = _ProcessLoginPacket(*pk);
		}
		else
		{
			sLog->outError("Packet_[%u] Unknow", pk->GetPacketHeader()->bySequence);

			ReadSkip(sizeInc);
			delete pk;
			return false;
		}
		/// if we reach here, it means that a valid opcode was found and the handler completed successfully
		ReadSkip(sizeInc);
		delete pk;
		return process;
	}
	return true;
}