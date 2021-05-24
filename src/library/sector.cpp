#include "../header/sector.h"

using namespace openFSL;

Sector::Sector(size_t sector_count_, uint32_t bytespersector) {
	/* Calculate total size of sector data buffer */
	size_t bufsize = sector_count_ * bytespersector;
	sector_count = sector_count_;
	
	/* Allocate memory and copy if the given address and allocated address are the same */
	data = new uint8_t[bufsize];
}

uint64_t Sector::getSectorCount() {
	return sector_count;
}

uint8_t* Sector::getData() {
	return data;
}

Sector::~Sector() {
	/* Free memory */
	delete data;
}