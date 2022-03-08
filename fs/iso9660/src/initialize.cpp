/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/iso9660/fs_iso9660.h"

extern void __hexdump(const uint8_t* p, size_t offset, size_t len);

error_t openfsl::ISO9660::initialize() {
    if (iod->getDiskParameter().bytesPerSector != 2048) {
        return OPENFSL_ERROR_SECTSIZE;
    }

    VolumeDescriptor vd;
    error_t result = iod->readSector(&vd, 16, 1);
    if (result)
        return result;

    __hexdump(reinterpret_cast<uint8_t*>(&vd), 0, 2048);

    if (memcmp(vd.descSignature, "CD001", 5) != 0)
        return OPENFSL_ERROR_SIGN;

    return OPENFSL_SUCCESS;
}
