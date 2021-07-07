/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

int FAT32_File::read(uint8_t* buf, uint32_t len) {
    if (openMode.find_first_of("r+") != std::string::npos) {
        uint32_t cluster = fileInfo.fileLocation;
        
        for (uint32_t i = 0; i < seekLocation / fileSystem->getDiskDevice()->getBytespersector(); i++)
        {
            cluster = fileSystem->getNextCluster(cluster);
            if (cluster >= 0xFFFFFFF7)
                return 1;
        }
        
        uint32_t readCount = len / fileSystem->getDiskDevice()->getBytespersector() + 1;
        readCount = fileSystem->getLinkedClusterCount(cluster) < readCount ? fileSystem->getLinkedClusterCount(cluster) : readCount;
    
        
        Sector sector(readCount, fileSystem->getSectorPerCluster() * fileSystem->getDiskDevice()->getBytespersector());
        fileSystem->getLinkedCluster(&sector, cluster, readCount);
        
        if (seekLocation + len > fileInfo.fileSize)
            return 1;
        else {
            int base = seekLocation % fileSystem->getDiskDevice()->getBytespersector();
            for (uint32_t i = 0; i < len; i++)
                buf[i] = sector.getData()[i + base];
            if (openMode.find_first_of('b') == std::string::npos)
            {
                buf[len] = 0;
            }
        }
    } else return 1;
        
    return 0;
}