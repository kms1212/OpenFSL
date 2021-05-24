#ifndef __FS_FAT32_H
#define __FS_FAT32_H

#include <stdint.h>
#include <string.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include <bitset>
#include <stddef.h>
#include <time.h>

#include "spdlog/spdlog.h"
#include "diskdevice.h"
#include "fslservices.h"

#define FAT32_OPTION_NONE           0x00000000
#define FAT32_OPTION_LFN            0x00000001

#define FAT32_ERROR_SUCCESS         0x00
#define FAT32_ERROR_NOT_INITIALIZED 0x01
#define FAT32_ERROR_DISKDEVICE      0x02
#define FAT32_ERROR_LOGDEVICE       0x03
#define FAT32_ERROR_SIGNATURE       0x04
	
#pragma pack (push, 1)
	typedef struct fatBPBStruct {
		uint8_t        bpbJumpCode[3];
		uint8_t        bpbOEMName[8];
		uint16_t       bpbBytesPerSector;
		uint8_t        bpbSectorPerCluster;
		uint16_t       bpbReservedSectors;
		uint8_t        bpbFATCount;
		uint16_t       bpbRootEntries;
		uint16_t       bpbTotalSectors;
		uint8_t        bpbMedia;
		uint16_t       bpbSectorsPerFAT16;
		uint16_t       bpbSectorsPerTrack;
		uint16_t       bpbDiskHeads;
		uint32_t       bpbHiddenSectors;
		uint32_t       bpbTotalSector32;
		
		uint32_t       ebpbSectorsPerFAT32;
		uint16_t       ebpbFlags;
		uint16_t       ebpbFSVersion;
		uint32_t       ebpbRootDirectoryCluster;
		uint16_t       ebpbFSInfoSector;
		uint16_t       ebpbBSBackupSector;
		uint8_t        ebpbReserved1[12];
		uint8_t        ebpbPhysicalDriveNum;
		uint8_t        ebpbReserved2;
		uint8_t        ebpbExtendedBootSign;
		uint32_t       ebpbVolumeSerial;
		uint8_t        ebpbVolumeLabel[11];
		uint8_t        ebpbFSType[8];
		
		uint8_t        bpbBootCode[420];
		uint16_t       bpbVBRSignature;
	} FAT32_bpb;
#pragma pack (pop)

#pragma pack (push, 1)
	typedef struct fileTimeStruct {
		uint8_t        time_tenth;
		uint8_t        time_sec;
		uint8_t        time_min;
		uint8_t        time_hour;
		uint8_t        time_day;
		uint8_t        time_month;
		uint16_t       time_year;
	} FAT32_fileInfoTime;
#pragma pack (pop)

#pragma pack (push, 1)
	typedef struct fatFSINFOStruct {
		uint32_t       fsinfoSignature1;
		uint8_t        fsinfoReserved1[480];
		uint32_t       fsinfoSignature2;
		uint32_t       fsinfoFreeCluster;
		uint32_t       fsinfoNextFree;
		uint8_t        fsinfoReserved2[12];
		uint32_t       fsinfoSignature3;
	} FAT32_fsinfo;
#pragma pack (pop)

#pragma pack (push, 1)
	typedef struct fileEntryStruct {
		uint8_t        fileName[8];
		uint8_t        fileExt[3];
		uint8_t        fileAttr;
		uint8_t        fileReserved1;
		uint8_t        fileCreateTenth;
		uint16_t       fileCreateTime;
		uint16_t       fileCreateDate;
		uint16_t       fileAccessDate;
		uint16_t       fileLocationHigh;
		uint16_t       fileModTime;
		uint16_t       fileModDate;
		uint16_t       fileLocationLow;
		uint32_t       fileSize;
	} FAT32_entry;
#pragma pack (pop)

#pragma pack (push, 1)
	typedef struct lfnEntryStruct {
		uint8_t        lfnSeqNum;
		uint16_t       lfnFileName1[5];
		uint8_t        lfnAttr;
		uint8_t        lfnReserved1;
		uint8_t        lfnCheckSum;
		uint16_t       lfnFileName2[6];
		uint16_t       lfnLocationLow;
		uint16_t       lfnFileName3[2];
	} FAT32_lfn;
#pragma pack (pop)

#pragma pack (push, 1)
	typedef struct fileInfoStruct {
		std::string    fileName;
		uint8_t        fileAttr;
		FAT32_fileInfoTime fileCreateTime;
		FAT32_fileInfoTime fileAccessTime;
		FAT32_fileInfoTime fileModTime;
		uint32_t       fileLocation;
		uint32_t       fileSize;
	} FAT32_fileInfo;
#pragma pack (pop)

namespace openFSL {
	class FS_FAT32 {
	private:
		DiskDevice*  dd;
		uint32_t     option;
		uint32_t     errorState = FAT32_ERROR_NOT_INITIALIZED;
		
		uint32_t     volumeID;
		std::string  oemLabel;
		std::string  volumeLabel;
		std::string  fsType;
		uint16_t     sectorPerCluster;
		uint16_t     resvSectorCount;
		uint32_t     totalSector32;
		uint32_t     fatSize32;
		uint16_t     fsinfoSector;
		uint32_t     rootCluster;
		uint32_t     freeCluster;
		uint32_t     nextCluster;
		uint8_t      fatCount;
		
		std::string  pathSeparator;
		std::string  currentPath;
		uint32_t     currentCluster;
		
		Sector*      fatArea;
		uint32_t*    fatClusterList;
	public:
		/* Generator & Destructor */
		FS_FAT32(DiskDevice* dd_, uint32_t option_, std::string pathSeparator_ = "\\");
		~FS_FAT32();
		
		void         initialize();
		void         setDiskDevice(DiskDevice* dd_);
		DiskDevice*  getDiskDevice();
		uint32_t     getState();
		char*        getData();
		std::string  getPath();
		
		uint32_t     getChildCount(std::string path);
		FAT32_fileInfo*  getDirList(std::string path, FAT32_fileInfo* buf);
		
		int          chdir(std::string path, std::vector<std::string>* subdir = NULL);
		uint32_t     getNextCluster(uint32_t cluster);
	}; 
}

#endif