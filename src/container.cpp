/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/container.h"

openfsl::Container::Container(const size_t size_) {
    size = size_;

    // Allocate memory
    data = new uint8_t[size];
}

size_t openfsl::Container::getSize() {
    return size;
}

void* openfsl::Container::getData() {
    return data;
}

void* openfsl::Container::getData(size_t offset) {
    return static_cast<void*>(static_cast<uint8_t*>(data) + offset);
}

void* openfsl::Container::operator[](const size_t index) {
    return static_cast<uint8_t*>(data) + index;
}

void* openfsl::Container::operator[](const int index) {
    return static_cast<uint8_t*>(data) + index;
}

openfsl::Container::~Container() {
    delete static_cast<uint8_t*>(data);
}