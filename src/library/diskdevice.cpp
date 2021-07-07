/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/diskdevice.h"

using namespace openFSL;

DiskDevice::DiskDevice(uint32_t bytespersector_) {
    bytespersector = bytespersector_;
}

int DiskDevice::initialize() {
    if (openDisk == NULL) {
        return 1;
    }
    else if (readDisk == NULL) 
    {
        return 1;
    }
    else if (writeDisk == NULL) 
    {
        return 1;
    }
    else if (closeDisk == NULL) 
    {
        return 1;
    }
    if (openDisk())
    {
        return 1;
    }
	return 0;
}

DiskDevice::~DiskDevice() {
    close();
}

void DiskDevice::close() {
    closeDisk();
}

uint32_t DiskDevice::getBytespersector() {
    return bytespersector;
}