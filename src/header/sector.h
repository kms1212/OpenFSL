#ifndef __SECTOR_H
#define __SECTOR_H

#include <stdint.h>

#include "fslservices.h"
#include "vint.h"

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
		vint_arch sector_count;
		uint8_t* data;
		
	public:
		/**
		 *
		 * @brief Sector class constructor
		 * @param sector_count_: Sector count
		 * @param bytespersector: Bytes per sector
		 *
		 */
		Sector(size_t sector_count_, uint32_t bytespersector);
		/**
		 *
		 * @brief Sector class destructor
		 *
		 */
		~Sector();
		
		/**
		 *
		 * @brief Sector count getter
		 * @return vint_arch: Sector count
		 *
		 */
		vint_arch getSectorCount();
		
		/**
		 *
		 * @brief Data buffer getter
		 * @return uint8_t*: Data buffer pointer
		 *
		 */
		uint8_t* getData();
	};
}

#endif