#ifndef __SECTOR_H__
#define __SECTOR_H__

#include "fixedint.h"
#include "fslservices.h"

namespace openFSL {
	class Sector {
	private:
		uint64_t sector_count;
		uint8_t* data;
		
	public:
		
		/* Generator & Destructor */
		Sector(size_t sector_count);
		~Sector();
		
		uint64_t getSectorCount();
		uint8_t* getData();
	};
}

#endif