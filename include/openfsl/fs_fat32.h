/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

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
#include <list>
#include <stddef.h>
#include <time.h>

#include "diskdevice.h"
#include "fslservices.h"
#include "file.h"


namespace openFSL {

    /**
     *
     * @brief FAT32 imeplementation
     * @details Implements FAT32 with configurable options
     * @author kms1212
     * @todo Directory write, File write, FS format, UCS-2 Unicode Codepage, Make class thread-safe
     *
     */
    class FS_FAT32 {
        private:
            
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
         * @brief FAT32 cache entry
         * @author kms1212
         * @details
         * Type               | Name                  
         * -------------------|-----------------
         * uint32_t           | cacheIndex
         * std::string        | cacheKey
         * uint8_t            | cacheType
         * uint8_t*           | cacheData
         */
        typedef struct cacheEntryStruct {
            uint32_t       cacheIndex = 0;
            std::string    cacheKey = "";
            uint8_t        cacheType = 0;
            uint8_t*       cacheData = NULL;
        } FAT32_cacheEntry;
    
        DiskDevice*  dd;
        bool         isDiskDeviceAllocated;
        
        std::string  pathSeparator;
        std::string  currentPath;
        uint32_t     currentCluster;
        
        uint32_t*    fatClusterList;
        FAT32_bpb* bpb;
        FAT32_fsinfo* fsinfo;
    public:
        class FILE;
        
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
        typedef struct fileInfoStruct {
            std::string    fileName = "";
            uint8_t        fileAttr = 0;
            FSL_Time       fileCreateTime = {0};
            FSL_Time       fileAccessTime = {0};
            FSL_Time       fileModTime = {0};
            uint32_t       fileLocation = 0;
            uint32_t       fileSize = 0;
        } FileInfo;
        
        /**
         *
         * @brief FS_FAT32 constructor
         * @param dd_: Disk device driver class pointer
         * @param option: FAT32 option (See enum FAT32_Option)
         * @param pathSeparator_: File system path separator (Default: "\")
         *
         */
        FS_FAT32(DiskDevice* dd_, std::string pathSeparator_ = "\\");
        
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
         * @return int: Number of errors
         *
         */
        int initialize();
        
        /**
         *
         * @brief Disk device setter
         * @details Sets disk driver class.
         * @param dd_: Disk device driver 
         *
         */
        void setDiskDevice(DiskDevice* dd_);
        
        /**
         *
         * @brief Disk device getter
         * @details Gets disk driver class.
         * @return DiskDevice*: Disk driver
         *
         */
        DiskDevice* getDiskDevice();
        
        /**
         *
         * @brief Path getter
         * @details Gets current path. 
         * @return std::string: Current path
         *
         */
        std::string getPath();
        
        /**
         *
         * @brief BPB pointer getter
         * @details Gets BPB pointer. 
         * @return FAT32_bpb: BPB pointer
         *
         */
        FAT32_bpb* getBPB();
        
        /**
         *
         * @brief Subitem counter
         * @details Gets subitem count of working directory. 
         * @param path: working directory (Default: "")
         * @return uint32_t: Subitem count
         *
         */
        uint32_t getChildCount(std::string path = "");
        
        /**
         *
         * @brief Directory lister
         * @details Gets subitem information of working directory. 
         * The size of file information buffer has to be same or more than the count of subitem of working directory.
         * @param buf: File information buffer
         * @param path: working directory (Default: "")
         * @param cluster: working cluster (Default: 0xFFFFFFFF)
         * @return std::vector<FAT32_fileInfo>: Parameter buf
         *
         */
        std::vector<FileInfo>* getDirList(std::vector<FileInfo>* buf, std::string path = "", uint32_t cluster = 0xFFFFFFFF);
        
        /**
         *
         * @brief Change directory
         * @details Changes working directory.
         * @param path: working directory
         * @param subdir: Parameter for recursive search. DO NOT SET THIS ARGUMENT ARBITRARILY (Default: NULL)
         * @param tmpPath: Parameter for recursive search. DO NOT SET THIS ARGUMENT ARBITRARILY (Default: "")
         * @param tmpCluster: Parameter for recursive search. DO NOT SET THIS ARGUMENT ARBITRARILY (Default: 0)
         * @return int: Error code
         *
         */
        int chdir(std::string path, std::vector<std::string>* subdir = NULL, std::string tmpPath = "", uint32_t tmpCluster = 0);
        
        /**
         *
         * @brief Get Last Cluster
         * @details Returns last index of linked cluster
         * @param cluster: Current cluster
         * @return uint32_t: Last cluster
         *
         */
        uint32_t getLastCluster(uint32_t cluster);
        
        /**
         *
         * @brief Get Next Free Cluster
         * @details Returns next free cluster (Next Available)
         * @return uint32_t: Next free cluster
         *
         */
        uint32_t getNextFreeCluster();
        
        /**
         *
         * @brief Allocate free cluster
         * @details Gets next free cluster and 
         * @param cluster: linked cluster address to allocate
         * @return uint32_t: allocated cluster
         *
         **/
        uint32_t allocateFreeCluster(uint32_t cluster);
        
        /**
         *
         * @brief Get file information
         * @details Gets file information to open file.
         * If file is not found, it returns an empty FAT32_fileInfo structure.
         * @param path: file path
         * @return FAT32_fileInfo: File information
         *
         */
        FileInfo getFileInformation(std::string path);
        
        /**
         *
         * @brief Get FAT Area cluster list
         * @details Gets FAT Area list.
         * @return uint32_t*: FAT Area cluster array
         *
         */
        uint32_t*    getFatClusterList();
        
        /**
         *
         * @brief Make directory
         * @details Makes directory
         * @param path: working directory + directory to create (no recursive)
         * @return int: Error code
         * @todo recursive directory creation
         *
         */
        int mkdir(std::string path);
        
        /**
         *
         * @brief Open file
         * @details Opens file and create FILE class
         * @param path: working directory + filename
         * @param mode_: file open mode
         * @return FSL_File*: file pointer
         *
         */
        FSL_File<FS_FAT32>* openFile(std::string path, FSL_OpenMode mode_);
        
        /**
         *
         * @brief Close file 
         * @details Closes file
         * @param file: file pointer
         *
         */
        void closeFile(FSL_File<FS_FAT32>* file);

        /**
         *
         * @brief FAT32 linked cluster
         * @details Implements cluster controls
         * @author kms1212
         *
         */
        class LinkedCluster {
        private:
            FS_FAT32* fileSystem;
            
            uint32_t* clusterList;
            
            Sector* fetchedSectorCache = NULL;
            size_t currentCacheClusterIndex = 0;
            size_t currentCacheClusterSize = 0;
            
            size_t lastFetchedCluster;
            
            int getLinkedClusterList(uint32_t* list, uint32_t cluster, size_t count = 0);
            size_t getLinkedClusterCount(uint32_t cluster);
            uint32_t getNextCluster(uint32_t cluster);
            
            size_t clusterToLBA(size_t cluster);
        public:
            LinkedCluster(FS_FAT32* fileSystem_, size_t startCluster);
            ~LinkedCluster();
            
            int fetch(void* buf, size_t pos, size_t size);
            
            int setCache(size_t cluster, size_t size);
            int shrinkCache(size_t clusterSize);
        };

        /**
         *
         * @brief FAT32 file
         * @details Implements fat32 file controls
         * @author kms1212
         *
         */
        class FILE {
        private:
            FS_FAT32::FileInfo fileInfo;
            FS_FAT32* fileSystem;
            FSL_OpenMode openMode;
            
            size_t         seekLocation = 0;
            LinkedCluster* fileCluster = NULL;
        public:
            
            FILE(FS_FAT32* fileSystem_, FS_FAT32::FileInfo fileInfo_, FSL_OpenMode openMode_);
			~FILE();
            
            FS_FAT32::FileInfo getFileInfo();

            /**
             *
             * @brief Read file
             * @details Reads file from disk
             * @param void* buf: Data buffer
             * @param size_t size: Element size
             * @param size_t count: Element count
             * @return size_t: Count of elements actually read
             *
             */
            size_t read(void* buf, size_t size, size_t count);
            
            /**
             *
             * @brief Seek file
             * @details Seek file location
             * @param uint32_t location: Seek location
             *
             */
            int seek(size_t location);
        };
    }; 
}

#endif