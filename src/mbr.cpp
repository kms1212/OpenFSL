/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
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

    if (leToSystem<uint16_t>(table.mbrSignature) != 0xAA55)  // Check MBR signature
        return OPENFSL_ERROR_INVALID_SIGNATURE;

    int currentEntryIndex = 0;
    for (int i = 0; i < 256; i++) {
        PartitionEntry currentEntry;
        
        currentEntry = table.mbrPartitionEntry[currentEntryIndex];
        currentEntryIndex++;
        if (currentEntry.entryPartitionType == 0)
            break;

        currentEntry.entryStartingLBAAddr =
            leToSystem<uint32_t>(currentEntry.entryStartingLBAAddr);
        currentEntry.entryLBASize =
            leToSystem<uint32_t>(currentEntry.entryLBASize);

        // Exended Partition Entry
        if ((currentEntry.entryPartitionType ==
                PartitionType::ExtendedPartitionCHS) ||
            (currentEntry.entryPartitionType ==
                PartitionType::ExtendedPartitionLBA)) {
            // If partition table created without LBA Addressing
            if (currentEntry.entryStartingLBAAddr == 0) {
            } else {  // If partition table created with LBA Addressing
                base_addr = currentEntry.entryStartingLBAAddr;

                // Read extended partition table
                errorcode =
                    bd->readSector(sector_lba0.getData(), base_addr, 1);
                memcpy(&table, sector_lba0.getData(), sizeof(PartitionTable));
                if (errorcode)
                    return errorcode;

                currentEntryIndex = 0;
                currentEntry =
                    sector_lba0.getDataCast<PartitionTable>()->
                        mbrPartitionEntry[currentEntryIndex];
                currentEntryIndex++;

                if (currentEntry.entryPartitionType == 0)
                    break;

                currentEntry.entryStartingLBAAddr =
                    leToSystem<uint32_t>(currentEntry.entryStartingLBAAddr);
                currentEntry.entryLBASize =
                    leToSystem<uint32_t>(currentEntry.entryLBASize);

                currentEntry.entryStartingLBAAddr += (uint32_t)base_addr;
                partitionList.push_back(currentEntry);
            }
        } else {  // Primary partition
            partitionList.push_back(currentEntry);
        }
    }

    return 0;
}

error_t openfsl::MBR::getPartitionInfo(
    std::vector<openfsl::MBR::PartitionInfo>* buf) {
    PartitionInfo temp;

    for (size_t i = 0; i < partitionList.size(); i++) {
        temp.partBootable = partitionList[i].entryBootFlag == 0x80;
        temp.partType = partitionList[i].entryPartitionType;

        if (partitionList[i].entryStartingLBAAddr == 0) {  // CHS
            CHS chs;
            memcpy(reinterpret_cast<uint8_t*>(&chs),
                &partitionList[i].entryStartingCHSAddr, 3);
            temp.partOffset = Result<lba48_t>::tryResult(
                convertCHSToLBA(chs,
                    bd->getDiskParameter().sectorPerTrack,
                    bd->getDiskParameter().headPerCylinder));
        } else {  // LBA
            temp.partOffset = partitionList[i].entryStartingLBAAddr;
        }

        if (partitionList[i].entryLBASize == 0) {  // CHS
            CHS chs;
            memcpy(reinterpret_cast<uint8_t*>(&chs),
                &partitionList[i].entryStartingCHSAddr, 3);
            lba48_t endlba = Result<lba48_t>::tryResult(
                convertCHSToLBA(chs,
                    bd->getDiskParameter().sectorPerTrack,
                    bd->getDiskParameter().headPerCylinder));

            temp.partSize = endlba - temp.partOffset;
        } else {  // LBA
            temp.partSize = partitionList[i].entryLBASize;
        }

        buf->push_back(temp);
    }
    return OPENFSL_SUCCESS;
}
