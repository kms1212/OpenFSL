/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_FBD_DISABLED

#include <ios>

#include "openfsl/memblockdevice.h"

openfsl::MemBlockDevice::MemBlockDevice() {}

openfsl::MemBlockDevice::~MemBlockDevice() {
    if (initialized)
        deinitialize();
}

error_t openfsl::MemBlockDevice::initialize(const size_t blockCount) {
    this->blockCount = blockCount;

    memoryAllocationTable = new uint8_t*[blockCount];
    for (size_t i = 0; i < blockCount; i++) {
        memoryAllocationTable[i] = nullptr;
    }

    IODevice::initialize();
    
    return 0;
}

error_t openfsl::MemBlockDevice::deinitialize() {
    for (size_t i = 0; i < blockCount; i++) {
        if (memoryAllocationTable[i] != nullptr) {
            delete[] memoryAllocationTable[i];
        }
    }
    delete[] memoryAllocationTable;

    IODevice::deinitialize();

    return 0;
}

error_t openfsl::MemBlockDevice::readSector
    (void* dest, const lba48_t lba, const lba48_t size) {
    diskOperationMutex.lock();

    size_t blockSize = diskParameter.bytesPerSector;

    for (size_t i = 0; i < size; i++) {
        if (memoryAllocationTable[lba + i] == nullptr) {
            memset((uint8_t*)dest + i * blockSize, 0x00, blockSize);
        } else {
            memcpy((uint8_t*)dest + i * blockSize, memoryAllocationTable[lba + i], blockSize);
        }
    }

    diskOperationMutex.unlock();

    return 0;
}

error_t openfsl::MemBlockDevice::writeSector(
    const void* src, const lba48_t lba, const lba48_t size) {
    diskOperationMutex.lock();
    
    size_t blockSize = diskParameter.bytesPerSector;

    for (size_t i = 0; i < size; i++) {
        if (memoryAllocationTable[lba + i] == nullptr) {
            memoryAllocationTable[lba + i] = new uint8_t[blockSize];
            memset(memoryAllocationTable[lba + i], 0, blockSize);
        }

        memcpy(memoryAllocationTable[lba + i], (uint8_t*)src + i * blockSize, blockSize);
    }

    diskOperationMutex.unlock();

    return 0;
}


#endif  // OPENFSL_FBD_DISABLED
