/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
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
#define OPENFSL_SUCCESS         0
// Disk error
#define OPENFSL_ERROR_DISK      1
// No method is assigned error
#define OPENFSL_ERROR_NOMETHOD  2
// Invalid signature error
#define OPENFSL_ERROR_SIGN      3
// File or directory not exist error
#define OPENFSL_ERROR_NOTEXIST  4
// File or directory already exists error
#define OPENFSL_ERROR_EXIST     5
// File name is invalid error
#define OPENFSL_ERROR_INVNAME   6
// Too large entry list error
#define OPENFSL_ERROR_LARGELS   7
// Bad cluster error
#define OPENFSL_ERROR_BADCLUS   8
// No permission error
#define OPENFSL_ERROR_NOPERM    9
// Cluster allocation error
#define OPENFSL_ERROR_ALLOC     10
// Cluster access error
#define OPENFSL_ERROR_CACCESS   11
// Invalid access
#define OPENFSL_ERROR_INVACC    12
// Too large LBA to convert to CHS
#define OPENFSL_ERROR_LARGELBA  13
// Volume label too long
#define OPENFSL_ERROR_VLABLONG  14
// Floppy not supported
#define OPENFSL_ERROR_NOFLOPPY  15
// Invalid format options
#define OPENFSL_ERROR_FRMATOPT  16
// Volume size is too big
#define OPENFSL_ERROR_SZTOOBIG  17
// Invalid cache
#define OPENFSL_ERROR_CACHE     18
// Not implemented
#define OPENFSL_ERROR_NOTIMPL   19
// Operation disabled
#define OPENFSL_ERROR_DISABLED  20
// Directory is not empty
#define OPENFSL_ERROR_DIRNEMPT  21
// Regex syntax is not valid
#define OPENFSL_ERROR_REGEX     22
// Invalid sector size
#define OPENFSL_ERROR_SECTSIZE  23
// Invalid GPT
#define OPENFSL_ERROR_INVALGPT  24

namespace openfsl {
extern const __maybe_unused char *__error_str[];

size_t geterrorlen(const error_t errnum);
char* geterrorstr(char* errstr, const error_t errnum);
std::string geterrorstr(const error_t errnum);
}  // namespace openfsl

#endif  // OPENFSL_ERROR_H_
