/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

size_t FS_FAT32::FILE::read(void* ptr, size_t size, size_t count) {
    if (openMode & FSL_OpenMode::read) {
        for (size_t i = 0; i < count; i++) // i: Number of elements actually read
        {
            if ((i + 1) * size > fileInfo.fileSize)
                return i;
            else if (fileCluster->fetch(((uint8_t*)ptr) + i * size, i * size + seekLocation, size))
                return i;
        }
    }
    return 0;
}