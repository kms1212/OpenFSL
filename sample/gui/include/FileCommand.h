/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_FILECOMMAND_H_
#define SAMPLE_FILECOMMAND_H_

#include <openfsl/openfsl.h>

#include "FileInfo.h"
#include "FSCommand.h"

class FileCommand {
protected:
    FSCommand* fs;

public:
    FileCommand(FSCommand* fs, std::string path, openfsl::FSL_OpenMode mode);

    virtual ~FileCommand();

    virtual error_t read(void* buf, const size_t bs, const size_t count) = 0;
    virtual error_t write(const void* buffer, const size_t bs, const size_t count) = 0;
    virtual error_t seekp(const size_t location, const openfsl::FSL_SeekMode whence) = 0;
    virtual error_t seekg(const size_t location, const openfsl::FSL_SeekMode whence) = 0;
    virtual error_t tellp() = 0;
    virtual error_t tellg() = 0;
    virtual error_t flush() = 0;

};

#endif  // SAMPLE_FSCOMMAND_H_
