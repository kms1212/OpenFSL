#ifndef __SECTOR_H__
#define __SECTOR_H__

#include <stddef.h>

#include "serviceswrapper.h"

class Sector {
private:
	char* data;
	size_t sector_size;
public:
	/* Generator & Destructor */
	Sector(size_t sector_count, char* data);
	~Sector();
	
	char* getData();
};

#endif