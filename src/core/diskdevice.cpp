/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/diskdevice.h"

using namespace openFSL;

DiskDevice::DiskDevice(size_t bytespersector_) {
    bytespersector = bytespersector_;
}

int DiskDevice::initialize() {
    if (open == NULL) {
        return 1;
    }
    else if (read == NULL) 
    {
        return 1;
    }
    else if (write == NULL) 
    {
        return 1;
    }
    else if (close == NULL) 
    {
        return 1;
    }
    if (open())
    {
        return 1;
    }
    return 0;
}

DiskDevice::~DiskDevice() {
    close();
}

size_t DiskDevice::getBytespersector() {
    return bytespersector;
}

int DiskDevice::readDisk(uint8_t* dest, size_t lba, size_t size) {
    if (read != NULL)
        read(this, dest, lba, size);
    else
        return 1;
    return 0;
}