#include <AuthSocket.h>
#include <Logger.h>
#include <stdio.h>
#include <cstdio>
#include <Opcodes.h>
#include <Encryptation.h>

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
	: Socket(service, closeHandler), _authed(false)
{
}
void AuthSocket::OnConnectionDone()
{
	uint8 rawData[] = { 0x06, 0x00, 0x03, 0x00, 0x30, 0x2C, 0x67, 0x4C };
	uint8 rawData2[] = { 0x22, 0x00, 0x10, 0x00, 0x49, 0xD1, 0xF1, 0x1C, 0x6D, 0x58, 0xF9, 0xC5, 0x30, 0x26, 0xA4, 0x7B,
		0xB2, 0xD8, 0x2C, 0x86, 0x58, 0x60, 0x7B, 0xDD, 0xF0, 0x77, 0xCF, 0x25, 0x48, 0xB3, 0x65, 0x45,
		0x38, 0x80, 0x14, 0x72 };

	Write((char*)rawData, sizeof(rawData));
	Write((char*)rawData2, sizeof(rawData2));
	Write((char*)rawData, sizeof(rawData));
	Write((char*)rawData2, sizeof(rawData2));

	/// the connection is now accepted, WTF
}
bool AuthSocket::_HandleOnLogin(Packet& packet)
{
	UA_LOGIN_REQ req = (UA_LOGIN_REQ&)*packet.GetPacketData();
	size_t converted;
	char userName[16 + 1];
	wcstombs_s(&converted, userName, req.awchUserId, 16);
	sLog->outDetail("Login request by: %s using client version: %d.%d", userName, req.wLVersion, req.wRVersion);
	return true;
}
bool AuthSocket::ProcessIncomingData()
{
	while (ReadLengthRemaining() > 0)
	{
		size_t sizeInc = ReadLengthRemaining();
		BYTE *data = (BYTE*)InPeak();
		Packet packet(data, static_cast<WORD>(sizeInc));
		/*
			///		 DECRYPT PACKET HERE ????		\\\
		*/
		sLog->outPacketDebugger(&packet);
		if ((int)packet.GetPacketHeader()->bySequence == 103) // Get the login request data
		{
			_HandleOnLogin(packet);
		}
		/// if we reach here, it means that a valid opcode was found and the handler completed successfully
		ReadSkip(static_cast<int>(sizeInc));
		packet.Destroy();
		return true;
	}
	return true;
}