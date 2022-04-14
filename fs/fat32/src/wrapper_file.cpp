/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

error_t openfsl::FAT32::makeFile(const std::string filename) {
    // Separate path and file name
    std::pair<std::string, std::string> filepath = __separateFileName(filename);
    std::string fileDirectory = filepath.first;
    std::string fileName = filepath.second;

    fsOperationMutex.lock();

    error_t result = __makeFile(fileDirectory, fileName);

    fsOperationMutex.unlock();

    return result;
}

error_t openfsl::FAT32::move(const std::string from, const std::string to) {
    // Separate path and file name
    std::pair<std::string, std::string> filepath = __separateFileName(from);
    std::string fromPath = filepath.first;
    std::string fromName = filepath.second;

    filepath = __separateFileName(to);
    std::string toPath = filepath.first;
    std::string toName = filepath.second;

    fsOperationMutex.lock();

    error_t result = __move(fromPath, fromName, toPath, toName);

    fsOperationMutex.unlock();

    return result;
}

error_t openfsl::FAT32::search(
    std::vector<std::pair<std::string, FileInfo>>* buf,
    const std::string path, const std::string regex,
    const FileAttribute attr, const bool recurse) {
    fsOperationMutex.lock();

    error_t result = __search(buf, path, regex, attr, recurse);

    fsOperationMutex.unlock();

    return result;
}

error_t openfsl::FAT32::removeFile(const std::string dirname) {
    // Separate path and directory name
    std::pair<std::string, std::string> filepath = __separateFileName(dirname);
    std::string fileDirectory = filepath.first;
    std::string fileName = filepath.second;

    fsOperationMutex.lock();

    error_t result = __removeFile(fileDirectory, fileName);

    fsOperationMutex.unlock();

    return result;
}
