/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

openfsl::FAT32::FAT32(openfsl::BlockDevice* bd_,
                      const FilesystemOptions fsOptions_,
                      const std::string startPath_,
                      const std::string pathSeparator_,
                      const std::string rootSymbol_) :
        FileSystem(bd_, startPath_, pathSeparator_, rootSymbol_) {
    fsOptions = fsOptions_;
    return;
}

openfsl::FAT32::FAT32(openfsl::BlockDevice* bd_,
                      const std::string startPath_,
                      const std::string pathSeparator_,
                      const std::string rootSymbol_) :
        FileSystem(bd_, startPath_, pathSeparator_, rootSymbol_) {
    fsOptions.AllowSFNSearch = 1;
    return;
}

openfsl::FAT32::~FAT32() {
    if (fatArea != nullptr)
        delete fatArea;
    disableCache();
}
