/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"
#include <iostream>

using namespace openFSL;

int FS_FAT32::chdir(std::string path, std::vector<std::string>* subdir, std::string tmpPath, uint32_t tmpCluster) {
    if (subdir == NULL) // If first call of recursion
    {
        std::vector<std::string> subdir_tmp;
        fsl_strtokenize(path, subdir_tmp, pathSeparator); // Tokenize string to subdir list
        
        if (subdir_tmp.empty()) // Return error if nothing given to path
            return 1;
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
            
    if (subdir->front() == "::")
    {
        subdir->erase(subdir->begin());
        return FS_FAT32::chdir(path, subdir, "::", bpb->ebpbRootDirectoryCluster); // Recurse function to next subdir
    }
    
    std::string path_tmp = subdir->front(); 
    for (auto & c: path_tmp) c = (char)toupper(c); // Transform string to upper
            
    if (tmpCluster == bpb->ebpbRootDirectoryCluster) { 
        if (path_tmp == "." || path_tmp == "..") { // If current directory is root and chdir to special directory
            subdir->erase(subdir->begin());
            return FS_FAT32::chdir(path, subdir, tmpPath, tmpCluster);
        }
    }
            
    std::vector<FileInfo> buf;
    getDirList(&buf, "", tmpCluster); // Get directory list
    
    for (size_t i = 0; i < buf.size(); i++) {
        if (buf.at(i).fileAttr == 0x10)
        {
            std::string filename = buf.at(i).fileName;
            for (auto & c: filename) c = (char)toupper(c); // Transform string to upper
            
            if (filename == path_tmp)
            {
                if (path_tmp == ".");
                else if (path_tmp == "..")
                    tmpPath = tmpPath.substr(0, tmpPath.find_last_of(pathSeparator)); // Delete last subdir from path
                else
                    tmpPath += pathSeparator.at(0) + subdir->front(); // Add selected subdir to path
                
                subdir->erase(subdir->begin()); // Delete first element from subdir list
                tmpCluster = buf.at(i).fileLocation; // Set current cluster to subdir
                if (tmpCluster == 0)
                    tmpCluster = bpb->ebpbRootDirectoryCluster;
                return FS_FAT32::chdir(path, subdir, tmpPath, tmpCluster); // Recurse function to next subdir
            }
        }
    }
    

    return 1;
}