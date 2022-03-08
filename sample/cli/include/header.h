/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef SAMPLE_HEADER_H_
#define SAMPLE_HEADER_H_

#include <openfsl/openfsl.h>

#include <cstdlib>
#include <cstring>
#include <csignal>
#include <clocale>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <codecvt>
#include <map>

size_t split(const std::string &txt, std::vector<std::string>* strs, char ch);
void hexdump(const uint8_t* p, size_t offset, size_t len);

extern std::map<std::string, bool> settings;

#ifdef FAT32_BUILD
int fat32shell(openfsl::FileBlockDevice* fbd,
    openfsl::DiskStructure diskStructure, size_t selectIndex);
#endif
#ifdef NTFS_BUILD
int ntfsshell(openfsl::FileBlockDevice* fbd,
    openfsl::DiskStructure diskStructure, size_t selectIndex);
#endif
#ifdef ISO9660_BUILD
int cdfsshell(openfsl::FileBlockDevice* fbd);
#endif

#endif  // SAMPLE_HEADER_H_
