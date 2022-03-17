/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_GPT_H_
#define OPENFSL_GPT_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <cstring>

#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#include "openfsl/detectstruct.h"
#include "openfsl/filesystem.h"
#include "openfsl/fslutils.h"
#include "openfsl/error.h"
#include "openfsl/mbr.h"
#include "openfsl/guid.h"
#include "openfsl/endian.h"

namespace openfsl {
class GPT {
 public:
    enum PartitionType {
        Unused,
        MBRPartitionScheme,
        EFISystemPartition,
        BIOSBootPartition,

        WindowsMsReserved,
        WindowsBasicData,
        WindowsLDMMetaData,
        WindowsLDMData,
        WindowsRecovery,
        WindowsStorageSpaces,
        WindowsStorageReplica,

        HPUXData,
        HPUXService,

        LinuxFilsystemData,
        LinuxRAID,
        LinuxRootx86,
        LinuxRootx8664,
        LinuxRootARM32,
        LinuxRootARM64,
        LinuxBoot,
        LinuxSwap,
        LinuxLVM,
        LinuxHome,
        LinuxServerData,
        LinuxLUKS,

        FreeBSDBoot,
        FreeBSDDiskLabel,
        FreeBSDSwap,
        FreeBSDUFS,
        FreeBSDVinum,
        FreeBSDZFS,
        FreeBSDNandfs,

        DarwinHFSPlus,
        DarwinAPFS,
        DarwinUFS,
        DarwinZFS,
        DarwinRAID,
        DarwinRAIDOffline,
        DarwinBoot,
        DarwinAppleLabel,
        DarwinAppleTVRecovery,
        DarwinCoreStorage,
        DarwinAPFSPreboot,
        DarwinAPFSRecovery,

        SolarisBoot,
        SolarisRoot,
        SolarisSwap,
        SolarisBackup,
        SolarisUsr,
        SolarisVar,
        SolarisHome,

        NetBSDSwap,
        NetBSDFFS,
        NetBSDLFS,
        NetBSDRAID,
        NetBSDConcatenated,
        NetBSDEncrypted,

        ChromeOSKernel,
        ChromeOSRootfs,
        ChromeOSFirmware,
        ChromeOSMiniOS,
        ChromeOSHibernate,

        HaikuBFS,

        MidnightBSDBoot,
        MidnightBSDData,
        MidnightBSDSwap,
        MidnightBSDUFS,
        MidnightBSDVinum,
        MidnightBSDZFS,

        OpenBSDData
    };

 private:
    enum class AttributeFlags : uint64_t {
        System = 0x0000000000000000,
        Ignore = 0x0000000000000001,
        LegacyBoot = 0x0000000000000002,
        MSBasicDataReadOnly = 0x1000000000000000,
        MSBasicDataHidden = 0x4000000000000000,
        MSBasicDataManualMount = 0x8000000000000000
    };

    friend inline GPT::AttributeFlags operator|
        (GPT::AttributeFlags a, GPT::AttributeFlags b)
    { return (GPT::AttributeFlags)((uint64_t)a | (uint64_t)b); }

    friend inline GPT::AttributeFlags operator&
        (GPT::AttributeFlags a, GPT::AttributeFlags b)
    { return (GPT::AttributeFlags)((uint64_t)a & (uint64_t)b); }

    /**
     * @brief GPT Header
     * @details
     * Type           | Name                    | Offset
     * ---------------|-------------------------|----------
     * uint8_t        | gptSignature[8]         | 0x0000
     * uint32_t       | gptRevision             | 0x0008
     * uint32_t       | gptHeaderSize           | 0x000C
     * uint32_t       | gptHeaderCrc32          | 0x0010
     * uint8_t        | __Reserved1[4]          | 0x0014
     * uint64_t       | gptCurrentLba           | 0x0018
     * uint64_t       | gptBackupLba            | 0x0020
     * uint64_t       | gptPartitionAreaStart   | 0x0028
     * uint64_t       | gptPartitionAreaEnd     | 0x0030
     * uint8_t        | gptDiskGuid[16]         | 0x0038
     * uint64_t       | gptPartitionEntryStart  | 0x0048
     * uint32_t       | gptPartitionEntryCount  | 0x0050
     * uint32_t       | gptPartitionEntrySize   | 0x0054
     * uint32_t       | gptPartitionEntryCrc32  | 0x0058
     * uint8_t        | __Reserved2[420]        | 0x005C
     */
#pragma pack(push, 1)
    typedef struct TableHeader {
        uint8_t    gptSignature[8];
        uint32_t   gptRevision;
        uint32_t   gptHeaderSize;
        uint32_t   gptHeaderCrc32;
        uint8_t    __Reserved1[4];
        uint64_t   gptCurrentLba;
        uint64_t   gptBackupLba;
        uint64_t   gptPartitionAreaStart;
        uint64_t   gptPartitionAreaEnd;
        uint8_t    gptDiskGuid[16];
        uint64_t   gptPartitionEntryStart;
        uint32_t   gptPartitionEntryCount;
        uint32_t   gptPartitionEntrySize;
        uint32_t   gptPartitionEntryCrc32;
        uint8_t    __Reserved2[420];
    } TableHeader;
#pragma pack(pop)

    /**
     * @brief GPT Partition Entry
     * @details
     * Type           | Name                       | Offset
     * ---------------|----------------------------|----------
     * uint8_t        | entryPartitionTypeGuid[16] | 0x0000
     * uint8_t        | entryPartitionGuid[16]     | 0x0010
     * uint64_t       | entryPartitionStart        | 0x0020
     * uint64_t       | entryPartitionEnd          | 0x0028
     * AttributeFlags | entryAttributeFlags        | 0x0030
     * uint16_t       | entryPartitionName[36]     | 0x0038
     */
#pragma pack(push, 1)
    typedef struct PartitionEntry {
        uint8_t entryPartitionTypeGuid[16];
        uint8_t entryPartitionGuid[16];
        uint64_t entryPartitionStart;
        uint64_t entryPartitionEnd;
        AttributeFlags entryAttributeFlags;
        uint16_t entryPartitionName[36];
    } PartitionEntry;
#pragma pack(pop)

    BlockDevice* bd;
    MBR* protMbr;
    std::vector<PartitionEntry> partitionList;

    TableHeader gptHeader;
    GUID diskGuid;

    lba48_t tableOffset;

 public:
    typedef struct PartitionInfo {
        AttributeFlags partAttribute;
        lba48_t partOffset;
        lba48_t partSize;
        PartitionType partType;
    } PartitionInfo;

    explicit GPT(BlockDevice* bd_);
    ~GPT();

    // int format();
    // int createEntry(const PartitionType type);

    error_t __check_result initialize();

    lba48_t getVolumeOffset(const uint8_t index);
    error_t getPartitionInfo(std::vector<PartitionInfo>* buf);
};

extern std::unordered_map<GUID, openfsl::GPT::PartitionType, GUIDHash> partitionGuidMap;
}  // namespace openfsl

#endif  // OPENFSL_GPT_H_
