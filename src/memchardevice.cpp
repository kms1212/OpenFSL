/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/memchardevice.h"

openfsl::MemCharDevice::MemCharDevice() {}

openfsl::MemCharDevice::~MemCharDevice() {
    if (initialized)
        deinitialize();
}

error_t openfsl::MemCharDevice::initialize(const size_t maxMemoryBlockCount, const size_t memoryBlockSize) {
    this->maxMemoryBlockCount = maxMemoryBlockCount;
    this->memoryBlockSize = memoryBlockSize;

    memoryAllocationTable = new uint8_t*[maxMemoryBlockCount];
    for (size_t i = 0; i < maxMemoryBlockCount; i++) {
        memoryAllocationTable[i] = nullptr;
    }

    IODevice::initialize();
    
    return 0;
}

error_t openfsl::MemCharDevice::deinitialize() {
    for (size_t i = 0; i < maxMemoryBlockCount; i++) {
        if (memoryAllocationTable[i] != nullptr) {
            delete[] memoryAllocationTable[i];
        }
    }
    delete[] memoryAllocationTable;

    IODevice::deinitialize();

    return 0;
}

error_t openfsl::MemCharDevice::readByte
    (void* dest, const size_t offset, const size_t size) {
    size_t firstBlock = (size_t)(offset / memoryBlockSize);
    size_t lastBlock = (size_t)((offset + size) / memoryBlockSize);

    size_t bytesRead = 0;
    diskOperationMutex.lock();
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
    diskOperationMutex.unlock();

    return 0;
}

error_t openfsl::MemCharDevice::writeByte
    (const void* src, const size_t offset, const size_t size) {
    size_t firstBlock = (size_t)(offset / memoryBlockSize);
    size_t lastBlock = (size_t)((offset + size) / memoryBlockSize);

    size_t bytesWritten = 0;
    diskOperationMutex.lock();
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
    diskOperationMutex.unlock();

    return 0;
}
