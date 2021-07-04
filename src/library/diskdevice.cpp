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

void DiskDevice::initialize() {
    if (openDisk == NULL) {
        errorState = DISKDEV_ERROR_INIT_NOFUNC;
        return;
    }
    else if (readDisk == NULL) 
    {
        errorState = DISKDEV_ERROR_INIT_NOFUNC;
        return;
    }
    else if (writeDisk == NULL) 
    {
        errorState = DISKDEV_ERROR_INIT_NOFUNC;
        return;
    }
    else if (closeDisk == NULL) 
    {
        errorState = DISKDEV_ERROR_INIT_NOFUNC;
        return;
    }
    errorState = DISKDEV_ERROR_SUCCESS;
    if (openDisk())
    {
        errorState = DISKDEV_ERROR_DISK_ERROR;
        return;
    }
}

DiskDevice::~DiskDevice() {
    if (errorState != DISKDEV_ERROR_NOT_INITIALIZED)
        close();
}

void DiskDevice::close() {
    errorState = DISKDEV_ERROR_NOT_INITIALIZED;
    if (closeDisk())
    {
        errorState = DISKDEV_ERROR_DISK_ERROR;
    }
}

uint32_t DiskDevice::getState() {
    return errorState;
}

uint32_t DiskDevice::getBytespersector() {
    return bytespersector;
}