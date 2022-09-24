/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_FAT32_INCLUDE_OPENFSL_FAT32_FAT32_H_
#define FS_FAT32_INCLUDE_OPENFSL_FAT32_FAT32_H_

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

#include "openfsl/fat32/internal/specialsector.h"
#include "openfsl/fat32/internal/clusterchain.h"
#include "openfsl/fat32/internal/cache.h"
#include "openfsl/fat32/internal/file.h"
#include "openfsl/fat32/internal/options.h"
#include "openfsl/fat32/internal/entry.h"

namespace openfsl {
namespace fat32 {

typedef uint32_t cluster_t;

/**
 * @brief FAT32 volume information
 * @author kms1212
 * @details
 * 
 */
typedef struct VolumeInfo {
    uint32_t       volumeSerial;
    std::string    volumeLabel;
    cluster_t      freeCluster;
    cluster_t      nextFreeCluster;
    uint8_t        sectorPerCluster;
    uint16_t       bytesPerSector;
    lba48_t        filesystemSize;
} VolumeInfo;

/**
 * @brief FAT32 file system class
 * @details
 * This class is a FAT32 file system class.
 */
class FAT32: public FileSystem<BlockDevice> {
 private:
    /// BIOS Parameter Block sector
    BPBSector bpb;
    /// File System Information sector
    FSINFOSector fsinfo;
    /// FAT Area sectors
    Sector* fatArea = nullptr;

    /// Current cluster cursor
    cluster_t     currentCluster;

    CacheManager cacheManager;

    std::map<std::string, CacheEntry> fsCache;
    std::mutex cacheMutex;

    std::condition_variable cacheCv;
    std::mutex cacheCvMutex;

    std::thread cacheMgrThread;
    bool cacheEnabled = false;

    size_t cacheMaxEntry;
    size_t cacheOpCount;
    std::mutex cacheOpCountMutex;

    FilesystemOptions fsOptions;

 public:
    static constexpr FileSystemType fsType = FileSystemType::FAT32;

    // General class functions ////////////////////////////////////////////
    /**
     * @brief FS_FAT32 constructor
     * @param bd_: Disk device driver class pointer
     * @param startpath_: File system starting path (Default: ""(rootSymbol_))
     * @param pathSeparator_: File system path separator (Default: '/')
     * @param rootSymbol_: File system root keyword (Default: "::")
     * @param fsOptions_: File system options
     */
    FAT32(BlockDevice* bd_,
        const FilesystemOptions fsOptions_,
        const std::string startPath_ = "",
        const std::string pathSeparator_ = "/",
        const std::string rootSymbol_ = "::");

    FAT32(BlockDevice* bd_,
          const std::string startPath_ = "",
          const std::string pathSeparator_ = "/",
          const std::string rootSymbol_ = "::");

    ~FAT32();

    /**
     * @brief FS_FAT32 intializer
     * @details Initializes FAT32 class.
     */
    error_t __check_result initialize();
    ///////////////////////////////////////////////////////////////////////


    // Format function ////////////////////////////////////////////////////
    error_t __check_result format(const lba48_t offset, const lba48_t size,
        const FormatOptions formatOptions, const bool fastFormat = false);
    ///////////////////////////////////////////////////////////////////////


    // Getter / Setter ////////////////////////////////////////////////////
    inline const FilesystemOptions getFsOptions() { return fsOptions; }

    inline void setFsOptions(const FilesystemOptions fsOptions_)
        {fsOptions = fsOptions_; }

    inline void setPath(const std::string path) { currentPath = path; }

    inline void setCluster(const cluster_t cluster)
        { currentCluster = cluster; }

    inline std::string getPathSeparator() { return pathSeparator; }

    inline BPBSector* getBPB() { return &bpb; }

    inline FSINFOSector* getFSINFO() { return &fsinfo; }

    inline Sector* getFatArea() { return fatArea; }

    inline std::string getPath() { return currentPath; }

    inline cluster_t getCluster() { return currentCluster; }

    inline std::mutex* getFsOperationMutex() { return &fsOperationMutex; }
    ///////////////////////////////////////////////////////////////////////

    // Parameter sector update function ///////////////////////////////////
    /**
     * @brief Update FSINFO
     * @details Writes FSINFO structure to disk.
     */
    error_t __check_result __updateFsInfo();

    /**
     * @brief Update BPB
     * @details Writes BPB structure to disk.
     */
    error_t __check_result __updateBpb();
    ///////////////////////////////////////////////////////////////////////


    // File Entry controls ////////////////////////////////////////////////
    /**
     * @brief Create entry
     * @details Gets information from createFileInfo and creates entry 
     * @param path: File create directory
     * @param createFileInfo: Entry information
     * @param ignoreReserved: Whether or not ignore reserved name
     * @return error code
     */
    error_t __check_result
        __createEntry(const std::string path, const FileInfo createFileInfo);

    /**
     * @brief Remove entry
     * @details Removes entry of given name
     * @param path: File remove directory
     * @param deletename: Entry name to remove
     * @param filter: Remove file search filter
     */
    error_t __check_result
        __removeEntry(const std::string path, const std::string deletename,
            const FileAttribute filter);

    /**
     * @brief Get file entry information
     * @param path: working directory
     * @param filename: file name to get information
     * @return FAT32_fileInfo: File information
     */
    std::pair<error_t, FileInfo> __getEntryInformation(
        const std::string path, const std::string filename);

    error_t __check_result
        __setEntryInformation(const std::string path,
            const std::string filename, const FileInfo fileInfo);
    ///////////////////////////////////////////////////////////////////////


    // Path controls /////////////////////////////////////////////////
    /**
     * @brief List directory contents as path.
     * @details Returns subitem information.
     * @param buf: File information buffer
     * @param searchAttr: Attribute filter
     * @param path: working directory (Default: "")
     * @return Error code
     */
    error_t __check_result
        __listDir(std::vector<FileInfo>* buf, const std::string path,
            const FileAttribute searchAttr =
                FileAttribute::Archive | FileAttribute::Directory);

    /**
     * @brief List directory contents as cluster.
     * @details Returns subitem information.
     * @param buf: File information buffer
     * @param searchAttr: Attribute filter
     * @param cluster: working cluster (Default: 0xFFFFFFFF)
     * @return Error code
     */
    error_t __check_result
        __listDir(std::vector<FileInfo>* buf, const cluster_t cluster,
            const FileAttribute searchAttr =
                FileAttribute::Archive | FileAttribute::Directory);

    /**
     * @brief List directory contents of current directory.
     * @details Returns subitem information.
     * @param buf: File information buffer
     * @param searchAttr: Attribute filter
     * @return Error code
     */
    error_t __check_result
        __listDir(std::vector<FileInfo>* buf,
            const FileAttribute searchAttr =
                FileAttribute::Archive | FileAttribute::Directory);

    /**
     * @brief Change directory
     * @details Changes working directory.
     * @param path: working directory
     * @param subdir: Parameter for recursive search. (Default: null)
     * @return Error code
     */
    error_t __check_result __chdir(
        const std::string path, std::vector<std::string>* subdir = nullptr);

    error_t __check_result
        __forEachEntry(std::function<bool(FileInfo)> forEachFunction,
            const std::string path = "");
    ///////////////////////////////////////////////////////////////////////


    // Directory / file controls //////////////////////////////////////////
    /**
     * @brief Create directory
     * @details Creates directory.
     * @param path: working directory
     * @param dirname: directory name to create
     * @param createTime: create time
     * @return Error code
     */
    error_t __check_result
        __makeDir(const std::string path, const std::string dirname,
            const openfsl::Time* createTime = nullptr);

    /**
     * @brief Create file
     * @details Creates file.
     * @param path: working directory
     * @param dirname: directory name to create
     * @param createTime: create time
     */
    error_t __check_result
        __makeFile(const std::string path, const std::string dirname,
            const openfsl::Time* createTime = nullptr);

    /**
     * @brief Remove directory
     * @details Removes directory.
     * @param path: working directory
     * @param name: directory name to remove
     * @return Error code
     */
    error_t __check_result __removeDir(
            const std::string path, const std::string name);

    /**
     * @brief Remove file
     * @details Removes file.
     * @param path: working directory
     * @param name: file name to remove
     * @return Error code
     */
    error_t __check_result __removeFile(
            const std::string path, const std::string name);

    error_t __move(const std::string fromPath, const std::string fromName,
        const std::string toPath, const std::string toName);

    error_t __search(std::vector<std::pair<std::string, FileInfo>>* buf,
        const std::string path, const std::string regex,
        const FileAttribute attr, const bool recurse,
        const bool isRecurseCall = false);

    error_t __search(std::vector<FileInfo>* buf,
        const std::string path, const std::string regex,
        const FileAttribute attr);

    error_t __search(std::vector<FileInfo>* buf,
        const cluster_t cluster, const std::string regex,
        const FileAttribute attr);

    std::pair<error_t, bool>
        __checkEmpty(const std::string path, const std::string dirname);
    ///////////////////////////////////////////////////////////////////////


    // Cluster Controls ///////////////////////////////////////////////////
    /**
     * @brief Get Last Cluster
     * @details Returns last index of linked cluster
     * @param cluster: Current cluster
     * @return cluster_t: Last cluster
     */
    cluster_t __getLastCluster(const cluster_t cluster);

    /**
     * @brief Get Next Free Cluster
     * @details Returns next free cluster (Next Available)
     * @return cluster_t: Next free cluster
     */
    cluster_t __getNextFreeCluster();

    /**
     * @brief Allocate free cluster
     * @details Gets next free cluster and 
     * @param cluster: linked cluster address to allocate
     * @return uint32_t: allocated cluster
     **/
    cluster_t __allocateFreeCluster(const cluster_t cluster);
    ///////////////////////////////////////////////////////////////////////


    // Class utilities ////////////////////////////////////////////////////
    /**
     * @brief Checksum
     * @details Generates a checksum of SFN filename.
     * @param src: source pointer
     * @return uint8_t: checksum
     */
    uint8_t __chksumSFN(const uint8_t* src);

    /**
     * @brief Convert FAT area index to LBA
     * @details Converts FAT area index to LBA that contains the element.
     * @param cluster: cluster index
     * @return LBA
     */
    lba48_t __fatAreaIndexToSector(const cluster_t cluster);

    /**
     * @brief Separate file name
     * @details Separates full path to path and filename
     * @param filepath: file path
     * @return std::pair<std::string, std::string>: left one is directory, right one is filename
     */
    std::pair<std::string, std::string>
        __separateFileName(const std::string filepath);
    ///////////////////////////////////////////////////////////////////////


    // Cache controls /////////////////////////////////////////////////////
    /**
     * @brief Cache manager thread
     * @details Manages cache and keeps the number of caches
     */
    void __cacheMgrThreadRunner();

    /**
     * @brief Insert cache
     * @details Inserts cache with given value and invoke cache manager thread
     */
    error_t __insertCache(const std::string key,
        const CacheType type, const void* data, const size_t dataSize);

    /**
     * @brief Delete cache
     * @details Deletes cache
     */
    error_t __deleteCache(const std::string key);

    /**
     * @brief Get cache value
     * @details Get cache type and cache value pointer
     */
    std::pair<CacheType, void*>
        __getCacheValue(const std::string key, const CacheType cacheType);
    ///////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////
    // Controller wrappers
    ///////////////////////////////////////////////////////////////////////

    // Cache controls /////////////////////////////////////////////////////
    /**
     * @brief Enable cache
     */
    error_t enableCache(const size_t cacheMaxEntry_);

    /**
     * @brief Disable cache
     */
    void disableCache();
    ///////////////////////////////////////////////////////////////////////

    // Volume controls ////////////////////////////////////////////////////
    VolumeInfo getVolumeInfo();
    error_t __check_result setVolumeName(const std::string volumeName);
    ///////////////////////////////////////////////////////////////////////


    // Path controls //////////////////////////////////////////////////////
    error_t changeDirectory(const std::string path);
    error_t listDirectory(std::vector<FileInfo>* buf,
        const std::string path = "",
        const FileAttribute searchAttr =
            FileAttribute::Archive | FileAttribute::Directory);
    ///////////////////////////////////////////////////////////////////////


    // File / Directory controls //////////////////////////////////////////
    error_t makeDirectory(const std::string path);
    error_t makeFile(const std::string path);

    error_t removeDirectory(const std::string path);
    error_t removeFile(const std::string path);

    error_t move(const std::string from, const std::string to);

    error_t search(std::vector<std::pair<std::string, FileInfo>>* buf,
        const std::string path, const std::string regex,
        const FileAttribute attr, const bool recurse);
    ///////////////////////////////////////////////////////////////////////


    // Entry controls /////////////////////////////////////////////////////
    std::pair<error_t, FileInfo> getEntryInfo(const std::string path);
    error_t setEntryInfo(const std::string path, FileInfo fileInfo);
    ///////////////////////////////////////////////////////////////////////

    // Utilities //////////////////////////////////////////////////////////
    /**
     * @brief Get entry count from given path
     * @details Returns 0 when path is invalid.
     * Returns 1 when path is SFN.
     * Returns required entry count when path is LFN.
     */
    int getEntryCount(const std::string path,
                      const bool ignoreReservedName);

    std::string convertToAbsolutePath(const std::string relativePath);
    ///////////////////////////////////////////////////////////////////////
};

}  // namespace fat32
}  // namespace openfsl


#endif  // FS_FAT32_INCLUDE_OPENFSL_FAT32_FS_FAT32_H_
