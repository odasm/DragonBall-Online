#include <iostream>
#include <GameServer.h>
#include <signal.h>

void signalHandler(int signum);
bool stopEvent = false;

int main()
{
	return sGameServer.Run();
}
