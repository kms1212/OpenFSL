/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_VERSION_H_
#define OPENFSL_VERSION_H_

#include <stdint.h>

#ifndef __cplusplus
#error C++ compiler required.
#endif

#define OPENFSL_VERSION_TYPE_NONE 0
#define OPENFSL_VERSION_TYPE_RELEASE 1
#define OPENFSL_VERSION_TYPE_BETA 2
#define OPENFSL_VERSION_TYPE_NIGHTLY 3

namespace openfsl {
    const int getBuildType();
    const uint32_t getMajorVersion();
    const uint32_t getMinorVersion();
    const uint32_t getPatchVersion();
    const uint32_t getBuildVersion();
    const uint32_t getBuildDate();
    const char* getVersionString();
    const char* getFullVersionString();
}  // namespace openfsl

#endif  // OPENFSL_VERSION_H_
