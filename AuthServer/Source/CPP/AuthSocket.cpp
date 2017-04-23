#include <AuthSocket.h>
#include <Logger.h>
#include <stdio.h>
#include <cstdio>
#include <Opcodes.h>
#include <Encryptation.h>
#include <mysqlconn_wrapper.h>
#include <Packet\Auth\PacketAU.h>
#include <XmlParser.h>
#include <string>

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

		res.wPacketSize = 2;
		res.bEncrypt = 0;
		res.wOpCode = Opcodes::AU_LOGIN_DISCONNECT_RES;
		//res.byChecksum = 3;

		Write((char*)&res, sizeof(sAU_LOGIN_DISCONNECT_RES));
		res = {};
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

	sAU_COMMERCIAL_SETTING_NFY commercial;
	memset(&commercial, 0, sizeof(sAU_COMMERCIAL_SETTING_NFY));

	commercial.wPacketSize = 2;
	commercial.bEncrypt = 0;
	commercial.wOpCode = Opcodes::AU_COMMERCIAL_SETTING_NFY;
	//commercial.byChecksum = 3;

	Write((char*)&commercial, sizeof(sAU_COMMERCIAL_SETTING_NFY));
	commercial = {};

	sAU_LOGIN_RES res;
	memset(&res, 0, sizeof(sAU_LOGIN_RES));

	res.wPacketSize = sizeof(sAU_LOGIN_RES) - 2; // packet body size (packet size - header)
	res.bEncrypt = 0;
	res.wOpCode = Opcodes::AU_LOGIN_RES;
	//res.byChecksum = 3;

	int AccountID = sDB->GetAccountID(username, password);
	res.wResultCode = sDB->ValidateLoginRequest(username, password, AccountID);
	memcpy(res.awchUserId, req->awchUserId, 16);
	memcpy(res.abyAuthKey, "SE@WASDE#$RFWD@D", 16);
	res.AccountID = AccountID;
	res.lastChannelID = 255;
	res.lastServerID = sDB->GetLastServerID(AccountID);
	res.dev = 65535;
	res.byServerInfoCount = sXmlParser->GetInt("CharServerList", "Count");
	int i = 0;
	for (i = 0; i < res.byServerInfoCount; ++i)
	{
		int srv = i + 1;
		std::string fieldName = "CharServer";
		fieldName.append(std::to_string(srv));

		std::string addr = sXmlParser->GetChildStr("CharServerList", (char*)fieldName.c_str(), "IP");
		int port = sXmlParser->GetChildInt("CharServerList", (char*)fieldName.c_str(), "Port");

		res.CharServerInfo[i].dwLoad = 0;
		memcpy(res.CharServerInfo[i].szCharacterServerIP, addr.c_str(), strlen(addr.c_str()));
		res.CharServerInfo[i].wCharacterServerPortForClient = port;
		res.CharServerInfo[i].unknow = 65535;
	}
	res.wPacketSize = (res.wPacketSize - (sizeof(sSERVER_INFO) * (9 - i))); // remove unused struct size (9 because we need always 1 struct size to send)
	Write((char*)&res, sizeof(sAU_LOGIN_RES));
	res = {};
	return true;
}
bool AuthSocket::ProcessIncomingData()
{
	while (ReadLengthRemaining() > 0)
	{
		size_t sizeInc = ReadLengthRemaining();
		Packet *pk = new Packet();
		pk->AttachData((BYTE*)InPeak(), sizeInc);
		PACKETDATA *header = (PACKETDATA*)InPeak();
		sLog->outDebug("~~~~~~~ opcode %u ~~~~~~~ checksum %u", header->wOpCode, pk->GetPacketHeader()->byChecksum);

		/*
			///		 DECRYPT PACKET HERE ????		\\\
		*/

		bool process = false;
		if (header->wOpCode == 4)
		{
			uint8 rawData2[] = { 0x22, 0x00, 0x10, 0x00, 0x49, 0xD1, 0xF1, 0x1C, 0x6D, 0x58, 0xF9, 0xC5, 0x30, 0x26, 0xA4, 0x7B,
				0xB2, 0xD8, 0x2C, 0x86, 0x58, 0x60, 0x7B, 0xDD, 0xF0, 0x77, 0xCF, 0x25, 0x48, 0xB3, 0x65, 0x45,
				0x38, 0x80, 0x14, 0x72 };
			Write((char*)rawData2, sizeof(rawData2));
			memset(&rawData2, 0, sizeof(rawData2));
			process = true;
		}
		else if (header->wOpCode == Opcodes::SYS_ALIVE)
		{
			process = true;
		}
		
		else if (header->wOpCode >= Opcodes::UA_OPCODE_BEGIN && pk->GetPacketHeader()->bySequence <= Opcodes::UA_LOGIN_DISCONNECT_REQ)
		{
			process = _ProcessLoginPacket(*pk);
		}
		else
		{
			sLog->outError("Packet_[%u] Unknow", header->wOpCode);

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