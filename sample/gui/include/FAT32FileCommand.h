/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_FILECOMMAND_H_
#define SAMPLE_FILECOMMAND_H_

#include <openfsl/openfsl.h>

#include "FAT32Command.h"
#include "FileInfo.h"

class FAT32FileCommand {
 private:
    FAT32Command* fs;
    openfsl::FAT32::FILE* file;

 public:
    FAT32FileCommand(FAT32Command* fs, std::string path, openfsl::FSL_OpenMode mode);

    ~FAT32FileCommand();

    error_t read(void* buf, const size_t bs, const size_t count);
    error_t write(const void* buffer, const size_t bs, const size_t count);
    error_t seekp(const size_t location, const openfsl::FSL_SeekMode whence);
    error_t seekg(const size_t location, const openfsl::FSL_SeekMode whence);
    error_t tellp();
    error_t tellg();
    error_t flush();

};

#endif  // SAMPLE_FSCOMMAND_H_
