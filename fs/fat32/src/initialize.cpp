/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fat32.h"

error_t openfsl::fat32::FAT32::initialize() {
    error_t result = 0;

    fsOperationMutex.lock();
    if (!initialized) {
        result = FileSystem::initialize();
        if (result)
            goto funcRet;

        // Read BPB sector
        Sector structSector(1, iod->getDiskParameter().bytesPerSector);
        result = iod->readSector(structSector.getData(), getActualLBA(0), 1);
        if (result)
            return result;

        memcpy(&bpb, structSector.getData(), sizeof(BPBSector));

        if (memcmp(bpb.ebpbFSType, "FAT32   ", 8) != 0) {
            result = OPENFSL_ERROR_INVALID_SIGNATURE;
            goto funcRet;
        }

        if (leToSystem<uint16_t>(bpb.bpbVBRSignature) != 0xAA55) {  // Check BPB signature
            result = OPENFSL_ERROR_INVALID_SIGNATURE;
            goto funcRet;
        }

        result = iod->readSector(structSector.getData(),
            getActualLBA(leToSystem<uint16_t>(bpb.ebpbFSInfoSector)), 1);  // Read FSINFO sector
        if (result)
            goto funcRet;

        memcpy(&fsinfo, structSector.getData(), sizeof(FSINFOSector));

        if (leToSystem<uint32_t>(fsinfo.fsinfoSignature1) != 0x41615252) {  // Check FSINFO signature
            result = OPENFSL_ERROR_INVALID_SIGNATURE;
            goto funcRet;
        }

        if (leToSystem<uint32_t>(fsinfo.fsinfoSignature2) != 0x61417272) {
            result = OPENFSL_ERROR_INVALID_SIGNATURE;
            goto funcRet;
        }

        if (leToSystem<uint16_t>(fsinfo.fsinfoSignature3) != 0xAA55) {
            result = OPENFSL_ERROR_INVALID_SIGNATURE;
            goto funcRet;
        }

        currentPath = rootSymbol;
        currentCluster = leToSystem<cluster_t>(bpb.ebpbRootDirectoryCluster);

        fatArea = new Sector(leToSystem<uint32_t>(bpb.ebpbSectorsPerFAT32),
            iod->getDiskParameter().bytesPerSector);
        result = iod->readSector(fatArea->getDataCast<uint8_t>(),
            getActualLBA(leToSystem<uint16_t>(bpb.bpbReservedSectors)),
            leToSystem<uint32_t>(bpb.ebpbSectorsPerFAT32));
        if (result)
            goto funcRet;
    }

funcRet:
    fsOperationMutex.unlock();

    return result;
}
