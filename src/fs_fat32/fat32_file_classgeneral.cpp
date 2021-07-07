/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

FAT32_File::FAT32_File(FS_FAT32* fileSystem_, FAT32_fileInfo fileInfo_, std::string mode_) {
    fileSystem = fileSystem_;
    fileInfo = fileInfo_;
    openMode = mode_;
}