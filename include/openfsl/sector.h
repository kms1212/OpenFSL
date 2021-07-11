/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef __SECTOR_H
#define __SECTOR_H

#include <stdint.h>

#include "fslservices.h"

namespace openFSL {
    /**
     *
     * @brief Sector buffer
     * @details Sector buffer with uint8_t(byte) array.
     * @author kms1212
     * @todo Change buffer addressing process
     *
     */
    class Sector {
    private:
        size_t sector_count;
        uint8_t* data;
        
    public:
        /**
         *
         * @brief Sector class constructor
         * @param sector_count_: Sector count
         * @param bytespersector: Bytes per sector
         *
         */
        Sector(size_t sector_count_, size_t bytespersector);
        /**
         *
         * @brief Sector class destructor
         *
         */
        ~Sector();
        
        /**
         *
         * @brief Sector count getter
         * @return size_t: Sector count
         *
         */
        size_t getSectorCount();
        
        /**
         *
         * @brief Data buffer getter
         * @return void*: Data buffer pointer
         *
         */
        void* getData();
    };
}

#endif