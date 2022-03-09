/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/ntfs/fs_ntfs.h"

openfsl::NTFS::NTFS(openfsl::BlockDevice* bd_,
    const std::string startPath_,
    const std::string pathSeparator_,
    const std::string rootSymbol_,
    const Version version_) :
        FileSystem(bd_, startPath_, pathSeparator_, rootSymbol_) {
    version = version_;

    return;
}

openfsl::NTFS::~NTFS() {}