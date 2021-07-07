/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

uint8_t* str16to8(uint8_t* dest, const uint16_t* src, size_t size) // Temporary function to convert 16bit UCS-2 value to 8bit ASCII (drain)
{
    for (int i = 0; i < size; i++)
        dest[i] = src[i] == 0xFF ? 0 : (uint8_t)src[i];
    return dest;
}


std::vector<FAT32_fileInfo>* FS_FAT32::getDirList(std::vector<FAT32_fileInfo>* buf, std::string path, uint32_t cluster) {
    if (path != "") { // Change directory if path is set
        chdir(path);
    }
    if (cluster == 0xFFFFFFFF) { // Set currentCluster to cluster if cluster is not set
        cluster = currentCluster;
    }
    
    uint32_t linkedClusterCount = getLinkedClusterCount(cluster);                       // Get linked clusters
    if (linkedClusterCount == 0)                                                        //
    {                                                                                   //
        return buf;                                                                     //
    }                                                                                   //
                                                                                        //
    Sector dir_cluster(sectorPerCluster * linkedClusterCount, dd->getBytespersector()); //
                                                                                        //
    if (getLinkedCluster(&dir_cluster, cluster))                                        //
    {                                                                                   //
        return buf;                                                                     //
    }                                                                                   //
    
    int index = 0;
    
    FAT32_entry* fileEntry = (FAT32_entry*)dir_cluster.getData(); // Clusters list to fileEntry
        
    for (uint32_t i = 0; i < 16 * linkedClusterCount; i++) { // Add entries to buuffer
        if (index > 65535) // Entry limit: 65535
        {
            return buf;
        }
        else if (fileEntry[i].fileAttr == 0x00) // Return if search hits bottom of the directory file entry list
        {
            return buf;
        }
        else if (fileEntry[i].fileName[0] != 0xE5) // If file is not deleted
        {
            if ((fileEntry[i].fileAttr == 0x10) || (fileEntry[i].fileAttr == 0x20)) { // If file is file or folder
                std::string filename;
                
                if ((i > 0) && (fileEntry[i - 1].fileAttr == 0x0F)){ // LFN to string
                    for (int lfnIndex = i - 1; fileEntry[lfnIndex].fileAttr == 0x0F; lfnIndex--)
                    {
                        char buf[14] = { 0 };
                        FAT32_lfn* entry = (FAT32_lfn*)&fileEntry[lfnIndex];
                        str16to8((uint8_t*)buf, entry->lfnFileName1, 5);
                        str16to8((uint8_t*)(buf + 5), entry->lfnFileName2, 6);
                        str16to8((uint8_t*)(buf + 11), entry->lfnFileName3, 2);
                        filename += buf;
                    }
                }
                else { // Get SFN name
                    filename = (char*)fileEntry[i].fileName;
                    std::string fileext;
                    if (fileext != "   ") {
                        fileext = filename.substr(8, 3);
                        fileext.erase(fileext.find_last_not_of(" ") + 1);
                    }
                    filename = filename.substr(0, 8);
                    filename.erase(filename.find_last_not_of(" ") + 1);
                    if (!fileext.empty()) 
                        filename += "." + fileext;
                }
                
                FAT32_fileInfo fileInfo; // Copy informatiton to buf
                
                fileInfo.fileName = filename;
                fileInfo.fileAttr = fileEntry[i].fileAttr;
                fileInfo.fileCreateTime.time_millis = (fileEntry[i].fileCreateTenth % 10) * 100;
                fileInfo.fileCreateTime.time_sec = ((fileEntry[i].fileCreateTime & 0b0000000000011111) * 2) + fileEntry[i].fileCreateTenth / 10;
                fileInfo.fileCreateTime.time_min = (fileEntry[i].fileCreateTime & 0b0000011111100000) >> 5;
                fileInfo.fileCreateTime.time_hour = (fileEntry[i].fileCreateTime & 0b1111100000000000) >> 11;
                fileInfo.fileCreateTime.time_day = (fileEntry[i].fileCreateDate & 0b0000000000011111);
                fileInfo.fileCreateTime.time_month = (fileEntry[i].fileCreateDate & 0b0000000111100000) >> 5;
                fileInfo.fileCreateTime.time_year = ((fileEntry[i].fileCreateDate & 0b1111111000000000) >> 9) + 1980;
                
                fileInfo.fileModTime.time_sec = (fileEntry[i].fileModTime & 0b0000000000011111) * 2;
                fileInfo.fileModTime.time_min = (fileEntry[i].fileModTime & 0b0000011111100000) >> 5;
                fileInfo.fileModTime.time_hour = (fileEntry[i].fileModTime & 0b1111100000000000) >> 11;
                fileInfo.fileModTime.time_day = (fileEntry[i].fileModDate & 0b0000000000011111);
                fileInfo.fileModTime.time_month = (fileEntry[i].fileModDate & 0b0000000111100000) >> 5;
                fileInfo.fileModTime.time_year = ((fileEntry[i].fileModDate & 0b1111111000000000) >> 9) + 1980;
                
                fileInfo.fileAccessTime.time_day = (fileEntry[i].fileAccessDate & 0b0000000000011111);
                fileInfo.fileAccessTime.time_month = (fileEntry[i].fileAccessDate & 0b0000000111100000) >> 5;
                fileInfo.fileAccessTime.time_year = ((fileEntry[i].fileAccessDate & 0b1111111000000000) >> 9) + 1980;
                
                fileInfo.fileSize = fileEntry[i].fileSize;
                fileInfo.fileLocation = (fileEntry[i].fileLocationHigh << 16) + fileEntry[i].fileLocationLow;
                
                buf->push_back(fileInfo);
                
                index++;
            }
        }
    }
    
    return buf;
}

int FS_FAT32::chdir(std::string path, std::vector<std::string>* subdir, std::string tmpPath, uint32_t tmpCluster) {
    int count = 0;
    int index = 0;
    
    if (subdir == NULL) // If first call of recursion
    {
        std::vector<std::string> subdir_tmp;
        fsl_strtokenize(path, subdir_tmp, pathSeparator); // Tokenize string to subdir list
        
        if (subdir_tmp.empty()) // Return error if nothing given to path
            return 1;
            
        if (subdir_tmp.front() == "::") // If path starts from root
        {
            if (subdir_tmp.size() <= 1) // Return success if only root identifier left
                return 0;
            subdir_tmp.erase(subdir_tmp.begin());
            return FS_FAT32::chdir(path, &subdir_tmp, "::", rootCluster); // Recurse function to next subdir
        }
        else
        {
            return FS_FAT32::chdir(path, &subdir_tmp, currentPath, currentCluster); // Recurse function to next subdir
        }
    } 
    
    if (subdir->empty()) { // Return if subdir list is empty
        currentPath = tmpPath;
        currentCluster = tmpCluster;
        return 0;
    }
    
    std::string path_tmp = subdir->front(); 
    std::transform(path_tmp.begin(), path_tmp.end(), path_tmp.begin(), ::toupper); // Transform string to upper
            
    if (tmpCluster == rootCluster) { 
        if (path_tmp == "." || path_tmp == "..") { // If current directory is root and chdir to special directory
            subdir->erase(subdir->begin());
            return FS_FAT32::chdir(path, subdir, tmpPath, tmpCluster);
        }
    }
            
    std::vector<FAT32_fileInfo> buf;
    getDirList(&buf, "", tmpCluster); // Get directory list
    
    for (int i = 0; i < buf.size(); i++) {
        if (buf[i].fileAttr == 0x10)
        {
            std::string filename = buf[i].fileName;
            std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper); // Transform string to upper
			
            if (filename == path_tmp)
            {
                if (path_tmp == ".");
                else if (path_tmp == "..")
                    tmpPath = tmpPath.substr(0, tmpPath.find_last_of(pathSeparator)); // Delete last subdir from path
                else
                    tmpPath += pathSeparator.at(0) + subdir->front(); // Add selected subdir to path
                
                subdir->erase(subdir->begin()); // Delete first element from subdir list
                tmpCluster = buf[i].fileLocation; // Set current cluster to subdir
                if (tmpCluster == 0)
                    tmpCluster = rootCluster;
                return FS_FAT32::chdir(path, subdir, tmpPath, tmpCluster); // Recurse function to next subdir
            }
        }
    }

    return 1;
}

int FS_FAT32::mkdir(std::string path) {
    
    return 1;
}