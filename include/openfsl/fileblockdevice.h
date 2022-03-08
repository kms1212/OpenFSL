/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_FILEBLOCKDEVICE_H_
#define OPENFSL_FILEBLOCKDEVICE_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif


#ifndef OPENFSL_FBD_DISABLED


#include <cstdint>

#include <iostream>
#include <fstream>
#include <string>

#include "openfsl/blockdevice.h"
#include "openfsl/error.h"
#include "openfsl/macros.h"
#include "openfsl/types.h"
#include "openfsl/sector.h"
#include "openfsl/iodevice.h"

namespace openfsl {
class FileBlockDevice : public BlockDevice {
 private:
    std::fstream file;
    std::string filename;

    using BlockDevice::initialize;

 public:
    enum class OpenMode {
        R,
        RW
    };

    /**
     * @brief FileBlockDevice constructor
     */
    FileBlockDevice();

    /**
     * @brief FileBlockDevice destructor
     */
    ~FileBlockDevice();

    /**
     *
     * @brief FileBlockDevice initializer
     * @details Initializes file device.
     */
    error_t initialize(const std::string filename, const OpenMode openMode);

    /**
     *
     * @brief FileBlockDevice deinitializer
     * @details Denitializes file device.
     */
    error_t deinitialize();

    inline std::fstream* getFileStream() { return &file; }
    inline std::string getFilename() { return filename; }

    error_t __check_result readSector(
        void* dest, const lba48_t lba, const lba48_t size) override;
    error_t __check_result writeSector(
        const void* src, const lba48_t lba, const lba48_t size) override;
};
}  // namespace openfsl


#endif  // OPENFSL_FBD_DISABLED


#endif  // OPENFSL_FILEBLOCKDEVICE_H_
