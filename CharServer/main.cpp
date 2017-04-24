#include <iostream>
#include <CharServer.h>
#include <signal.h>

void signalHandler(int signum);
bool stopEvent = false;

int main()
{
	signal(SIGINT, signalHandler);
	signal(SIGABRT, signalHandler);
	signal(SIGFPE, signalHandler);
	signal(SIGILL, signalHandler);
	signal(SIGSEGV, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGBREAK, signalHandler);

	std::cout << blue << "\n\n" << std::endl;
	std::cout << "\t     ____                              ____        _ _ " << std::endl;
	std::cout << "\t    |  _ \\ _ __ __ _  __ _  ___  _ __ | __ )  __ _| | |" << std::endl;
	std::cout << "\t    | | | | '__/ _` |/ _` |/ _ \\| '_ \\|  _ \\ / _` | | |" << std::endl;
	std::cout << "\t    | |_| | | | (_| | (_| | (_) | | | | |_) | (_| | | |" << std::endl;
	std::cout << "\t    |____/|_|  \\__,_|\\__, |\\___/|_| |_|____/ \\__,_|_|_|" << std::endl;
	std::cout << "\t                     |___/                             " << std::endl;
	std::cout << yellow << "\t   	           AKCore 2017					\n\n" << white << std::endl;

	CharServer *server = new CharServer();

	if (server->Start() == false)
	{
		delete server;
		return -1;
	}

	while (!stopEvent)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	delete server;
	system("PAUSE");
	return 0;
}
void signalHandler(int signum)
{
	sLog.outError("Interrupt signal (%d) received.", signum);
	system("PAUSE");
	stopEvent = true;
}