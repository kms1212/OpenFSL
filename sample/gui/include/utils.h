/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_UTILS_H_
#define SAMPLE_UTILS_H_

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

#include "openfsl/openfsl.h"

size_t split(const std::string &txt, std::vector<std::string>* strs, char ch);
void hexdump(const uint8_t* p, size_t offset, size_t len);

#endif  // SAMPLE_UTILS_H_
