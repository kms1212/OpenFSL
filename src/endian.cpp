/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/endian.h"

static bool isLittleEndianData = false;
static bool isEndianCheckedData = false;

bool openfsl::isLittleEndian() {
    return isLittleEndianData;
}

bool openfsl::isEndianChecked() {
    return isEndianCheckedData;
}


void openfsl::checkEndian() {
    uint16_t ecOrig = 1;
    uint8_t* ecArr = (uint8_t*)&ecOrig;
    isLittleEndianData = ecArr[0] == 1;

    isEndianCheckedData = true;
}
