/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_BLOCKDEVICE_H_
#define OPENFSL_BLOCKDEVICE_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>

#include <iostream>
#include <functional>

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
class BlockDevice : public IODevice {
 public:
    /**
     * @brief IO functions
     */
    typedef struct IOFunctions {
        /**
         * @brief Device read sector function
         * @details Reads from I/O device and stores data by sector.
         * @param BlockDevice* dd: this
         * @param uint8_t* dest: buffer to read data
         * @param lba48_t lba: LBA location to read
         * @param lba48_t size: Sector size to read
         * @return int: Error code
         */
        std::function<int(
            BlockDevice*, uint8_t*, const lba48_t, const lba48_t)> readSector;

        /**
         * @brief Device write sector function
         * @details Reads data and writes to I/O device by sector.
         * @param BlockDevice* dd: this
         * @param const uint8_t* src: buffer to write from
         * @param lba48_t lba: LBA location to write
         * @param lba48_t size: Sector size to write
         * @return int: Error code
         */
        std::function<int(
            BlockDevice*, const uint8_t*, const lba48_t, const lba48_t)>
            writeSector;
    } IOFunctions;

    /**
     * @brief Disk Parameter
     */
    typedef struct DiskParameter {
        size_t sectorPerTrack = 63;
        size_t headPerCylinder = 16;
        size_t bytesPerSector = 512;
    } DiskParameter;

 protected:
    DiskParameter diskParameter;
    IOFunctions ioFunc;

 public:
    /**
     *
     * @brief BlockDevice constructor
     *
     */
    BlockDevice();

    /**
     *
     * @brief BlockDevice destructor
     *
     */
    virtual ~BlockDevice();

    /**
     *
     * @brief BlockDevice initializer
     * @details Initializes Disk driver.
     * Function checks function wrappers necessary.
     */
    error_t initialize(const IOFunctions func);

    inline DiskParameter getDiskParameter() { return diskParameter; }

    inline void setDiskParameter(const DiskParameter diskParameter_)
    { diskParameter = diskParameter_; }

    virtual error_t __check_result readSector(
        void* dest, const lba48_t lba, const lba48_t size);
    virtual error_t __check_result writeSector(
        const void* src, const lba48_t lba, const lba48_t size);
};
}  // namespace openfsl

#endif  // OPENFSL_BLOCKDEVICE_H_
