/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fat32.h"

error_t openfsl::fat32::FAT32::changeDirectory(const std::string directory) {
    fsOperationMutex.lock();

    error_t result = __chdir(directory);

    fsOperationMutex.unlock();

    return result;
}

error_t openfsl::fat32::FAT32::listDirectory
    (std::vector<FileInfo>* buf, const std::string directory,
    FileAttribute searchAttr) {
    fsOperationMutex.lock();

    error_t result = __listDir(buf, directory, searchAttr);

    fsOperationMutex.unlock();

    return result;
}

error_t openfsl::fat32::FAT32::makeDirectory(const std::string dirname) {
    // Separate path and directory name
    std::pair<std::string, std::string> filepath = __separateFileName(dirname);
    std::string fileDirectory = filepath.first;
    std::string fileName = filepath.second;

    fsOperationMutex.lock();

    error_t result = __makeDir(fileDirectory, fileName);

    fsOperationMutex.unlock();

    return result;
}

error_t openfsl::fat32::FAT32::removeDirectory(const std::string dirname) {
    // Separate path and directory name
    std::pair<std::string, std::string> filepath = __separateFileName(dirname);
    std::string fileDirectory = filepath.first;
    std::string fileName = filepath.second;

    fsOperationMutex.lock();

    error_t result = __removeDir(fileDirectory, fileName);

    fsOperationMutex.unlock();

    return result;
}
