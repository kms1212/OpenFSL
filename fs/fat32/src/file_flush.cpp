/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

error_t openfsl::FAT32::FILE::flush() {
    if ((openMode & FSL_OpenMode::Write) != (FSL_OpenMode)0) {
        fileOperationMutex.lock();

        error_t result = fileCluster->flush();

        fileOperationMutex.unlock();

        return result;
    }

    return 0;
}
