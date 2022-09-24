/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fat32.h"

std::pair<error_t, openfsl::fat32::FileInfo>
    openfsl::fat32::FAT32::__getEntryInformation(
    const std::string path, const std::string filename) {
    std::string tempPath = currentPath;
    cluster_t tempCluster = currentCluster;

    if (path != "") {
        error_t result = __chdir(path);
        if (result) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return std::make_pair(result, FileInfo());
        }
    }

    FileInfo ret;

    std::string lfilename = filename;
    for (auto & c : lfilename) c = static_cast<char>(toupper(c));

    std::string searchname;
    error_t result = __forEachEntry([=, &ret, &searchname](FileInfo fileInfo) {
        searchname = fileInfo.fileName;
        for (auto & c : searchname) c = static_cast<char>(toupper(c));

        if ((fsOptions.AllowSFNSearch && (fileInfo.fileSFNName == lfilename)) ||
            (lfilename == searchname)) {
            ret = fileInfo;
            return true;  // Stop loop
        }
        return false;
        });
    if (result)
        return std::make_pair(result, FileInfo());

    currentPath = tempPath;
    currentCluster = tempCluster;

    return std::make_pair(0, ret);
}
