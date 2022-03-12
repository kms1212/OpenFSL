/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fslutils.h"

#include "openfsl/fat32/fs_fat32.h"

error_t openfsl::FAT32::__chdir(
    const std::string path, std::vector<std::string>* subdir) {
    if (subdir == nullptr) {  // If first call of recursion
        if (pathSeparator.find(path.at(0)) != std::string::npos) {
            currentPath = rootSymbol;
            currentCluster = bpb.ebpbRootDirectoryCluster;
        }

        std::vector<std::string> subdir_tmp;
        // Tokenize string to subdir list
        fsl_strtokenize(path, &subdir_tmp, pathSeparator);

        if (subdir_tmp.empty()) {  // Return error if nothing given to path
            return 0;
        } else {
            if (cacheEnabled) {
                std::string absolutepath = convertToAbsolutePath(path);
                std::string absoluteCap = absolutepath;
                for (auto & c : absoluteCap) c = static_cast<char>(toupper(c));
                if (absolutepath == rootSymbol) {
                    goto cacheNotFound;
                }

                std::pair<CacheType, void*> cacheval =
                    __getCacheValue(absoluteCap, CacheType::PathClusterCache);
                if (cacheval.second != nullptr) {
                    currentPath = absolutepath;
                    currentCluster =
                        *(reinterpret_cast<cluster_t*>(cacheval.second));
                } else {
                    goto cacheNotFound;
                }

                return OPENFSL_SUCCESS;
            } else {
cacheNotFound:
                std::string tempPath = currentPath;
                cluster_t tempCluster = currentCluster;

                // Recurse function to next subdir
                error_t result = __chdir(path, &subdir_tmp);
                if (result) {
                    currentPath = tempPath;
                    currentCluster = tempCluster;
                }
                return result;
            }
        }
    }

    if (subdir->empty()) {  // Return if subdir list is empty
        return 0;
    }

    if (subdir->front() == rootSymbol) {
        subdir->erase(subdir->begin());
        currentPath = rootSymbol;
        currentCluster = bpb.ebpbRootDirectoryCluster;
        return __chdir(path, subdir);  // Recurse function to next subdir
    }

    std::string path_tmp = subdir->front();
    for (auto & c : path_tmp) c = static_cast<char>(toupper(c));

    if (path_tmp == "." ||
        (currentCluster == bpb.ebpbRootDirectoryCluster && path_tmp == "..")) {
        subdir->erase(subdir->begin());
        return __chdir(path, subdir);
    }

    bool fileFound = false;
    std::string pathTemp = currentPath;
    cluster_t clusterTemp = currentCluster;

    int errcode = __forEachEntry(
        [=, &subdir, &fileFound, &pathTemp, &clusterTemp](FileInfo fileInfo) {
        if ((fileInfo.fileAttr & FileAttribute::Directory) !=
            (FileAttribute)0) {
            std::string filename = fileInfo.fileName;
            for (auto & c : filename) c = static_cast<char>(toupper(c));

            if ((fsOptions.AllowSFNSearch && (fileInfo.fileSFNName == path_tmp)) ||
                (filename == path_tmp)) {
                if (path_tmp == ".") {}
                else if (path_tmp == "..")
                    // Delete last subdir from path
                    pathTemp = currentPath.substr
                        (0, currentPath.find_last_of(pathSeparator));
                else
                    // Add selected subdir to path
                    pathTemp += pathSeparator.at(0) + subdir->front();

                // Delete first element from subdir list
                subdir->erase(subdir->begin());
                // Set current cluster to subdir
                clusterTemp = fileInfo.fileLocation;
                if (clusterTemp == 0)
                    clusterTemp = bpb.ebpbRootDirectoryCluster;

                fileFound = true;
                return true;
            }
        }

        return false;
        });

    if (errcode) {
        currentPath = pathTemp;
        currentCluster = clusterTemp;
        return errcode;
    }

    if (fileFound) {
        if (cacheEnabled) {
            // Add current cluster to cache if current directory is not root
            if (clusterTemp != bpb.ebpbRootDirectoryCluster) {
                std::string tempcachekey = pathTemp;
                for (auto & c : tempcachekey) c = static_cast<char>(toupper(c));
                __insertCache(tempcachekey, CacheType::PathClusterCache,
                    &clusterTemp, sizeof(cluster_t));
            }
        }

        currentPath = pathTemp;
        currentCluster = clusterTemp;
        return __chdir(path, subdir);
    } else {
        return OPENFSL_ERROR_NO_SUCH_FILE_OR_DIR;
    }
    return 0;
}
