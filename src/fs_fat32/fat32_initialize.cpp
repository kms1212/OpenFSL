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
    result += dd->readDisk((uint8_t*)fat32_sector.getData(), 0, 1);                    // Read BPB sector
    bpb = new FAT32_bpb();
    memcpy(bpb, fat32_sector.getData(), sizeof(FAT32_bpb));
    
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
    
    result += dd->readDisk((uint8_t*)fat32_sector.getData(), bpb->ebpbFSInfoSector, 1);         // Read FSINFO sector
    
    fsinfo = new FAT32_fsinfo();
    memcpy(fsinfo, fat32_sector.getData(), sizeof(FAT32_fsinfo));
    
    if (fsinfo->fsinfoSignature3 != 0xAA550000)                     // Check FSINFO signature
    {
        result++;
        return result;
    }
    
    currentPath = "::";
    currentCluster = bpb->ebpbRootDirectoryCluster;
    
    fatClusterList = new uint32_t[bpb->ebpbSectorsPerFAT32 * dd->getBytespersector() / 4]();
    result += dd->readDisk((uint8_t*)fatClusterList, bpb->bpbReservedSectors, bpb->ebpbSectorsPerFAT32);
    
    return result;
}