/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/blockdevice.h"

openfsl::BlockDevice::BlockDevice() {}

error_t openfsl::BlockDevice::initialize(IOFunctions func) {
    IODevice::initialize();
    ioFunc = func;

    if (!ioFunc.writeSector) {
        return 1;
    }
    if (!ioFunc.readSector) {
        return 1;
    }

    return 0;
}

openfsl::BlockDevice::~BlockDevice() {}

error_t openfsl::BlockDevice::readSector
    (void* dest, const lba48_t lba, const lba48_t size) {
    if (!ioFunc.readSector) {
        return OPENFSL_ERROR_DISK;
    } else {
        diskOperationMutex.lock();
        error_t result = ioFunc.readSector(this,
            reinterpret_cast<uint8_t*>(dest), lba, size);
        diskOperationMutex.unlock();

        return result;
    }
}

error_t openfsl::BlockDevice::writeSector
    (const void* src, const lba48_t lba, const lba48_t size) {
    if (!ioFunc.writeSector) {
        return OPENFSL_ERROR_DISK;
    } else {
        diskOperationMutex.lock();
        error_t result = ioFunc.writeSector(this,
            reinterpret_cast<const uint8_t*>(src), lba, size);
        diskOperationMutex.unlock();

        return result;
    }
}
