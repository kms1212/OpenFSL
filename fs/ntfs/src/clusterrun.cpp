/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/ntfs/fs_ntfs.h"

std::vector<std::pair<
    openfsl::NTFS::cluster_t, openfsl::NTFS::cluster_t>>
        openfsl::NTFS::__parseClusterRun(const uint8_t* array) {
    std::vector<std::pair<cluster_t, cluster_t>> ret;
    return ret;
}
