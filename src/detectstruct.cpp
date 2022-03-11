/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.https://stackoverflow.com/

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/gpt.h"
#include "openfsl/mbr.h"

#include "openfsl/detectstruct.h"

error_t openfsl::detectDiskStructure(DiskStructure* buf, BlockDevice* bd) {
    // Read first sector of disk
    Sector checkSector(1, bd->getDiskParameter().bytesPerSector);
    error_t result = bd->readSector(checkSector.getData(), 0, 1);
    if (result)
        return result;

    // Clear struct
    buf->partTable = PartitionTableType::None;
    buf->partList.clear();

    fsStructure::COMMAA55h* tailSigChk =
        checkSector.getDataCast<fsStructure::COMMAA55h>();

    // Check 0xAA55 Signature
    if (tailSigChk->tailSig == 0xAA55) {
        // Detect file system
        FileSystemType fsType;
        result = detectFileSystem(&fsType, bd, 0);
        if (result)
            return result;

        // If file system is not detected, it will assume that the disk is partitioned.
        if (fsType == FileSystemType::None) {
            MBR mbr(bd);
            result = mbr.initialize();
            if (result)
                return result;

            std::vector<MBR::PartitionInfo> partList;
            result = mbr.getPartitionInfo(&partList);
            if (result)
                return result;
            if (partList.size() == 1 &&
                partList[0].partType == openfsl::MBR::PartitionType::GPTProtective) {
                // If partition table is GPT
                buf->partTable = PartitionTableType::GPT;

                GPT gpt(bd);
                result = gpt.initialize();
                if (result)
                    return result;

                std::vector<GPT::PartitionInfo> partList;
                result = gpt.getPartitionInfo(&partList);
                if (result)
                    return result;
                
                for (size_t i = 0; i < partList.size(); i++) {
                    // Detect file system
                    FileSystemType fsType;
                    result = detectFileSystem(&fsType, bd, partList[i].partOffset);
                    if (result)
                        return result;

                    buf->partList.push_back(fsType);
                }
            } else {
                buf->partTable = PartitionTableType::MBR;

                for (MBR::PartitionInfo part : partList) {
                    // Detect file system
                    FileSystemType fsType;
                    result = detectFileSystem(&fsType, bd, part.partOffset);
                    if (result)
                        return result;

                    buf->partList.push_back(fsType);
                }
            }
        } else {
            buf->partList.push_back(fsType);
        }
    }

    return 0;
}

error_t openfsl::detectFileSystem(FileSystemType* buf, BlockDevice* bd, lba48_t bpbSectorAddr) {
    Sector checkSector(1, bd->getDiskParameter().bytesPerSector);
    error_t result = bd->readSector(checkSector.getData(), bpbSectorAddr, 1);
    if (result)
        return result;

    fsStructure::COMMAA55h* tailSigChk =
        checkSector.getDataCast<fsStructure::COMMAA55h>();

    if (tailSigChk->tailSig == 0xAA55) {
        fsStructure::FAT1216BPB* fat1216Chk =
            checkSector.getDataCast<fsStructure::FAT1216BPB>();
        if (memcmp(fat1216Chk->fatFsType, "FAT12   ", 8) == 0) {
            *buf = FileSystemType::FAT12;
            return 0;
        } else if (memcmp(fat1216Chk->fatFsType, "FAT16   ", 8) == 0) {
            *buf = FileSystemType::FAT16;
            return 0;
        }

        fsStructure::FAT32BPB* fat32Chk =
            checkSector.getDataCast<fsStructure::FAT32BPB>();
        if (memcmp(fat32Chk->fatFsType, "FAT32   ", 8) == 0) {
            *buf = FileSystemType::FAT32;
            return 0;
        }

        fsStructure::NTFSBPB* ntfsChk =
            checkSector.getDataCast<fsStructure::NTFSBPB>();
        if (memcmp(ntfsChk->fsType, "NTFS    ", 8) == 0) {
            *buf = FileSystemType::NTFS;
            return 0;
        }
    }

    *buf = FileSystemType::None;

    return OPENFSL_SUCCESS;
}

std::string openfsl::partitionTableTypeToString(const PartitionTableType type) {
    switch (type) {
    case PartitionTableType::None:
        return "NONE";
    case PartitionTableType::MBR:
        return "MBR";
    case PartitionTableType::GPT:
        return "GPT";
    default:
        return "ERR";
    }
}

std::string openfsl::fileSystemTypeToString(const FileSystemType type) {
    switch (type) {
    case FileSystemType::APFS:
        return "APFS";
    case FileSystemType::BtrFS:
        return "Btrfs";
    case FileSystemType::ExFAT:
        return "exFAT";
    case FileSystemType::Ext2:
        return "ext2";
    case FileSystemType::Ext3:
        return "ext3";
    case FileSystemType::Ext4:
        return "ext2";
    case FileSystemType::FAT12:
        return "FAT12";
    case FileSystemType::FAT16:
        return "FAT16";
    case FileSystemType::FAT32:
        return "FAT32";
    case FileSystemType::HFSPlus:
        return "HFS+";
    case FileSystemType::ISO9660:
        return "ISO9660";
    case FileSystemType::NTFS:
        return "NTFS";
    case FileSystemType::UDF:
        return "UDF";
    default:
        return "ERR";
    }
}
