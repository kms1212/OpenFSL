/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

FS_FAT32::FS_FAT32(DiskDevice* dd_, FAT32_Option option_, std::string pathSeparator_) {
    if (dd_ == NULL) {
        isDiskDeviceAllocated = true;
        dd = new DiskDevice();
    }
    else {
        isDiskDeviceAllocated = false;
        dd = dd_;
    }
    option = option_;
    pathSeparator = pathSeparator_;
}

FS_FAT32::~FS_FAT32() {
    errorState = FAT32_ERROR_NOT_INITIALIZED; 
    
    if (isDiskDeviceAllocated) {
        dd->close();                               // Close disk device
        delete dd;                             // Delete disk device
    }
    delete fatArea;                            // Delete loaded fat area
}

void FS_FAT32::initialize() {
    dd->initialize();                                               // Initialize disk device
    if (dd->getState() != DISKDEV_ERROR_SUCCESS) {                  // Return if disk device is not initialized
        errorState = FAT32_ERROR_DISKDEVICE;
        return;
    }
        
    Sector fat32_sector(1, dd->getBytespersector());
    dd->readDisk(&fat32_sector, 0, 1);                              // Read BPB sector
    FAT32_bpb* bpb = (FAT32_bpb*)fat32_sector.getData();
    
    uint32_t sigCode = 0x009058EB;
        
    if (memcmp(bpb->bpbJumpCode, &sigCode, 3) != 0)                 // Check boot jump code
    {
        errorState = FAT32_ERROR_SIGNATURE;
        return;
    }
    
    if (bpb->bpbVBRSignature != 0xAA55)                             // Check BPB signature
    {
        errorState = FAT32_ERROR_SIGNATURE;
        return;
    }
    
    volumeID = bpb->ebpbVolumeSerial;                               // Copy data to class.
                                                                    //
    char buffer[12];                                                //
                                                                    //
    memcpy(buffer, bpb->bpbOEMName, 8);                             //
    oemLabel = buffer;                                              //
    memset(buffer, 0, 8);                                           //
                                                                    //
    memcpy(buffer, bpb->ebpbVolumeLabel, 11);                       //
    volumeLabel = buffer;                                           //
    memset(buffer, 0, 11);                                          //
                                                                    //
    memcpy(buffer, bpb->ebpbFSType, 8);                             //
    fsType = buffer;                                                //
    memset(buffer, 0, 8);                                           //
                                                                    //
    sectorPerCluster = bpb->bpbSectorPerCluster;                    //
    resvSectorCount = bpb->bpbReservedSectors;                      //
    totalSector32 = bpb->bpbTotalSector32;                          //
    fatSize32 = bpb->ebpbSectorsPerFAT32;                           //
    fsinfoSector = bpb->ebpbFSInfoSector;                           //
    rootCluster = bpb->ebpbRootDirectoryCluster;                    //
    fatCount = bpb->bpbFATCount;                                    //
    
    dd->readDisk(&fat32_sector, fsinfoSector, 1);                   // Read FSINFO sector
    
    FAT32_fsinfo* fsinfo = (FAT32_fsinfo*)fat32_sector.getData();
    
    if (fsinfo->fsinfoSignature3 != 0xAA550000)                     // Check FSINFO signature
    {
        errorState = FAT32_ERROR_SIGNATURE;
        return;
    }
    
    freeCluster = fsinfo->fsinfoFreeCluster;                        // Copy data to class.
    nextCluster = fsinfo->fsinfoNextFree;                           //
                                                                    //
    currentPath = "::";                                             //
    currentCluster = rootCluster;                                   //
    
    fatArea = new Sector(1, dd->getBytespersector() * fatSize32);   // Read FAT area
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

uint16_t FS_FAT32::getSectorPerCluster() {
    return sectorPerCluster;
}