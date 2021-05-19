#ifndef __DISKDEVICE_H__
#define __DISKDEVICE_H__

#include "device.h"
#include "fixedint.h"
#include "sector.h"

#define DISKDEV_ERROR_SUCCESS 0x0
#define DISKDEV_ERROR_NOT_INITIALIZED 0x01
#define DISKDEV_ERROR_INIT_NOFUNC 0x02

namespace openFSL {
	class DiskDevice : public Device {
	private:
		uint32_t errorState = DISKDEV_ERROR_NOT_INITIALIZED;
		
	public:
		/* Generator & Destructor */
		DiskDevice();
		~DiskDevice();
		
		void initialize();
		void close();
		uint32_t getState();
		
		/* Open disk function wrapper */
		void (*openDisk)();
		
		/* Read disk function wrapper */
	#if defined(_M_X64) || defined(__amd64__)
		void (*readDisk)(Sector*, uint64_t, uint64_t); // uint8_t* dest, uint64_t lba, uint64_t size
	#else
		void (*readDisk)(Sector*, uint32_t, uint32_t); // uint8_t dest, uint32_t lba, uint32_t size
	#endif 

		/* Close disk function wrapper */
		void (*closeDisk)();
	};
}


#endif