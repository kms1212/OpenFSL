/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_TYPES_H_
#define OPENFSL_TYPES_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>

#include "openfsl/result.h"
#include "openfsl/error.h"

namespace openfsl {
#pragma pack(push, 1)
typedef struct CHS {
    uint32_t head : 8;
    uint32_t sect : 6;
    uint32_t cyl : 10;
    uint8_t __UNUSED = 0;
} CHS;
#pragma pack(pop)

typedef uint64_t lba48_t;

Result<lba48_t> convertCHSToLBA(
    const CHS chs, const size_t sectPerTra, const size_t headPerCyl);

Result<CHS> convertLBAToCHS(const lba48_t lba, const uint8_t sectPerTra,
    const uint8_t headPerCyl);
}  // namespace openfsl

#endif  // OPENFSL_TYPES_H_
