/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

error_t openfsl::FAT32::__updateFsInfo() {
    Sector fsinfoSector(1, iod->getDiskParameter().bytesPerSector);
    memcpy(fsinfoSector.getData(), &fsinfo, sizeof(FSINFOSector));
    return iod->writeSector(fsinfoSector.getData(),
        getActualLBA(bpb.ebpbFSInfoSector), 1);
}
