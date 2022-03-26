/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "FAT32FileCommand.h"

FAT32FileCommand::FAT32FileCommand(FAT32Command* fs, std::string path, openfsl::FSL_OpenMode mode) {
    this->fs = fs;
    file = new openfsl::FAT32::FILE(fs->getFileSystem());
}

FAT32FileCommand::~FAT32FileCommand() {
    delete file;
}

error_t FAT32FileCommand::read(void* buf, const size_t bs, const size_t count) {

}

error_t FAT32FileCommand::write(const void* buffer, const size_t bs, const size_t count) {

}

error_t FAT32FileCommand::seekp(const size_t location, const openfsl::FSL_SeekMode whence) {

}

error_t FAT32FileCommand::seekg(const size_t location, const openfsl::FSL_SeekMode whence) {

}

error_t FAT32FileCommand::tellp() {

}

error_t FAT32FileCommand::tellg() {

}

error_t FAT32FileCommand::flush() {

}
