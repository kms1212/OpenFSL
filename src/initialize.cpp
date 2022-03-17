/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/initialize.h"

#include <iostream>

namespace openfsl {
namespace {
    struct Initializer {
        Initializer();
        ~Initializer();
    };
    Initializer initializer;
}
}

openfsl::Initializer::Initializer() {
    // Check endianness
    uint16_t ecOrig = 1;
    uint8_t* ecArr = (uint8_t*)&ecOrig;
    openfsl::isLittleEndian = ecArr[0] == 1;
}

openfsl::Initializer::~Initializer() {
    
}
