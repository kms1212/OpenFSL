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

#include "openfsl/openfsl.h"

int readDisk(openfsl::BlockDevice* dd, uint8_t* dest,
    const openfsl::lba48_t lba, const openfsl::lba48_t size);
int writeDisk(openfsl::BlockDevice* dd, const uint8_t* src,
    const openfsl::lba48_t lba, const openfsl::lba48_t size);

size_t split(const std::string &txt, std::vector<std::string>* strs, char ch);
void hexdump(const uint8_t* p, size_t offset, size_t len);
openfsl::BlockDevice::DiskParameter getParameterFromFile(std::string fileName);

std::fstream disk;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " [image_file] [test_name]";
        return 1;
    }

    std::string imageFileName = std::string(argv[1]);
    std::string testName = std::string(argv[2]);

    if (testName == "CONTAINER_CONTAINER") {
        /////////////////////////////////////////////
        // CLASS CONTAINER TEST
        /////////////////////////////////////////////

        std::cout << "Testing class Container............ ";
        openfsl::Container containertest(10);  // Container class with size 10
        (reinterpret_cast<char*>(containertest.getData()))[5] = 'A';
        containertest.getDataCast<uint32_t>()[0] = 0x1A2B3C4D;
        if ((containertest.getSize() == 10)) {
            if ((reinterpret_cast<char*>(containertest.getData()))[5] == 'A') {
                if (containertest.getDataCast<uint32_t>()[0] == 0x1A2B3C4D)  {
                    std::cout << "Pass\n";
                    return 0;
                } else {
                    std::cout << "Fail... In getDataCast<>()\n";
                }
            } else {
                std::cout << "Fail........ In operator[]\n";
            }
        } else {
            std::cout << "Fail....... In getSize()\n";
        }
        return 1;
    } else if (testName == "CONTAINER_SECTOR") {
        /////////////////////////////////////////////
        // CLASS SECTOR TEST
        /////////////////////////////////////////////

        std::cout << "Testing class Sector............... ";

        // Sector class with 2 sectors long
        openfsl::Sector sectortest(2, 512);
        if (sectortest.getSize() == 2 * 512) {
            std::cout << "Pass\n";
            return 0;
        } else {
            std::cout << "Fail\n";
            return 1;
        }
    } else if (testName == "UTIL_STRTOK") {
        /////////////////////////////////////////////
        // STRTOK TEST
        /////////////////////////////////////////////

        std::cout << "Testing utility strtok............. ";

        std::vector<std::string> tokens;
        openfsl::fsl_strtokenize("std::string/to/tokenize", &tokens, "/");

        if (tokens[0] == "std::string") {
            std::cout << "Pass\n";
        } else {
            std::cout << "Fail\n";
            return 1;
        }
        std::cout << "tokens[1] == \"to\": ";
        if (tokens[1] == "to") {
            std::cout << "Pass\n";
        } else {
            std::cout << "Fail\n";
            return 1;
        }
        std::cout << "tokens[2] == \"tokenize\": ";
        if (tokens[2] == "tokenize") {
            std::cout << "Pass\n";
        } else {
            std::cout << "Fail\n";
            return 1;
        }

        return 0;
    } else if (testName == "UTIL_CHS") {
        /////////////////////////////////////////////
        // CHS CONVERT TEST
        /////////////////////////////////////////////

        std::cout << "Testing utility strtok............. ";

        openfsl::CHS chs;
        chs.cyl = 5;
        chs.head = 6;
        chs.sect = 4;
        openfsl::lba48_t lba = openfsl::convertCHSToLBA(chs, 63, 16);
        if (lba == 5421) {
            std::cout << "Pass\n";
        } else {
            std::cout << "Fail... In convertCHSToLBA()\n";
            return 1;
        }

        convertLBAToCHS(lba, &chs, 63, 16);
        if ((chs.cyl == 5) && (chs.head == 6) && (chs.sect == 4)) {
            std::cout << "Pass\n";
        } else {
            std::cout << "Fail... In convertLBAToCHS()\n";
            return 1;
        }

        return 0;
    } else if (testName == "BLOCKDEVICE_READ") {
        /////////////////////////////////////////////
        // BLOCKDEVICE READ TEST
        /////////////////////////////////////////////

        std::cout << "Testing disk device read........... ";

        disk.open(imageFileName,
            std::ios::in | std::ios::out | std::ios::binary);

        openfsl::BlockDevice bd;

        openfsl::BlockDevice::IOFunctions ioFunc;
        ioFunc.readSector = readDisk;
        ioFunc.writeSector = writeDisk;

        bd.setDiskParameter(getParameterFromFile(imageFileName + ".info"));
        if (bd.initialize(ioFunc)) {
            std::cout << "Fail... on initialization\n";
            disk.close();
            return 1;
        }

        for (uint8_t i = 0; i < 4; i++) {
            openfsl::Sector sector(1, bd.getDiskParameter().bytesPerSector);

            if (bd.readSector(sector.getDataCast<uint8_t>(), i, 1)) {
                std::cout << "Fail... on sector "
                    << static_cast<int>(i) << "\n";
                disk.close();
                return 1;
            }

            if (sector.getDataCast<uint8_t>()[3] != i + 1) {
                std::cout << "Fail... on sector "
                    << static_cast<int>(i) << "\n";
                disk.close();
                return 1;
            }
        }

        std::cout << "Pass\n";
        disk.close();
        return 0;
    } else if (testName == "BLOCKDEVICE_WRITE") {
        /////////////////////////////////////////////
        // BLOCKDEVICE WRITE TEST
        /////////////////////////////////////////////

        std::cout << "Testing disk device write........... ";

        disk.open(imageFileName,
            std::ios::in | std::ios::out | std::ios::binary);

        openfsl::BlockDevice bd;

        openfsl::BlockDevice::IOFunctions ioFunc;
        ioFunc.readSector = readDisk;
        ioFunc.writeSector = writeDisk;

        bd.setDiskParameter(getParameterFromFile(imageFileName + ".info"));
        if (bd.initialize(ioFunc)) {
            std::cout << "Fail... on initialization\n";
        }

        for (uint8_t i = 0; i < 4; i++) {
            openfsl::Sector sector(1, bd.getDiskParameter().bytesPerSector);

            if (bd.readSector(sector.getDataCast<uint8_t>(), i, 1)) {
                std::cout << "Fail... on sector "
                    << static_cast<int>(i) << "\n";
                disk.close();
                return 1;
            }
            ((sector.getDataCast<uint32_t>())[4]) = 0x12345678;
            if (bd.writeSector(sector.getDataCast<uint8_t>(), i, 1)) {
                std::cout << "Fail... on sector "
                    << static_cast<int>(i) << "\n";
                disk.close();
                return 1;
            }

            if ((sector.getDataCast<uint32_t>())[4] != 0x12345678) {
                std::cout << "Fail... on sector "
                    << static_cast<int>(i) << "\n";
                break;
            }
        }

        std::cout << "Pass\n";
        disk.close();
        return 0;
    } else if (testName == "FILEBLOCKDEVICE_READ") {
        /////////////////////////////////////////////
        // FILEBLOCKDEVICE READ TEST
        /////////////////////////////////////////////

        std::cout << "Testing disk device read........... ";

        openfsl::FileBlockDevice fbd;

        if (fbd.initialize(imageFileName,
            openfsl::FileBlockDevice::OpenMode::RW)) {
            std::cout << "Fail... on initialization\n";
            return 1;
        }

        for (uint8_t i = 0; i < 4; i++) {
            openfsl::Sector sector(1, fbd.getDiskParameter().bytesPerSector);

            if (fbd.readSector(sector.getDataCast<uint8_t>(), i, 1)) {
                std::cout << "Fail... on sector "
                    << static_cast<int>(i) << "\n";
                return 1;
            }

            if (sector.getDataCast<uint8_t>()[3] != i + 1) {
                std::cout << "Fail... on sector "
                    << static_cast<int>(i) << "\n";
                return 1;
            }
        }

        std::cout << "Pass\n";
        return 0;
    } else if (testName == "FILEBLOCKDEVICE_WRITE") {
        /////////////////////////////////////////////
        // FILEBLOCKDEVICE WRITE TEST
        /////////////////////////////////////////////

        std::cout << "Testing disk device write........... ";

        openfsl::FileBlockDevice fbd;

        if (fbd.initialize(imageFileName,
            openfsl::FileBlockDevice::OpenMode::RW)) {
            std::cout << "Fail... on initialization\n";
        }

        for (uint8_t i = 0; i < 4; i++) {
            openfsl::Sector sector(1, fbd.getDiskParameter().bytesPerSector);

            if (fbd.readSector(sector.getDataCast<uint8_t>(), i, 1)) {
                std::cout << "Fail... on sector "
                    << static_cast<int>(i) << "\n";
                return 1;
            }
            ((sector.getDataCast<uint32_t>())[4]) = 0x12345678;
            if (fbd.writeSector(sector.getDataCast<uint8_t>(), i, 1)) {
                std::cout << "Fail... on sector "
                    << static_cast<int>(i) << "\n";
                return 1;
            }

            if ((sector.getDataCast<uint32_t>())[4] != 0x12345678) {
                std::cout << "Fail... on sector "
                    << static_cast<int>(i) << "\n";
                break;
            }
        }

        std::cout << "Pass\n";
        return 0;
    } else if (testName  == "MEMDEVICE") {
        openfsl::MemDevice md;

        uint8_t origData[] = 
            { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
              0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

        uint8_t readData[16];

        md.initialize(16, 8);

        size_t errorCount = 0;

        for (int i = 0; i < 7; i++) {
            md.writeByte(origData, i * 16 + 4, 16);
            md.readByte(readData, i * 16 + 4, 16);

            if (memcmp(origData, readData, 16) != 0) {
                errorCount++;
            }
        }

        if (errorCount) {
            std::cout << "Fail\n";
            return errorCount;
        }

        std::cout << "Pass\n";
        return 0;
    }

    /*
    /////////////////////////////////////////////
    // DISKSTRUCTURE TEST
    /////////////////////////////////////////////
    std::cout << "===========================\n";
    std::cout << " DISKSTRUCTURE TEST\n";
    std::cout << "===========================\n";

    DiskStructure diskStructure;

    detectDiskStructure(&diskStructure, dd);

    std::cout << "Partition table: ";
    */

    return 1;
}

void hexdump(const uint8_t* p, size_t offset, size_t len) {
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
                std::cout << ' ' << std::setw(2)
                    << static_cast<int>(p[16 * row + i + offset]);
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

size_t split(const std::string& txt, std::vector<std::string>* strs, char ch) {
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

int readDisk(openfsl::BlockDevice* dd, uint8_t* dest,
    const openfsl::lba48_t lba, const openfsl::lba48_t size) {
    disk.seekg(lba * dd->getDiskParameter().bytesPerSector, std::ios::beg);
    if (disk.read(reinterpret_cast<char*>(dest),
        size * dd->getDiskParameter().bytesPerSector).fail())
        return 1;

    return disk.tellg() == -1 ? 1 : 0;
}

int writeDisk(openfsl::BlockDevice* dd, const uint8_t* src,
    const openfsl::lba48_t lba, const openfsl::lba48_t size) {
    disk.seekp(lba * dd->getDiskParameter().bytesPerSector, std::ios::beg);
    if (disk.write(reinterpret_cast<const char*>(src),
        size * dd->getDiskParameter().bytesPerSector).fail())
        return 1;
    disk.flush();

    return disk.tellp() == -1 ? 1 : 0;
}

openfsl::BlockDevice::DiskParameter
    getParameterFromFile(std::string fileName) {
    openfsl::BlockDevice::DiskParameter parameter;

    std::fstream diskInfo;
    diskInfo.open(fileName, std::ios::in);
    if (!diskInfo.fail()) {
        std::string line;

        while (getline(diskInfo, line)) {
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

        diskInfo.close();
    }

    return parameter;
}
