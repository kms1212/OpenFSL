/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_DETECTSTRUCT_H_
#define OPENFSL_DETECTSTRUCT_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <cstring>

#include <string>
#include <vector>
#include <sstream>

#include "openfsl/filesystem.h"
#include "openfsl/blockdevice.h"

namespace openfsl {
namespace {
/**
 * @brief Common signature for FAT(12/16/32), exFAT, NTFS, MBR, etc..
 */
#pragma pack(push, 1)
typedef struct COMMAA55h {
    uint8_t        _nochk1[510];
    uint16_t        tailSig;
} COMMAA55h;
#pragma pack(pop)

/**
 * @brief FAT12, 16 BPB for determining partition type
 */
#pragma pack(push, 1)
typedef struct FAT1216BPB {
    uint8_t        _nochk1[54];
    uint8_t        fatFsType[8];
    uint8_t        _nochk2[450];
} FAT1216BPB;
#pragma pack(pop)

/**
 * @brief FAT32 BPB for determining partition type
 */
#pragma pack(push, 1)
typedef struct FAT32BPB {
    uint8_t        _nochk1[82];
    uint8_t        fatFsType[8];
    uint8_t        _nochk2[422];
} FAT32BPB;
#pragma pack(pop)

/**
 * @brief NTFS BPB for determining partition type
 */
#pragma pack(push, 1)
typedef struct NTFSBPB {
    uint8_t        _nochk1[3];
    uint8_t        fsType[8];
    uint8_t        _nochk2[501];
} NTFSBPB;
#pragma pack(pop)
}

enum class PartitionTableType {
    None,
    MBR,
    GPT
};

typedef struct {
    PartitionTableType partTable;
    std::vector<FileSystemType> partList;
} DiskStructure;

error_t __check_result detectDiskStructure(DiskStructure* buf, BlockDevice* dd);
error_t __check_result detectFileSystem(
    FileSystemType* buf, BlockDevice* bd, lba48_t bpbSectorAddr = 0);

std::string partitionTableTypeToString(const PartitionTableType type);
std::string fileSystemTypeToString(const FileSystemType type);

}  // namespace openfsl

#endif  // OPENFSL_DETECTSTRUCT_H_
