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

#include "../diskdevice.h"
#include "../fslservices.h"

#define FAT32_ERROR_SUCCESS         0x00
#define FAT32_ERROR_NOT_INITIALIZED 0x01
#define FAT32_ERROR_DISKDEVICE      0x02
#define FAT32_ERROR_SIGNATURE       0x03


namespace openFSL {
	/**
	 *
	 * @brief FAT32 option
	 * @author kms1212
	 * @details 
	 * Option            | Value
	 * ------------------|----------------
	 * FAT32_OPTION_NONE | 0x00000000
	 * FAT32_OPTION_LFN  | 0x00000001
	 *
	 */
	enum FAT32_Option {
		FAT32_OPTION_NONE =   0x00000000,
		FAT32_OPTION_LFN =    0x00000001
	};
	
	/**
	 *
	 * @brief FAT32 BIOS Parameter Block
	 * @author kms1212
	 * @details
	 * Type       | Name                    | Offset
	 * -----------|-------------------------|----------
	 * uint8_t    | bpbJumpCode[3]          | 0x0000
	 * uint8_t    | bpbOEMName[8]           | 0x0003
	 * uint16_t   | bpbBytesPerSector       | 0x000B
	 * uint8_t    | bpbSectorPerCluster     | 0x000D
	 * uint16_t   | bpbReservedSectors      | 0x000E
	 * uint8_t    | bpbFATCount             | 0x0010
	 * uint16_t   | bpbRootEntries          | 0x0011
	 * uint16_t   | bpbTotalSectors         | 0x0013
	 * uint8_t    | bpbMedia                | 0x0015
	 * uint16_t   | bpbSectorsPerFAT16      | 0x0016
	 * uint16_t   | bpbSectorsPerTrack      | 0x0018
	 * uint16_t   | bpbDiskHeads            | 0x001A
	 * uint32_t   | bpbHiddenSectors        | 0x001C
	 * uint32_t   | bpbTotalSector32        | 0x0020
	 * uint32_t   | ebpbSectorsPerFAT32     | 0x0024
	 * uint16_t   | ebpbFlags               | 0x0028
	 * uint16_t   | ebpbFSVersion           | 0x002A
	 * uint32_t   | ebpbRootDirectoryCluster| 0x002C
	 * uint16_t   | ebpbFSInfoSector        | 0x0030
	 * uint16_t   | ebpbBSBackupSector      | 0x0032
	 * uint8_t    | ebpbReserved1[12]       | 0x0034
	 * uint8_t    | ebpbPhysicalDriveNum    | 0x0040
	 * uint8_t    | ebpbReserved2           | 0x0041
	 * uint8_t    | ebpbExtendedBootSign    | 0x0042
	 * uint32_t   | ebpbVolumeSerial        | 0x0043
	 * uint8_t    | ebpbVolumeLabel[11]     | 0x0047
	 * uint8_t    | ebpbFSType[8]           | 0x0052
	 * uint8_t    | bpbBootCode[420]        | 0x005A
	 * uint16_t   | bpbVBRSignature         | 0x01FE
	 *
	 */
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

	/**
	 *
	 * @brief FAT32 time
	 * @author kms1212
	 * @details
	 * Type       | Name                    
	 * -----------|-------------------------
	 * uint8_t    | time_tenth
	 * uint8_t    | time_sec
	 * uint8_t    | time_min
	 * uint8_t    | time_hour
	 * uint8_t    | time_day
	 * uint8_t    | time_month
	 * uint16_t   | time_year
	 *
	 */
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

	/**
	 *
	 * @brief FAT32 FSINFO structure
	 * @author kms1212
	 * @details
	 * Type       | Name                    | Offset
	 * -----------|-------------------------|----------
	 * uint32_t   | fsinfoSignature1        | 0x0000
	 * uint8_t    | fsinfoReserved1[480]    | 0x0004
	 * uint32_t   | fsinfoSignature2        | 0x01E4
	 * uint32_t   | fsinfoFreeCluster       | 0x01E8
	 * uint32_t   | fsinfoNextFree          | 0x01EC
	 * uint8_t    | fsinfoReserved2[12]     | 0x01F0
	 * uint32_t   | fsinfoSignature3        | 0x01FE
	 *
	 */
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

	/**
	 *
	 * @brief FAT32 directory entry struct
	 * @author kms1212
	 * @details
	 * Type       | Name                    | Offset
	 * -----------|-------------------------|----------
	 * uint8_t    | fileName[8]             | 0x00
	 * uint8_t    | fileExt[3]              | 0x08
	 * uint8_t    | fileAttr                | 0x0B
	 * uint8_t    | fileReserved1           | 0x0C
	 * uint8_t    | fileCreateTenth         | 0x0D
	 * uint16_t   | fileCreateTime          | 0x0E
	 * uint16_t   | fileCreateDate          | 0x10
	 * uint16_t   | fileAccessDate          | 0x12
	 * uint16_t   | fileLocationHigh        | 0x14
	 * uint16_t   | fileModTime             | 0x16
	 * uint16_t   | fileModDate             | 0x18
	 * uint16_t   | fileLocationLow         | 0x1A
	 * uint32_t   | fileSize                | 0x1C
	 *
	 */
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

	/**
	 *
	 * @brief FAT32 directory entry for LFN
	 * @author kms1212
	 * @details
	 * Type       | Name                    | Offset
	 * -----------|-------------------------|----------
	 * uint8_t    | lfnSeqNum               | 0x00
	 * uint16_t   | lfnFileName1[5]         | 0x01
	 * uint8_t    | lfnAttr                 | 0x0B
	 * uint8_t    | lfnReserved1            | 0x0C
	 * uint8_t    | lfnCheckSum             | 0x0D
	 * uint16_t   | lfnFileName2[6]         | 0x0E
	 * uint16_t   | lfnLocationLow          | 0x1A
	 * uint16_t   | lfnFileName3[2]         | 0x1C
	 * 
	 */
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

	/**
	 *
	 * @brief FAT32 file information
	 * @author kms1212
	 * @details
	 * Type               | Name                  
	 * -------------------|-----------------
	 * std::string        | fileName
	 * uint8_t            | fileAttr
	 * FAT32_fileInfoTime | fileCreateTime
	 * FAT32_fileInfoTime | fileAccessTime
	 * FAT32_fileInfoTime | fileModTime
	 * uint32_t           | fileLocation
	 * uint32_t           | fileSize
	 *
	 */
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

	/**
	 *
	 * @brief FAT32 imeplementation
	 * @details Implements FAT32 with configurable options
	 * @author kms1212
	 * @todo File Read, Directory write, File write, FS format, UCS-2 Unicode Codepage
	 *
	 */
	class FS_FAT32 {
	private:
		DiskDevice*  dd;
		FAT32_Option option;	
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
		/**
		 *
		 * @brief FS_FAT32 constructor
		 * @param dd_: Disk device driver class pointer
		 * @param option: FAT32 option     
		 * @param pathSeparator_ = "\": File system path separator
		 *
		 */
		FS_FAT32(DiskDevice* dd_, FAT32_Option option, std::string pathSeparator_ = "\\");
		
		/**
		 *
		 * @brief FS_FAT32 destructor
		 *
		 */
		~FS_FAT32();
		
		/**
		 *
		 * @brief FS_FAT32 intializer
		 * @details Initializes Disk driver and FAT32 class.
		 * Function checks signatures:
		 * @li Jump code in BPB (0xEB 0x58 0x90)
		 * @li VBR Signature in BPB (0x55 0xAA)
		 * @li FSINFO Signature (0x55 0xAA)
		 * 
		 * Warns if free cluster amount is unknown (0xFFFFFFFF)
		 * Warns if path separator is not given (default="\")
		 * Loads FATArea to memory
		 * When Fail:
		 *   If disk fails, the errorState variable is set to FAT32_ERROR_DISKDEVICE
		 *   If it fails to verify signature, the errorState variable is set to FAT32_ERROR_SIGNATURE
		 *
		 */
		void         initialize();
		
		/**
		 *
		 * @brief Disk device setter
		 * @details Sets disk driver class.
		 * @param dd_: Disk device driver 
		 *
		 */
		void         setDiskDevice(DiskDevice* dd_);
		
		/**
		 *
		 * @brief Disk device getter
		 * @details Gets disk driver class.
		 * @return DiskDevice*: Disk driver
		 *
		 */
		DiskDevice*  getDiskDevice();
		
		/**
		 *
		 * @brief Error state getter
		 * @details Gets error state.
		 * @return uint32_t: Error state
		 *
		 */
		uint32_t     getState();
		
		/**
		 *
		 * @brief Path getter
		 * @details Gets current path. 
		 * @return std::string: Current path
		 *
		 */
		std::string  getPath();
		
		/**
		 *
		 * @brief Subitem counter
		 * @details Gets subitem count of working directory. 
		 * @param path: working directory
		 * @return uint32_t: Subitem count
		 *
		 */
		uint32_t     getChildCount(std::string path);
		
		/**
		 *
		 * @brief Directory lister
		 * @details Gets subitem information of working directory. 
		 * The size of file information buffer has to be same or more than the count of subitem of working directory.
		 * @param path: working directory
		 * @param buf: File information buffer
		 * @return uint32_t: Subitem count
		 *
		 */
		FAT32_fileInfo*  getDirList(std::string path, FAT32_fileInfo* buf);
		
		/**
		 *
		 * @brief Change directory
		 * @details Changes working directory.
		 * @param path: working directory
		 * @param subdir = NULL: Parameter for recursive search. DO NOT SET THIS ARGUMENT ARBITRARILY
		 * @return uint32_t: Subitem count
		 *
		 */
		int          chdir(std::string path, std::vector<std::string>* subdir = NULL);
		
		/**
		 *
		 * @brief Next cluster picker
		 * @details Gets next cluster number from FAT Area.
		 * Return Value              | Status
		 * --------------------------|----------------------------------------------------
		 * 0x?0000000                | Free cluster
		 * 0x?0000002 - 0x?FFFFFEF   | Using cluster, points next linked cluster
		 * 0x?FFFFFF0 - 0x?FFFFFF6   | Reserved values
		 * 0x?FFFFFF7                | Bad cluster (function returns 0xFFFFFFF7)
		 * 0x?FFFFFF8 - 0x?FFFFFFF   | End of linked cluster (function returns 0xFFFFFFF8)
		 * @param cluster: Current cluster
		 * @return uint32_t: Next cluster
		 *
		 */
		uint32_t     getNextCluster(uint32_t cluster);
	}; 
}

#endif