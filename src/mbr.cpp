/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/mbr.h"

openfsl::MBR::MBR(BlockDevice* bd_) {
    bd = bd_;
}

openfsl::MBR::~MBR() { }

int openfsl::MBR::initialize() {
    lba48_t base_addr = 0;

    Sector sector_lba0(1, bd->getDiskParameter().bytesPerSector);
    // Read partition table sector
    int errorcode = bd->readSector(sector_lba0.getData(), 0, 1);
    if (errorcode)
        return errorcode;
    PartitionTable table;
    memcpy(&table, sector_lba0.getData(), sizeof(PartitionTable));

    if (table.mbrSignature != 0xAA55)  // Check MBR signature
        return OPENFSL_ERROR_SIGN;

    for (int i = 0; i < 256; i++) {
        PartitionEntry currentEntry;

        if (i < 4) {
            currentEntry = table.mbrPartitionEntry[i];

            if (currentEntry.entryPartitionType == 0)
                break;

            // Exended Partition
            if ((currentEntry.entryPartitionType == PartitionType::ExtPart_S) ||
                (currentEntry.entryPartitionType == PartitionType::ExtPart_L)) {
                // If partition table created without LBA Addressing
                if (currentEntry.entryStartingLBAAddr == 0) {
                } else {  // If partition table created with LBA Addressing
                    base_addr = currentEntry.entryStartingLBAAddr;

                    // Read extended partition table
                    errorcode =
                        bd->readSector(sector_lba0.getData(), base_addr, 1);
                    if (errorcode)
                        return errorcode;

                    currentEntry =
                        sector_lba0.getDataCast<PartitionTable>()->
                            mbrPartitionEntry[0];

                    if (currentEntry.entryPartitionType == 0)
                        break;

                    currentEntry.entryStartingLBAAddr += (uint32_t)base_addr;
                    partitionList.push_back(currentEntry);

                    if (sector_lba0.getDataCast<PartitionTable>()->
                        mbrPartitionEntry[1].entryPartitionType == 0) {
                        break;
                    } else {
                        // If partition table created without LBA Addressing
                        if (sector_lba0.getDataCast<PartitionTable>()->
                            mbrPartitionEntry[1].entryStartingLBAAddr == 0) {
                            // add lba-converted chs address to base_addr
                        } else {  // With LBA Addressing
                            base_addr +=
                                sector_lba0.getDataCast<PartitionTable>()->
                                    mbrPartitionEntry[1].entryStartingLBAAddr;
                        }
                    }
                }
            } else {  // Primary partition
                partitionList.push_back(currentEntry);
            }
        } else {
            // Read extended partition table
            errorcode = bd->readSector(sector_lba0.getData(), base_addr, 1);
            if (errorcode)
                return errorcode;

            PartitionEntry currentEntry =
                sector_lba0.getDataCast<PartitionTable>()->
                    mbrPartitionEntry[0];

            currentEntry.entryStartingLBAAddr += (uint32_t)base_addr;
            partitionList.push_back(currentEntry);

            if (sector_lba0.getDataCast<PartitionTable>()->
                mbrPartitionEntry[1].entryPartitionType == 0) {
                break;
            } else {
                // If partition table created without LBA Addressing
                if (sector_lba0.getDataCast<PartitionTable>()->
                    mbrPartitionEntry[1].entryStartingLBAAddr == 0) {
                    // add lba-converted chs address to base_addr
                } else {  // With LBA Addressing
                    base_addr += sector_lba0.getDataCast<PartitionTable>()->
                        mbrPartitionEntry[1].entryStartingLBAAddr;
                }
            }
        }
    }

    return 0;
}

error_t openfsl::MBR::getPartitionInfo(std::vector<openfsl::MBR::PartitionInfo>* buf) {
    PartitionInfo temp;

    for (size_t i = 0; i < partitionList.size(); i++) {
        temp.partBootable = partitionList[i].entryBootFlag == 0x80;

        if (partitionList[i].entryPartitionType == PartitionType::FAT12)
            temp.partFileSystem = FileSystemType::FAT12;
        else if (partitionList[i].entryPartitionType ==
            PartitionType::FAT16_LBA)
            temp.partFileSystem = FileSystemType::FAT16;
        else if ((partitionList[i].entryPartitionType ==
                    PartitionType::FAT32) ||
                (partitionList[i].entryPartitionType ==
                    PartitionType::FAT32_LBA))
            temp.partFileSystem = FileSystemType::FAT32;
        else if (partitionList[i].entryPartitionType == PartitionType::NTFS)
            temp.partFileSystem = FileSystemType::NTFS;
        else if (partitionList[i].entryPartitionType == PartitionType::GPT)
            temp.partFileSystem = FileSystemType::GPT;
            
        if (partitionList[i].entryStartingLBAAddr == 0) {  // CHS
            CHS chs;
            memcpy(reinterpret_cast<uint8_t*>(&chs),
                &partitionList[i].entryStartingCHSAddr, 3);
            temp.partOffset = convertCHSToLBA(chs,
                bd->getDiskParameter().sectorPerTrack,
                bd->getDiskParameter().headPerCylinder);
        } else {  // LBA
            temp.partOffset = partitionList[i].entryStartingLBAAddr;
        }
            
        if (partitionList[i].entryLBASize == 0) {  // CHS
            CHS chs;
            memcpy(reinterpret_cast<uint8_t*>(&chs),
                &partitionList[i].entryStartingCHSAddr, 3);
            lba48_t endlba = convertCHSToLBA(chs,
                bd->getDiskParameter().sectorPerTrack,
                bd->getDiskParameter().headPerCylinder);
            
            temp.partSize = endlba - temp.partOffset;
        } else {  // LBA
            temp.partSize = partitionList[i].entryLBASize;
        }

        buf->push_back(temp);
    }
    return OPENFSL_SUCCESS;
}
