/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

FS_FAT32::LinkedCluster::LinkedCluster(FS_FAT32* fileSystem_, size_t startCluster)
{
    fileSystem = fileSystem_;
    
    clusterList = new uint32_t[getLinkedClusterCount(startCluster)]();
    getLinkedClusterList(clusterList, startCluster);
}

FS_FAT32::LinkedCluster::~LinkedCluster()
{
    if (fetchedSectorCache != NULL)
        delete fetchedSectorCache;
    delete clusterList;
}