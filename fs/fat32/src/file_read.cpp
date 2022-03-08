/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

size_t openfsl::FAT32::FILE::read(void* ptr, const size_t bs, const size_t count) {
    if ((openMode & FSL_OpenMode::Read) != (FSL_OpenMode)0) {
        size_t ret = 0;

        fileOperationMutex.lock();

        // i: Number of elements actually read
        for (size_t i = 0; i < count; i++) {
            if (readPointer + bs > currentFileInfo.fileSize) {
                ret = i;
                break;
            }

            int errorcode = fileCluster->read(
                (reinterpret_cast<uint8_t*>(ptr)) + i * bs, readPointer, bs);
            if (errorcode) {
                ret = i;
                break;
            }

            readPointer += bs;
        }

        fileOperationMutex.unlock();

        return ret;
    }

    return 0;
}
