/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_OPTIONS_H_
#define FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_OPTIONS_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <cstddef>

#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <condition_variable>
#include <utility>
#include <map>

#include "openfsl/blockdevice.h"
#include "openfsl/sector.h"
#include "openfsl/filesystem.h"
#include "openfsl/file.h"
#include "openfsl/fsltime.h"
#include "openfsl/endian.h"

namespace openfsl {
namespace fat32 {

class FAT32;
typedef uint32_t cluster_t;

typedef struct FormatOptions {
    std::string volumeLabel;
    uint64_t volumeSerial;
    uint8_t biosDriveNum;
    uint8_t sectorsPerCluster;
    lba48_t backupBSLocation;
    lba48_t fsinfoLocation;
    lba48_t reservedSectorCount;
    uint8_t fatAreaCount;
    uint8_t fatMediaType;
    cluster_t rootDirectoryCluster;
} FormatOptions;

FormatOptions getDefaultFormatOptions(const lba48_t fsSize);

typedef struct FilesystemOptions {
    uint64_t AllowSFNSearch : 1;
    uint64_t __Reserved : 63;
} FilesystemOptions;

}  // namespace fat32
}  // namespace openfsl

#endif  // FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_OPTIONS_H_
