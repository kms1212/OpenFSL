/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/endian.h"

bool openfsl::isLittleEndian = false;
bool openfsl::isEndianChecked = false;

void openfsl::checkEndian() {
    uint16_t ecOrig = 1;
    uint8_t* ecArr = (uint8_t*)&ecOrig;
    isLittleEndian = ecArr[0] == 1;

    isEndianChecked = true;
}
