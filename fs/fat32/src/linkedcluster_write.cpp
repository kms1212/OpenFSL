/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <iomanip>

#include "openfsl/fat32/fs_fat32.h"

int openfsl::FAT32::LinkedCluster::write(
    const void* buf, const size_t pos, const size_t size) {
    size_t bytespersector =
        fileSystem->getIODevice()->getDiskParameter().bytesPerSector;
    uint8_t sectorpercluster = fileSystem->getBPB()->bpbSectorPerCluster;

    cluster_t startCluster =
        static_cast<cluster_t>(pos / (bytespersector * sectorpercluster));
    cluster_t clusterSize =
        static_cast<cluster_t>(((pos + size - 1) /
            (bytespersector * sectorpercluster)) + 1 - startCluster);

    int errorcode = fetch(startCluster, clusterSize);
    if (errorcode)
        return errorcode;

    memcpy(  // Cache data addr
        fetchedSectorCache->getDataCast<uint8_t>() +
        // Data position
        (startCluster - currentCacheClusterIndex) *
        bytespersector + pos % bytespersector,
        buf, size);

    for (size_t i = 0; i < clusterSize; i++) {
        if (modifiedClusters.find(clusterList.at(startCluster + i)) ==
            modifiedClusters.end()) {
            modifiedClusters.insert({clusterList.at(startCluster + i),
                new Sector(sectorpercluster, bytespersector)});
        }
        memcpy(modifiedClusters.at(
            clusterList.at(startCluster + i))->getDataCast<uint8_t>(),
            fetchedSectorCache->getDataCast<uint8_t>() +
            (startCluster - currentCacheClusterIndex) * bytespersector,
            bytespersector * sectorpercluster);
    }

    return 0;
}
