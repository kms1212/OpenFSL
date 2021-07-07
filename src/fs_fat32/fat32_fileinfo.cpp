/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

FAT32_fileInfo FS_FAT32::getFileInformation(std::string path) {
    std::string currentPath_temp = currentPath;
    uint32_t currentCluster_temp = currentCluster;
    
    std::string filename_path = "";
    FAT32_fileInfo ret;
    
    for (int i = 0; i < pathSeparator.length(); i++)
    {
        if (path.find(pathSeparator.at(i)) < path.length()) {
            chdir(path.substr(0, path.find_last_of(pathSeparator)));
            filename_path = path.substr(path.find_last_of(pathSeparator) + 1, path.length() - path.find_last_of(pathSeparator) - 1);
        }
    }
    if (filename_path == "")
        filename_path = path;
    
    std::transform(filename_path.begin(), filename_path.end(), filename_path.begin(), ::toupper);
    
    std::vector<FAT32_fileInfo> buf;
    getDirList(&buf);
    
    std::string filename;
    for (uint32_t i = 0; i < buf.size(); i++)
    {
        filename = buf[i].fileName;
        std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);
        if (filename == filename_path && buf[i].fileAttr == 0x20)
        {
            ret = buf[i];
        }
    }
    
    currentPath = currentPath_temp;
    currentCluster = currentCluster_temp;
    
    return ret;
}