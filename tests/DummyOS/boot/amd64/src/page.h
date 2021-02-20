#ifndef __PAGE_H__
#define __PAGE_H__

#include "type.h"

#define PAGE_FLAG_P       0x00000001
#define PAGE_FLAG_RW      0x00000002
#define PAGE_FLAG_US      0x00000004
#define PAGE_FLAG_PWT     0x00000008
#define PAGE_FLAG_PCD     0x00000010
#define PAGE_FLAG_A       0x00000020
#define PAGE_FLAG_D       0x00000040
#define PAGE_FLAG_PS      0x00000080
#define PAGE_FLAG_G       0x00000100
#define PAGE_FLAG_PAT     0x00001000
#define PAGE_FLAG_EXB     0x80000000
#define PAGE_FLAG_DEFAULT (PAGE_FLAG_P | PAGE_FLAG_RW)
#define PAGE_TABLESIZE    0x1000
#define PAGE_MAXENTRY     512
#define PAGE_DEFAULTSIZE  0x200000

#pragma pack (push, 1)
typedef struct pageTableEntryStruct {
	DWORD entryLow;
	DWORD entryHigh;
}PML4, PDP, PD, PT;
#pragma pack (pop)

void initPageTable(void);
void setPageEntry(PML4* pml, DWORD upper, DWORD lower, DWORD upperFlags, DWORD lowerFlags);

#endif
