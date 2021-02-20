#ifndef __FAT_H__
#define __FAT_H__

#include "type.h"

#pragma pack (push, 1)
typedef struct fatBPBStruct {
	BYTE  bpbJumpCode[3];
	BYTE  bpbOEMName[8];
	WORD  bpbBytesPerSector;
	BYTE  bpbSectorPerCluster;
	WORD  bpbReservedSectors;
	BYTE  bpbFATCount;
	WORD  bpbRootEntries;
	WORD  bpbTotalSectors;
	BYTE  bpbMedia;
	WORD  bpbSectorsPerFAT16;
	WORD  bpbSectorsPerTrack;
	WORD  bpbDiskHeads;
	DWORD bpbHiddenSectors;
	DWORD bpbTotalLargeSectors;
	
	DWORD ebpbSectorsPerFAT32;
	WORD  ebpbFlags;
	WORD  ebpbFSVersion;
	DWORD ebpbRootDirectoryCluster;
	WORD  ebpbFSInfoSector;
	WORD  ebpbBSBackupSector;
	BYTE  ebpbReserved1[12];
	BYTE  ebpbPhysicalDriveNum;
	BYTE  ebpbReserved2;
	BYTE  ebpbExtendedBootSign;
	DWORD ebpbVolumeSerial;
	BYTE  ebpbVolumeLabel[11];
	BYTE  ebpbFSType[8];
} BPB;
#pragma pack (pop)

#pragma pack (push, 1)
typedef struct fileEntryStruct {
	BYTE  fileName[8];
	BYTE  fileExt[3];
	BYTE  fileAttr;
	BYTE  fileReserved1;
	BYTE  fileCreateMillis;
	WORD  fileCreateTime;
	WORD  fileCreateDate;
	WORD  fileAccessDate;
	WORD  fileLocationHigh;
	WORD  fileModTime;
	WORD  fileModDate;
	WORD  fileLocationLow;
	DWORD fileSize;
	
} FILE;
#pragma pack (pop)

#endif
