/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

std::pair<error_t, openfsl::FAT32::FileInfo>
    openfsl::FAT32::getEntryInfo(const std::string path) {
    // Separate path and entry name
    std::pair<std::string, std::string> filepath = __separateFileName(path);
    std::string fileDirectory = filepath.first;
    std::string fileName = filepath.second;

    fsOperationMutex.lock();

    std::pair<error_t, FileInfo> ret =
        __getEntryInformation(fileDirectory, fileName);

    fsOperationMutex.unlock();

    return ret;
}

error_t openfsl::FAT32::setEntryInfo(
        const std::string path, const openfsl::FAT32::FileInfo fileInfo) {
    // Separate path and entry name
    std::pair<std::string, std::string> filepath = __separateFileName(path);
    std::string fileDirectory = filepath.first;
    std::string fileName = filepath.second;

    fsOperationMutex.lock();

    error_t ret = __setEntryInformation(fileDirectory, fileName, fileInfo);

    fsOperationMutex.unlock();

    return ret;
}
