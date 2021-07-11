/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <codecvt> 
#include <bitset> 
#include <stdlib.h>
#include <string.h>

#include "openfsl/fslservices.h"
#include "openfsl/diskdevice.h"
#include "openfsl/sector.h"
#include "openfsl/fs_fat32.h"

using namespace std;
using namespace openFSL;

int readDisk(DiskDevice* dd, uint8_t* dest, size_t lba, size_t size);
int writeDisk(Sector* src, size_t lba);
int openDisk();
int closeDisk();

fstream disk;
FS_FAT32* fat32;

void hexdump(uint8_t* p, int offset, int len)
{
    ios init(NULL);
    init.copyfmt(cout);
    
    int address = 0;
    int row = 0;
 
    std::cout << std::hex << std::setfill('0');
    while (1) {
        if (address >= len) break;
        int nread = ((len - address) > 16) ? 16 : (len - address);
        
        // Show the address
        std::cout << std::setw(8) << address + offset;
 
        // Show the hex codes
        for (int i = 0; i < 16; i++)
        {
            if (i % 8 == 0) std::cout << ' ';
            if (i < nread)
                    std::cout << ' ' << std::setw(2) << (int)p[16 * row + i + offset];
            else
                std::cout << "   ";
        }
 
        // Show printable characters
        std::cout << "  ";
        for (int i = 0; i < nread; i++)
        {
            char ch = p[16 * row + i + offset];
            if (ch < 32 || ch > 125) std::cout << '.';
            else std::cout << ch;
        }
 
        std::cout << "\n";
        address += 16;
        row++;
    }
    cout.copyfmt(init);
}

int main(int argc, char** argv) {
    fat32 = new FS_FAT32(NULL, "\\/");
    
    fat32->getDiskDevice()->read = readDisk;
    fat32->getDiskDevice()->write = writeDisk;
    fat32->getDiskDevice()->open = openDisk;
    fat32->getDiskDevice()->close = closeDisk;
    
    int result = fat32->initialize();
    
    FSL_File<FS_FAT32>* file = fat32->openFile("file1.txt", FSL_OpenMode::read);
    
    if (file == NULL)
    {
        result++;
    }
    else {
        char* buf = new char[file->getFileController()->getFileInfo().fileSize - 14]();
		file->seek(14);
        file->read((uint8_t*)buf, 1, file->getFileController()->getFileInfo().fileSize);
        buf[file->getFileController()->getFileInfo().fileSize - 14] = 0;
        
        cout << file->getFileController()->getFileInfo().fileSize << endl;
        
        string buf_s(buf);
        string comp_s("OpenFSL\n");
        cout << buf_s << endl;
        
        if (buf_s != comp_s) {
            result++;
        }
    }
    
    fat32->closeFile(file);

    delete fat32;
    
    return result;
}

int openDisk()
{
    disk.open("fat32.img", ios::in | ios::out | ios::binary | ios::ate);
    return disk.fail();
}

int closeDisk()
{
    disk.close();
    return 0;
}

int readDisk(DiskDevice* dd, uint8_t* dest, size_t lba, size_t size)
{
    disk.seekg(lba * dd->getBytespersector(), ios::beg);
    disk.read((char*)dest, size * dd->getBytespersector());
    
    return disk.tellg() == -1 ? 1 : 0;
}

int writeDisk(Sector* src, size_t lba)
{
    disk.seekp(lba * fat32->getDiskDevice()->getBytespersector(), ios::beg);
    disk.write((char*)src->getData(), src->getSectorCount() * fat32->getDiskDevice()->getBytespersector());
    return disk.tellp() == -1 ? 1 : 0;
}