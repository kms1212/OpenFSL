/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fs_fat32.h"

using namespace openFSL;

int FS_FAT32::initialize() {
    int result = 0;
    
    result += dd->initialize();                                     // Initialize disk device
    
    Sector fat32_sector(1, dd->getBytespersector());
    result += dd->readDisk(&fat32_sector, 0, 1);                    // Read BPB sector
    FAT32_bpb* bpb = (FAT32_bpb*)fat32_sector.getData();
    
    uint32_t sigCode = 0x009058EB;
        
    if (memcmp(bpb->bpbJumpCode, &sigCode, 3) != 0)                 // Check boot jump code
    {
        result++;
        return result;
    }
    
    if (bpb->bpbVBRSignature != 0xAA55)                             // Check BPB signature
    {
        result++;
        return result;
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
    
    result += dd->readDisk(&fat32_sector, fsinfoSector, 1);         // Read FSINFO sector
    
    FAT32_fsinfo* fsinfo = (FAT32_fsinfo*)fat32_sector.getData();
    
    if (fsinfo->fsinfoSignature3 != 0xAA550000)                     // Check FSINFO signature
    {
        result++;
        return result;
    }
    
    freeCluster = fsinfo->fsinfoFreeCluster;                        // Copy data to class.
    nextCluster = fsinfo->fsinfoNextFree;                           //
                                                                    //
    currentPath = "::";                                             //
    currentCluster = rootCluster;                                   //
    
    fatArea = new Sector(fatSize32, dd->getBytespersector());       // Read FAT area
    result += dd->readDisk(fatArea, resvSectorCount, fatSize32);
    fatClusterList = (uint32_t*)fatArea->getData();
    
    return result;
}