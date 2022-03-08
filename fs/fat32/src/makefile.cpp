/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fslutils.h"

#include "openfsl/fat32/fs_fat32.h"

int openfsl::FAT32::__makeFile(
    const std::string path, const std::string filename, const Time* createTime) {
    int valid = getEntryCount(filename, false);
    if (!valid)
        return OPENFSL_ERROR_INVNAME;

    FileInfo fileInfo;

    LinkedCluster lcluster(this, __getNextFreeCluster());

    fileInfo.fileName = filename;
    fileInfo.fileAttr = FAT32::FileAttribute::Archive;
    fileInfo.fileLocation = lcluster.getClusterList()->at(0);

    if (createTime == nullptr) {
        struct tm tmp = fsl_localtime(time(nullptr));

        fileInfo.fileCreateTime = convertToFSLTime(&tmp);
        fileInfo.fileAccessTime = fileInfo.fileCreateTime;
        fileInfo.fileModTime = fileInfo.fileCreateTime;
    } else {
        fileInfo.fileCreateTime = *createTime;
        fileInfo.fileAccessTime = *createTime;
        fileInfo.fileModTime = *createTime;
    }

    int result = __createEntry(path, fileInfo);
    if (result)
        return result;

    std::string tempcachekey =
        convertToAbsolutePath(filename);
    for (auto & c : tempcachekey) c = static_cast<char>(toupper(c));
    if (cacheEnabled) {
        __insertCache(tempcachekey, CacheType::PathClusterCache,
            &fileInfo.fileLocation, sizeof(cluster_t));
    }

    return 0;
}
