#ifndef __ELF_H__
#define __ELF_H__

#include "type.h"

#pragma pack (push, 1)
typedef struct elf64HeaderStruct {
	BYTE  elfMagicNum[4];
	BYTE  elfClass;
	BYTE  elfEndian;
	BYTE  elfVersion;
	BYTE  elfOSABI;
	BYTE  elfReserved[8];
	WORD  elfType;
	WORD  elfMachine;
	DWORD elfVersionMir;
	PNTR  elfEntry;
	PNTR  elfProgramHeaderOffset;
	PNTR  elfSectionHeaderOffset;
	DWORD elfFlags;
	WORD  elfHeaderSize;
	WORD  elfProgramHeaderEntryCount;
	WORD  elfSectionHeaderEntrySize;
	WORD  elfSectionHeaderEntryCount;
	WORD  elfStrtabIndex;
} ELFHeader;
#pragma pack (pop)

#pragma pack (push, 1)
typedef struct elf64ProgramheaderStruct {
	DWORD elfType;
	DWORD elfFlags;
	PNTR  elfSegmentOffset;
	PNTR  elfVirtualAddr;
	PNTR  elfPhysicalAddr;
	QWORD elfFileSize;
	QWORD elfSegmentSize;
	QWORD elfAlign;
} ELFProgramHeader;
#pragma pack (pop)

#endif
