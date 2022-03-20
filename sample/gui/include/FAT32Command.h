/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_FAT32COMMAND_H_
#define SAMPLE_FAT32COMMAND_H_

#ifdef FAT32_BUILD

#include <openfsl/openfsl.h>
#include <openfsl/fat32/fs_fat32.h>

#include "FSCommand.h"
#include "FileInfo.h"

class FAT32Command : public FSCommand {
private:
    openfsl::FAT32* fat32;

public:
    FAT32Command(openfsl::FileBlockDevice* fbd) : FSCommand(fbd) {}
    ~FAT32Command() { Deinitialize(); }

    error_t Initialize(openfsl::DiskStructure diskStructure, size_t selectIndex);
    error_t GetVolumeInfo();
    error_t SetVolumeName();
    error_t GetPathSeparator(std::string* separator);
    error_t ListDirectoryChild(std::vector<FileInfo>* outList, const std::string path = "");
    error_t GetCurrentDirectory(std::string* outPath);
    error_t NavigateDirectory(const std::string dest);
    error_t CreateDirectory();
    error_t RemoveDirectory();
    error_t OpenFile();
    error_t CloseFile();
    error_t CreateFile();
    error_t RemoveFile();
    error_t GetFileInformation();
    error_t Search();
    void Deinitialize();
};

#endif  // FAT32_BUILD

#endif  // SAMPLE_FAT32COMMAND_H_
