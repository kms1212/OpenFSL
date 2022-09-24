/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <iomanip>
#include <iostream>

#include "openfsl/types.h"

typedef openfsl::Result<openfsl::lba48_t> lba48Result;
typedef openfsl::Result<openfsl::CHS> CHSResult;

lba48Result
openfsl::convertCHSToLBA(const CHS chs,
                         const size_t sectPerTra,
                         const size_t headPerCyl) {
    return lba48Result((lba48_t)((chs.cyl * headPerCyl + chs.head) * sectPerTra + (chs.sect - 1)));
}

CHSResult
openfsl::convertLBAToCHS(const lba48_t lba,
                         const uint8_t sectPerTra,
                         const uint8_t headPerCyl) {
    if (lba > 16515135)
        return CHSResult(OPENFSL_ERROR_CANNOT_CONVERT_TO_CHS);
    CHS chs;
    chs.cyl = lba / (headPerCyl * sectPerTra);
    chs.head = (lba / sectPerTra) % headPerCyl;
    chs.sect = (lba % sectPerTra) + 1;
    return CHSResult(chs);
}
