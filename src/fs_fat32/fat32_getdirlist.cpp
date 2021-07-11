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

std::vector<FS_FAT32::FileInfo>* FS_FAT32::getDirList(std::vector<FS_FAT32::FileInfo>* buf, std::string path, uint32_t cluster) {
    if (path != "") { // Change directory if path is set
        chdir(path);
    }
    if (cluster == 0xFFFFFFFF) { // Set currentCluster to cluster if cluster is not set
        cluster = currentCluster;
    }
    
    LinkedCluster lcluster(this, cluster);
    
    int index = 0;
    
    FAT32_entry* fileEntry = new FAT32_entry();
        
    std::string lfnBuf = "";
    for (size_t i = 0; i < 65535; i++) { // Add entries to buffer
        if (lcluster.fetch(fileEntry, i * sizeof(FAT32_entry), sizeof(FAT32_entry)) || fileEntry->fileAttr == 0x00) // Return if search hits bottom of the directory file entry list
        {
            return buf;
        }
        else if (fileEntry->fileName[0] != 0xE5) // If file is not deleted
        {
            if ((fileEntry->fileAttr == 0x10) || (fileEntry->fileAttr == 0x20)) { // If file is file or folder
                std::string filename;
                
                if (lfnBuf != ""){ // Get LFN name
                    filename = lfnBuf;
                    lfnBuf = "";
                }
                else { // Get SFN name
                    filename = (char*)fileEntry->fileName;
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
                
                FileInfo fileInfo; // Copy information to buf
                
                fileInfo.fileName = filename;
                fileInfo.fileAttr = fileEntry->fileAttr;
                fileInfo.fileCreateTime.time_millis = (fileEntry->fileCreateTenth % 10) * 100;
                fileInfo.fileCreateTime.time_sec = ((fileEntry->fileCreateTime & 0x001F) * 2) + fileEntry->fileCreateTenth / 10;
                fileInfo.fileCreateTime.time_min = (fileEntry->fileCreateTime & 0x07E0) >> 5;
                fileInfo.fileCreateTime.time_hour = (fileEntry->fileCreateTime & 0xF800) >> 11;
                fileInfo.fileCreateTime.time_day = (fileEntry->fileCreateDate & 0x001F);
                fileInfo.fileCreateTime.time_month = (fileEntry->fileCreateDate & 0x01E0) >> 5;
                fileInfo.fileCreateTime.time_year = ((fileEntry->fileCreateDate & 0xFE00) >> 9) + 1980;
                
                fileInfo.fileModTime.time_sec = (fileEntry->fileModTime & 0x001F) * 2;
                fileInfo.fileModTime.time_min = (fileEntry->fileModTime & 0x07E0) >> 5;
                fileInfo.fileModTime.time_hour = (fileEntry->fileModTime & 0xF800) >> 11;
                fileInfo.fileModTime.time_day = (fileEntry->fileModDate & 0x001F);
                fileInfo.fileModTime.time_month = (fileEntry->fileModDate & 0x01E0) >> 5;
                fileInfo.fileModTime.time_year = ((fileEntry->fileModDate & 0xFE00) >> 9) + 1980;
                
                fileInfo.fileAccessTime.time_day = (fileEntry->fileAccessDate & 0x001F);
                fileInfo.fileAccessTime.time_month = (fileEntry->fileAccessDate & 0x01E0) >> 5;
                fileInfo.fileAccessTime.time_year = ((fileEntry->fileAccessDate & 0xFE00) >> 9) + 1980;
                
                fileInfo.fileSize = fileEntry->fileSize;
                fileInfo.fileLocation = (fileEntry->fileLocationHigh << 16) + fileEntry->fileLocationLow;
                
                buf->push_back(fileInfo);
                
                index++;
            }
            else if (fileEntry->fileAttr == 0x0F)
            {
                char buf[14] = { 0 };
                FAT32_lfn* entry = (FAT32_lfn*)fileEntry;
                str16to8((uint8_t*)buf, entry->lfnFileName1, 5);
                str16to8((uint8_t*)(buf + 5), entry->lfnFileName2, 6);
                str16to8((uint8_t*)(buf + 11), entry->lfnFileName3, 2);
                lfnBuf.insert(0, buf);
            }
        }
    }
    
    return buf;
}