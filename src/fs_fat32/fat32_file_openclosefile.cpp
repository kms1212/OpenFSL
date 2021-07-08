/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

FAT32_File* FS_FAT32::openFile(std::string path, std::string mode_) {
    FAT32_File* ret = new FAT32_File(this, getFileInformation(path), std::string(mode_));
    
    if (ret->getFileInfo().fileName == "")
        return NULL;
    
    return ret;
}

void FS_FAT32::closeFile(FAT32_File* file) {
    delete file;
}