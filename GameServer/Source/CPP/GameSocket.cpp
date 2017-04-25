#include <GameServer.h>
#include <Logger.h>
#include <stdio.h>
#include <cstdio>
#include <Opcodes.h>
#include <Encryptation.h>
#include <mysqlconn_wrapper.h>

GameSocket::GameSocket(boost::asio::io_service &service, std::function<void(Socket *)> closeHandler)
	: Socket(service, closeHandler)
{
}
void GameSocket::OnConnectionDone()
{
	uint8 rawData[] = { 0x06, 0x00, 0x03, 0x00, 0x30, 0x2C, 0x67, 0x4C };

	Write((char*)rawData, sizeof(rawData));
	memset(&rawData, 0, sizeof(rawData));
	sLog.outDebug("Client connected: [%s]", m_address);
}
void GameSocket::OnClosed()
{
	sLog.outDebug("Client disconnected: [%s]", m_address);
}
bool GameSocket::ProcessIncomingData()
{
	while (ReadLengthRemaining() > 0)
	{
		ReadSkip(ReadLengthRemaining());
		return true;
	}
}