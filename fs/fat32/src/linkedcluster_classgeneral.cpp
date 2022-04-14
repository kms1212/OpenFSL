/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

openfsl::FAT32::LinkedCluster::LinkedCluster(
    FAT32* fileSystem_, const cluster_t startCluster_) {
    fileSystem = fileSystem_;
    startCluster = startCluster_;

    cluster_t* fatArea = fileSystem->getFatArea()->getDataCast<cluster_t>();

    if (fatArea[startCluster] == 0) {  // If cluster chain is not initialized
        error_t result;
        fatArea[startCluster] = 0x0FFFFFFF;

        lba48_t modifiedSector =
            static_cast<lba48_t>
            (fileSystem->__fatAreaIndexToSector(startCluster));
        size_t bytespersector =
            fileSystem->getIODevice()->getDiskParameter().bytesPerSector;

        uint8_t* blankSector = new uint8_t[bytespersector];
        memset(blankSector, 0, bytespersector);

        for (lba48_t i = 0;
            i < fileSystem->getBPB()->bpbSectorPerCluster; i++) {
            result = fileSystem->getIODevice()->writeSector(blankSector,
                fileSystem->getActualLBA(clusterToLBA(startCluster) + i), 1);
            if (result)
                throw result;
        }
        delete[] blankSector;

        for (uint8_t i = 0; i < fileSystem->getBPB()->bpbFATCount; i++) {
            result = fileSystem->getIODevice()->writeSector(
                reinterpret_cast<uint8_t*>(fatArea) +
                (modifiedSector - fileSystem->getBPB()->bpbReservedSectors)
                * bytespersector,
                fileSystem->getActualLBA(modifiedSector) +
                i * fileSystem->getBPB()->ebpbSectorsPerFAT32, 1);
            if (i == 0 && result)
                throw result;
        }

        // Write result to FSINFO structure
        fileSystem->fsinfo.fsinfoFreeCluster++;
        fileSystem->fsinfo.fsinfoNextFree = fileSystem->__getNextFreeCluster();
        result = fileSystem->__updateFsInfo();
        if (result)
            throw result;
    }

    updateClusterList();
}

openfsl::FAT32::LinkedCluster::~LinkedCluster() {
    if (fetchedSectorCache != nullptr)
        delete fetchedSectorCache;
}