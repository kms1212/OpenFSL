/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fat32.h"

int openfsl::fat32::ClusterChain::read(
    void* buf, const size_t pos, const size_t size) {
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

    memcpy(buf, reinterpret_cast<uint8_t*>(fetchedSectorCache->getData()) +
        (startCluster - currentCacheClusterIndex) * bytespersector +
        pos % bytespersector, size);

    return 0;
}
