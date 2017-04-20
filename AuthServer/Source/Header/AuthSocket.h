#ifndef _AUTHSOCKET_H
#define _AUTHSOCKET_H

#include <Socket.h>
#include <boost/asio.hpp>
#include <functional>
#include <Packet.h>
#include <Packet\Packets.h>

class AuthSocket : public Socket
{
public:
	AuthSocket(boost::asio::io_service &service, std::function<void(Socket *)> closeHandler);
	bool _HandleOnLogin(Packet& packet);
	bool _ProcessLoginPacket(Packet& packet);
	void Send(BYTE* pData, int size, int opcode);
private:
	bool _authed;
	virtual bool ProcessIncomingData() override;
	virtual void OnConnectionDone() override;
};
#endif