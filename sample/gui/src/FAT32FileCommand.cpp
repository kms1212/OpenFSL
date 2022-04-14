/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "FAT32FileCommand.h"

FAT32FileCommand::FAT32FileCommand(FAT32Command* fs, const std::string path, openfsl::FSL_OpenMode mode) : FileCommand(fs, path, mode) {
    this->fs = fs;
    file = new openfsl::FAT32::FILE(fs->getFileSystem());
    file->open(path, mode);
}

FAT32FileCommand::~FAT32FileCommand() {
    file->close();
    delete file;
}

error_t FAT32FileCommand::read(void* buf, const size_t bs, const size_t count) {
    return file->read(buf, bs, count);
}

error_t FAT32FileCommand::write(const void* buf, const size_t bs, const size_t count) {
    return file->write(buf, bs, count);
}

error_t FAT32FileCommand::seekp(const size_t location, const openfsl::FSL_SeekMode whence) {
    return file->seekp(location, whence);
}

error_t FAT32FileCommand::seekg(const size_t location, const openfsl::FSL_SeekMode whence) {
    return file->seekg(location, whence);
}

size_t FAT32FileCommand::tellp() {
    return file->tellp();
}

size_t FAT32FileCommand::tellg() {
    return file->tellg();
}

error_t FAT32FileCommand::flush() {
    return file->flush();
}
