/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_MEMBLOCKDEVICE_H_
#define OPENFSL_MEMBLOCKDEVICE_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>

#include <iostream>
#include <string>

#include "openfsl/blockdevice.h"
#include "openfsl/error.h"
#include "openfsl/macros.h"
#include "openfsl/types.h"
#include "openfsl/sector.h"
#include "openfsl/iodevice.h"

namespace openfsl {
class MemBlockDevice : public BlockDevice {
 private:
    size_t blockCount;

    uint8_t** memoryAllocationTable;

 public:
    /**
     * @brief FileBlockDevice constructor
     */
    MemBlockDevice();

    /**
     * @brief FileBlockDevice destructor
     */
    ~MemBlockDevice();

    /**
     *
     * @brief FileBlockDevice initializer
     * @details Initializes file device.
     */
    error_t initialize(const size_t blockCount);

    /**
     *
     * @brief FileBlockDevice deinitializer
     * @details Denitializes file device.
     */
    error_t deinitialize();

    error_t __check_result readSector(
        void* dest, const lba48_t lba, const lba48_t size) override;
    error_t __check_result writeSector(
        const void* src, const lba48_t lba, const lba48_t size) override;
};
}  // namespace openfsl


#endif  // OPENFSL_FILEBLOCKDEVICE_H_
