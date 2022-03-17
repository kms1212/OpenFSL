/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_ENDIAN_H_
#define OPENFSL_ENDIAN_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include "openfsl/types.h"
#include "openfsl/error.h"
#include "openfsl/symbols.h"

namespace openfsl {

extern bool isLittleEndian;

template<typename T> T leToSystem(T leValue);
template<typename T> T beToSystem(T beValue);

template<typename T> T systemToLe(T value);
template<typename T> T systemToBe(T value);

template<typename T> T flipEndian(T value);

}  // namespace openfsl

#define OPENFSL_ENDIAN_H_TEMPLATE_

#include "openfsl/endian.hpp"

#undef OPENFSL_ENDIAN_H_TEMPLATE_

#endif  // OPENFSL_ENDIAN_H_
