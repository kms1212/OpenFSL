/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

error_t openfsl::FAT32::__move(const std::string fromPath,
    const std::string fromName,
    const std::string toPath,
    const std::string toName) {
    std::string tempPath = currentPath;
    uint32_t tempCluster = currentCluster;

    if (fromPath != "") {  // Change directory if path is set
        error_t result = __chdir(fromPath);
        if (result) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return result;
        }
    }

    std::string sourceDirectoryPath = currentPath;
    uint32_t sourceDirectoryCluster = currentCluster;
    std::pair<error_t, FileInfo> eresult =
            __getEntryInformation(currentPath, fromName);
    if (eresult.first) {
        currentPath = tempPath;
        currentCluster = tempCluster;
        return eresult.first;
    }

    FileInfo fileInfo = eresult.second;

    fileInfo.fileName = toName;

    currentPath = tempPath;
    currentCluster = tempCluster;

    if (toPath != "") {  // Change directory if path is set
        error_t result = __chdir(toPath);
        if (result) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return result;
        }
    }

    error_t result = __createEntry("", fileInfo);
    if (result) {
        currentPath = tempPath;
        currentCluster = tempCluster;
        return result;
    }

    currentPath = sourceDirectoryPath;
    currentCluster = sourceDirectoryCluster;

    result = __removeEntry("", fromName, FileAttribute::Any);
    if (result) {
        currentPath = tempPath;
        currentCluster = tempCluster;
        return result;
    }

    currentPath = tempPath;
    currentCluster = tempCluster;

    return 0;
}
