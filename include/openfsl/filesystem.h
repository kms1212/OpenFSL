/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_FILESYSTEM_H_
#define OPENFSL_FILESYSTEM_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <string>

#include "openfsl/types.h"
#include "openfsl/blockdevice.h"

namespace openfsl {
enum class FileSystemType {
    None,
    FAT12,
    FAT16,
    FAT32,
    ExFAT,
    NTFS,
    Ext2,
    Ext3,
    Ext4,
    BtrFS,
    HFSPlus,
    APFS,
    ISO9660,
    UDF,
    GPT
};

template<class IOD> class FileSystem {
 protected:
    lba48_t fsLBAOffset = 0;
    bool initialized = false;

    IOD* iod;

    std::string  rootSymbol;
    std::string  pathSeparator;

    /// Current path cursor
    std::string  currentPath;

    /// Mutex for file system operation.
    std::mutex fsOperationMutex;

 public:
    static constexpr FileSystemType fsType = FileSystemType::None;

    // General class functions ////////////////////////////////////////////
    FileSystem(IOD* iodev_,
        const std::string startPath_ = "",
        const std::string pathSeparator_ = "/",
        const std::string rootSymbol_ = "::");
    virtual ~FileSystem() = 0;
    ///////////////////////////////////////////////////////////////////////


    // Getter / Setter ////////////////////////////////////////////////////
    inline void setFsLBAOffset(lba48_t fsLBAOffset_)
    { fsLBAOffset = fsLBAOffset_; }

    inline bool isInitialized() { return initialized; }
    inline lba48_t getFsLBAOffset() { return fsLBAOffset; }
    inline lba48_t getActualLBA(lba48_t lba) { return fsLBAOffset + lba; }
    inline IOD* getIODevice() { return iod; }
    ///////////////////////////////////////////////////////////////////////


    // File system basic methods //////////////////////////////////////////
    error_t __check_result initialize();
    ///////////////////////////////////////////////////////////////////////
};
}  // namespace openfsl


///////////////////////////////////////////////////////////////////////////
// Implementations
///////////////////////////////////////////////////////////////////////////

template <class IOD> openfsl::FileSystem<IOD>::FileSystem
    (IOD* iodev_, const std::string startPath_,
    const std::string pathSeparator_, const std::string rootSymbol_) {
    iod = iodev_;
    currentPath = startPath_;
    pathSeparator = pathSeparator_;
    rootSymbol = rootSymbol_;
}
template <class IOD> openfsl::FileSystem<IOD>::~FileSystem() {}

template <class IOD> error_t openfsl::FileSystem<IOD>::initialize() {
    initialized = true;
    return 0;
}

#endif  // OPENFSL_FILESYSTEM_H_
