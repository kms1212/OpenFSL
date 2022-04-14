/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/iso9660/fs_iso9660.h"

openfsl::ISO9660::ISO9660(openfsl::BlockDevice* bd_,
    const std::string startPath_,
    const std::string pathSeparator_,
    const std::string rootSymbol_) :
        FileSystem(bd_, startPath_, pathSeparator_, rootSymbol_) { }

openfsl::ISO9660::~ISO9660() {}
