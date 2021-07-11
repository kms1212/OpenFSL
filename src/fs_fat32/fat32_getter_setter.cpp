/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

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

uint32_t* FS_FAT32::getFatClusterList() {
    return fatClusterList;
}

FS_FAT32::FAT32_bpb* FS_FAT32::getBPB() {
    return bpb;
}