/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

int openfsl::FAT32::LinkedCluster::fetch(
    const cluster_t cluster, const cluster_t clusterSize) {
    cluster_t startCluster = cluster;
    cluster_t endCluster = startCluster + clusterSize;

    size_t bytespersector =
        fileSystem->getIODevice()->getDiskParameter().bytesPerSector;
    uint8_t sectorpercluster = fileSystem->getBPB()->bpbSectorPerCluster;

    error_t result;

    if (endCluster > clusterList.size())
        return OPENFSL_ERROR_CANNOT_ACCESS_CLUSTER;

    // Refetch if cache range is smaller than the given range
    if ((currentCacheClusterIndex > startCluster) ||
        (currentCacheClusterIndex + currentCacheClusterSize < endCluster)) {
        currentCacheClusterIndex = startCluster;
        currentCacheClusterSize = clusterSize;

        if (fetchedSectorCache != nullptr)
            delete fetchedSectorCache;
        fetchedSectorCache =
            new Sector(currentCacheClusterSize * sectorpercluster,
            bytespersector * sectorpercluster);

        // Read every linked cluster in clusterSize
        for (lba48_t i = 0; i < clusterSize; i++) {
            result = fileSystem->getIODevice()->readSector(
                (reinterpret_cast<uint8_t*>(fetchedSectorCache->getData())) +
                // buffer + i * cache cluster index
                i * sectorpercluster * bytespersector,
                // get actual lba of current reading cluster
                fileSystem->getActualLBA(
                    clusterToLBA(clusterList.at(startCluster + i))),
                sectorpercluster);
            if (result)
                return result;
        }
    }

    return 0;
}
