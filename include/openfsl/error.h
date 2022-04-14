/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_ERROR_H_
#define OPENFSL_ERROR_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include <cstring>

#include <string>

#include "openfsl/macros.h"
#include "openfsl/symbols.h"

#if ERROR_T_EXIST == FALSE
typedef int error_t;
#endif

// Disk error
#define OPENFSL_SUCCESS                       0
// Disk error
#define OPENFSL_ERROR_DISK                    1
// Method not assigned error
#define OPENFSL_ERROR_METHOD_NOT_ASSIGNED     2
// Invalid signature error
#define OPENFSL_ERROR_INVALID_SIGNATURE       3
// File or directory not exist error
#define OPENFSL_ERROR_NO_SUCH_FILE_OR_DIR     4
// File or directory exists error
#define OPENFSL_ERROR_FILE_OR_DIR_EXISTS      5
// File name is invalid error
#define OPENFSL_ERROR_INVALID_FILE_NAME       6
// Too large entry list error
#define OPENFSL_ERROR_TOO_LARGE_ENTRY_LIST    7
// Bad cluster error
#define OPENFSL_ERROR_BAD_CLUSTER             8
// No permission error
#define OPENFSL_ERROR_OPERATION_NOT_PERMITTED 9
// Cluster allocation error
#define OPENFSL_ERROR_CANNOT_ALLOCATE_CLUSTER 10
// Cluster access error
#define OPENFSL_ERROR_CANNOT_ACCESS_CLUSTER   11
// Invalid access
#define OPENFSL_ERROR_INVALID_ACCESS          12
// Too large LBA to convert to CHS
#define OPENFSL_ERROR_CANNOT_CONVERT_TO_CHS   13
// Volume label too long
#define OPENFSL_ERROR_TOO_LONG_VOLUME_LABEL   14
// Floppy not supported
#define OPENFSL_ERROR_FLOPPY_NOT_SUPPORTED    15
// Invalid format options
#define OPENFSL_ERROR_INVALID_FORMAT_OPTIONS  16
// Volume size is too big
#define OPENFSL_ERROR_TOO_LARGE_VOLUME_SIZE   17
// Invalid cache
#define OPENFSL_ERROR_INVALID_CACHE           18
// Not implemented
#define OPENFSL_ERROR_NOT_IMPLEMENTED         19
// Operation disabled
#define OPENFSL_ERROR_OPERATION_DISABLED      20
// Directory is not empty
#define OPENFSL_ERROR_DIRECTORY_NOT_EMPTY     21
// Regex syntax is not valid
#define OPENFSL_ERROR_INVALID_REGEX           22
// Invalid sector size
#define OPENFSL_ERROR_INVALID_SECTOR_SIZE     23
// File size is too big
#define OPENFSL_ERROR_TOO_LARGE_FILE_SIZE     24
// Selected is not a directory
#define OPENFSL_ERROR_NOT_A_DIRECTORY         25
// Selected is not a file
#define OPENFSL_ERROR_NOT_A_FILE              26

namespace openfsl {
extern const __maybe_unused char *__error_str[];

size_t geterrorlen(const error_t errnum);
char* geterrorstr(char* errstr, const error_t errnum);
std::string geterrorstr(const error_t errnum);
}  // namespace openfsl

#endif  // OPENFSL_ERROR_H_
