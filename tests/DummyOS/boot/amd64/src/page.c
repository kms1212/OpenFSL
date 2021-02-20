#include "page.h"

void initPageTable(void)
{
	PML4* pml4main;
	PDP* pageDirPointer;
	PD* pageDir;
	DWORD mappingAddress;
	int i;
	
	pml4main = (PML4*)0x100000;
	setPageEntry(&(pml4main[0]), 0x00, 0x101000, 0, PAGE_FLAG_DEFAULT);
	for (i = 1; i < PAGE_MAXENTRY; i++)
	{
		setPageEntry(&(pml4main[i]), 0, 0, 0, 0);
	}
	
	pageDirPointer = (PDP*)0x101000;
	for (i = 0; i < 64; i++)
	{
		setPageEntry(&(pageDirPointer[i]), 0, 0x102000 + (i * PAGE_TABLESIZE), 0, PAGE_FLAG_DEFAULT);
	}
	for (i = 64; i < PAGE_MAXENTRY; i++)
	{
		setPageEntry(&(pageDirPointer[i]), 0, 0, 0, 0);
	}
	
	pageDir = (PD*)0x102000;
	mappingAddress = 0;
	for (i = 0; i < PAGE_MAXENTRY * 64; i++)
	{
		setPageEntry(&(pageDir[i]), (i * (PAGE_DEFAULTSIZE >> 20)) >> 12, mappingAddress, 0, PAGE_FLAG_DEFAULT | PAGE_FLAG_PS);
		mappingAddress += PAGE_DEFAULTSIZE;
	}
}

void setPageEntry(PML4* pml, DWORD upper, DWORD lower, DWORD upperFlags, DWORD lowerFlags)
{
	pml->entryLow = lower | lowerFlags;
	pml->entryHigh = (upper & 0xFF) | upperFlags;
}