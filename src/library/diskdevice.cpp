#include "../header/diskdevice.h"

using namespace openFSL;

DiskDevice::DiskDevice(uint32_t bytespersector_) {
	bytespersector = bytespersector_;
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
	if (closeDisk())
	{
		errorState = DISKDEV_ERROR_DISK_ERROR;
	}
}

uint32_t DiskDevice::getState() {
	return errorState;
}

uint32_t DiskDevice::getBytespersector() {
	return bytespersector;
}