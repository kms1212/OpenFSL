/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fslutils.h"
#include "openfsl/bootcode.h"

#include "openfsl/fat32/fs_fat32.h"

extern void __hexdump(const uint8_t* p, size_t offset, size_t len);

openfsl::FAT32::FormatOptions
    openfsl::FAT32::getDefaultFormatOptions(const lba48_t fsSize) {
    FormatOptions ret;
    ret.volumeLabel = "NO NAME";

    auto tpoint = std::chrono::system_clock::now();
    time_t ttenth = std::chrono::duration_cast<std::chrono::seconds>
        (tpoint.time_since_epoch()).count() / 10;
    ttenth %= 100;
    time_t t = std::chrono::system_clock::to_time_t(tpoint);
    std::tm currentTime = fsl_localtime(t);

    uint16_t tmp1 = (((uint8_t)currentTime.tm_mon + 1) << 8) +
        (uint8_t)currentTime.tm_mday;
    uint16_t tmp2 = ((uint8_t)currentTime.tm_sec << 8) + (uint8_t)ttenth;
    ret.volumeSerial = (tmp1 + tmp2) << 16;

    tmp1 = ((uint8_t)currentTime.tm_hour << 8) + (uint8_t)currentTime.tm_min;
    tmp2 = (uint16_t)currentTime.tm_year + 1900;
    ret.volumeSerial += tmp1 + tmp2;

    std::cout << std::hex << ret.volumeSerial << std::dec << std::endl;

    ret.biosDriveNum = 0x80;

    if (fsSize < 532480)
        ret.sectorsPerCluster = 1;
    else if (fsSize < 16777216)
        ret.sectorsPerCluster = 8;
    else if (fsSize < 33554432)
        ret.sectorsPerCluster = 16;
    else if (fsSize < 67108864)
        ret.sectorsPerCluster = 32;
    else
        ret.sectorsPerCluster = 64;

    ret.backupBSLocation = 6;
    ret.fsinfoLocation = 1;
    ret.reservedSectorCount = 32;

    ret.fatAreaCount = 2;
    ret.fatMediaType = 0xF8;
    ret.rootDirectoryCluster = 2;

    return ret;
}

error_t openfsl::FAT32::format(const lba48_t offset, const lba48_t size,
    const FAT32::FormatOptions formatOptions, const bool fastFormat) {
    BPBSector bpb;
    FSINFOSector fsinfo;

    memset(&bpb, 0, sizeof(BPBSector));
    memset(&fsinfo, 0, sizeof(FSINFOSector));

    // Set signatures
    bpb.bpbVBRSignature = 0xAA55;
    bpb.ebpbExtendedBootSign = 0x29;

    fsinfo.fsinfoSignature1 = 0x41615252;
    fsinfo.fsinfoSignature2 = 0x61417272;
    fsinfo.fsinfoSignature3 = 0xAA55;

    // Copy VBR boot codes
    bpb.bpbJumpCode[0] = 0xEB;
    bpb.bpbJumpCode[1] = 0x58;
    bpb.bpbJumpCode[2] = 0x90;

    memcpy(bpb.bpbBootCode, _blob_vbrboot_bin_data, _blob_vbrboot_bin_len);

    // Copy volume OEM ID, Label, Volume ID, FS Type
    if (formatOptions.volumeLabel.length() > 11) {
        return OPENFSL_ERROR_TOO_LONG_VOLUME_LABEL;
    }

    memcpy(bpb.bpbOEMName, "OPENFSL ", 8);

    memset(bpb.ebpbVolumeLabel, ' ', 11);
    memcpy(bpb.ebpbVolumeLabel,
        formatOptions.volumeLabel.c_str(),
        formatOptions.volumeLabel.length());

    bpb.ebpbVolumeSerial = formatOptions.volumeSerial;

    memcpy(bpb.ebpbFSType, "FAT32   ", 8);

    // Disk informations
    if (formatOptions.biosDriveNum < 80) {
        return OPENFSL_ERROR_FLOPPY_NOT_SUPPORTED;
    }

    bpb.bpbMedia = 0xF8;
    bpb.ebpbPhysicalDriveNum = formatOptions.biosDriveNum;
    bpb.bpbBytesPerSector =
        static_cast<uint16_t>(iod->getDiskParameter().bytesPerSector);
    bpb.bpbSectorsPerTrack =
        static_cast<uint16_t>(iod->getDiskParameter().sectorPerTrack);
    bpb.bpbDiskHeads =
        static_cast<uint16_t>(iod->getDiskParameter().headPerCylinder);

    // Cluster, sector informations
    bpb.bpbHiddenSectors = (uint32_t)offset;
    bpb.bpbTotalSector32 = (uint32_t)size;

    bpb.bpbSectorPerCluster = formatOptions.sectorsPerCluster;
    bpb.bpbReservedSectors = (uint16_t)formatOptions.reservedSectorCount;

    if ((formatOptions.backupBSLocation >= formatOptions.reservedSectorCount) ||
        (formatOptions.fsinfoLocation >= formatOptions.reservedSectorCount)) {
        return OPENFSL_ERROR_INVALID_FORMAT_OPTIONS;
    }

    bpb.ebpbBSBackupSector = (uint16_t)formatOptions.backupBSLocation;
    bpb.ebpbFSInfoSector = (uint16_t)formatOptions.fsinfoLocation;

    // FAT Informations
    lba48_t tmpval1 = size - formatOptions.reservedSectorCount;
    lba48_t tmpval2 =
        128 * formatOptions.sectorsPerCluster + formatOptions.fatAreaCount;

    bpb.bpbFATCount = formatOptions.fatAreaCount;
    bpb.ebpbSectorsPerFAT32 = (uint32_t)((tmpval1 + tmpval2 - 1) / tmpval2);
    bpb.ebpbRootDirectoryCluster = formatOptions.rootDirectoryCluster;

    cluster_t clusterCount = static_cast<cluster_t>
        ((size - (formatOptions.reservedSectorCount +
        bpb.ebpbSectorsPerFAT32 * formatOptions.fatAreaCount))
        / formatOptions.sectorsPerCluster);

    if (bpb.ebpbRootDirectoryCluster >= clusterCount)
        return OPENFSL_ERROR_INVALID_FORMAT_OPTIONS;

    fsinfo.fsinfoFreeCluster = clusterCount - 1;
    if (bpb.ebpbRootDirectoryCluster == 1)
        fsinfo.fsinfoNextFree = 2;
    else
        fsinfo.fsinfoNextFree = 0;

    __hexdump(reinterpret_cast<uint8_t*>(&bpb), 0, 512);
    __hexdump(reinterpret_cast<uint8_t*>(&fsinfo), 0, 512);

    error_t result;

    // Zerofill disk
    if (!fastFormat) {
        char* emptySector = new char[iod->getDiskParameter().bytesPerSector];
        memset(emptySector, 0, iod->getDiskParameter().bytesPerSector);

        for (lba48_t i = offset + formatOptions.reservedSectorCount +
            bpb.ebpbSectorsPerFAT32 * formatOptions.fatAreaCount;
            i < offset + size;
            i++) {
            result = iod->writeSector(emptySector, i, 1);
            if (result)
                return result;
        }

        delete[] emptySector;
    }

    // Disk operations
    result = iod->writeSector(&bpb, offset, 1);
    if (result)
        return result;
    result = iod->writeSector(&fsinfo,
        offset + formatOptions.fsinfoLocation, 1);
    if (result)
        return result;

    // FAT Area initialization
    Sector fatArea(bpb.ebpbSectorsPerFAT32,
        iod->getDiskParameter().bytesPerSector);
    memset(fatArea.getData(), 0, fatArea.getSize());
    fatArea.getDataCast<uint32_t>()[0] =
        0x0FFFFF00 + formatOptions.fatMediaType;
    fatArea.getDataCast<uint32_t>()[1] = 0x0FFFFFFF;
    fatArea.getDataCast<uint32_t>()[formatOptions.rootDirectoryCluster] =
        0x0FFFFFFF;

    for (int i = 0; i < formatOptions.fatAreaCount; i++) {
        result = iod->writeSector(fatArea.getDataCast<uint8_t>(),
            offset + formatOptions.reservedSectorCount +
            bpb.ebpbSectorsPerFAT32 * i,
            bpb.ebpbSectorsPerFAT32);
        if (result)
            return result;
    }

    uint8_t* emptycluster =
        new uint8_t[iod->getDiskParameter().bytesPerSector *
            formatOptions.sectorsPerCluster];
    memset(emptycluster, 0, iod->getDiskParameter().bytesPerSector *
        formatOptions.sectorsPerCluster);

    result = iod->writeSector(emptycluster,
        offset + formatOptions.reservedSectorCount +
        bpb.ebpbSectorsPerFAT32 * formatOptions.fatAreaCount +
        formatOptions.sectorsPerCluster *
        (formatOptions.rootDirectoryCluster - 2),
        formatOptions.sectorsPerCluster);

    delete[] emptycluster;

    return OPENFSL_SUCCESS;
}
