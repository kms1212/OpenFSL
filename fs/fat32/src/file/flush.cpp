/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fat32.h"

error_t openfsl::fat32::File::flush() {
    if ((openMode & OpenMode::Write) != (OpenMode)0) {
        fileOperationMutex.lock();

        error_t result = fileCluster->flush();

        fileOperationMutex.unlock();

        return result;
    }

    return 0;
}
