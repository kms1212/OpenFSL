/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/memdevice.h"

openfsl::MemDevice::MemDevice() {}

openfsl::MemDevice::~MemDevice() {
    if (initialized)
        deinitialize();
}

error_t openfsl::MemDevice::initialize(size_t maxMemoryBlockCount, size_t memoryBlockSize) {
    this->maxMemoryBlockCount = maxMemoryBlockCount;
    this->memoryBlockSize = memoryBlockSize;

    memoryAllocationTable = new uint8_t*[maxMemoryBlockCount];
    for (int i = 0; i < maxMemoryBlockCount; i++) {
        memoryAllocationTable[i] = nullptr;
    }

    IODevice::initialize();
    
    return 0;
}

error_t openfsl::MemDevice::deinitialize() {
    for (int i = 0; i < maxMemoryBlockCount; i++) {
        if (memoryAllocationTable[i] != nullptr) {
            delete[] memoryAllocationTable[i];
        }
    }
    delete[] memoryAllocationTable;

    return 0;
}

error_t openfsl::MemDevice::readByte
    (void* dest, const size_t offset, const size_t size) {
    size_t firstBlock = (size_t)(offset / memoryBlockSize);
    size_t lastBlock = (size_t)((offset + size) / memoryBlockSize);

    size_t bytesRead = 0;
    for (size_t i = firstBlock; i <= lastBlock; i++) {
        size_t blockReadOffset = !(i - firstBlock) ? offset % memoryBlockSize : 0;
        size_t blockReadSize = std::min(size - bytesRead, memoryBlockSize - blockReadOffset);

        if (memoryAllocationTable[i] == nullptr) {
            memset((uint8_t*)dest + bytesRead, 0, blockReadSize);
        } else {
            memcpy((uint8_t*)dest + bytesRead, memoryAllocationTable[i] + blockReadOffset, blockReadSize);
        }

        bytesRead += blockReadSize;
    }

    return 0;
}

error_t openfsl::MemDevice::writeByte
    (const void* src, const size_t offset, const size_t size) {
    size_t firstBlock = (size_t)(offset / memoryBlockSize);
    size_t lastBlock = (size_t)((offset + size) / memoryBlockSize);

    size_t bytesWritten = 0;
    for (size_t i = firstBlock; i <= lastBlock; i++) {
        if (memoryAllocationTable[i] == nullptr) {
            memoryAllocationTable[i] = new uint8_t[memoryBlockSize];
            memset(memoryAllocationTable[i], 0, memoryBlockSize);
        }
        
        size_t blockWriteOffset = !(i - firstBlock) ? offset % memoryBlockSize : 0;
        size_t blockWriteSize = std::min(size - bytesWritten, memoryBlockSize - blockWriteOffset);
        memcpy(memoryAllocationTable[i] + blockWriteOffset, (uint8_t*)src + bytesWritten, blockWriteSize);

        bytesWritten += blockWriteSize;
    }

    return 0;
}
