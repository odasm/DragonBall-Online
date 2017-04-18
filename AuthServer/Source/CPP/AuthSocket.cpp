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
struct AU_LOGINRESP
{
	WORD		wResultCode;
	WCHAR		awchUserId[16];
	BYTE		abyAuthKey[16];
	DWORD		AccountID;
	BYTE		lastServerID;
	BYTE		lastChannelID;
	DWORD		dev;
	BYTE		byServerInfoCount;
	BYTE		CharServerCount;
};
AuthSocket::AuthSocket(boost::asio::io_service &service, std::function<void(Socket *)> closeHandler)
	: Socket(service, closeHandler), _authed(false)
{
}
void AuthSocket::OnConnectionDone()
{
	uint8 rawData[] = { 0x06, 0x00, 0x03, 0x00, 0x30, 0x2C, 0x67, 0x4C };
	/*uint8 rawData2[] = { 0x22, 0x00, 0x10, 0x00, 0x49, 0xD1, 0xF1, 0x1C, 0x6D, 0x58, 0xF9, 0xC5, 0x30, 0x26, 0xA4, 0x7B,
		0xB2, 0xD8, 0x2C, 0x86, 0x58, 0x60, 0x7B, 0xDD, 0xF0, 0x77, 0xCF, 0x25, 0x48, 0xB3, 0x65, 0x45,
		0x38, 0x80, 0x14, 0x72 };*/

	Write((char*)rawData, sizeof(rawData));
	//Write((char*)rawData2, sizeof(rawData2));
	//Write((char*)rawData, sizeof(rawData));
	//Write((char*)rawData2, sizeof(rawData2));

	/// the connection is now accepted, WTF
}
void AuthSocket::Send(void* pData, int size)
{
	Packet* packet = new Packet((unsigned char*)pData, size);
	BYTE*data = (BYTE*)pData;
	packet->GetPacketHeader()->bySequence = (USHORT)data[0];
	packet->GetPacketHeader()->bEncrypt = BYTE(0);
	packet->GetPacketHeader()->byChecksum = USHORT(3);
	sLog->outPacketDebugger(packet);
	Write((char*)packet, size);
}
bool AuthSocket::_HandleOnLogin(Packet& packet)
{
	UA_LOGIN_REQ req = (UA_LOGIN_REQ&)*packet.GetPacketData();
	size_t converted;
	char userName[16 + 1];
	wcstombs_s(&converted, userName, req.awchUserId, 16);
	sLog->outDetail("Login request by: %s using client version: %d.%d", userName, req.wLVersion, req.wRVersion);

	sDB->prepare("SELECT * FROM accounts WHERE UserName = ?");
	sDB->setString(1, userName);
	sDB->execute();
	sDB->fetch();
	if (sDB->rowsCount() != 0)
	{
		sLog->outDetail("User: %s found in database", userName);
		Packet paquet(sizeof(sAU_LOGIN_RES));
		sAU_LOGIN_RES *res = (sAU_LOGIN_RES*)paquet.GetPacketData();

		/*res->wOpCode = (BYTE)1002;
		res->accountId = 0;
		memcpy(res->abyAuthKey, "AaAaAaAaAaAaAaAa", 16);
		res->lastServerFarmId = 0;
		res->wResultCode = 0;
		res->dwAllowedFunctionForDeveloper = 1;
		memcpy(res->awchUserId, userName, 16);
		res->aServerInfo[0].dwLoad = 0;
		strcpy(res->aServerInfo[0].szCharacterServerIP, "88.215.108.150000");
		res->aServerInfo[0].wCharacterServerPortForClient = 12345;*/

		//paquet.SetPacketLen(sizeof(sAU_LOGIN_RES));
		sLog->outPacketDebugger(&paquet);

		/*PacketEncoder encoder;
		encoder.TxEncrypt(paquet);*/

		sLog->outPacketDebugger(&paquet);

		Write((char*)&paquet, sizeof(Packet));
	}
	return true;
}
bool AuthSocket::ProcessIncomingData()
{
	while (ReadLengthRemaining() > 0)
	{
		size_t sizeInc = ReadLengthRemaining();
		
		BYTE *datas = (BYTE*)InPeak();
		Packets pk;
		pk.FillPacket(datas, static_cast<WORD>(sizeInc));
		
		if (pk.getOpcode() == 4)
		{
			uint8 rawData2[] = { 0x22, 0x00, 0x10, 0x00, 0x49, 0xD1, 0xF1, 0x1C, 0x6D, 0x58, 0xF9, 0xC5, 0x30, 0x26, 0xA4, 0x7B,
				0xB2, 0xD8, 0x2C, 0x86, 0x58, 0x60, 0x7B, 0xDD, 0xF0, 0x77, 0xCF, 0x25, 0x48, 0xB3, 0x65, 0x45,
				0x38, 0x80, 0x14, 0x72 };
			Write((char*)rawData2, sizeof(rawData2));
		}
		else if (pk.getOpcode() == Opcodes::UA_LOGIN_TW_REQ)
		{
			{
				BYTE *data = new BYTE(4);
				data[0] = USHORT(2);
				data[1] = BYTE(0);
				data[2] = USHORT(1005);
				data[3] = BYTE(3);
				Write((char*)data, 4);

				UA_LOGIN_REQ req = (UA_LOGIN_REQ&)*pk.GetPacketData();

				AU_LOGINRESP *res = new AU_LOGINRESP;
				res->AccountID = 0;
				res->byServerInfoCount = 1;
				res->dev = 0;
				res->lastChannelID = 0;
				res->lastServerID = 255;
				res->wResultCode = ResultCodes::AUTH_SUCCESS;

				memcpy(res->awchUserId, req.awchUserId, 16);
				memcpy(res->abyAuthKey, L"SE@WASDE#$RFWD@D", 16);

				BYTE* data2 = new BYTE(4);
				data2[0] = USHORT(64);
				data2[1] = BYTE(0);
				data2[2] = USHORT(1002);
				data2[3] = BYTE(3);
				

				BYTE *result = new BYTE(4 + sizeof(AU_LOGINRESP));
				memcpy(&result[0], &data2[0], 4);
				memcpy(&result[4], &res[0], sizeof(AU_LOGINRESP));
				Write((char*)result, 4 + sizeof(AU_LOGINRESP));
			}
			sLog->outDebug("Sended ?");
		}
		ReadSkip(sizeInc);
		//pk.Destroy();
		/*Packet packet(data, static_cast<WORD>(sizeInc));
		LPPACKETDATA data1 = (LPPACKETDATA)packet.GetPacketData();
		sLog->outDebug("LPPACKETDATA: %u HEADER: %u", data1->wOpCode, packet.GetPacketHeader()->bySequence);*/
		/*
			///		 DECRYPT PACKET HERE ????		\\\
		*/
		//sLog->outPacketDebugger(&packet);
		/*if ((int)packet.GetPacketHeader()->bySequence == Opcodes::UA_LOGIN_TW_REQ) // Get the login request data
		{
			_HandleOnLogin(packet);
		}*/
		/// if we reach here, it means that a valid opcode was found and the handler completed successfully
		//packet.Destroy();
		return true;
	}
	return true;
}