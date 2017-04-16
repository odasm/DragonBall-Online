#include <Logger.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <cstdarg>
#include <time.h>
#include <ctime>

Log Log::singleton;

Log::Log()
{
}
void Log::outTime()
{
	time_t rawtime;
	struct tm * timeinfo = new struct tm;
	char buffer[80];

	time(&rawtime);
	localtime_s(timeinfo, &rawtime);
	strftime(buffer, sizeof(buffer), "%I:%M:%S", timeinfo);
	std::string str(buffer);
	std::cout << white << "[" << str.c_str() << "] " << white;
	delete timeinfo;
}
void Log::outString()
{
	printf("\n");
	fflush(stdout);
}
void Log::outString(const char* str, ...)
{
	if (!str)
		return;

	va_list ap;

	va_start(ap, str);
	vprintf(str, ap);
	va_end(ap);

	printf("\n");
	fflush(stdout);
}

void Log::outError(const char* err, ...)
{
	if (!err)
		return;

	va_list ap;
	outTime();
	std::cout << red << "[ERROR]: " << white;
	va_start(ap, err);
	vprintf(err, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	fflush(stderr);
}
void Log::outDetail(const char* str, ...)
{
	if (!str)
		return;

	va_list ap;
	outTime();
	std::cout << yellow << "[INFO]: " << white;
	va_start(ap, str);
	vprintf(str, ap);
	va_end(ap);
	printf("\n");
	fflush(stdout);
}
void Log::outDebug(const char* str, ...)
{
	if (!str)
		return;

	va_list ap;
	outTime();
	std::cout << blue << "[DEBUG]: " << white;
	va_start(ap, str);
	vprintf(str, ap);
	va_end(ap);
	printf("\n");
	fflush(stdout);
}
void Log::outPacketDebugger(Packet* paquet)
{
	if (paquet == nullptr)
	{
		sLog->outError("Packet logger: null value");
		return;
	}
	outTime();
	std::cout << green << "[PACKET]: " << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	std::cout << "\tPacket Length: \t\t\t" << yellow << paquet->GetPacketLen() << std::endl;
	void* addr = paquet->GetPacketData();
	std::cout << green << "\ttPacket Data: \t\t\t" << yellow << &addr << std::endl;
	std::cout << green << "\ttPacket Data Size: \t\t" << yellow << paquet->GetPacketDataSize() << std::endl;
	std::cout << green << "\ttPacket Used Size: \t\t" << yellow << paquet->GetUsedSize() << std::endl;
	
	std::cout << green << "\tHeader Encrypted: \t\t" << yellow << (paquet->GetPacketHeader()->bEncrypt ? "true" : "false") << std::endl;
	std::cout << green << "\ttHeader Length: \t\t" << yellow << (int)paquet->GetHeaderSize() << std::endl;
	std::cout << green << "\ttHeader byChecksum: \t\t" << yellow << (int)paquet->GetPacketHeader()->byChecksum << std::endl;
	std::cout << green << "\ttHeader bySequence: \t\t" << yellow << (int)paquet->GetPacketHeader()->bySequence << std::endl;
	std::cout << green  << "-----------------------------------" << std::endl;
	std::cout << white;
}