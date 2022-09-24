/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_CACHE_H_
#define FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_CACHE_H_

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
 * @brief FAT32 cache type
 * @details
 * Type               | Name                  
 * -------------------|-----------------
 * PathClusterCache   | Cache that contains path, cluster pair. Data type is cluster_t*.
 * Struct for internal usage.
 */
enum class CacheType {
    None,
    PathClusterCache
};

/**
 * @brief FAT32 cache entry
 * @details
 * Type               | Name                  
 * -------------------|-----------------
 * size_t             | cacheIndex
 * std::string        | cacheKey
 * uint8_t            | cacheType
 * void*              | cacheData
 * Struct for internal usage.
 */
typedef struct CacheEntry {
    void*          cacheData = nullptr;
    size_t         cacheLastUsed = 0;
    size_t         cacheReadCount = 0;
    size_t         cacheCreated = 0;
    CacheType      cacheType;
} CacheEntry;

class CacheManager {
 private:
    std::map<std::string, CacheEntry> fsCache;
    std::mutex cacheMutex;
    std::condition_variable cacheCond;

 public:
};

}  // namespace fat32
}  // namespace openfsl

#endif  // FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_CACHE_H_
