/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef __DISKDEVICE_H
#define __DISKDEVICE_H

#include <stdint.h>

#include "sector.h"
#include "vint.h"

#define DISKDEV_ERROR_SUCCESS         0x0
#define DISKDEV_ERROR_NOT_INITIALIZED 0x01
#define DISKDEV_ERROR_INIT_NOFUNC     0x02
#define DISKDEV_ERROR_DISK_ERROR      0x03

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
		uint32_t errorState = DISKDEV_ERROR_NOT_INITIALIZED;
		uint32_t bytespersector;
		
	public:
		/**
		 *
		 * @brief DiskDevice constructor
		 * @param bytespersector_ = 512: Bytes per sector
		 *
		 */
		DiskDevice(uint32_t bytespersector_ = 512);
		
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
		void     initialize();
		
		/**
		 *
		 * @brief DiskDevice closer
		 * @details Stops disk driver.
		 *
		 */
		void     close();
		
		/**
		 *
		 * @brief Error state getter
		 * @details Gets error state.
		 * @return uint32_t: Error state
		 *
		 */
		uint32_t getState();
		
		/**
		 *
		 * @brief Sector size getter
		 * @details Gets bytes per sector.
		 * @return uint32_t: Bytes per sector
		 *
		 */
		uint32_t getBytespersector();
		
		/**
		 *
		 * @brief Disk open function wrapper
		 * @details Opens disk.
		 * @return int: Error code
		 *
		 */
		int      (*openDisk)();
	
		/**
		 *
		 * @brief Disk read function wrapper
		 * @details Reads disk and stores data by sector.
		 * @param Sector* dest: buffer to store data
		 * @param vint_arch lba: Data location
		 * @param vint_arch size: Sector count
		 * @return int: Error code
		 *
		 */
		int      (*readDisk)(Sector*, vint_arch, vint_arch);
		
		/**
		 *
		 * @brief Disk close function wrapper
		 * @details Closes disk.
		 * @return int: Error code
		 *
		 */
		int      (*closeDisk)();
	};
}


#endif