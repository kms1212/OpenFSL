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
 public:
    enum PartitionType : uint8_t {
        None = 0x00,
        FAT12PrimaryFirst32MB = 0x01,
        FAT16LessThan32MB = 0x04,
        ExtendedPartitionCHS = 0x05,
        FAT16MoreThan32MBFirst8GB = 0x06,
        IFS = 0x07,
        NTFS = 0x07,
        HPFS = 0x07,
        ExFAT = 0x07,
        FAT32CHS = 0x0B,
        FAT32LBA = 0x0C,
        FAT16BLBA = 0x0E,
        ExtendedPartitionLBA = 0x0F,
        WindowsRENTFS = 0x27,
        LinuxLegacy = 0x41,
        PPCReferencePlatformBoot = 0x41,
        LinuxSwapLegacy = 0x41,
        LinuxSwap = 0x82,
        LinuxNativeFileSystem = 0x83,
        LinuxExtended = 0x85,
        LinuxHidden = 0x93,
        ISO9660 = 0x96,
        FreeBSDSlice = 0xA5,
        OpenBSDSlice = 0xA6,
        DarwinUFS = 0xA8,
        NetBSDSlice = 0xA9,
        DarwinBoot = 0xAB,
        DarwinRAID = 0xAC,
        HFSPlus = 0xAF,
        LUKS = 0xE8,
        GPTProtective = 0xEE,
        EFISystem = 0xEF,
        VMwareVMFS = 0xFB,
        VMwareSwap = 0xFC
    };
    
 private:
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
        PartitionType partType;
        lba48_t partOffset;
        lba48_t partSize;
    } PartitionInfo;
    
    explicit MBR(BlockDevice* bd_);
    ~MBR();

    // int format();
    // int createEntry(const PartitionType type);

    error_t __check_result initialize();

    error_t getPartitionInfo(std::vector<PartitionInfo>* buf);
};
}  // namespace openfsl

#endif  // OPENFSL_MBR_H_
