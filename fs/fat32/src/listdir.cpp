/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fat32.h"

error_t openfsl::fat32::FAT32::__listDir(std::vector<FileInfo>* buf,
    const std::string path, const FileAttribute searchAttr) {
    std::string tempPath = currentPath;
    uint32_t tempCluster = currentCluster;

    error_t result;

    if (path != "") {  // Change directory if path is set
        result = __chdir(path);
        if (result) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return result;
        }
    }

    result = __listDir(buf, searchAttr);

    currentPath = tempPath;
    currentCluster = tempCluster;

    return result;
}

error_t openfsl::fat32::FAT32::__listDir(std::vector<FileInfo>* buf,
    const uint32_t cluster, const FileAttribute searchAttr) {
    std::string tempPath = currentPath;
    uint32_t tempCluster = currentCluster;

    currentCluster = cluster;

    error_t result = __listDir(buf, searchAttr);

    currentPath = tempPath;
    currentCluster = tempCluster;

    return result;
}

int openfsl::fat32::FAT32::__listDir(std::vector<FileInfo>* buf,
    const FileAttribute searchAttr) {
    buf->clear();

    int errcode = __forEachEntry([=, &buf](FileInfo fileInfo) {
        // Filter file attribute
        if ((fileInfo.fileAttr & searchAttr) != (FileAttribute)0) {
            buf->push_back(fileInfo);
        }
        return false;
        });

    if (errcode)
        return errcode;

    return 0;
}
