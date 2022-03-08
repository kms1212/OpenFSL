/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_ISO9660_INCLUDE_OPENFSL_ISO9660_FS_ISO9660_H_
#define FS_ISO9660_INCLUDE_OPENFSL_ISO9660_FS_ISO9660_H_

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
#include <algorithm>
#include <iterator>
#include <vector>
#include <set>
#include <codecvt>
#include <locale>
#include <regex>

#include "openfsl/blockdevice.h"
#include "openfsl/filesystem.h"

#define FS_ISO9660_H_INTERNAL_

namespace openfsl {
/**
 * @brief ISO9660 imeplementation
 * @details Implements ISO9660
 */
class ISO9660 : public FileSystem<BlockDevice> {
 private:
#include "openfsl/iso9660/iso9660_structures.h"

 public:
    static const FileSystemType fsType = FileSystemType::ISO9660;

    // General class functions ////////////////////////////////////////////
    /**
     * @brief ISO9660 constructor
     * @param bd_: Block device driver class pointer
     * @param pathSeparator_: File system path separator (Default: '/')
     * @param rootPath_: File system root keyword (Default: "::")
     */
    ISO9660(BlockDevice* bd_,
        const std::string startPath_ = "",
        const std::string pathSeparator_ = "/",
        const std::string rootSymbol_ = "::");

    ~ISO9660();

    /**
     * @brief ISO9660 intializer
     * @details Initializes ISO9660 filesystem.
     */
    error_t __check_result initialize();
    ///////////////////////////////////////////////////////////////////////
};
}  // namespace openfsl

#undef FS_ISO9660_H_INTERNAL_

#endif  // FS_ISO9660_INCLUDE_OPENFSL_ISO9660_FS_ISO9660_H_
