/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/sector.h"

openfsl::Sector::Sector(
    const size_t sector_count_, const size_t bytespersector_) :
    Container(sector_count_ * bytespersector_) {
    sector_count = sector_count_;
    bytespersector = bytespersector_;
}

size_t openfsl::Sector::getSectorCount() {
    return sector_count;
}

size_t openfsl::Sector::getBytespersector() {
    return bytespersector;
}

void* openfsl::Sector::getData() {
    return static_cast<void*>(data);
}

openfsl::Sector::~Sector() {}
