/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

int openfsl::FAT32::FILE::seekg(
    const size_t offset, const openfsl::FSL_SeekMode whence) {
    if (whence == FSL_SeekMode::SeekSet) {
        if (offset > currentFileInfo.fileSize - 1) {
            return 1;
        } else {
            readPointer = offset;
            return 0;
        }
    } else if (whence == FSL_SeekMode::SeekCur) {
        if (readPointer + offset > currentFileInfo.fileSize - 1) {
            return 1;
        } else {
            readPointer += offset;
            return 0;
        }
    } else if (whence == FSL_SeekMode::SeekEnd) {
        if (offset > currentFileInfo.fileSize - 1) {
            return 1;
        } else {
            readPointer = currentFileInfo.fileSize - offset;
            return 0;
        }
    } else {
        return 1;
    }
}

int openfsl::FAT32::FILE::seekp(
    const size_t offset, const openfsl::FSL_SeekMode whence) {
    if (whence == openfsl::FSL_SeekMode::SeekSet) {
        if (offset > currentFileInfo.fileSize - 1) {
            return 1;
        } else {
            writePointer = offset;
            return 0;
        }
    } else if (whence == openfsl::FSL_SeekMode::SeekCur) {
        if (writePointer + offset > currentFileInfo.fileSize - 1) {
            return 1;
        } else {
            writePointer += offset;
            return 0;
        }
    } else if (whence == openfsl::FSL_SeekMode::SeekEnd) {
        if (offset > currentFileInfo.fileSize - 1) {
            return 1;
        } else {
            writePointer = currentFileInfo.fileSize - offset;
            return 0;
        }
    } else {
        return 1;
    }
}
