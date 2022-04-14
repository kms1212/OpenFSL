/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/chardevice.h"

openfsl::CharDevice::CharDevice() {}

error_t openfsl::CharDevice::initialize(IOFunctions func) {
    ioFunc = func;

    if (!ioFunc.writeByte) {
        return 1;
    }
    if (!ioFunc.readByte) {
        return 1;
    }
    
    IODevice::initialize();

    return 0;
}

openfsl::CharDevice::~CharDevice() {}

error_t openfsl::CharDevice::readByte
    (void* dest, const size_t offset, const size_t size) {
    if (!ioFunc.readByte) {
        return OPENFSL_ERROR_DISK;
    } else {
        diskOperationMutex.lock();
        error_t result = ioFunc.readByte(this,
            reinterpret_cast<uint8_t*>(dest), offset, size);
        diskOperationMutex.unlock();

        return result;
    }
}

error_t openfsl::CharDevice::writeByte
    (const void* src, const size_t offset, const size_t size) {
    if (!ioFunc.writeByte) {
        return OPENFSL_ERROR_DISK;
    } else {
        diskOperationMutex.lock();
        error_t result = ioFunc.writeByte(this,
            reinterpret_cast<const uint8_t*>(src), offset, size);
        diskOperationMutex.unlock();

        return result;
    }
}
