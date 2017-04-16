#ifndef _AUTHSOCKET_H
#define _AUTHSOCKET_H

#include <Socket.h>
#include <boost/asio.hpp>
#include <functional>
#include <Packet.h>

struct REALM_RESULT;

class AuthSocket : public Socket
{
public:
	AuthSocket(boost::asio::io_service &service, std::function<void(Socket *)> closeHandler);
	bool _HandleOnLogin(Packet& packet);
private:
	bool _authed;
	virtual bool ProcessIncomingData() override;
	virtual void OnConnectionDone() override;
};
#endif