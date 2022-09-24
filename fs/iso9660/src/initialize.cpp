/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/utils.h"

#include "openfsl/iso9660/fs_iso9660.h"

error_t openfsl::ISO9660::initialize() {
    if (iod->getDiskParameter().bytesPerSector != 2048) {
        return OPENFSL_ERROR_INVALID_SECTOR_SIZE;
    }

    VolumeDescriptor vd;
    error_t result = iod->readSector(&vd, 16, 1);
    if (result)
        return result;

#ifdef DEBUG
    __hexdump(reinterpret_cast<uint8_t*>(&vd), 0, 2048);
#endif

    if (memcmp(vd.descSignature, "CD001", 5) != 0)
        return OPENFSL_ERROR_INVALID_SIGNATURE;

    return OPENFSL_SUCCESS;
}
