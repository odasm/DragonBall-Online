#ifndef _CHARSOCKET_H
#define _CHARSOCKET_H

#include <Socket.h>
#include <boost/asio.hpp>
#include <functional>
#include <Packet.h>

class CharSocket : public Socket
{
public:
	CharSocket(boost::asio::io_service &service, std::function<void(Socket *)> closeHandler);
	bool _ProcessCharPacket(Packet& packet, WORD wOpCode);
	// Packet Management
	bool GetLoginRequest(Packet &packet);
	bool GetCharacterServerList(Packet &packet, bool one);
	bool GetCharacterLoad(Packet &packet);
private:
	virtual bool ProcessIncomingData() override;
	virtual void OnConnectionDone() override;
};
#endif