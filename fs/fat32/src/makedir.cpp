/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#include "openfsl/fslutils.h"

#include "openfsl/fat32/fs_fat32.h"

error_t openfsl::FAT32::__makeDir(
    const std::string path, const std::string dirname, const Time* createTime) {
    int valid = getEntryCount(dirname, false);
    if (!valid)
        return OPENFSL_ERROR_INVALID_FILE_NAME;

    std::string tempPath = currentPath;
    uint32_t tempCluster = currentCluster;

    if (path != "") {
        int errcode = __chdir(path);
        if (errcode) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return errcode;
        }
    }

    FileInfo fileInfo;

    fileInfo.fileName = dirname;
    fileInfo.fileAttr = FAT32::FileAttribute::Directory;
    fileInfo.fileLocation = __getNextFreeCluster();

    LinkedCluster lcluster(this, fileInfo.fileLocation);

    if (createTime == nullptr) {
        struct tm time_tm = openfsl::fsl_localtime(time(nullptr));
        fileInfo.fileCreateTime = convertToFSLTime(&time_tm);
        fileInfo.fileAccessTime = convertToFSLTime(&time_tm);
        fileInfo.fileModTime = convertToFSLTime(&time_tm);
    } else {
        fileInfo.fileCreateTime = *createTime;
        fileInfo.fileAccessTime = *createTime;
        fileInfo.fileModTime = *createTime;
    }

    int result = __createEntry("", fileInfo);
    if (result)
        return result;

    currentCluster = fileInfo.fileLocation;

    fileInfo.fileName = ".";
    int errcode = __createEntry("", fileInfo);
    if (errcode) {
        currentCluster = tempCluster;
        return errcode;
    }

    fileInfo.fileName = "..";
    fileInfo.fileLocation = tempCluster;
    errcode = __createEntry("", fileInfo);
    if (errcode) {
        currentCluster = tempCluster;
        return errcode;
    }

    std::string tempcachekey =
        convertToAbsolutePath(dirname);
    for (auto & c : tempcachekey) c = static_cast<char>(toupper(c));
    if (cacheEnabled) {
        __insertCache(tempcachekey, CacheType::PathClusterCache,
            &currentCluster, sizeof(cluster_t));
    }

    currentPath = tempPath;
    currentCluster = tempCluster;

    return 0;
}
