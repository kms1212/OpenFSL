/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

int FS_FAT32::LinkedCluster::fetch(void* buf, size_t pos, size_t size)
{
    int result = 0;
    
    size_t startCluster = (pos / fileSystem->getDiskDevice()->getBytespersector());
    size_t endCluster = startCluster + (size / fileSystem->getDiskDevice()->getBytespersector()) + 1;
    
    if ((currentCacheClusterIndex > startCluster) || (currentCacheClusterIndex + currentCacheClusterSize < endCluster)) // Refetch if cache range is smaller than the given range
    {
        currentCacheClusterIndex = startCluster;
        currentCacheClusterSize = (size / fileSystem->getDiskDevice()->getBytespersector()) + 1;
        
        if (fetchedSectorCache != NULL)
            delete fetchedSectorCache;
        fetchedSectorCache = new Sector(currentCacheClusterSize * fileSystem->getBPB()->bpbSectorPerCluster, fileSystem->getDiskDevice()->getBytespersector());
        
        for (int i = 0; i < currentCacheClusterSize; i++)
            result += fileSystem->getDiskDevice()->readDisk(((uint8_t*)fetchedSectorCache->getData()) + i * fileSystem->getBPB()->bpbSectorPerCluster * fileSystem->getDiskDevice()->getBytespersector()
            , clusterToLBA(clusterList[currentCacheClusterIndex]), fileSystem->getBPB()->bpbSectorPerCluster); //
    }
    
    for (int i = 0; i < size; i++)
    {
        ((uint8_t*)buf)[i] = ((uint8_t*)fetchedSectorCache->getData())[pos % fileSystem->getDiskDevice()->getBytespersector() + i];
    }
    
    return result;
}