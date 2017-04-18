#include <Packet\Packets.h>
#include <Logger.h>
#include <Define.h>

Packets::Packets()
{
	Init();
}

Packets::~Packets()
{
	Destroy();
}
void Packets::Destroy()
{
	pHeader = NULL;
	pData = NULL;
	SAFE_FREE(mallocBuffer);
}
void Packets::Init()
{
	pHeader = NULL;
	pData = NULL;
	mallocBuffer = NULL;
}
void Packets::Realloc(int nSize)
{
	SAFE_FREE(mallocBuffer);
	mallocBuffer = (BYTE*)calloc(nSize, sizeof(BYTE));
}
void Packets::Alloc(WORD pSize)
{
	Realloc(pSize);
	if (mallocBuffer) // check inc value
	{
		pData = (BYTE*)&mallocBuffer[4];
		pHeader = (HEADER*)&mallocBuffer[0];
	}
}
void Packets::FillPacket(BYTE* pBuffer, WORD pSize)
{
	Realloc(pSize);
	if (mallocBuffer) // check inc value
	{
		if (pBuffer)
			memcpy(mallocBuffer, pBuffer, pSize);
		pData = (BYTE*)&mallocBuffer[4];
		pHeader = (HEADER*)&mallocBuffer[0];
	}
}
void Packets::setLenght(int value)
{
	/*unsigned short converted = static_cast<unsigned short>(value);
	setShort(0, ((getShort(0) & ~0x7FFF) | (converted & 0x7FFF)));*/
	pHeader->wPacketLen = static_cast<WORD>(value);
	sLog->outDebug("Set Lenght: %u", pHeader->wPacketLen);
}
unsigned short Packets::getLenght()
{
	//return static_cast<unsigned short>(getShort(0) & 0x7FFF);
	
	sLog->outDebug("Get Lenght: %u", pHeader->wPacketLen);
	return pHeader->wPacketLen;
}
void Packets::setEncrypt(int value)
{
	//unsigned short converted = (value);
	//setShort(0, ((getShort(0) & (~0x7FFF << 15)) | (converted & 0x7FFF) << 15));

	pHeader->bEncrypt = static_cast<WORD>(value);
	sLog->outDebug("Set Encrypt: %u %d", pHeader->bEncrypt, value);
}
bool Packets::getEncrypt()
{
	//unsigned short value = static_cast<unsigned short>(((getShort(0) >> 15) & 0x7FFF));
	//unsigned short value = (((data[0] >> 15) & 0x7FFF));

	sLog->outDebug("Get Encrypt: %u", pHeader->bEncrypt);
	if (pHeader->bEncrypt == true)
		return true;
	else
		return false;
}
void Packets::setOpcode(int value)
{
	pHeader->bySequence = static_cast<BYTE>(value);
}
unsigned short Packets::getOpcode()
{
	return static_cast<WORD>(pHeader->bySequence);
}

void Packets::setShort(int position, unsigned short value)
{
	if (pData)
	{
		pData[position] = value;
	}
}
unsigned short Packets::getShort(int position)
{
	unsigned short value = -1;
	if (pData)
		value = static_cast<unsigned short>(pData[position]);
	return value;
}
void Packets::setString(int position, char* text, int size)
{
	memcpy(&pData[position], &text[0], size);
}
char* Packets::getString(int position, int size)
{
	char *data = new char(size);
	memcpy(&data[0], &pData[position], size);
	return data;
}