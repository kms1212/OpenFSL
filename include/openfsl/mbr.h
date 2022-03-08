/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_MBR_H_
#define OPENFSL_MBR_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <cstring>

#include <vector>
#include <iostream>

#include "openfsl/detectstruct.h"
#include "openfsl/filesystem.h"
#include "openfsl/fslutils.h"
#include "openfsl/error.h"

namespace openfsl {
class MBR {
 private:
    enum PartitionType : uint8_t {
        None = 0x00,
        FAT12 = 0x01,
        ExtPart_S = 0x05,
        NTFS = 0x07,
        FAT32 = 0x0B,
        FAT32_LBA = 0x0C,
        FAT16_LBA = 0x0E,
        ExtPart_L = 0x0F,
        GPT = 0xEE
    };

    /**
     * @brief MBR Partition Entry
     * @details
     * Type         | Name                    | Offset
     * -------------|-------------------------|----------
     * uint8_t      | entryBootFlag           | 0x0000
     * uint8_t      | entryStartingCHSAddr[3] | 0x0001
     * PartitionType| entryPartitionType      | 0x0004
     * uint8_t      | entryEndingCHSAddr[3]   | 0x0005
     * uint32_t     | entryStartingLBAAddr    | 0x0008
     * uint32_t     | entryLBASize            | 0x000C
     */
#pragma pack(push, 1)
    typedef struct PartitionEntry {
        uint8_t entryBootFlag;
        uint8_t entryStartingCHSAddr[3];
        PartitionType entryPartitionType;
        uint8_t entryEndingCHSAddr[3];
        uint32_t entryStartingLBAAddr;
        uint32_t entryLBASize;
    } PartitionEntry;
#pragma pack(pop)

    /**
     * @brief MBR Partition Table
     * @details
     * Type           | Name                    | Offset
     * ---------------|-------------------------|----------
     * uint8_t        | mbrBootCode[446]        | 0x0000
     * PartitionEntry | mbrPartitionEntry[4]    | 0x01BD
     * uint16_t       | mbrSignature            | 0x0FFE
     *
     */
#pragma pack(push, 1)
    typedef struct PartitionTable {
        uint8_t mbrBootCode[446];
        PartitionEntry mbrPartitionEntry[4];
        uint16_t mbrSignature;
    } PartitionTable;
#pragma pack(pop)

    BlockDevice* bd;
    std::vector<PartitionEntry> partitionList;

 public:
    typedef struct PartitionInfo {
        bool partBootable;
        lba48_t partOffset;
        lba48_t partSize;
        FileSystemType partFileSystem;
    } PartitionInfo;
    
    explicit MBR(BlockDevice* bd_);
    ~MBR();

    // int format();
    // int createEntry(const PartitionType type);

    error_t __check_result initialize();

    std::vector<PartitionInfo> getPartitionInfo();
};
}  // namespace openfsl

#endif  // OPENFSL_MBR_H_
