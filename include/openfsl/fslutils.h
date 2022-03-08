/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_FSLUTILS_H_
#define OPENFSL_FSLUTILS_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <cstddef>
#include <ctime>

#include <vector>
#include <string>

#include "openfsl/types.h"
#include "openfsl/error.h"
#include "openfsl/symbols.h"

namespace openfsl {
/**
 *
 * @brief String tokenizer
 *
 */
void fsl_strtokenize(const std::string& str, std::vector<std::string>* tokens,
    const std::string& delimiters);

std::tm fsl_localtime(const std::time_t time);

lba48_t convertCHSToLBA(
    const CHS chs, const size_t sectPerTra, const size_t headPerCyl);

int convertLBAToCHS(const lba48_t lba, CHS* chs, const uint8_t sectPerTra,
    const uint8_t headPerCyl);
}  // namespace openfsl

#endif  // OPENFSL_FSLUTILS_H_
