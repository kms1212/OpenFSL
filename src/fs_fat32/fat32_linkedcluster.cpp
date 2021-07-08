/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

uint32_t FS_FAT32::getLinkedClusterCount(uint32_t cluster)
{
    uint32_t count = 0;
    for (uint32_t temp = cluster; (temp != 0xFFFFFFF7) && (temp != 0xFFFFFFF8); temp = getNextCluster(temp)) // Repeat getNextCluster() while end
        count++;
    return count;
}

int FS_FAT32::getLinkedCluster(Sector* sector, uint32_t cluster, uint32_t count)
{
    uint32_t linkedClusterCount = getLinkedClusterCount(cluster);
    if (linkedClusterCount == 0)
    {
        return 1;
    }
    
    int clusterCursor = cluster;
    int clusterCountToRead = ((count != 0) && count < linkedClusterCount) ? count : linkedClusterCount; // Get linked cluster count and compare with given count and set smaller value
    
    for (int i = 0; i < clusterCountToRead; i++)
    {
        Sector temp(sectorPerCluster, dd->getBytespersector());
        dd->readDisk(&temp, resvSectorCount + fatSize32 * fatCount + clusterCursor - rootCluster, sectorPerCluster);
        
        memcpy(sector->getData() + i * dd->getBytespersector(), temp.getData(), dd->getBytespersector());
        clusterCursor = getNextCluster(clusterCursor);
        if (clusterCursor == 0xFFFFFFF7)
            return 1;
        if (clusterCursor == 0xFFFFFFF8)
            break;
    }
    
    return 0;
}