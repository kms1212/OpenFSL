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
    Sector checkSector(1, bd->getDiskParameter().bytesPerSector);
    error_t result = bd->readSector(checkSector.getData(), 0, 1);
    if (result)
        return result;

    buf->partTable = PartitionTableType::None;
    buf->partList.clear();

    fsStructure::COMMAA55h* tailSigChk =
        checkSector.getDataCast<fsStructure::COMMAA55h>();

    if (tailSigChk->tailSig == 0xAA55) {
        fsStructure::FAT1216BPB* fat1216Chk =
            checkSector.getDataCast<fsStructure::FAT1216BPB>();
        if (memcmp(fat1216Chk->fatFsType, "FAT12   ", 8) == 0) {
            buf->partList.push_back(FileSystemType::FAT12);
            return 0;
        } else if (memcmp(fat1216Chk->fatFsType, "FAT16   ", 8) == 0) {
            buf->partList.push_back(FileSystemType::FAT16);
            return 0;
        }

        fsStructure::FAT32BPB* fat32Chk =
            checkSector.getDataCast<fsStructure::FAT32BPB>();
        if (memcmp(fat32Chk->fatFsType, "FAT32   ", 8) == 0) {
            buf->partList.push_back(FileSystemType::FAT32);
            return 0;
        }

        fsStructure::NTFSBPB* ntfsChk =
            checkSector.getDataCast<fsStructure::NTFSBPB>();
        if (memcmp(ntfsChk->fsType, "NTFS    ", 8) == 0) {
            buf->partList.push_back(FileSystemType::NTFS);
            return 0;
        }

        MBR mbr(bd);
        result = mbr.initialize();
        if (result)
            return result;

        std::vector<MBR::PartitionInfo> fsList = mbr.getPartitionInfo();
        if (fsList.size() == 1 &&
            fsList[0].partFileSystem == FileSystemType::GPT) {
            buf->partTable = PartitionTableType::GPT;

            GPT gpt(bd);
            result = gpt.initialize();
            if (result)
                return result;

            // fsList = gpt.getPartitionInfo();
        } else {
            buf->partTable = PartitionTableType::MBR;
        }

        for (size_t i = 0; i < fsList.size(); i++) {
            buf->partList.push_back(fsList[i].partFileSystem);
        }
    }

    return 0;
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
