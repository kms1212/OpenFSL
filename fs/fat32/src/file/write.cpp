/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/utils.h"
#include "openfsl/fsltime.h"

#include "openfsl/fat32/fat32.h"

size_t openfsl::fat32::File::write(
    const void* ptr, const size_t bs, const size_t count) {
    if ((openMode & openfsl::OpenMode::Write) != (openfsl::OpenMode)0) {
        // size_t startPos = writePointer;
        size_t endPos = writePointer + bs * count;
        
        if (endPos > std::numeric_limits<uint32_t>::max())
            return OPENFSL_ERROR_TOO_LARGE_FILE_SIZE;

        size_t bytespersector =
            fileSystem->getIODevice()->getDiskParameter().bytesPerSector;
        size_t sectorpercluster = fileSystem->getBPB()->bpbSectorPerCluster;

        size_t ret = 0;
        error_t result;

        fileOperationMutex.lock();

        if (fileCluster->getClusterList()->size() <
            endPos / bytespersector / sectorpercluster) {
            result = fileCluster->addCluster(
                static_cast<openfsl::fat32::cluster_t>
                (endPos / bytespersector / sectorpercluster));
            if (result)
                return result;
        }

        for (size_t i = 0; i < count; i++) {
            // i: Number of elements actually read
            result =
                fileCluster->write(
                    reinterpret_cast<uint8_t*>(const_cast<void*>(ptr)) +
                        i * bs,
                    writePointer, bs);
            if (result) {
                ret = i;
                break;
            }

            writePointer += bs;
        }

        if (currentFileInfo.fileSize < writePointer)
            currentFileInfo.fileSize = writePointer;

        struct tm timetmp = fsl_localtime(std::time(NULL));
        currentFileInfo.fileModTime = convertToFSLTime(&timetmp);

        fileSystem->setEntryInfo(currentFileInfo.fileName, currentFileInfo);

        fileOperationMutex.unlock();

        return ret;
    }

    return 0;
}
