/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_IODEVICE_H_
#define OPENFSL_IODEVICE_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>

#include <iostream>
#include <functional>
#include <mutex>  // NOLINT

#include "openfsl/error.h"
#include "openfsl/macros.h"
#include "openfsl/types.h"
#include "openfsl/sector.h"

namespace openfsl {
class IODevice {
 protected:
    bool initialized;
    std::mutex diskOperationMutex;

    /**
     *
     * @brief IODevice initializer
     * @details Initializes I/O device driver.
     */
    error_t initialize();

    /**
     *
     * @brief IODevice deinitializer
     * @details Denitializes I/O device driver.
     */
    error_t deinitialize();

 public:
    /**
     *
     * @brief IODevice constructor
     *
     */
    IODevice();

    /**
     *
     * @brief IODevice destructor
     *
     */
    ~IODevice();

    inline bool isInitialized() { return initialized; }
};
}  // namespace openfsl

#endif  // OPENFSL_IODEVICE_H_
