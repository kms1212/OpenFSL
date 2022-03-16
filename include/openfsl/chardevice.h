/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_CHARDEVICE_H_
#define OPENFSL_CHARDEVICE_H_

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
class CharDevice : public IODevice {
 public:
    /**
     * @brief IO functions
     */
    typedef struct IOFunctions {
        /**
         * @brief Device read byte function
         * @details Reads from I/O device and stores data by byte.
         * @param CharDevice* cd: this
         * @param uint8_t* dest: buffer to read data
         * @param size_t offset: Read area offset
         * @param size_t size: Read area size
         * @return error_t: Error code
         */
        std::function<error_t(
            CharDevice*, uint8_t*, const size_t, const size_t)> readByte;

        /**
         * @brief Device write byte function
         * @details Reads data and writes to I/O device by byte.
         * @param CharDevice* cd: this
         * @param const uint8_t* src: buffer to write from
         * @param size_t offset: Write area offset
         * @param size_t size: Write area size
         * @return error_t: Error code
         */
        std::function<error_t(
            CharDevice*, const uint8_t*, const size_t, const size_t)>
            writeByte;
    } IOFunctions;

 protected:
    IOFunctions ioFunc;

 public:
    /**
     *
     * @brief CharDevice constructor
     *
     */
    CharDevice();

    /**
     *
     * @brief CharDevice destructor
     *
     */
    virtual ~CharDevice();

    /**
     *
     * @brief CharDevice initializer
     * @details Initializes Disk driver.
     * Function checks function wrappers necessary.
     */
    error_t initialize(const IOFunctions func);

    error_t __check_result readByte(
        void* dest, const size_t offset, const size_t size);
    error_t __check_result writeByte(
        const void* src, const size_t offset, const size_t size);
};
}  // namespace openfsl

#endif  // OPENFSL_CHARDEVICE_H_
