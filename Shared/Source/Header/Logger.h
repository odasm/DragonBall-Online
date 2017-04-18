#pragma once

#include <iostream>
#include <stdarg.h>
#include <ConsoleColor.h>
#include "../../../NetworkLib/Source/Header/Packet.h"
#include "../../../NetworkLib/Source/Header/Packet/Packets.h"

#  define ATTR_PRINTF(F,V)

class Log
{
	static Log singleton;
	Log();
public:
	void outString();                                   // any log level
	void outString(const char* str, ...)      ATTR_PRINTF(2, 3);
	// any log level
	void outError(const char* err, ...)       ATTR_PRINTF(2, 3);
	// log level >= 1
	void outBasic(const char* str, ...)       ATTR_PRINTF(2, 3);
	// log level >= 2
	void outDetail(const char* str, ...)      ATTR_PRINTF(2, 3);
	// log level >= 3
	void outDebug(const char* str, ...)       ATTR_PRINTF(2, 3);

	void outTime();
	void outPacketDebugger(Packet* paquet);
	void outPacketDebugger(Packets* paquet);
	static Log *get() noexcept 
	{ // pour obtenir le singleton
		return &singleton;
	}

private:

};

#define sLog Log::get()