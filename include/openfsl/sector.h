/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_SECTOR_H_
#define OPENFSL_SECTOR_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>

#include "openfsl/container.h"

namespace openfsl {
/**
 *
 * @brief Sector buffer
 * @details Sector buffer with uint8_t(byte) array.
 *
 */
class Sector : public Container {
 private:
    size_t sector_count;
    size_t bytespersector;

 public:
    /**
     *
     * @brief Sector class constructor
     * @param sector_count_: Sector count
     * @param bytespersector_: Bytes per sector
     *
     */
    Sector(const size_t sector_count_, const size_t bytespersector_);
    /**
     * @brief Sector class destructor
     */
    ~Sector();

    size_t getSectorCount();
    size_t getBytespersector();

    /**
     * @brief Data buffer getter
     * @return void*: Data buffer pointer
     */
    void* getData();
};
}  // namespace openfsl

#endif  // OPENFSL_SECTOR_H_
