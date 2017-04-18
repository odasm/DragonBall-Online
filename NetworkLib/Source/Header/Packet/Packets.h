#ifndef __PACKETS__H
# define __PACKETS__H

#include <Windows.h>
#include <stdlib.h>
#include <vector>

#define SAFE_FREE(ptr) if (ptr) free(ptr)

typedef struct HEADER
{
	WORD		bEncrypt : 1;
	WORD		wPacketLen : 15;
	BYTE		bySequence;
	BYTE		byChecksum;
};

class Packets
{
public:
	Packets();
	~Packets();
	void Destroy();
	void FillPacket(BYTE* pBuffer, WORD pSize);
	void Alloc(WORD pSize);
	BYTE* GetPacketData() { return pData; }
	HEADER* GetPacketHeader() { return pHeader; }
	void BuildPacket()
	{
		setEncrypt(false);
		setLenght(static_cast<unsigned short>(4));
	}

	/**
	* Packet Size (Offset 0) & 0x7FFF
	**/
	void setLenght(int value);
	unsigned short getLenght();
	/**
	* Packet Encrypt (Offset 0) 1 BIT
	**/
	void setEncrypt(int value);
	bool getEncrypt();
	/**
	* Packet Opcode (Offset 2)
	**/
	void setOpcode(int value);
	unsigned short getOpcode();
private:
	void Init();
	void Realloc(int nSize);
	BYTE* mallocBuffer;
	BYTE* pData;
	HEADER* pHeader;
public:
	void setShort(int position, unsigned short value);
	unsigned short getShort(int position);
	void setString(int position, char* text, int size);
	char* getString(int position, int size);
};

#endif // !__PACKETS__H
