/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

FSL_File<FS_FAT32>* FS_FAT32::openFile(std::string path, FSL_OpenMode mode_) {
    FSL_File<FS_FAT32>* ret = new FSL_File<FS_FAT32>(this, getFileInformation(path), mode_);
    
    if (ret->getFileController()->getFileInfo().fileName == "")
        return NULL;
    
    return ret;
}

void FS_FAT32::closeFile(FSL_File<FS_FAT32>* file) {
    delete file;
}