/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/gpt.h"

extern "C" {
#include <zlib.h>
}

std::unordered_map<int, openfsl::GUID> openfsl::PartitionGuid::partitionGuidMap = {
    {openfsl::PartitionGuid::Unused,                openfsl::GUID(0, 0, 0, 0, 0)},
    {openfsl::PartitionGuid::MBRPartitionScheme,    openfsl::GUID(0x024DEE41, 0x33E7, 0x11D3, 0x9D69, 0x0008C781F39F)},
    {openfsl::PartitionGuid::EFISystemPartition,    openfsl::GUID(0xC12A7328, 0xF81F, 0x11D2, 0xBA4B, 0x00A0C93EC93B)},
    {openfsl::PartitionGuid::BIOSBootPartition,     openfsl::GUID(0x21686148, 0x6449, 0x6E6F, 0x744E, 0x656564454649)},

    {openfsl::PartitionGuid::WindowsBasicData,      openfsl::GUID(0xEBD0A0A2, 0xB9E5, 0x4433, 0x87C0, 0x68B6B72699C7)},
    {openfsl::PartitionGuid::WindowsLDMMetaData,    openfsl::GUID(0x5808C8AA, 0x7E8F, 0x42E0, 0x85D2, 0xE1E90434CFB3)},
    {openfsl::PartitionGuid::WindowsLDMData,        openfsl::GUID(0xAF9B60A0, 0x1431, 0x4F62, 0xBC68, 0x3311714A69AD)},
    {openfsl::PartitionGuid::WindowsRecovery,       openfsl::GUID(0xDE94BBA4, 0x06D1, 0x4D40, 0xA16A, 0xBFD50179D6AC)},
    {openfsl::PartitionGuid::WindowsStorageSpaces,  openfsl::GUID(0xE75CAF8F, 0xF680, 0x4CEE, 0xAFA3, 0xB001E56EFC2D)},
    {openfsl::PartitionGuid::WindowsStorageReplica, openfsl::GUID(0x558D43C5, 0xA1AC, 0x43C0, 0xAAC8, 0xD1472B2923D1)},

    {openfsl::PartitionGuid::HPUXData,              openfsl::GUID(0x75894C1E, 0x3AEB, 0x11D3, 0xB7C1, 0x7B03A0000000)},
    {openfsl::PartitionGuid::HPUXService,           openfsl::GUID(0xE2A1E728, 0x32E3, 0x11D6, 0xA682, 0x7B03A0000000)},

    {openfsl::PartitionGuid::LinuxFilsystemData,    openfsl::GUID(0x0FC63DAF, 0x8483, 0x4772, 0x8E79, 0x3D69D8477DE4)},
    {openfsl::PartitionGuid::LinuxRAID,             openfsl::GUID(0xA19D880F, 0x05FC, 0x4D3B, 0xA006, 0x743F0F84911E)},
    {openfsl::PartitionGuid::LinuxRootx86,          openfsl::GUID(0x44479540, 0xF297, 0x41B2, 0x9AF7, 0xD131D5F0458A)},
    {openfsl::PartitionGuid::LinuxRootx8664,        openfsl::GUID(0x4F68BCE3, 0xE8CD, 0x4DB1, 0x96E7, 0xFBCAF984B709)},
    {openfsl::PartitionGuid::LinuxRootARM32,        openfsl::GUID(0x69DAD710, 0x2CE4, 0x4E3C, 0xB16C, 0x21A1D49ABED3)},
    {openfsl::PartitionGuid::LinuxRootARM64,        openfsl::GUID(0xB921B045, 0x1DF0, 0x41C3, 0xAF44, 0x4C6F280D3FAE)},
    {openfsl::PartitionGuid::LinuxBoot,             openfsl::GUID(0xBC13C2FF, 0x59E6, 0x4262, 0xA352, 0xB275FD6F7172)},
    {openfsl::PartitionGuid::LinuxSwap,             openfsl::GUID(0x0657FD6D, 0xA4AB, 0x43C4, 0x84E5, 0x0933C84B4F4F)},
    {openfsl::PartitionGuid::LinuxLVM,              openfsl::GUID(0xE6D6D379, 0xF507, 0x44C2, 0xA23C, 0x238F2A3DF928)},
    {openfsl::PartitionGuid::LinuxHome,             openfsl::GUID(0x933AC7E1, 0x2EB4, 0x4F13, 0xB844, 0x0E14E2AEF915)},
    {openfsl::PartitionGuid::LinuxServerData,       openfsl::GUID(0x3B8F8425, 0x20E0, 0x4F3B, 0x907F, 0x1A25A76F98E8)},
    {openfsl::PartitionGuid::LinuxLUKS,             openfsl::GUID(0xCA7D7CCB, 0x63ED, 0x4C53, 0x861C, 0x1742536059CC)},

    {openfsl::PartitionGuid::FreeBSDBoot,           openfsl::GUID(0x83BD6B9D, 0x7F41, 0x11DC, 0xBE0B, 0x001560B84F0F)},
    {openfsl::PartitionGuid::FreeBSDDiskLabel,      openfsl::GUID(0x516E7CB4, 0x6ECF, 0x11D6, 0x8FF8, 0x00022D09712B)},
    {openfsl::PartitionGuid::FreeBSDSwap,           openfsl::GUID(0x516E7CB5, 0x6ECF, 0x11D6, 0x8FF8, 0x00022D09712B)},
    {openfsl::PartitionGuid::FreeBSDUFS,            openfsl::GUID(0x516E7CB6, 0x6ECF, 0x11D6, 0x8FF8, 0x00022D09712B)},
    {openfsl::PartitionGuid::FreeBSDVinum,          openfsl::GUID(0x516E7CB8, 0x6ECF, 0x11D6, 0x8FF8, 0x00022D09712B)},
    {openfsl::PartitionGuid::FreeBSDZFS,            openfsl::GUID(0x516E7CBA, 0x6ECF, 0x11D6, 0x8FF8, 0x00022D09712B)},
    {openfsl::PartitionGuid::FreeBSDNandfs,         openfsl::GUID(0x74BA7DD9, 0xA689, 0x11E1, 0xBD04, 0x00E081286ACF)},

    {openfsl::PartitionGuid::DarwinHFSPlus,         openfsl::GUID(0x48465300, 0x0000, 0x11AA, 0xAA11, 0x00306543ECAC)},
    {openfsl::PartitionGuid::DarwinAPFS,            openfsl::GUID(0x7C3457EF, 0x0000, 0x11AA, 0xAA11, 0x00306543ECAC)},
    {openfsl::PartitionGuid::DarwinUFS,             openfsl::GUID(0x55465300, 0x0000, 0x11AA, 0xAA11, 0x00306543ECAC)},
    {openfsl::PartitionGuid::DarwinZFS,             openfsl::GUID(0x6A898CC3, 0x1DD2, 0x11B2, 0x99A6, 0x080020736631)},
    {openfsl::PartitionGuid::DarwinRAID,            openfsl::GUID(0x52414944, 0x0000, 0x11AA, 0xAA11, 0x00306543ECAC)},
    {openfsl::PartitionGuid::DarwinRAIDOffline,     openfsl::GUID(0x52414944, 0x5F4F, 0x11AA, 0xAA11, 0x00306543ECAC)},
    {openfsl::PartitionGuid::DarwinBoot,            openfsl::GUID(0x426F6F74, 0x0000, 0x11AA, 0xAA11, 0x00306543ECAC)},
    {openfsl::PartitionGuid::DarwinAppleLabel,      openfsl::GUID(0x4C616265, 0x6C00, 0x11AA, 0xAA11, 0x00306543ECAC)},
    {openfsl::PartitionGuid::DarwinAppleTVRecovery, openfsl::GUID(0x5265636F, 0x7665, 0x11AA, 0xAA11, 0x00306543ECAC)},
    {openfsl::PartitionGuid::DarwinCoreStorage,     openfsl::GUID(0x53746F72, 0x6167, 0x11AA, 0xAA11, 0x00306543ECAC)},
    {openfsl::PartitionGuid::DarwinAPFSPreboot,     openfsl::GUID(0x69646961, 0x6700, 0x11AA, 0xAA11, 0x00306543ECAC)},
    {openfsl::PartitionGuid::DarwinAPFSRecovery,    openfsl::GUID(0x52637672, 0x7900, 0x11AA, 0xAA11, 0x00306543ECAC)},

    {openfsl::PartitionGuid::SolarisBoot,           openfsl::GUID(0x6A82CB45, 0x1DD2, 0x11B2, 0x99A6, 0x080020736631)},
    {openfsl::PartitionGuid::SolarisRoot,           openfsl::GUID(0x6A85CF4D, 0x1DD2, 0x11B2, 0x99A6, 0x080020736631)},
    {openfsl::PartitionGuid::SolarisSwap,           openfsl::GUID(0x6A87C46F, 0x1DD2, 0x11B2, 0x99A6, 0x080020736631)},
    {openfsl::PartitionGuid::SolarisBackup,         openfsl::GUID(0x6A8B642B, 0x1DD2, 0x11B2, 0x99A6, 0x080020736631)},
    {openfsl::PartitionGuid::SolarisUsr,            openfsl::GUID(0x6A898CC3, 0x1DD2, 0x11B2, 0x99A6, 0x080020736631)},
    {openfsl::PartitionGuid::SolarisVar,            openfsl::GUID(0x6A8EF2E9, 0x1DD2, 0x11B2, 0x99A6, 0x080020736631)},
    {openfsl::PartitionGuid::SolarisHome,           openfsl::GUID(0x6A90BA39, 0x1DD2, 0x11B2, 0x99A6, 0x080020736631)},

    {openfsl::PartitionGuid::NetBSDSwap,            openfsl::GUID(0x49F48D32, 0xB10E, 0x11DC, 0xB99B, 0x0019D1879648)},
    {openfsl::PartitionGuid::NetBSDFFS,             openfsl::GUID(0x49F48D5A, 0xB10E, 0x11DC, 0xB99B, 0x0019D1879648)},
    {openfsl::PartitionGuid::NetBSDLFS,             openfsl::GUID(0x49F48D82, 0xB10E, 0x11DC, 0xB99B, 0x0019D1879648)},
    {openfsl::PartitionGuid::NetBSDRAID,            openfsl::GUID(0x49F48DAA, 0xB10E, 0x11DC, 0xB99B, 0x0019D1879648)},
    {openfsl::PartitionGuid::NetBSDConcatenated,    openfsl::GUID(0x2DB519C4, 0xB10F, 0x11DC, 0xB99B, 0x0019D1879648)},
    {openfsl::PartitionGuid::NetBSDEncrypted,       openfsl::GUID(0x2DB519EC, 0xB10F, 0x11DC, 0xB99B, 0x0019D1879648)},

    {openfsl::PartitionGuid::ChromeOSKernel,        openfsl::GUID(0xFE3A2A5D, 0x4F32, 0x41A7, 0xB725, 0xACCC3285A309)},
    {openfsl::PartitionGuid::ChromeOSRootfs,        openfsl::GUID(0x3CB8E202, 0x3B7E, 0x47DD, 0x8A3C, 0x7FF2A13CFCEC)},
    {openfsl::PartitionGuid::ChromeOSFirmware,      openfsl::GUID(0xCAB6E88E, 0xABF3, 0x4102, 0xA07A, 0xD4BB9BE3C1D3)},
    {openfsl::PartitionGuid::ChromeOSMiniOS,        openfsl::GUID(0x09845860, 0x705F, 0x4BB5, 0xB16C, 0x8A8A099CAF52)},
    {openfsl::PartitionGuid::ChromeOSHibernate,     openfsl::GUID(0x3F0F8318, 0xF146, 0x4E6B, 0x8222, 0xC28C8F02E0D5)},

    {openfsl::PartitionGuid::HaikuBFS,              openfsl::GUID(0x42465331, 0x3BA3, 0x10F1, 0x802A, 0x4861696B7521)},

    {openfsl::PartitionGuid::MidnightBSDBoot,       openfsl::GUID(0x85D5E45E, 0x237C, 0x11E1, 0xB4B3, 0xE89A8F7FC3A7)},
    {openfsl::PartitionGuid::MidnightBSDData,       openfsl::GUID(0x85D5E45A, 0x237C, 0x11E1, 0xB4B3, 0xE89A8F7FC3A7)},
    {openfsl::PartitionGuid::MidnightBSDSwap,       openfsl::GUID(0x85D5E45B, 0x237C, 0x11E1, 0xB4B3, 0xE89A8F7FC3A7)},
    {openfsl::PartitionGuid::MidnightBSDUFS,        openfsl::GUID(0x0394EF8B, 0x237E, 0x11E1, 0xB4B3, 0xE89A8F7FC3A7)},
    {openfsl::PartitionGuid::MidnightBSDVinum,      openfsl::GUID(0x85D5E45C, 0x237C, 0x11E1, 0xB4B3, 0xE89A8F7FC3A7)},
    {openfsl::PartitionGuid::MidnightBSDZFS,        openfsl::GUID(0x85D5E45D, 0x237C, 0x11E1, 0xB4B3, 0xE89A8F7FC3A7)},

    {openfsl::PartitionGuid::OpenBSDData,           openfsl::GUID(0x824CC7A0, 0x36A8, 0x11E3, 0x890A, 0x952519AD3F61)}
};

openfsl::GPT::GPT(BlockDevice* bd_) {
    bd = bd_;
    protMbr = new MBR(bd_);
}

openfsl::GPT::~GPT() { 
    delete protMbr;
}

error_t openfsl::GPT::initialize() {
    error_t result = protMbr->initialize();

    if (result)
        return result;

    uint8_t gptMajVersion;
    uint8_t gptMinVersion;

    uint32_t headerSize;

    lba48_t currentLBA;
    lba48_t backupLBA;

    lba48_t partitionLBAStart;
    lba48_t partitionLBAEnd;

    lba48_t partitionEntryArrayLBA;

    uint32_t partitionEntryCount;
    uint32_t partitionEntrySize;
    uint32_t partitionEntryCrc32;


    std::vector<MBR::PartitionInfo> fsList = protMbr->getPartitionInfo();
    
    // Check if GPT entry is a partition of MBR
    if (fsList.size() != 1 ||
        fsList[0].partFileSystem != FileSystemType::GPT)
        return OPENFSL_ERROR_INVALGPT;
    
    // Read first sector
    Sector headerSector(1, bd->getDiskParameter().bytesPerSector);
    result = bd->readSector(headerSector.getData(), 0, 1);

    if (result)
        return result;

    if (memcmp("EFI PART", headerSector.getData(), 8) != 0)
        return OPENFSL_ERROR_INVALGPT;

    gptMajVersion = *(headerSector.getDataCast<uint8_t>() + 10);
    gptMinVersion = *(headerSector.getDataCast<uint8_t>() + 11);

    headerSize = *(headerSector.getDataCast<uint32_t>() + 12);

    uint32_t origCrc32 = *(headerSector.getDataCast<uint32_t>() + 16);
    *(headerSector.getDataCast<uint32_t>() + 16) = 0;

    uint32_t calculatedCrc32 = crc32(0, headerSector.getDataCast<uint8_t>(), headerSize);

    if (origCrc32 != calculatedCrc32)
        return OPENFSL_ERROR_INVALGPT;

    GUID diskGuid(headerSector.getDataCast<uint8_t>());

    return 0;
}

std::vector<openfsl::GPT::PartitionInfo> openfsl::GPT::getPartitionInfo() {
    std::vector<PartitionInfo> ret;

    /*
    for (size_t i = 0; i < partitionList.size(); i++) {
        FileSystemType fs;
        if (partitionList[i].entryPartitionType == PartitionType::FAT12)
            fs = FileSystemType::FAT12;
        else if (partitionList[i].entryPartitionType ==
            PartitionType::FAT16_LBA)
            fs = FileSystemType::FAT16;
        else if ((partitionList[i].entryPartitionType ==
                    PartitionType::FAT32) ||
                (partitionList[i].entryPartitionType ==
                    PartitionType::FAT32_LBA))
            fs = FileSystemType::FAT32;
        else if (partitionList[i].entryPartitionType == PartitionType::NTFS)
            fs = FileSystemType::NTFS;
        else if (partitionList[i].entryPartitionType == PartitionType::GPT)
            fs = FileSystemType::GPT;

        ret.push_back(fs);
    } */

    return ret;
}
