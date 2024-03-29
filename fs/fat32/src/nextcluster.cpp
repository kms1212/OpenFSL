/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fat32.h"

openfsl::fat32::cluster_t openfsl::fat32::FAT32::__getNextFreeCluster() {
    for (cluster_t i = 0; i < bpb.ebpbSectorsPerFAT32 * 128; i++) {
        if (fatArea->getDataCast<cluster_t>()[i] == 0) {
            return i;
        }
    }

    return 0;
}
