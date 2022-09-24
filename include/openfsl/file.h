/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_FILE_H_
#define OPENFSL_FILE_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstddef>
#include <cstdint>

namespace openfsl {
/**
 * @brief File open mode
 * @author kms1212
 * @details
 * Mode      | Description
 * ----------|-----------------------------------------------
 * Read      | Read file
 * Write     | Write file
 * Append    | Append to the end of the file. (WRITE enabled)
 * Trunc     | Truncate file if existts. (WRITE enabled)
 * Nocreate  | Fail if file not exists. (WRITE enabled)
 * Noreplace | Fail if file exists. (WRITE enabled)
 * Binary    | Binary mode
 */
enum class OpenMode : uint16_t {
    Read = 0x0001,
    Write = 0x0002,
    Append = 0x0006,  // 0x4 + 0x2
    Trunc = 0x0000A,  // 0x8 + 0x2
    Nocreate = 0x0012,  // 0x10 + 0x2
    Noreplace = 0x0022,  // 0x20 + 0x2
    Binary = 0x0040
};

inline OpenMode operator| (OpenMode a, OpenMode b)
{ return (OpenMode)((uint16_t)a | (uint16_t)b); }

inline OpenMode operator& (OpenMode a, OpenMode b)
{ return (OpenMode)((uint16_t)a & (uint16_t)b); }

/**
 * @brief File seek
 * @author kms1212
 * @details
 * Mode     | Description
 * ---------|-----------------------------------------------
 * SeekSet | Seek from beginning of file
 * SeekCur | Seek from cursor
 * SeekEnd | Seek from end of file
 */
enum class SeekMode{
    SeekSet,
    SeekCur,
    SeekEnd
};

/**
 *
 * @brief OpenFSL file
 * @details Filesystem call wrapper for file control.
 * @author kms1212
 */
class File {
 protected:
    OpenMode openMode;

    size_t readPointer = 0;
    size_t writePointer = 0;

 public:
    File();
    virtual ~File() = 0;

    virtual int seekp(
        const size_t offset, const SeekMode whence) = 0;
    virtual int seekg(
        const size_t offset, const SeekMode whence) = 0;
    virtual size_t tellp() = 0;
    virtual size_t tellg() = 0;
    virtual size_t read(
        void* ptr, const size_t bs, const size_t count) = 0;
    virtual size_t write(
        const void* buffer, const size_t bs, const size_t count) = 0;
    virtual int flush() = 0;
};
}  // namespace openfsl

#endif  // OPENFSL_FILE_H_
