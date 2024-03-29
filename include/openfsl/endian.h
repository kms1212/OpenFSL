/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_ENDIAN_H_
#define OPENFSL_ENDIAN_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>

#include "openfsl/error.h"
#include "openfsl/symbols.h"

namespace openfsl {

bool isLittleEndian();
bool isEndianChecked();

template<typename T> T leToSystem(T leValue);
template<typename T> T beToSystem(T beValue);

template<typename T> T systemToLe(T value);
template<typename T> T systemToBe(T value);

template<typename T> T flipEndian(T value);

void checkEndian();

}  // namespace openfsl

#define OPENFSL_ENDIAN_H_TEMPLATE_

#include "openfsl/endian.hpp"

#undef OPENFSL_ENDIAN_H_TEMPLATE_

#endif  // OPENFSL_ENDIAN_H_
