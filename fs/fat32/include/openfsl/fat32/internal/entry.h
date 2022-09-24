/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_FAT32_INCLUDE_OPENFSL_FAT32_ENTRY_H_
#define FS_FAT32_INCLUDE_OPENFSL_FAT32_ENTRY_H_

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

#include "openfsl/fat32/internal/time.h"

namespace openfsl {
namespace fat32 {

class FAT32;
typedef uint32_t cluster_t;

/**
 * @brief FAT32 directory entry struct
 * @details
 * Type         | Name                    | Offset
 * -------------|-------------------------|----------
 * uint8_t      | fileName[8]             | 0x00
 * uint8_t      | fileExt[3]              | 0x08
 * FileAttribute| fileAttr                | 0x0B
 * uint8_t      | fileReserved1           | 0x0C
 * uint8_t      | fileCreateTenth         | 0x0D
 * Time         | fileCreateTime          | 0x0E
 * Date         | fileCreateDate          | 0x10
 * Date         | fileAccessDate          | 0x12
 * uint16_t     | fileLocationHigh        | 0x14
 * Time         | fileModTime             | 0x16
 * Date         | fileModDate             | 0x18
 * uint16_t     | fileLocationLow         | 0x1A
 * uint32_t     | fileSize                | 0x1C
 *
 */
#pragma pack(push, 1)
typedef struct FileEntry {
    uint8_t        fileName[8];
    uint8_t        fileExt[3];
    FileAttribute  fileAttr;
    uint8_t        fileReserved1;
    uint8_t        fileCreateTenth;
    Time           fileCreateTime;
    Date           fileCreateDate;
    Date           fileAccessDate;
    uint16_t       fileLocationHigh;
    Time           fileModTime;
    Date           fileModDate;
    uint16_t       fileLocationLow;
    uint32_t       fileSize;
} FileEntry;
#pragma pack(pop)

/**
 * @brief FAT32 directory entry for LFN
 * @details
 * Type         | Name                    | Offset
 * -------------|-------------------------|----------
 * uint8_t      | lfnSeqNum               | 0x00
 * uint16_t     | lfnFileName1[5]         | 0x01
 * FileAttribute| lfnAttr                 | 0x0B
 * uint8_t      | lfnReserved1            | 0x0C
 * uint8_t      | lfnCheckSum             | 0x0D
 * uint16_t     | lfnFileName2[6]         | 0x0E
 * uint16_t     | lfnLocationLow          | 0x1A
 * uint16_t     | lfnFileName3[2]         | 0x1C
 * 
 */
#pragma pack(push, 1)
typedef struct LFNEntry {
    uint8_t        lfnSeqNum;
    uint16_t       lfnFileName1[5];
    FileAttribute  lfnAttr;
    uint8_t        lfnReserved1;
    uint8_t        lfnCheckSum;
    uint16_t       lfnFileName2[6];
    uint16_t       lfnLocationLow;
    uint16_t       lfnFileName3[2];
} LFNEntry;
#pragma pack(pop)

}  // namespace fat32
}  // namespace openfsl

#endif  // FS_FAT32_INCLUDE_OPENFSL_FAT32_ENTRY_H_
