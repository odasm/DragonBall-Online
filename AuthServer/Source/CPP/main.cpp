#include <iostream>
#include <AuthServer.h>

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

	std::cout << "\n\n" << std::endl;
	std::cout << "\t  ____                              ____        _ _ " << std::endl;
	std::cout << "\t |  _ \\ _ __ __ _  __ _  ___  _ __ | __ )  __ _| | |" << std::endl;
	std::cout << "\t | | | | '__/ _` |/ _` |/ _ \\| '_ \\|  _ \\ / _` | | |" << std::endl;
	std::cout << "\t | |_| | | | (_| | (_| | (_) | | | | |_) | (_| | | |" << std::endl;
	std::cout << "\t |____/|_|  \\__,_|\\__, |\\___/|_| |_|____/ \\__,_|_|_|" << std::endl;
	std::cout << "\t                  |___/                             " << std::endl;
	std::cout << "\t______           AKCore :O 2017					______\n\n" << std::endl;

	AuthServer *server = new AuthServer();

	server->Start();
	
	while (!stopEvent)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	delete server;
	return 0;
}
void signalHandler(int signum)
{
	std::cout << "Interrupt signal (" << signum << ") received.\n" << std::endl;
	stopEvent = true;
}