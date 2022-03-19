/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef OPENFSL_OPENFSL_H_
#define OPENFSL_OPENFSL_H_

#ifndef __cplusplus
#error C++ compiler required.
#endif

#include "openfsl/version.h"

#include "openfsl/iodevice.h"
#include "openfsl/blockdevice.h"
#include "openfsl/fileblockdevice.h"
#include "openfsl/chardevice.h"
#include "openfsl/memchardevice.h"
#include "openfsl/memblockdevice.h"

#include "openfsl/bootcode.h"

#include "openfsl/container.h"
#include "openfsl/sector.h"

#include "openfsl/detectstruct.h"
#include "openfsl/mbr.h"
#include "openfsl/gpt.h"

#include "openfsl/file.h"
#include "openfsl/filesystem.h"

#include "openfsl/endian.h"
#include "openfsl/fsltime.h"
#include "openfsl/fslutils.h"

#include "openfsl/macros.h"
#include "openfsl/symbols.h"
#include "openfsl/types.h"
#include "openfsl/error.h"

#endif  // OPENFSL_OPENFSL_H_
