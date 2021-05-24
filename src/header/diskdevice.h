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
	class DiskDevice {
	private:
		uint32_t errorState = DISKDEV_ERROR_NOT_INITIALIZED;
		uint32_t bytespersector;
		
	public:
		/* Generator & Destructor */
		DiskDevice(uint32_t bytespersector_ = 512);
		~DiskDevice();
		
		void     initialize();
		void     close();
		uint32_t getState();
		uint32_t getBytespersector();
		
		/* Open disk function wrapper */
		int      (*openDisk)();
		
		/* Read disk function wrapper */
		int      (*readDisk)(Sector*, vint_arch, vint_arch); // uint8_t* dest, vint_arch lba, vint_arch size

		/* Close disk function wrapper */
		int      (*closeDisk)();
	};
}


#endif