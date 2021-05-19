#include "../header/fs_fat32.h"

using namespace openFSL;

FS_FAT32::FS_FAT32(DiskDevice disk_, LogDevice log_, uint32_t option_) {
	disk = disk_;
	log = log_;
	option = option_;
}

FS_FAT32::~FS_FAT32() {
	
}