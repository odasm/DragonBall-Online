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
	bool _ProcessCharPacket(Packet& packet);
	void Send(BYTE* pData, int size, int opcode);
private:
	virtual bool ProcessIncomingData() override;
	virtual void OnConnectionDone() override;
};
#endif