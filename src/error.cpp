/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/error.h"

const char *openfsl::__error_str[] = {
    "Operation successfully finished.",
    "An error occured from disk.",
    "Method is not assigned.",
    "Invalid file system signature.",
    "No such file or directory.",
    "File already exists.",
    "Invalid file name.",
    "Entry list size too large.",
    "Bad cluster has found.",
    "Operation not permitted.",
    "Cannot allocate cluster.",
    "Cannot access cluster.",
    "Invalid access.",
    "An error occured when converting LBA to CHS.",
    "Volume label is too long.",
    "Floppy disk drive is not supported.",
    "Invalid format options.",
    "Volume size is too big to format.",
    "Cache is invalid.",
    "Operation not implemented.",
    "Disabled operation.",
    "Directory is not empty.",
    "Regex syntax is not valid.",
    "Sector size is not valid.",
    "File size is too big."
};

size_t openfsl::geterrorlen(const int errnum) {
    return std::strlen(__error_str[errnum]) + 1;
}

char* openfsl::geterrorstr(char* errstr, const int errnum) {
    std::memcpy(errstr, __error_str[errnum], geterrorlen(errnum));
    return errstr;
}

std::string openfsl::geterrorstr(const int errnum) {
    return std::string(__error_str[errnum]);
}
