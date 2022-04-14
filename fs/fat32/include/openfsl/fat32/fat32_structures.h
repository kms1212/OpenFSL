/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_FAT32_INCLUDE_OPENFSL_FAT32_FAT32_STRUCTURES_H_
#define FS_FAT32_INCLUDE_OPENFSL_FAT32_FAT32_STRUCTURES_H_

#ifndef FS_FAT32_H_INTERNAL_
#error This file has to be included by fs_fat32.h.
#endif  // FS_FAT32_H_INTERNAL_

 public:
    /**
     * @brief FAT32 file attribute
     */
    enum class FileAttribute : uint8_t {
        ReadOnly = 0x01,
        Hidden = 0x02,
        System = 0x04,
        VolumeID = 0x08,
        Directory = 0x10,
        Archive = 0x20,
        LFN = 0x0F,
        Any = Directory | Archive,
        All = Directory | Archive | VolumeID
    };

    friend inline FAT32::FileAttribute operator|
        (FAT32::FileAttribute a, FAT32::FileAttribute b)
    { return (FAT32::FileAttribute)((uint8_t)a | (uint8_t)b); }

    friend inline FAT32::FileAttribute operator&
        (FAT32::FileAttribute a, FAT32::FileAttribute b)
    { return (FAT32::FileAttribute)((uint8_t)a & (uint8_t)b); }

 private:
    /**
     * @brief FAT32 BIOS Parameter Block
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
     * cluster_t  | ebpbRootDirectoryCluster| 0x002C
     * uint16_t   | ebpbFSInfoSector        | 0x0030
     * uint16_t   | ebpbBSBackupSector      | 0x0032
     * uint8_t    | __Reserved1[12]         | 0x0034
     * uint8_t    | ebpbPhysicalDriveNum    | 0x0040
     * uint8_t    | ebpbReserved2           | 0x0041
     * uint8_t    | ebpbExtendedBootSign    | 0x0042
     * uint32_t   | ebpbVolumeSerial        | 0x0043
     * uint8_t    | ebpbVolumeLabel[11]     | 0x0047
     * uint8_t    | ebpbFSType[8]           | 0x0052
     * uint8_t    | bpbBootCode[420]        | 0x005A
     * uint16_t   | bpbVBRSignature         | 0x01FE
     */
#pragma pack(push, 1)
    typedef struct BPBSector {
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
        cluster_t      ebpbRootDirectoryCluster;
        uint16_t       ebpbFSInfoSector;
        uint16_t       ebpbBSBackupSector;
        uint8_t        __Reserved1[12];
        uint8_t        ebpbPhysicalDriveNum;
        uint8_t        ebpbReserved2;
        uint8_t        ebpbExtendedBootSign;
        uint32_t       ebpbVolumeSerial;
        uint8_t        ebpbVolumeLabel[11];
        uint8_t        ebpbFSType[8];

        uint8_t        bpbBootCode[420];
        uint16_t       bpbVBRSignature;
    } BPBSector;
#pragma pack(pop)

    /**
     * @brief FAT32 FSINFO structure
     * @details
     * Type       | Name                    | Offset
     * -----------|-------------------------|----------
     * uint32_t   | fsinfoSignature1        | 0x0000
     * uint8_t    | fsinfoReserved1[480]    | 0x0004
     * uint32_t   | fsinfoSignature2        | 0x01E4
     * cluster_t  | fsinfoFreeCluster       | 0x01E8
     * cluster_t  | fsinfoNextFree          | 0x01EC
     * uint8_t    | fsinfoReserved2[12]     | 0x01F0
     * uint32_t   | fsinfoSignature3        | 0x01FE
     *
     */
#pragma pack(push, 1)
    typedef struct FSINFOSector {
        uint32_t       fsinfoSignature1;
        uint8_t        fsinfoReserved1[480];
        uint32_t       fsinfoSignature2;
        cluster_t      fsinfoFreeCluster;
        cluster_t      fsinfoNextFree;
        uint8_t        fsinfoReserved2[14];
        uint16_t       fsinfoSignature3;
    } FSINFOSector;
#pragma pack(pop)

    /**
     * @brief FAT32 directory entry struct
     * @details
     * Type         | Name                    | Offset
     * -------------|-------------------------|----------
     * uint8_t      | fileName[8]             | 0x00
     * uint8_t      | fileExt[3]              | 0x08
     * FileAttribute| fileAttr                | 0x0B
     * uint8_t      | fileReserved1           | 0x0C
     * uint8_t      | fileCreateTenth         | 0x0D
     * uint16_t     | fileCreateTime          | 0x0E
     * uint16_t     | fileCreateDate          | 0x10
     * uint16_t     | fileAccessDate          | 0x12
     * uint16_t     | fileLocationHigh        | 0x14
     * uint16_t     | fileModTime             | 0x16
     * uint16_t     | fileModDate             | 0x18
     * uint16_t     | fileLocationLow         | 0x1A
     * uint32_t     | fileSize                | 0x1C
     *
     */
#pragma pack(push, 1)
    typedef struct FileEntry {
        uint8_t        fileName[8];
        uint8_t        fileExt[3];
        FileAttribute  fileAttr;
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
    } FileEntry;
#pragma pack(pop)

    /**
     * @brief FAT32 directory entry for LFN
     * @details
     * Type         | Name                    | Offset
     * -------------|-------------------------|----------
     * uint8_t      | lfnSeqNum               | 0x00
     * uint16_t     | lfnFileName1[5]         | 0x01
     * FileAttribute| lfnAttr                 | 0x0B
     * uint8_t      | lfnReserved1            | 0x0C
     * uint8_t      | lfnCheckSum             | 0x0D
     * uint16_t     | lfnFileName2[6]         | 0x0E
     * uint16_t     | lfnLocationLow          | 0x1A
     * uint16_t     | lfnFileName3[2]         | 0x1C
     * 
     */
#pragma pack(push, 1)
    typedef struct LFNEntry {
        uint8_t        lfnSeqNum;
        uint16_t       lfnFileName1[5];
        FileAttribute  lfnAttr;
        uint8_t        lfnReserved1;
        uint8_t        lfnCheckSum;
        uint16_t       lfnFileName2[6];
        uint16_t       lfnLocationLow;
        uint16_t       lfnFileName3[2];
    } LFNEntry;
#pragma pack(pop)

#endif  // FS_FAT32_INCLUDE_OPENFSL_FAT32_FAT32_STRUCTURES_H_
