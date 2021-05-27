/* Copyright (c) 2021. kms1212(권민수)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "../header/fs_fat32/fs_fat32.h"

using namespace openFSL;

FS_FAT32::FS_FAT32(DiskDevice* dd_, FAT32_Option option_, std::string pathSeparator_) {
	dd = dd_;
	option = option_;
	pathSeparator = pathSeparator_;
}

FS_FAT32::~FS_FAT32() {
	dd->close();
	errorState = FAT32_ERROR_NOT_INITIALIZED;
	delete fatArea;
}

void FS_FAT32::initialize() {
	dd->initialize();
	if (dd->getState() != DISKDEV_ERROR_SUCCESS) {
		errorState = FAT32_ERROR_DISKDEVICE;
		return;
	}
		
	Sector fat32_sector(1, dd->getBytespersector());
	dd->readDisk(&fat32_sector, 0, 1);
	FAT32_bpb* bpb = (FAT32_bpb*)fat32_sector.getData();
	
	uint32_t sigCode = 0x009058EB;
		
	if (memcmp(bpb->bpbJumpCode, &sigCode, 3) != 0)
	{
		errorState = FAT32_ERROR_SIGNATURE;
		return;
	}
	
	if (bpb->bpbVBRSignature != 0xAA55)
	{
		errorState = FAT32_ERROR_SIGNATURE;
		return;
	}
	
	volumeID = bpb->ebpbVolumeSerial;
	
	char buffer[12];
	
	memcpy(buffer, bpb->bpbOEMName, 8);
	oemLabel = buffer;
	memset(buffer, 0, 8);
	
	memcpy(buffer, bpb->ebpbVolumeLabel, 11);
	volumeLabel = buffer;
	memset(buffer, 0, 11);
	
	memcpy(buffer, bpb->ebpbFSType, 8);
	fsType = buffer;
	memset(buffer, 0, 8);
	
	sectorPerCluster = bpb->bpbSectorPerCluster;
	resvSectorCount = bpb->bpbReservedSectors;
	totalSector32 = bpb->bpbTotalSector32;
	fatSize32 = bpb->ebpbSectorsPerFAT32;
	fsinfoSector = bpb->ebpbFSInfoSector;
	rootCluster = bpb->ebpbRootDirectoryCluster;
	fatCount = bpb->bpbFATCount;
	
	dd->readDisk(&fat32_sector, fsinfoSector, 1);
	
	FAT32_fsinfo* fsinfo = (FAT32_fsinfo*)fat32_sector.getData();
	
	if (fsinfo->fsinfoSignature3 != 0xAA550000)
	{
		errorState = FAT32_ERROR_SIGNATURE;
		return;
	}
	
	freeCluster = fsinfo->fsinfoFreeCluster;
	nextCluster = fsinfo->fsinfoNextFree;
	
	currentPath = "::";
	currentCluster = rootCluster;
	
	fatArea = new Sector(1, dd->getBytespersector() * fatSize32);
	dd->readDisk(fatArea, resvSectorCount, fatSize32);
	fatClusterList = (uint32_t*)fatArea->getData();
	
	if (pathSeparator == "")
	{
		pathSeparator = "\\";
	}
	
	errorState = DISKDEV_ERROR_SUCCESS;
	
}

void FS_FAT32::setDiskDevice(DiskDevice* dd_)
{
	dd = dd_;
}

DiskDevice* FS_FAT32::getDiskDevice()
{
	return dd;
}

uint32_t FS_FAT32::getState() {
	return errorState;
}

std::string FS_FAT32::getPath() {
	return currentPath;
}