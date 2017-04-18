#ifndef _AUTHSOCKET_H
#define _AUTHSOCKET_H

#include <Socket.h>
#include <boost/asio.hpp>
#include <functional>
#include <Packet.h>
#include <Packet\Packets.h>

struct REALM_RESULT;

class AuthSocket : public Socket
{
public:
	AuthSocket(boost::asio::io_service &service, std::function<void(Socket *)> closeHandler);
	bool _HandleOnLogin(Packet& packet);
	void Send(void* pData, int size);
private:
	bool _authed;
	virtual bool ProcessIncomingData() override;
	virtual void OnConnectionDone() override;
};
#endif