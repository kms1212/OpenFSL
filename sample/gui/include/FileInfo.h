/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_FILEINFO_H_
#define SAMPLE_FILEINFO_H_

#include <openfsl/openfsl.h>

enum class FileType {
    File,
    Directory
};

typedef struct FileInfo {
    std::string fileName;
    openfsl::Time fileCreateTime;
    openfsl::Time fileAccessTime;
    openfsl::Time fileModTime;
    size_t fileSize;
    FileType fileType;
} FileInfo;

#endif  // SAMPLE_FILEINFO_H_
