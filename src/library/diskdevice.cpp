#include "../header/diskdevice.h"

using namespace openFSL;

DiskDevice::DiskDevice() {
	
}

void DiskDevice::initialize() {
	if (openDisk == NULL) {
		errorState = DISKDEV_ERROR_INIT_NOFUNC;
		return;
	}
	else if (readDisk == NULL) 
	{
		errorState = DISKDEV_ERROR_INIT_NOFUNC;
		return;
	}
	else if (closeDisk == NULL) 
	{
		errorState = DISKDEV_ERROR_INIT_NOFUNC;
		return;
	}
	errorState = DISKDEV_ERROR_SUCCESS;
	openDisk();
}

DiskDevice::~DiskDevice() {
	if (errorState != DISKDEV_ERROR_NOT_INITIALIZED)
		close();
}

void DiskDevice::close() {
	errorState = DISKDEV_ERROR_NOT_INITIALIZED;
	closeDisk();
}

uint32_t DiskDevice::getState() {
	return errorState;
}