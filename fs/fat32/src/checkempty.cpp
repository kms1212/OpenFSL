/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

std::pair<error_t, bool>
    openfsl::FAT32::__checkEmpty(
            const std::string path, const std::string dirname) {
    bool result = true;

    error_t errcode = __forEachEntry([&](FileInfo fileInfo) {
        if ((fileInfo.fileName != ".") && (fileInfo.fileName != "..")) {
            result = false;
            return true;
        }
        return false;
        }, path + pathSeparator.at(0) + dirname);

    return std::make_pair(errcode, result);
}
