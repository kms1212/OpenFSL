/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_SPECIALSECTOR_H_
#define FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_SPECIALSECTOR_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <cstddef>

#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <utility>
#include <map>

#include "openfsl/blockdevice.h"
#include "openfsl/sector.h"
#include "openfsl/filesystem.h"
#include "openfsl/file.h"
#include "openfsl/fsltime.h"
#include "openfsl/endian.h"

namespace openfsl {
namespace fat32 {

class FAT32;
typedef uint32_t cluster_t;

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

}  // namespace fat32
}  // namespace openfsl

#endif  // FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_SPECIALSECTOR_H_
