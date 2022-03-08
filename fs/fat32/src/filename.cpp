/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <regex>

#include "openfsl/fat32/fs_fat32.h"

int openfsl::FAT32::getEntryCount(const std::string filename,
                                   const bool ignoreReservedName) {
    if (filename.length() == 0) return 0;
    
    int requiredEntrySize = 0;
    size_t dotpos = filename.find_last_of('.');
    size_t dotcount = 0;

    if (dotpos == std::string::npos) {
        dotpos = filename.size();
    }

    if (dotpos > 8) {
        goto lfn;
    } else if (filename.size() > 11) {
        lfn:
        requiredEntrySize = (filename.size() / 13) + 2;
    } else if (filename.size() > 255) {
        return 0;
    } else {
        requiredEntrySize = 1;
    }

    for (auto c : filename) {
        if ((requiredEntrySize == 1) && (c == '.')) {
            dotcount++;
        } else if ((c > 'A' - 1 && c < 'Z' + 1) ||
            (c > '0' - 1 && c < '9' + 1) ||
            (c > '#' - 1 && c < ')' + 1) ||
            (c > ']' - 1 && c < '`' + 1) ||
            c == '!' ||
            c == '-' ||
            c == '@' ||
            c == '{' ||
            c == '}' ||
            c == '~') {
        } else {
            if ((c > 'a' - 1 && c < 'z' + 1) ||
                (c > 0x7F) ||
                (c == '+') ||
                (c == ',') ||
                (c == ';') ||
                (c == '=') ||
                (c == '[') ||
                (c == ']') ||
                (c == ' ')) {
                if (requiredEntrySize == 1) {
                    requiredEntrySize = 2;
                }
            } else {
                return 0;
            }
        }
    }

    std::string ufilename = filename;
    for (auto& c : ufilename) c = static_cast<char>(toupper(c));

    if (!ignoreReservedName) {
        if ((ufilename == ".") ||
            (ufilename == "..") ||
            (ufilename == "CON") ||
            (ufilename == "PRN") ||
            (ufilename == "AUX") ||
            (ufilename == "NUL") ||
            ((filename.size() == 4) &&
                (ufilename.substr(0, 2) == "COM")) ||
            ((filename.size() == 4) &&
                (ufilename.substr(0, 2) == "LPT"))) {
            return 0;
        }
    }
    
    return requiredEntrySize;
}