#include "drivers.h"

void* ATAPIORead(void* dest, uint32_t lba, uint8_t s_s)
{
	ATAPIOWaitBusy();
	portOutB(0x1F6, 0xE0 | ((lba >> 24) & 0xF));
	portOutB(0x1F2, s_s);
	portOutB(0x1F3, (uint8_t)lba);
	portOutB(0x1F4, (uint8_t)(lba >> 8));
	portOutB(0x1F5, (uint8_t)(lba >> 16));
	portOutB(0x1F7, 0x20);
	
	uint16_t* read = (uint16_t*)dest;
	for(int i = 0; i < s_s; i++)
	{
		ATAPIOWaitBusy();
		ATAPIOWaitDRQ();
		for(int j = 0; j < SIZE_SECTOR / 2; j++)
			read[j] = portInW(0x1F0);
		read += SIZE_SECTOR / 2;
	}
	return dest;
}

void* ATAPIOWrite(uint32_t lba, uint8_t s_s, void* src)
{
	ATAPIOWaitBusy();
	portOutB(0x1F6, 0xE0 | ((lba >> 24) & 0xF));
	portOutB(0x1F2, s_s);
	portOutB(0x1F3, (uint8_t)lba);
	portOutB(0x1F4, (uint8_t)(lba >> 8));
	portOutB(0x1F5, (uint8_t)(lba >> 16));
	portOutB(0x1F7, 0x30);
	
	for(int i = 0; i < s_s; i++)
	{
		ATAPIOWaitBusy();
		ATAPIOWaitDRQ();
		for(int j = 0; j < SIZE_SECTOR / 2; j++)
			portOutDW(0x1F0, ((uint32_t*)src)[i]);
	}
	return src;
}

static void ATAPIOWaitBusy()
{
	while(portInB(0x1F7)&ATAPIO_STATUS_BSY);
}

static void ATAPIOWaitDRQ()
{
	while(!portInB(0x1F7)&ATAPIO_STATUS_RDY);
}