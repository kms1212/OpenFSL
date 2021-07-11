/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

int FS_FAT32::FILE::seek(size_t location) {
    if (seekLocation > fileInfo.fileSize)
        return 1;
    else {
        seekLocation = location;
        return 0;
    }
}