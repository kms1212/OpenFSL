/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

using namespace openFSL;

FS_FAT32::FileInfo FS_FAT32::getFileInformation(std::string path) {
    std::string filename_path = "";
    FileInfo ret;
    
    for (size_t i = 0; i < pathSeparator.length(); i++)
    {
        if (path.find(pathSeparator.at(i)) < path.length()) {
            chdir(path.substr(0, path.find_last_of(pathSeparator)));
            filename_path = path.substr(path.find_last_of(pathSeparator) + 1, path.length() - path.find_last_of(pathSeparator) - 1);
        }
    }
    if (filename_path == "")
        filename_path = path;
    
    for (auto & c: filename_path) c = (char)toupper(c);
    
    std::vector<FileInfo> buf;
    getDirList(&buf);
    
    std::string filename;
    for (uint32_t i = 0; i < buf.size(); i++)
    {
        filename = buf[i].fileName;
        for (auto & c: filename) c = (char)toupper(c);;
        if (filename == filename_path && buf[i].fileAttr == 0x20)
        {
            ret = buf[i];
        }
    }
    
    return ret;
}