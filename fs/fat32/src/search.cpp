/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <regex>

#include "openfsl/fslutils.h"

#include "openfsl/fat32/fs_fat32.h"

error_t openfsl::FAT32::__search(
    std::vector<std::pair<std::string, FileInfo>>* buf,
    const std::string path, const std::string regex,
    const FileAttribute attr, const bool recurse,
    const bool isRecurseCall) {
    std::string tempPath = currentPath;
    cluster_t tempCluster = currentCluster;
    if (!isRecurseCall)
        buf->clear();

    if (path != "") {  // Change directory if path is set
        int errcode = __chdir(path);
        if (errcode) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return errcode;
        }
    }

    std::vector<FileInfo> buftemp;

    error_t result = __listDir(&buftemp);
    if (result) {
        currentPath = tempPath;
        currentCluster = tempCluster;
        return result;
    }

    try {
        std::regex regexScript(regex, std::regex::icase);

        for (FileInfo fileInfo : buftemp) {
            std::string currentFileFullname =
                currentPath + pathSeparator.at(0) + fileInfo.fileName;

            if (((fileInfo.fileAttr & attr) != (FileAttribute)0) &&
                std::regex_match(fileInfo.fileName, regexScript) &&
                !((fileInfo.fileName == ".") || (fileInfo.fileName == ".."))) {
                buf->push_back(std::make_pair(currentPath, fileInfo));
            }
            if (((fileInfo.fileAttr &
                openfsl::FAT32::FileAttribute::Directory) != (FileAttribute)0) &&
                recurse &&
                !((fileInfo.fileName == ".") || (fileInfo.fileName == ".."))) {
                result = __search(
                    buf, currentFileFullname, regex, attr, true, true);
                if (result) {
                    currentPath = tempPath;
                    currentCluster = tempCluster;
                    return result;
                }
            }
        }
    } catch (std::regex_error&) {
        currentPath = tempPath;
        currentCluster = tempCluster;
        return OPENFSL_ERROR_REGEX;
    }

    currentPath = tempPath;
    currentCluster = tempCluster;

    return 0;
}

error_t openfsl::FAT32::__search(std::vector<FileInfo>* buf,
    const std::string path, const std::string regex,
    const FileAttribute attr) {
    return 0;
}

error_t openfsl::FAT32::__search(std::vector<FileInfo>* buf,
    const cluster_t cluster, const std::string regex,
    const FileAttribute attr) {
    return 0;
}
