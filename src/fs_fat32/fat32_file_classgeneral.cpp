/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

FS_FAT32::FILE::FILE(FS_FAT32* fileSystem_, FS_FAT32::FileInfo fileInfo_, FSL_OpenMode openMode_) {
    fileSystem = fileSystem_;
    fileInfo = fileInfo_;
    openMode = openMode_;
	
	fileCluster = new LinkedCluster(fileSystem, fileInfo.fileLocation);
}

FS_FAT32::FILE::~FILE() {
	delete fileCluster;
}