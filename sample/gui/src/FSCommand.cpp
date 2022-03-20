/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "FSCommand.h"

FSCommand::FSCommand(openfsl::FileBlockDevice* fbd) {
    this->fbd = fbd;
}

FSCommand::~FSCommand() {}
