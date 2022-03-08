/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

openfsl::lba48_t
    openfsl::FAT32::LinkedCluster::clusterToLBA(const uint32_t cluster) {
    return fileSystem->getBPB()->bpbReservedSectors +
        fileSystem->getBPB()->ebpbSectorsPerFAT32 *
        fileSystem->getBPB()->bpbFATCount +
        (cluster - fileSystem->getBPB()->ebpbRootDirectoryCluster) *
        fileSystem->getBPB()->bpbSectorPerCluster;
}

openfsl::FAT32::cluster_t openfsl::FAT32::LinkedCluster::calculateLinkLength() {
    cluster_t count = 0;
    // Repeat getNextCluster() while end
    for (uint32_t temp = static_cast<uint32_t>(startCluster);
        temp < 0x0FFFFFF0; temp = getNextCluster(temp))
        count++;
    return count;
}
