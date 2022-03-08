/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

error_t openfsl::FAT32::__setEntryInformation(
    const std::string path, const std::string filename,
    const FileInfo fileInfo) {
    std::string tempPath = currentPath;
    uint32_t tempCluster = currentCluster;

    if (path != "") {  // Change directory if path is set
        error_t result = __chdir(path);
        if (result) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return result;
        }
    }

    error_t result = __removeEntry("", filename, FileAttribute::Any);
    if (result) {
        currentPath = tempPath;
        currentCluster = tempCluster;
        return result;
    }

    result = __createEntry("", fileInfo);
    if (result) {
        currentPath = tempPath;
        currentCluster = tempCluster;
        return result;
    }

    currentPath = tempPath;
    currentCluster = tempCluster;

    return 0;
}
