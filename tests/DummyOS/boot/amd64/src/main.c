#include "type.h"
#include "memory.h"
#include "string.h"
#include "page.h"
#include "swamd64.h"

void main(void)
{
	printString(0, 1, "Memory 32MiB ");
	int ret = memtest((void*)0x2000000);
	if (ret)
		printString(13, 1, "OK");
	else {
		printString(13, 1, "Error");
		while (1);
	}
	
	printString(0, 2, "Page Table Initialization ");
	initPageTable();
	printString(26, 2, "OK");
	
	printString(0, 3, "CPU Vendor : ");
	DWORD eax, ebx, ecx, edx;
	char vendorString[13] = { 0, };
	getCPUID(0x00, &eax, &ebx, &ecx, &edx);
	*(DWORD*)vendorString = ebx;
	*((DWORD*)vendorString + 1) = edx;
	*((DWORD*)vendorString + 2) = ecx;
	printString(13, 3, vendorString);
	
	printString(0, 4, "AMD64 Instruction Set ");
	getCPUID(0x80000001, &eax, &ebx, &ecx, &edx);
	if (edx & (1 << 29))
		printString(22, 4, "Available");
	else {
		printString(22, 4, "Not Available");
		while (1);
	}
	
	printString(0, 5, "Copying Bootloader To Upper Area ");
	memset((void*)0x200000, 0x00, 0x200 * 13);
	memcpy((void*)0x200000, (void*)0x8800, 0x200 * 13);
	printString(33, 5, "OK");
	
	printString(0, 6, "Switching Mode to AMD64 ");
	switch64();

	while (1);
}