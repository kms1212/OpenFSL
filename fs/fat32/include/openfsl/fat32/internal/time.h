/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_TIME_H_
#define FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_TIME_H_

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

/**
 * @brief FAT32 time
 * @details FAT32 time struct
 */
typedef struct Time {
    uint16_t second: 5;
    uint16_t minute: 6;
    uint16_t hour: 5;
} Time;

/**
 * @brief FAT32 date
 * @details FAT32 date struct
 */
typedef struct Date {
    uint16_t day: 5;
    uint16_t month: 4;
    uint16_t year: 7;
} Date;

openfsl::Time __toFslTime(const Time fatTime);
openfsl::Time __toFslTime(const Date fatDate);
openfsl::Time __toFslTime(const Time fatTime, const Date fatDate);

Time __toFatTime(const openfsl::Time fslTime);
Date __toFatDate(const openfsl::Time fslTime);

}  // namespace fat32
}  // namespace openfsl

#endif  // FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_TIME_H_
