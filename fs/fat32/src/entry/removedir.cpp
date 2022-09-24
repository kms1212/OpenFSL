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

#include "openfsl/fat32/fat32.h"

error_t openfsl::fat32::FAT32::__removeDir(
    const std::string path, const std::string name) {
    std::string lname = name;
    std::vector<FileInfo> buf;

    uint32_t deleteLocation = 0;
    FileAttribute entryAttribute;

    error_t result = __listDir(&buf, path);
    if (result)
        return result;

    for (auto & c : lname) c = static_cast<char>(toupper(c));
    for (size_t i = 0; i < buf.size(); i++) {
        std::string filename = buf[i].fileName;
        for (auto & c : filename) c = static_cast<char>(toupper(c));

        if (lname == filename) {
            deleteLocation = buf[i].fileLocation;
            entryAttribute = buf[i].fileAttr;

            break;
        }
    }

    if (deleteLocation == 0) {
        return OPENFSL_ERROR_NO_SUCH_FILE_OR_DIR;
    }

    if ((entryAttribute & FileAttribute::Directory) != (FileAttribute)0) {
        std::pair<error_t, bool> empty = __checkEmpty(path, name);
        if (empty.first) {
            return empty.first;
        } else if (!empty.second) {
            return OPENFSL_ERROR_DIRECTORY_NOT_EMPTY;
        }
    } else {
        return OPENFSL_ERROR_NOT_A_DIRECTORY;
    }

    ClusterChain lcluster(this, deleteLocation);
    result = lcluster.removeCluster(
        static_cast<cluster_t>(lcluster.getClusterList()->size()));
    if (result)
        return result;

    result = __removeEntry(path, lname, FileAttribute::Directory);
    if (result)
        return result;

    if (cacheEnabled) {
        std::string tempcachekey = convertToAbsolutePath(
            path + pathSeparator.at(0) + name);
        for (auto & c : tempcachekey) c = static_cast<char>(toupper(c));
        result = __deleteCache(tempcachekey);
        if (result)
            return result;
    }

    return 0;
}
