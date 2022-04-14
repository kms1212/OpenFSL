/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

openfsl::FAT32::VolumeInfo openfsl::FAT32::getVolumeInfo() {
    VolumeInfo ret;

    ret.volumeSerial = bpb.ebpbVolumeSerial;
    ret.volumeLabel =
        std::string(reinterpret_cast<char*>(bpb.ebpbVolumeLabel))
            .substr(0, 11);
    ret.freeCluster = fsinfo.fsinfoFreeCluster;
    ret.nextFreeCluster = fsinfo.fsinfoNextFree;
    ret.sectorPerCluster = bpb.bpbSectorPerCluster;
    ret.bytesPerSector = bpb.bpbBytesPerSector;
    ret.filesystemSize = bpb.bpbTotalSector32;

    return ret;
}

error_t openfsl::FAT32::setVolumeName(const std::string volumeName) {
    memset(bpb.ebpbVolumeLabel, ' ', 11);
    memcpy(bpb.ebpbVolumeLabel, volumeName.c_str(), volumeName.size());

    fsOperationMutex.lock();

    error_t result = __updateBpb();

    fsOperationMutex.unlock();
    return result;
}
