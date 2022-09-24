/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/charset.h"

#include "openfsl/fat32/fat32.h"

error_t openfsl::fat32::File::open(const std::string path,
    const openfsl::OpenMode openMode_) {
    // Separate path and directory name
    size_t lastPathSeparator =
        path.find_last_of(fileSystem->getPathSeparator());
    std::string fileName;
    std::string fileDirectory;

    if (lastPathSeparator == std::string::npos) {
        fileDirectory = ".";
        fileName = path;
    } else {
        fileDirectory = path.substr(0, lastPathSeparator);
        fileName = path.substr(lastPathSeparator + 1);
    }

    std::pair<error_t, FileInfo> finfo;

    fileSystem->getFsOperationMutex()->lock();

    finfo = fileSystem->__getEntryInformation(fileDirectory, fileName);

    fileSystem->getFsOperationMutex()->unlock();

    if (finfo.first)
        return finfo.first;

    if (finfo.second.fileName == "") {
        return OPENFSL_ERROR_NO_SUCH_FILE_OR_DIR;
    } else if ((finfo.second.fileAttr & FileAttribute::Archive) ==
        (FileAttribute)0) {
        return OPENFSL_ERROR_INVALID_ACCESS;
    } else {
        currentFileInfo = finfo.second;
        openMode = openMode_;

        fileCluster = new ClusterChain(fileSystem,
            currentFileInfo.fileLocation);

        seekp(0, SeekMode::SeekSet);
        seekg(0, SeekMode::SeekSet);

        if ((openMode & OpenMode::Append) != (OpenMode)0) {
            seekp(0, SeekMode::SeekEnd);
        }
    }

    return 0;
}

error_t openfsl::fat32::File::close() {
    error_t result = flush();

    currentFileInfo = FileInfo();
    openMode = OpenMode();
    delete fileCluster;
    fileCluster = nullptr;
    readPointer = 0;
    writePointer = 0;

    return result;
}

error_t openfsl::fat32::File::reopen(
    const std::string path, const openfsl::OpenMode openMode_) {
    close();
    return open(path, openMode_);
}
