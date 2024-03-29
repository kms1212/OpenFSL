/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fat32.h"

error_t openfsl::fat32::FAT32::FAT32::__updateBpb() {
    Sector bpbSector(1, iod->getDiskParameter().bytesPerSector);
    memcpy(bpbSector.getData(), &bpb, sizeof(BPBSector));
    return iod->writeSector(bpbSector.getData(), getActualLBA(0), 1);
}

error_t openfsl::fat32::FAT32::__updateFsInfo() {
    Sector fsinfoSector(1, iod->getDiskParameter().bytesPerSector);
    memcpy(fsinfoSector.getData(), &fsinfo, sizeof(FSINFOSector));
    return iod->writeSector(fsinfoSector.getData(),
        getActualLBA(bpb.ebpbFSInfoSector), 1);
}
