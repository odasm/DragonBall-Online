#include <CharSocket.h>
#include <Logger.h>
#include <stdio.h>
#include <cstdio>
#include <Opcodes.h>
#include <Encryptation.h>
#include <mysqlconn_wrapper.h>

CharSocket::CharSocket(boost::asio::io_service &service, std::function<void(Socket *)> closeHandler)
	: Socket(service, closeHandler)
{
}
void CharSocket::OnConnectionDone()
{
	uint8 rawData[] = { 0x06, 0x00, 0x03, 0x00, 0x30, 0x2C, 0x67, 0x4C };

	Write((char*)rawData, sizeof(rawData));
	memset(&rawData, 0, sizeof(rawData));
}
void CharSocket::Send(BYTE* pData, int size, int opcode)
{
	Packet packet(size);
	packet.SetPacket(pData, size);

	packet.GetPacketHeader()->bySequence = opcode;
	packet.GetPacketHeader()->byChecksum = 7;

	sLog->outPacketDebugger(&packet);
	Write((char*)&packet, size);
}
bool CharSocket::_ProcessCharPacket(Packet& packet)
{
}
bool CharSocket::ProcessIncomingData()
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
		else
		{
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