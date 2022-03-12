/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <set>

#include "openfsl/fat32/fs_fat32.h"

int openfsl::FAT32::LinkedCluster::addCluster(const cluster_t addCount) {
    cluster_t nextFreeCluster = fileSystem->__getNextFreeCluster();
    std::set<lba48_t> modifiedSectorList;
    cluster_t* fatArea = fileSystem->getFatArea()->getDataCast<cluster_t>();

    size_t bytespersector =
        fileSystem->getIODevice()->getDiskParameter().bytesPerSector;
    error_t result;

    for (size_t i = 0; i < addCount; i++) {
        if (nextFreeCluster == 0) {
            return OPENFSL_ERROR_CANNOT_ALLOCATE_CLUSTER;
        }

        cluster_t prevLastCluster = clusterList.at(clusterList.size() - 1);

        fatArea[prevLastCluster] = nextFreeCluster;
        fatArea[nextFreeCluster] = 0x0FFFFFFF;

        updateClusterList();

        uint8_t* blankSector = new uint8_t[bytespersector];
        memset(blankSector, 0, bytespersector);
        for (lba48_t i = 0; i < bytespersector; i++) {
            result = fileSystem->getIODevice()->writeSector(blankSector,
                fileSystem->getActualLBA(clusterToLBA(nextFreeCluster) + i), 1);
            if (result)
                return result;
        }
        delete[] blankSector;

        modifiedSectorList.insert(
            fileSystem->__fatAreaIndexToSector(prevLastCluster));
        modifiedSectorList.insert(
            fileSystem->__fatAreaIndexToSector(nextFreeCluster));

        nextFreeCluster = fileSystem->__getNextFreeCluster();
    }

    for (std::set<lba48_t>::iterator i = modifiedSectorList.begin();
         i != modifiedSectorList.end(); ++i) {
        for (uint8_t fatMirr = 0;
            fatMirr < fileSystem->getBPB()->bpbFATCount; fatMirr++) {
            result = fileSystem->getIODevice()->writeSector(
                reinterpret_cast<uint8_t*>(fatArea) + bytespersector *
                (*i - fileSystem->getBPB()->bpbReservedSectors),
                fileSystem->getActualLBA(*i) +
                fatMirr * fileSystem->getBPB()->ebpbSectorsPerFAT32, 1);
            if (fatMirr == 0 && result)
                throw result;
        }
    }

    // Write result to FSINFO structure
    fileSystem->fsinfo.fsinfoFreeCluster -= addCount;
    fileSystem->fsinfo.fsinfoNextFree = nextFreeCluster;
    result = fileSystem->__updateFsInfo();
    if (result)
        return result;

    return 0;
}

int openfsl::FAT32::LinkedCluster::removeCluster(cluster_t removeCount) {
    std::set<lba48_t> modifiedSectorList;
    cluster_t* fatArea = fileSystem->getFatArea()->getDataCast<cluster_t>();

    size_t bytespersector =
        fileSystem->getIODevice()->getDiskParameter().bytesPerSector;
    error_t result;

    for (lba48_t i = 0; i < removeCount; i++) {
        fatArea[clusterList.at(clusterList.size() - 1 - i)] = 0x0;
        modifiedSectorList.insert(static_cast<lba48_t>
            (fileSystem->__fatAreaIndexToSector(
                clusterList.at(clusterList.size() - 1 - i))));
    }

    for (std::set<lba48_t>::iterator i = modifiedSectorList.begin();
         i != modifiedSectorList.end(); ++i) {
        for (uint8_t fatMirr = 0;
            fatMirr < fileSystem->getBPB()->bpbFATCount; fatMirr++) {
            result = fileSystem->getIODevice()->writeSector(
                reinterpret_cast<uint8_t*>(fatArea) + bytespersector *
                (*i - fileSystem->getBPB()->bpbReservedSectors),
                fileSystem->getActualLBA(*i) +
                fatMirr * fileSystem->getBPB()->ebpbSectorsPerFAT32, 1);
            if (fatMirr == 0 && result)
                throw result;
        }
    }

    // Write result to FSINFO structure
    fileSystem->fsinfo.fsinfoFreeCluster += removeCount;
    fileSystem->fsinfo.fsinfoNextFree = fileSystem->__getNextFreeCluster();
    result = fileSystem->__updateFsInfo();
    if (result)
        return result;

    updateClusterList();

    return 0;
}
