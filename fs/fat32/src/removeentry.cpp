/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

error_t openfsl::FAT32::__removeEntry(
    const std::string path, const std::string deletename,
    const FAT32::FileAttribute filter) {
    if ((deletename == ".") || (deletename == ".."))
        return OPENFSL_ERROR_OPERATION_NOT_PERMITTED;

    std::string tempPath = currentPath;
    uint32_t tempCluster = currentCluster;

    if (path != "") {  // Change directory if path is set
        error_t result = __chdir(path);
        if (result)
            return result;
    }

    // Check if file exists
    std::vector<FileInfo> fileList;
    std::string searchFilename = deletename;
    FileInfo entryInfo;

    std::pair<error_t, FileInfo> eresult =
            __getEntryInformation(currentPath, searchFilename);
    if (eresult.first) {
        currentPath = tempPath;
        currentCluster = tempCluster;
        return eresult.first;
    }

    entryInfo = eresult.second;

    LinkedCluster lcluster(this, currentCluster);

    uint8_t deletionMark = 0xE5;

    for (size_t i = entryInfo.fileEntryIndex;
        i >= entryInfo.fileEntryIndex + 1 - entryInfo.fileEntrySize; i--) {
        error_t result = lcluster.write(&deletionMark,
            i * sizeof(FileEntry), sizeof(uint8_t));
        if (result) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return result;
        }

        result = lcluster.flush();
        if (result) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return result;
        }
    }

    currentPath = tempPath;
    currentCluster = tempCluster;

    return 0;
}
