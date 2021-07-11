/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

int FS_FAT32::LinkedCluster::getLinkedClusterList(uint32_t* list, uint32_t cluster, size_t count)
{
    if (count == 0)
    {
        count = getLinkedClusterCount(cluster); // Get cluster count to the end if the argument is not set
        if (count == 0)
            return 1;
    }
    
    for (int i = 0; i < count; i++)
    {
        list[i] = cluster;
        cluster = getNextCluster(cluster);
        if (cluster == 0xFFFFFFF7)
            return 1;
        if (cluster == 0xFFFFFFF8)
            break;
    }
    
    return 0;
}

size_t FS_FAT32::LinkedCluster::getLinkedClusterCount(uint32_t cluster)
{
    size_t count = 0;
    for (uint32_t temp = cluster; (temp != 0xFFFFFFF7) && (temp != 0xFFFFFFF8); temp = getNextCluster(temp)) // Repeat getNextCluster() while end
        count++;
    return count;
}

uint32_t FS_FAT32::LinkedCluster::getNextCluster(uint32_t cluster)
{
    uint32_t temp = cluster & ~(0xF << 28);     // Ignore first 4 bits
    temp = fileSystem->getFatClusterList()[temp] & ~(0xF << 28); // Get next cluster
    if (0xFFFFFFF - temp <= 7)                  // Check bad cluster or cluster end
        return 0xFFFFFFF8;
    else if (0xFFFFFFF - temp == 8)
        return 0xFFFFFFF7;
    else
        return temp;
}

size_t FS_FAT32::LinkedCluster::clusterToLBA(size_t cluster)
{
    return fileSystem->getBPB()->bpbReservedSectors + fileSystem->getBPB()->ebpbSectorsPerFAT32 * fileSystem->getBPB()->bpbFATCount + cluster - fileSystem->getBPB()->ebpbRootDirectoryCluster;
}