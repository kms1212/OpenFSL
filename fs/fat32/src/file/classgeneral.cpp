/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/internal/file.h"

openfsl::fat32::File::File(FAT32* fileSystem_) {
    fileSystem = fileSystem_;
}

openfsl::fat32::File::~File() {
    delete fileCluster;
}
