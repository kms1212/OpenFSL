/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

uint8_t openfsl::FAT32::__chksumSFN(const uint8_t* src) {
    uint8_t ret = 0;

    for (int i = 11; i > 0; i--) {
        ret = ((ret & 1) ? 0x80 : 0) + (ret >> 1) + *src++;
    }
    return ret;
}
