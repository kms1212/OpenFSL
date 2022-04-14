/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_FAT32FILECOMMAND_H_
#define SAMPLE_FAT32FILECOMMAND_H_

#include <openfsl/openfsl.h>

#include "FAT32Command.h"
#include "FileCommand.h"
#include "FileInfo.h"

class FAT32FileCommand : public FileCommand {
 private:
    FAT32Command* fs;
    openfsl::FAT32::FILE* file;

 public:
    FAT32FileCommand(FAT32Command* fs, const std::string path, openfsl::FSL_OpenMode mode);

    ~FAT32FileCommand();

    error_t read(void* buf, const size_t bs, const size_t count);
    error_t write(const void* buffer, const size_t bs, const size_t count);
    error_t seekp(const size_t location, const openfsl::FSL_SeekMode whence);
    error_t seekg(const size_t location, const openfsl::FSL_SeekMode whence);
    size_t tellp();
    size_t tellg();
    error_t flush();

};

#endif  // SAMPLE_FAT32FILECOMMAND_H_
