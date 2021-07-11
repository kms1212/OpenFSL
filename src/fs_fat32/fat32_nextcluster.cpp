/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

uint32_t FS_FAT32::getNextFreeCluster()
{
    int freeCluster;
    for (int i = 0; i < bpb->bpbReservedSectors; i++)
        if (fatClusterList[i] == 0) {
            std::cout << i << "\n";
            return i;
        }
    return 0;
}