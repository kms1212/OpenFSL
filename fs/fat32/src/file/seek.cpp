/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fat32.h"

int openfsl::fat32::File::seekg(
    const size_t offset, const openfsl::SeekMode whence) {
    if (whence == SeekMode::SeekSet) {
        if (offset > currentFileInfo.fileSize - 1) {
            return 1;
        } else {
            readPointer = offset;
            return 0;
        }
    } else if (whence == SeekMode::SeekCur) {
        if (readPointer + offset > currentFileInfo.fileSize - 1) {
            return 1;
        } else {
            readPointer += offset;
            return 0;
        }
    } else if (whence == SeekMode::SeekEnd) {
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

int openfsl::fat32::File::seekp(
    const size_t offset, const openfsl::SeekMode whence) {
    if (whence == openfsl::SeekMode::SeekSet) {
        if (offset > currentFileInfo.fileSize - 1) {
            return 1;
        } else {
            writePointer = offset;
            return 0;
        }
    } else if (whence == openfsl::SeekMode::SeekCur) {
        if (writePointer + offset > currentFileInfo.fileSize - 1) {
            return 1;
        } else {
            writePointer += offset;
            return 0;
        }
    } else if (whence == openfsl::SeekMode::SeekEnd) {
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
