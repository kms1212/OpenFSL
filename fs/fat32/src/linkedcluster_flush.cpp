/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

int openfsl::FAT32::LinkedCluster::flush() {
    // size_t bytespersector =
    //     fileSystem->getDiskDevice()->getDiskParameter().bytesPerSector;
    uint8_t sectorpercluster = fileSystem->getBPB()->bpbSectorPerCluster;

    error_t result;

    for (auto i = modifiedClusters.begin(); i != modifiedClusters.end(); i++) {
        result = fileSystem->getIODevice()->writeSector(i->second->getData(),
            fileSystem->getActualLBA(clusterToLBA(i->first)), sectorpercluster);
        if (result)
            return result;
    }

    modifiedClusters.clear();

    return 0;
}
