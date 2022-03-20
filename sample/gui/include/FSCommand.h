/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_FSCOMMAND_H_
#define SAMPLE_FSCOMMAND_H_

#include <openfsl/openfsl.h>

#include "FileInfo.h"

class FSCommand {
protected:
    openfsl::FileBlockDevice* fbd;

public:
    FSCommand(openfsl::FileBlockDevice* fbd);

    virtual ~FSCommand();

    virtual error_t Initialize(const openfsl::DiskStructure diskStructure, const size_t selectIndex) = 0;
    virtual error_t GetVolumeInfo() = 0;
    virtual error_t SetVolumeName() = 0;
    virtual error_t GetPathSeparator(std::string* separator) = 0;
    virtual error_t ListDirectoryChild(std::vector<FileInfo>* outList, const std::string path = "") = 0;
    virtual error_t GetCurrentDirectory(std::string* outPath) = 0;
    virtual error_t NavigateDirectory(const std::string dest) = 0;
    virtual error_t CreateDirectory() = 0;
    virtual error_t RemoveDirectory() = 0;
    virtual error_t OpenFile() = 0;
    virtual error_t CloseFile() = 0;
    virtual error_t CreateFile() = 0;
    virtual error_t RemoveFile() = 0;
    virtual error_t GetFileInformation() = 0;
    virtual error_t Search() = 0;
    virtual void Deinitialize() = 0;
};

#endif  // SAMPLE_FSCOMMAND_H_
