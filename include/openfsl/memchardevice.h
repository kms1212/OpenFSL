/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_MEMCHARDEVICE_H_
#define OPENFSL_MEMCHARDEVICE_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>

#include <iostream>
#include <functional>

#include "openfsl/chardevice.h"
#include "openfsl/error.h"
#include "openfsl/macros.h"
#include "openfsl/iodevice.h"
#include "openfsl/types.h"
#include "openfsl/sector.h"

namespace openfsl {
/**
 *
 * @brief Disk driver
 * @details Disk driver class with open, read, write, close function
 * @author kms1212
 * @todo Write function wrapper
 *
 */
class MemCharDevice : public CharDevice {
 private:
    size_t maxMemoryBlockCount;
    size_t memoryBlockSize;

    uint8_t** memoryAllocationTable;

 public:
    /**
     *
     * @brief CharDevice constructor
     *
     */
    MemCharDevice();

    /**
     *
     * @brief CharDevice destructor
     *
     */
    ~MemCharDevice();

    /**
     * 
     * @brief CharDevice initializer
     * 
     */
    error_t initialize(const size_t maxMemoryBlockCount, const size_t memoryBlockSize);

    /**
     *
     * @brief ChearDevice deinitializer
     * 
     */
    error_t deinitialize();

    error_t __check_result readByte(
        void* dest, const size_t offset, const size_t size);
    error_t __check_result writeByte(
        const void* src, const size_t offset, const size_t size);
};
}  // namespace openfsl

#endif  // OPENFSL_MEMCHARDEVICE_H_
