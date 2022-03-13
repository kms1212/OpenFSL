/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_FBD_DISABLED

#include <ios>

#include "openfsl/fileblockdevice.h"

openfsl::FileBlockDevice::FileBlockDevice() {}

openfsl::FileBlockDevice::~FileBlockDevice() {
    if (initialized)
        deinitialize();
}

error_t openfsl::FileBlockDevice::initialize
    (const std::string filename, const OpenMode openMode) {
    this->filename = filename;

    std::ios_base::openmode fileflag = std::ios::in | std::ios::binary;
    if (openMode == OpenMode::RW)
        fileflag |= std::ios::out;
    file.open(filename, fileflag);

    if (file.fail())
        return OPENFSL_ERROR_DISK;
        
    IODevice::initialize();

    return 0;
}

error_t openfsl::FileBlockDevice::deinitialize() {
    IODevice::deinitialize();

    file.close();

    return 0;
}

error_t openfsl::FileBlockDevice::readSector
    (void* dest, const lba48_t lba, const lba48_t size) {
    diskOperationMutex.lock();

    file.seekg(lba * diskParameter.bytesPerSector, std::ios::beg);
    bool fail = file.read(reinterpret_cast<char*>(dest),
        size * diskParameter.bytesPerSector).fail();
    if (!fail)
        fail = file.tellg() == -1;
    diskOperationMutex.unlock();

    if (fail)
        return OPENFSL_ERROR_DISK;

    return 0;
}

error_t openfsl::FileBlockDevice::writeSector(
    const void* src, const lba48_t lba, const lba48_t size) {
    diskOperationMutex.lock();

    file.seekp(lba * diskParameter.bytesPerSector, std::ios::beg);
    bool fail = file.write(reinterpret_cast<const char*>(src),
        size * diskParameter.bytesPerSector).fail();
    if (!fail)
        fail = file.tellp() == -1;
    file.flush();
    diskOperationMutex.unlock();

    if (fail)
        return OPENFSL_ERROR_DISK;

    return 0;
}


#endif  // OPENFSL_FBD_DISABLED
