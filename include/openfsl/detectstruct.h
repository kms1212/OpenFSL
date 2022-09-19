/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_DETECTSTRUCT_H_
#define OPENFSL_DETECTSTRUCT_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <cstring>

#include <string>
#include <vector>
#include <sstream>

#include "openfsl/filesystem.h"
#include "openfsl/blockdevice.h"

namespace openfsl {
enum class PartitionTableType {
    None,
    MBR,
    GPT
};

typedef struct {
    PartitionTableType partTable;
    std::vector<FileSystemType> partList;
} DiskStructure;

error_t __check_result detectDiskStructure(DiskStructure* buf, BlockDevice* dd);
error_t __check_result detectFileSystem(
    FileSystemType* buf, BlockDevice* bd, lba48_t bpbSectorAddr = 0);

std::string partitionTableTypeToString(const PartitionTableType type);
std::string fileSystemTypeToString(const FileSystemType type);

}  // namespace openfsl

#endif  // OPENFSL_DETECTSTRUCT_H_
