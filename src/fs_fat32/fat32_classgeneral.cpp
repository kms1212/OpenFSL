/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

FS_FAT32::FS_FAT32(DiskDevice* dd_, FAT32_Option option_, std::string pathSeparator_) {
    if (dd_ == NULL) {
        isDiskDeviceAllocated = true;
        dd = new DiskDevice();
    }
    else {
        isDiskDeviceAllocated = false;
        dd = dd_;
    }
    option = option_;
    pathSeparator = pathSeparator_;
}

FS_FAT32::~FS_FAT32() {
    if (isDiskDeviceAllocated) {
        dd->close();                               // Close disk device
        delete dd;                             // Delete disk device
    }
    delete fatArea;                            // Delete loaded fat area
}

void FS_FAT32::setDiskDevice(DiskDevice* dd_)
{
    dd = dd_;
}

DiskDevice* FS_FAT32::getDiskDevice()
{
    return dd;
}

std::string FS_FAT32::getPath() {
    return currentPath;
}

uint16_t FS_FAT32::getSectorPerCluster() {
    return sectorPerCluster;
}