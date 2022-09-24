/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_NTFS_INCLUDE_OPENFSL_NTFS_FS_NTFS_H_
#define FS_NTFS_INCLUDE_OPENFSL_NTFS_FS_NTFS_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstdint>
#include <cstring>
#include <cstddef>
#include <ctime>

#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <iterator>
#include <vector>
#include <set>
#include <codecvt>
#include <locale>
#include <regex>

#include "openfsl/blockdevice.h"
#include "openfsl/filesystem.h"

#define FS_NTFS_H_INTERNAL_

namespace openfsl {
/**
 * @brief NTFS imeplementation
 * @details Implements NTFS
 */
class NTFS : public FileSystem<BlockDevice> {
 public:
    typedef uint64_t cluster_t;
    typedef uint64_t filetime_t;

    enum class Version {
        v1_0,
        v1_1,
        v1_2,
        v3_0,
        v3_1
    };

#include "openfsl/ntfs/ntfs_structures.h"

 private:
    Version version;

    VBRSector vbr;

 public:
    static const FileSystemType fsType = FileSystemType::NTFS;

    // General class functions ////////////////////////////////////////////
    /**
     * @brief NTFS constructor
     * @param bd_: Block device driver class pointer
     * @param pathSeparator_: File system path separator (Default: '/')
     * @param rootPath_: File system root keyword (Default: "::")
     */
    NTFS(BlockDevice* bd_,
        const std::string startPath_ = "",
        const std::string pathSeparator_ = "/",
        const std::string rootSymbol_ = "::",
        const Version version = Version::v3_1);

    ~NTFS();

    /**
     * @brief NTFS intializer
     * @details Initializes NTFS filesystem.
     */
    error_t __check_result initialize();
    ///////////////////////////////////////////////////////////////////////

 private:
    

    // Cluster Run Tools //////////////////////////////////////////////////
    std::vector<std::pair<cluster_t, cluster_t>>
        __parseClusterRun(const uint8_t* array);
    ///////////////////////////////////////////////////////////////////////


};
}  // namespace openfsl

#undef FS_NTFS_H_INTERNAL_

#endif  // FS_NTFS_INCLUDE_OPENFSL_NTFS_FS_NTFS_H_
