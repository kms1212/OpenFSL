/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

/*
int FS_FAT32::updateFAT(uint32_t cluster)
{
    int next = getNextFreeCluster(); // get free cluster
    if (next == 0) // return if cannot get free cluster
        return 0;
    fatClusterList[next] = 0x0FFFFFFF; // allocate cluster
    fatClusterList[getLastCluster(cluster)] = next; // set pointer to allocated cluster
    nextCluster = getNextFreeCluster(); // set next free cluster
    freeCluster--; // decrease free cluster
    return nextCluster;
}
*/