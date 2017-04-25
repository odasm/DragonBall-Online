#ifndef _GAMESOCKET_H
#define _GAMESOCKET_H

class GameSocket : public Socket
{
public:
	GameSocket(boost::asio::io_service &service, std::function<void(Socket *)> closeHandler);

private:
	virtual bool ProcessIncomingData() override;
	virtual void OnConnectionDone() override;
	virtual void OnClosed() override;
};
#endif