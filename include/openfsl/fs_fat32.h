#ifndef __FS_FAT32_H__
#define __FS_FAT32_H__

#include "fixedint.h"
#include "diskdevice.h"
#include "logdevice.h"

#define FAT32_OPTION_NONE        0x00000000
#define FAT32_OPTION_LFN         0x00000001

namespace openFSL {
	class FS_FAT32 {
	private:
		DiskDevice disk;
		LogDevice log;
		uint32_t option;
	public:
		/* Generator & Destructor */
		FS_FAT32(DiskDevice disk_, LogDevice log_, uint32_t option_);
		~FS_FAT32();
		
		char* getData();
	};
}

#endif