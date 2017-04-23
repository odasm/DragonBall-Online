#ifndef DEFINE_H
#define DEFINE_H

#  define WIN32_LEAN_AND_MEAN
#  include <Windows.h>
#  ifndef _WIN32_WINNT
#    define _WIN32_WINNT 0x0603
#  endif

typedef std::int64_t int64;
typedef std::int32_t int32;
typedef std::int16_t int16;
typedef std::int8_t int8;
typedef std::uint64_t uint64;
typedef std::uint32_t uint32;
typedef std::uint16_t uint16;
typedef std::uint8_t uint8;

#pragma warning(disable : 4328)

//------------------------------------------------------------------
enum eDBO_SERVER_STATUS
{
	DBO_SERVER_STATUS_UP = 0,
	DBO_SERVER_STATUS_DOWN,
	DBO_SERVER_STATUS_LOCKED
};
//------------------------------------------------------------------
//------------------------------------------------------------------
typedef unsigned int ACCOUNTID;
typedef BYTE SERVERFARMID;
const SERVERFARMID INVALID_SERVERFARMID = 0xFF;
//------------------------------------------------------------------
typedef unsigned char SLOTID;
const SLOTID INVALID_SLOTID = 0xFF;
//------------------------------------------------------------------
typedef unsigned int CHARACTERID;
const CHARACTERID INVALID_CHARACTERID = 0xFFFFFFFF;
struct sPACKETHEADER
{
	sPACKETHEADER(BYTE wGivenOpCode) :
		wOpCode(wGivenOpCode) {}
	BYTE		wPacketSize;
	BYTE		bEncrypt;
	WORD		wOpCode;
	//BYTE		byChecksum;
};
//------------------------------------------------------------------
//------------------------------------------------------------------
//
//------------------------------------------------------------------
#define BEGIN_PROTOCOL(opcode)						\
struct s##opcode :									\
	public sPACKETHEADER							\
{													\
	s##opcode() :									\
		sPACKETHEADER(opcode)						\
	{												\
	}

#define END_PROTOCOL()	};
/*#define BEGIN_PROTOCOL(opcode)						\
struct s##opcode 									\
{													\
s##opcode()										\
{												\
}
#define END_PROTOCOL()	};*/
//------------------------------------------------------------------
//------------------------------------------------------------------
//
//------------------------------------------------------------------
#define BEGIN_PROTOCOL_IDENTITY( opcode, identity )	\
typedef s##identity s##opcode						\

#define END_PROTOCOL_IDENTITY()	;
//------------------------------------------------------------------


//------------------------------------------------------------------
#define DECLARE_PACKET_NAME( opcode )	{ #opcode }
//------------------------------------------------------------------
#endif DEFINE_H