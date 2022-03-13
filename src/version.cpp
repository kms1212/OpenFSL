/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/version.h"

const int openfsl::getBuildType() {
    return OPENFSL_VERSION_TYPE_NONE;
}

const uint32_t openfsl::getMajorVersion() {
    return 0;
}

const uint32_t openfsl::getMinorVersion() {
    return 0;
}

const uint32_t openfsl::getPatchVersion() {
    return 0;
}

const uint32_t openfsl::getBuildVersion() {
    return 1326;
}

const uint32_t openfsl::getBuildDate() {
    return 20220313;
}

const char* openfsl::getVersionString() {
    return "0.0.0";
}

const char* openfsl::getFullVersionString() {
    return "OpenFSL 0.0.0 (AppleClang 13.0.0.13000029 arm64) Running On Darwin";
}
