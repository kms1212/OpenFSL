/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_FAT32_INCLUDE_OPENFSL_FAT32_FS_FAT32_H_
#define FS_FAT32_INCLUDE_OPENFSL_FAT32_FS_FAT32_H_

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

#define FS_FAT32_H_INTERNAL_

namespace openfsl {
/**
 * @brief FAT32 imeplementation
 * @details Implements FAT32
 * @todo Reduce disk io count, FS format
 */
class FAT32 : public FileSystem<BlockDevice> {
 public:
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

    typedef struct FilesystemOptions {
        uint64_t AllowSFNSearch : 1;
        uint64_t __Reserved : 63;
    } FilesystemOptions;

#include "openfsl/fat32/fat32_structures.h"

 private:
    /**
     * @brief FAT32 linked cluster
     * @details 
     * Implements linked cluster controls.
     * Class for internal usage.
     */
    class LinkedCluster {
     private:
        /// File system class pointer
        FAT32* fileSystem;

        /// Sector cursor cache
        Sector* fetchedSectorCache = nullptr;
        cluster_t currentCacheClusterIndex = 0;
        cluster_t currentCacheClusterSize = 0;

        /// Cluster chain start point
        cluster_t startCluster;

        /// List of cluster chain members
        std::vector<cluster_t> clusterList;

        /// List of modified clusters
        std::map<cluster_t, Sector*> modifiedClusters;

        // Internal functions /////////////////////////////////////////////////
        int updateClusterList();
        cluster_t getNextCluster(const cluster_t cluster);

        lba48_t clusterToLBA(const cluster_t cluster);
        ///////////////////////////////////////////////////////////////////////
     public:
        // General class functions /////////////////////////////////////////////
        LinkedCluster(FAT32* fileSystem_, const cluster_t startCluster_);
        ~LinkedCluster();
        ///////////////////////////////////////////////////////////////////////


        // Data controls //////////////////////////////////////////////////////
        error_t __check_result fetch(
            const cluster_t cluster, const cluster_t clusterSize);
        error_t __check_result flush();
        error_t __check_result read(
            void* buf, const size_t pos, const size_t size);
        error_t __check_result write(
            const void* buf, const size_t pos, const size_t size);
        ///////////////////////////////////////////////////////////////////////


        // Cluster controls ///////////////////////////////////////////////////
        error_t __check_result addCluster(const cluster_t addCount);
        error_t __check_result removeCluster(const cluster_t removeCount);
        ///////////////////////////////////////////////////////////////////////


        // Getter / Setter ////////////////////////////////////////////////////
        cluster_t calculateLinkLength();
        inline std::vector<cluster_t>* getClusterList() { return &clusterList; }
        ///////////////////////////////////////////////////////////////////////


        // Cache controls /////////////////////////////////////////////////////
        error_t __check_result shrinkCache(const cluster_t clusterSize);
        ///////////////////////////////////////////////////////////////////////
    };

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

    /// BIOS Parameter Block sector
    BPBSector bpb;
    /// File System Information sector
    FSINFOSector fsinfo;
    /// FAT Area sectors
    Sector* fatArea = nullptr;

    /// Current cluster cursor
    cluster_t     currentCluster;

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

    static FormatOptions getDefaultFormatOptions(const lba48_t fsSize);

    /**
     * @brief FAT32 file information
     * @author kms1212
     * @details
     * Type          | Name           | Description
     * --------------|----------------|----------------
     * std::string   | fileName       | Filename
     * std::string   | fileSFNName    | Filename in SFN. (No check in entry creation)
     * FileAttribute | fileAttr       | File attribute
     * Time          | fileCreateTime | File create time
     * Time          | fileAccessTime | File access time
     * Time          | fileModTime    | File moderation time
     * cluster_t     | fileLocation   | File location as cluster index (No check in entry creation)
     * cluster_t     | fileSize       | File size (No check in entry creation)
     * size_t        | fileEntryIndex | SFN entry index (No check in entry creation)
     * size_t        | fileEntrySize  | LFN entry count + 1(SFN entry count) (No check in entry creation)
     */
    typedef struct FileInfo {
        std::string    fileName = "";
        std::string    fileSFNName = "";
        Time           fileCreateTime;
        Time           fileAccessTime;
        Time           fileModTime;
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
    class FILE : public FSL_File {
     private:
        FAT32* fileSystem;

        LinkedCluster* fileCluster = nullptr;

        FileInfo currentFileInfo;

        std::mutex fileOperationMutex;

     public:
        // General class functions ////////////////////////////////////////////
        explicit FILE(FAT32* fileSystem_);
        ~FILE();
        ///////////////////////////////////////////////////////////////////////

        inline FAT32::FileInfo getFileInfo() { return currentFileInfo; }

        error_t open(const std::string path, const FSL_OpenMode openMode_);

        error_t reopen(const std::string path, const FSL_OpenMode openMode_);

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
        error_t seekg(const size_t location, const FSL_SeekMode whence);

        /**
         * @brief Seek put position
         * @param location: Seek location
         * @param whence: Seek mode
         */
        error_t seekp(const size_t location, const FSL_SeekMode whence);

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
    inline const FilesystemOptions getFsOptions() { return fsOptions; };

    inline void setFsOptions(const FilesystemOptions fsOptions_)
        {fsOptions = fsOptions_; };

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

 private:
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
     * @brief Get file information
     * @details Gets file information to open file.
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
            const Time* createTime = nullptr);

    /**
     * @brief Create file
     * @details Creates file.
     * @param path: working directory
     * @param dirname: directory name to create
     * @param createTime: create time
     */
    error_t __check_result
        __makeFile(const std::string path, const std::string dirname,
            const Time* createTime = nullptr);

    /**
     * @brief Remove directory or file
     * @details Removes directory or file.
     * @param path: working directory
     * @param name: directory or file name to create
     * @return Error code
     */
    error_t __check_result __remove(
            const std::string path, const std::string name,
            const FileAttribute attribute);

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


    // Time controls //////////////////////////////////////////////////////
    uint16_t __dateToWord(const Time date);
    uint16_t __timeToWord(const Time date);
    Time* __wordToDate(Time* d, const uint16_t date);
    Time* __wordToTime(Time* t, const uint16_t date);
    ///////////////////////////////////////////////////////////////////////


    // Class utilities ////////////////////////////////////////////////////
    /**
     * @brief UCS-2 string to UTF-8 string
     * @details Converts UCS-2 u16string to UTF-8 string.
     * @param src: source string
     * @return std::string: converted string
     */
    std::string __ucs2ToU8(const std::u16string src);

    /**
     * @brief UTF-8 string to UCS-2 string
     * @details Converts UTF-8 string to UCS-2 u16string.
     * @param src: source string
     * @return std::u16string: converted string
     */
    std::u16string __u8ToUCS2(const std::string src);

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
    std::pair<FAT32::CacheType, void*>
        __getCacheValue(const std::string key, const CacheType cacheType);
    ///////////////////////////////////////////////////////////////////////

 public:
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
    error_t listDirectory(std::vector<FAT32::FileInfo>* buf,
        const std::string path = "",
        const FileAttribute searchAttr =
            FileAttribute::Archive | FileAttribute::Directory);
    ///////////////////////////////////////////////////////////////////////


    // File / Directory controls //////////////////////////////////////////
    error_t makeDirectory(const std::string path);
    error_t makeFile(const std::string path);

    error_t remove(const std::string path, FileAttribute attribute);

    error_t move(const std::string from, const std::string to);

    error_t search(std::vector<std::pair<std::string, FileInfo>>* buf,
        const std::string path, const std::string regex,
        const FileAttribute attr, const bool recurse);
    ///////////////////////////////////////////////////////////////////////


    // Entry controls /////////////////////////////////////////////////////
    std::pair<error_t, FAT32::FileInfo> getEntryInfo(const std::string path);
    error_t setEntryInfo(const std::string path, FAT32::FileInfo fileInfo);
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

}  // namespace openfsl

#undef FS_FAT32_H_INTERNAL_

#endif  // FS_FAT32_INCLUDE_OPENFSL_FAT32_FS_FAT32_H_
