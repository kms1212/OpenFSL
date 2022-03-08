/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fslutils.h"

#include "openfsl/fat32/fs_fat32.h"

std::pair<std::string, std::string>
    openfsl::FAT32::__separateFileName(const std::string filepath) {
    size_t lastPathSeparator = filepath.find_last_of(pathSeparator);
    std::string fileName;
    std::string fileDirectory;

    if (lastPathSeparator == std::string::npos) {
        fileDirectory = ".";
        fileName = filepath;
    } else {
        fileDirectory = filepath.substr(0, lastPathSeparator);
        fileName = filepath.substr(lastPathSeparator + 1);
    }

    return std::make_pair(fileDirectory, fileName);
}

std::string openfsl::FAT32::convertToAbsolutePath(
    const std::string relativePath) {
    std::vector<std::string> v_absolutedir;
    std::vector<std::string> v_relativedir;
    fsl_strtokenize(currentPath, &v_absolutedir, pathSeparator);
    fsl_strtokenize(relativePath, &v_relativedir, pathSeparator);

    if (v_relativedir.size() == 1 && v_relativedir[0] == rootSymbol) {
        return rootSymbol;
    }

    size_t reldirsize = v_relativedir.size();
    for (size_t i = 0; i < reldirsize; i++) {
        if (*v_relativedir.begin() == ".") {
        } else if (*v_relativedir.begin() == "..") {
            if (*(v_absolutedir.end() - 1) != rootSymbol) {
                v_absolutedir.erase(v_absolutedir.end() - 1);
            }
        } else if (*v_relativedir.begin() == rootSymbol) {
            v_absolutedir.clear();
            v_absolutedir.push_back(rootSymbol);
        } else {
            v_absolutedir.push_back(*v_relativedir.begin());
        }
        v_relativedir.erase(v_relativedir.begin());
    }

    std::stringstream ret;
    for (size_t i = 0; i < v_absolutedir.size(); i++) {
        ret << v_absolutedir[i] << pathSeparator.at(0);
    }
    std::string pathstr = ret.str();
    pathstr.pop_back();

    return pathstr;
}
