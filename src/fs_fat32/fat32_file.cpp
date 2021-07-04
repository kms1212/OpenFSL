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
    
    FAT32_fileInfo* buf = new FAT32_fileInfo[getChildCount()];
        
    getDirList(buf);
    
    std::string filename;
    for (uint32_t i = 0; i < getChildCount(); i++)
    {
        filename = buf[i].fileName;
        std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);
        if (filename == filename_path && buf[i].fileAttr == 0x20)
        {
            ret = buf[i];
        }
    }
    
    delete[] buf;
    
    currentPath = currentPath_temp;
    currentCluster = currentCluster_temp;
    
    return ret;
}

FAT32_File::FAT32_File(FS_FAT32* fileSystem_, FAT32_fileInfo fileInfo_, std::string mode_) {
    fileSystem = fileSystem_;
    fileInfo = fileInfo_;
    openMode = mode_;
}

FAT32_File* FS_FAT32::openFile(std::string path, std::string mode_) {
    FAT32_File* ret = new FAT32_File(this, getFileInformation(path), std::string(mode_));
    
    if (ret->getFileInfo().fileName == "")
        return NULL;
    
    return ret;
}

void FS_FAT32::closeFile(FAT32_File* file) {
    delete file;
}

FAT32_fileInfo FAT32_File::getFileInfo() {
    return fileInfo;
}

int FAT32_File::read(uint8_t* buf, uint32_t len) {
    if (openMode.find_first_of("r+") != std::string::npos) {
        uint32_t cluster = fileInfo.fileLocation;
        
        for (uint32_t i = 0; i < seekLocation / fileSystem->getDiskDevice()->getBytespersector(); i++)
        {
            cluster = fileSystem->getNextCluster(cluster);
            if (cluster >= 0xFFFFFFF7)
                return 1;
        }
        
        uint32_t readCount = len / fileSystem->getDiskDevice()->getBytespersector() + 1;
        readCount = fileSystem->getLinkedClusterCount(cluster) < readCount ? fileSystem->getLinkedClusterCount(cluster) : readCount;
    
        
        Sector sector(readCount, fileSystem->getSectorPerCluster() * fileSystem->getDiskDevice()->getBytespersector());
        fileSystem->getLinkedCluster(&sector, cluster, readCount);
        
        if (seekLocation + len > fileInfo.fileSize)
            return 1;
        else {
            int base = seekLocation % fileSystem->getDiskDevice()->getBytespersector();
            for (uint32_t i = 0; i < len; i++)
                buf[i] = sector.getData()[i + base];
            if (openMode.find_first_of('b') == std::string::npos)
            {
                buf[len] = 0;
            }
        }
    } else return 1;
        
    return 0;
}

int FAT32_File::seek(uint32_t location) {
    if (seekLocation > fileInfo.fileSize)
        return 1;
    else {
        seekLocation = location;
        return 0;
    }
}