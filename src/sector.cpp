#include "sector.h"

Sector::Sector(size_t sector_count_, char* data_) {
	/* Calculate total size of sector data buffer */
	size_t bufsize = sector_count_ * sector_size;
	
	/* Allocate memory and copy if the given address and allocated address are the same */
	data = (char*)FSLServices::malloc(bufsize);
	if (data != data_) {
		FSLServices::memcpy(data, data_, bufsize);
	}
}

char* Sector::getData() {
	return data;
}

Sector::~Sector() {
	/* Free memory */
	FSLServices::free(Sector::data);
}