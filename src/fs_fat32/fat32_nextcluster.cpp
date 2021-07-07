/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

uint32_t FS_FAT32::getNextCluster(uint32_t cluster)
{
    uint32_t temp = cluster & ~(0xF << 28);     // Ignore first 4 bits
    temp = fatClusterList[temp] & ~(0xF << 28); // Get next cluster
    if (0xFFFFFFF - temp <= 7)                  // Check bad cluster or cluster end
        return 0xFFFFFFF8;
    else if (0xFFFFFFF - temp == 8)
        return 0xFFFFFFF7;
    else
        return temp;
}

uint32_t FS_FAT32::getNextFreeCluster()
{
    int freeCluster;
    for (int i = 0; i < fatSize32; i++)
        if (fatClusterList[i] == 0) {
            std::cout << i << "\n";
            return i;
        }
    return 0;
}