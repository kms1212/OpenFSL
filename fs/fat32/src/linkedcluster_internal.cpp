/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

int openfsl::FAT32::LinkedCluster::updateClusterList() {
    clusterList.clear();
    cluster_t cluster = startCluster;

    for (;;) {
        clusterList.push_back(cluster);
        cluster = getNextCluster(cluster);
        if (cluster == 0xFFFFFFF7)
            return OPENFSL_ERROR_BAD_CLUSTER;
        if (cluster == 0xFFFFFFF8)
            break;
    }

    return 0;
}

openfsl::FAT32::cluster_t
    openfsl::FAT32::LinkedCluster::getNextCluster(const cluster_t cluster) {
    cluster_t temp = cluster & ~(0xF << 28);  // Ignore first 4 bits
    // Get next cluster
    temp = (reinterpret_cast<cluster_t*>
        (fileSystem->getFatArea()->getData()))[temp] & ~(0xF << 28);
    if (0xFFFFFFF - temp <= 7)  // Check bad cluster or cluster end
        return 0xFFFFFFF8;
    else if (0xFFFFFFF - temp == 8)
        return 0xFFFFFFF7;
    else
        return temp;
}
