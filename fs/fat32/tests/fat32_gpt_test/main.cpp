/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <cstdlib>
#include <cstring>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <codecvt>
#include <bitset>

#include "openfsl/fslutils.h"
#include "openfsl/fileblockdevice.h"
#include "openfsl/sector.h"
#include "openfsl/gpt.h"
#include "openfsl/filesystem.h"
#include "openfsl/fat32/fs_fat32.h"

int readCount = 0;
int writeCount = 0;

size_t split(const std::string &txt, std::vector<std::string>* strs, char ch);
void hexdump(uint8_t* p, size_t offset, size_t len);

std::fstream disk;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " [image_file] [test_name]";
        return 1;
    }

    std::string imageFileName = std::string(argv[1]);
    std::string testName = std::string(argv[2]);

    disk.open(imageFileName, std::ios::in | std::ios::out | std::ios::binary);

    openfsl::FileBlockDevice bd;

    std::cout << "Reading image file parameter...\n";
    std::fstream diskInfo;
    diskInfo.open(imageFileName + ".info", std::ios::in);
    if (!diskInfo.fail()) {
        std::string line;
        openfsl::BlockDevice::DiskParameter parameter;

        while (std::getline(diskInfo, line)) {
            std::cout << line << "\n";

            std::vector<std::string> value;

            split(line, &value, ' ');
            if (value[0] == "SectorPerTrack") {
                parameter.sectorPerTrack = stoi(value[1]);
            } else if (value[0] == "HeadPerCylinder") {
                parameter.headPerCylinder = stoi(value[1]);
            } else if (value[0] == "BytesPerSector") {
                parameter.bytesPerSector = stoi(value[1]);
            }
        }

        bd.setDiskParameter(parameter);

        diskInfo.close();
    } else {
        std::cout << "Fail to read disk parameter file. ";
        std::cout << "Default values are will be used.\n";
    }

    bd.initialize(imageFileName, openfsl::FileBlockDevice::OpenMode::RW);

    openfsl::GPT gpt(&bd);

    error_t result = gpt.initialize();
    if (result) {
        disk.close();
        return result;
    }

    openfsl::FAT32 fat32(&bd, "", "\\/");

    std::vector<openfsl::GPT::PartitionInfo> partitionInfo;
    result = gpt.getPartitionInfo(&partitionInfo);
    if (result)
        return result;

    fat32.setFsLBAOffset(partitionInfo[0].partOffset);

    result = fat32.initialize();
    if (result) {
        disk.close();
        return result;
    }

    fat32.enableCache(512);

    std::vector<openfsl::FAT32::FileInfo> buf;
    fat32.listDirectory(&buf);

    for (size_t i = 0; i < buf.size(); i++) {
        std::cout << i << ": " << buf[i].fileName << std::endl;
    }

    /////////////////////////////////////////////
    // CHDIR TEST
    /////////////////////////////////////////////
    std::cout << "===========================\n";
    std::cout << " CHDIR TEST\n";
    std::cout << "===========================\n";

    std::vector<std::string> chdirChecklist;  // Checklist
    chdirChecklist.push_back("::/.");
    chdirChecklist.push_back("::/..");
    chdirChecklist.push_back("::/directory1");
    chdirChecklist.push_back("directory2");
    chdirChecklist.push_back("::");
    chdirChecklist.push_back("directory1/directory2");
    chdirChecklist.push_back("../..");
    chdirChecklist.push_back("directory1/directory2/directory3");

    for (size_t i = 0; i < chdirChecklist.size(); i++) {
        fat32.changeDirectory(chdirChecklist.at(i));
        std::cout << "chdir(\"" << chdirChecklist.at(i) << "\") -> "
             << fat32.getPath() << "\n";
    }

    std::cout << "Total read count: " << readCount << std::endl;
    std::cout << "Total write count: " << writeCount << std::endl;

    disk.close();

    return result;
}

void hexdump(uint8_t* p, size_t offset, size_t len) {
    std::ios init(nullptr);
    init.copyfmt(std::cout);

    size_t address = 0;
    size_t row = 0;

    std::cout << std::hex << std::setfill('0');
    while (1) {
        if (address >= len) break;
        size_t nread = ((len - address) > 16) ? 16 : (len - address);

        // Show the address
        std::cout << std::setw(8) << address + offset;

        // Show the hex codes
        for (size_t i = 0; i < 16; i++) {
            if (i % 8 == 0) std::cout << ' ';
            if (i < nread)
                std::cout << ' ' << std::setw(2) <<
                    static_cast<int>(p[16 * row + i + offset]);
            else
                std::cout << "   ";
        }

        // Show printable characters
        std::cout << "  ";
        for (size_t i = 0; i < nread; i++) {
            uint8_t ch = p[16 * row + i + offset];
            if (ch < 32 || ch > 125) std::cout << '.';
            else
                std::cout << ch;
        }

        std::cout << "\n";
        address += 16;
        row++;
    }
    std::cout.copyfmt(init);
}

size_t split(const std::string& txt,
    std::vector<std::string>* strs, char ch) {
    std::string temp = txt;
    size_t pos = temp.find(ch);
    size_t initialPos = 0;
    strs->clear();

    // Decompose statement
    while (pos != std::string::npos) {
        if (temp.at(pos - 1) != '\\') {
            strs->push_back(temp.substr(initialPos, pos - initialPos));
            initialPos = pos + 1;
        } else {
            temp.erase(temp.begin() + pos - 1);
        }

        pos = temp.find(ch, pos + 1);
    }

    // Add the last one
    strs->push_back(temp.substr(initialPos,
        std::min(pos, temp.size()) - initialPos + 1));

    return strs->size();
}
