/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_FILE_H_
#define FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_FILE_H_

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

#include "openfsl/fat32/internal/clusterchain.h"

namespace openfsl {
namespace fat32 {

class FAT32;
class ClusterChain;
typedef uint32_t cluster_t;

/**
 * @brief FAT32 file attribute
 */
enum class FileAttribute : uint8_t {
    ReadOnly = 0x01,
    Hidden = 0x02,
    System = 0x04,
    VolumeID = 0x08,
    Directory = 0x10,
    Archive = 0x20,
    LFN = 0x0F,
    Any = Directory | Archive,
    All = Directory | Archive | VolumeID
};

inline FileAttribute operator|
    (FileAttribute a, FileAttribute b)
{ return (FileAttribute)((uint8_t)a | (uint8_t)b); }

inline FileAttribute operator&
    (FileAttribute a, FileAttribute b)
{ return (FileAttribute)((uint8_t)a & (uint8_t)b); }

/**
 * @brief FAT32 file information
 * @author kms1212
 * @details
 * Type          | Name           | Description
 * --------------|----------------|----------------
 * std::string   | fileName       | Filename
 * std::string   | fileSFNName    | Filename in SFN. (No check in entry creation)
 * FileAttribute | fileAttr       | File attribute
 * openfsl::Time | fileCreateTime | File create time
 * openfsl::Time | fileAccessTime | File access time
 * openfsl::Time | fileModTime    | File moderation time
 * cluster_t     | fileLocation   | File location as cluster index (No check in entry creation)
 * cluster_t     | fileSize       | File size (No check in entry creation)
 * size_t        | fileEntryIndex | SFN entry index (No check in entry creation)
 * size_t        | fileEntrySize  | LFN entry count + 1(SFN entry count) (No check in entry creation)
 */
typedef struct FileInfo {
    std::string    fileName = "";
    std::string    fileSFNName = "";
    openfsl::Time  fileCreateTime;
    openfsl::Time  fileAccessTime;
    openfsl::Time  fileModTime;
    cluster_t      fileLocation = 0;
    size_t         fileSize = 0;
    size_t         fileEntryIndex = 0;
    size_t         fileEntrySize = 0;
    FileAttribute  fileAttr = (FileAttribute)0;
} FileInfo;

/**
 * @brief FAT32 file
 * @details Implements fat32 file controls
 */
class File : public openfsl::File {
 private:
    FAT32* fileSystem;

    ClusterChain* fileCluster = nullptr;

    FileInfo currentFileInfo;

    std::mutex fileOperationMutex;

 public:
    // General class functions ////////////////////////////////////////////
    explicit File(FAT32* fileSystem_);
    ~File();
    ///////////////////////////////////////////////////////////////////////

    inline FileInfo getFileInfo() { return currentFileInfo; }

    error_t open(const std::string path, const OpenMode openMode_);

    error_t reopen(const std::string path, const OpenMode openMode_);

    error_t close();

    /**
     * @brief Read file
     * @details Reads file from disk
     * @param buf: Data buffer
     * @param bs: Element size
     * @param count: Element count
     * @return size_t: Count of elements actually read
     */
    size_t read(void* buf, const size_t bs, const size_t count);

    /**
     * @brief Write file
     * @details Writes file to disk
     * @param buffer: Data buffer
     * @param bs: Element size
     * @param count: Element count
     * @return size_t: Count of elements actually wrote
     */
    size_t write(const void* buffer, const size_t bs, const size_t count);

    /**
     * @brief Seek get position
     * @param location: Seek location
     * @param whence: Seek mode
     */
    error_t seekg(const size_t location, const SeekMode whence);

    /**
     * @brief Seek put position
     * @param location: Seek location
     * @param whence: Seek mode
     */
    error_t seekp(const size_t location, const SeekMode whence);

    /**
     * @brief Tell get location
     **/
    size_t tellg();

    /**
     * @brief Tell put location
     **/
    size_t tellp();

    int flush();
};

}  // namespace fat32
}  // namespace openfsl

#endif  // FS_FAT32_INCLUDE_OPENFSL_FAT32_INTERNAL_FILE_H_
