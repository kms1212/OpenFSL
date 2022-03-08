/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_INITIALIZE_H_
#define OPENFSL_INITIALIZE_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include "openfsl/types.h"
#include "openfsl/error.h"
#include "openfsl/symbols.h"

namespace openfsl {

extern bool initialized;

/**
 *
 * @brief Library initializer
 * @return error_t: Error code 
 *
 */
error_t initialize();
}  // namespace openfsl

#endif  // OPENFSL_INITIALIZE_H_
