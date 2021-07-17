/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/sector.h"

using namespace openFSL;

Sector::Sector(size_t sector_count_, size_t bytespersector) {
    // Calculate total size of sector data buffer
    size_t bufsize = sector_count_ * bytespersector;
    sector_count = sector_count_;
    
    // Allocate memory
    data = new uint8_t[bufsize];
}

size_t Sector::getSectorCount() {
    return sector_count;
}

void* Sector::getData() {
    return (void*)data;
}

Sector::~Sector() {
    /* Free memory */
    delete data;
}