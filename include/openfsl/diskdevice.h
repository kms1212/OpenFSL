/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef __DISKDEVICE_H
#define __DISKDEVICE_H

#include <stdint.h>
#include <iostream>

#include "sector.h"

namespace openFSL {
    /**
     *
     * @brief Disk driver
     * @details Disk driver class with open, read, write(--), close function
     * @author kms1212
     * @todo Write function wrapper
     *
     */
    class DiskDevice {
    private:
        size_t bytespersector;
        
    public:
        /**
         *
         * @brief DiskDevice constructor
         * @param bytespersector_ = 512: Bytes per sector
         *
         */
        DiskDevice(size_t bytespersector_ = 512);
        
        /**
         *
         * @brief DiskDevice destructor
         *
         */
        ~DiskDevice();
        
        /**
         *
         * @brief DiskDevice initializer
         * @details Initializes Disk driver.
         * Function checks function wrappers:
         * @li openDisk()
         * @li readDisk()
         * @li writeDisk() (--)
         * @li closeDisk()
         *
         */
        int      initialize();
        
        /**
         *
         * @brief Sector size getter
         * @details Gets bytes per sector.
         * @return size_t: Bytes per sector
         *
         */
        size_t getBytespersector();
        
        /**
         *
         * @brief Disk open function
         * @details Opens disk.
         * @return int: Error code
         *
         */
        int      (*open)() = NULL;
    
        /**
         *
         * @brief Disk read function
         * @details Reads disk and stores data by sector.
         * @param DiskDevice* dd: this
         * @param Sector* dest: buffer to read data
         * @param size_t lba: Offset location
         * @param size_t size: Sector count
         * @return int: Error code
         *
         */
        int (*read)(DiskDevice* dd, uint8_t* dest, size_t lba, size_t size) = NULL;
        int readDisk(uint8_t* dest, size_t lba, size_t size);
    
        /**
         *
         * @brief Disk write function
         * @details Reads data and writes to disk by sector.
         * @param Sector* src: buffer to write data
         * @param size_t lba: Location to write
         * @return int: Error code
         *
         */
        int      (*write)(Sector*, size_t) = NULL;
        
        
        /**
         *
         * @brief Disk close function
         * @details Closes disk.
         * @return int: Error code
         *
         */
        int      (*close)() = NULL;
    };
}


#endif