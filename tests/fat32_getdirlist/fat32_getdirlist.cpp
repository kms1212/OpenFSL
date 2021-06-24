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
#include "openfsl/vint.h"
#include "openfsl/fs/fs_fat32.h"

using namespace std;
using namespace openFSL;

int readDisk(Sector* dest, vint_arch lba, vint_arch size);
int writeDisk(Sector* src, vint_arch lba);
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
    int nread = 0;
 
    std::cout << std::hex << std::setfill('0');
    while (1) {
        if (address >= len) break;
        nread = ((len - address) > 16) ? 16 : (len - address);
        
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
    fat32 = new FS_FAT32(NULL, FAT32_OPTION_NONE, "\\/");
    
    fat32->getDiskDevice()->readDisk = readDisk;
    fat32->getDiskDevice()->writeDisk = writeDisk;
    fat32->getDiskDevice()->openDisk = openDisk;
    fat32->getDiskDevice()->closeDisk = closeDisk;
    
    fat32->initialize();
    
    fat32->chdir("::");
    
    vector<string> list;
    list.push_back("DIRECTORY1");
    list.push_back("DIRECTORY2");
    list.push_back("DIRECTORY3");
    list.push_back("DIRECTORY4");
    list.push_back("DIRECTORY5");
    list.push_back("DIRECTORY6");
    list.push_back("DIRECTORY7");
    list.push_back("DIRECTORY8");
    list.push_back("FILE1.TXT");
    list.push_back("FILE2.TXT");
    list.push_back("FILE3.TXT");
    list.push_back("FILE4.TXT");
    list.push_back("FILE5.TXT");
    list.push_back("FILE6.TXT");
    list.push_back("FILE7.TXT");
    list.push_back("FILE8.TXT");
    list.push_back("LFNFILENAME1.TXT");
    list.push_back("LFNFILENAME2.TXT");
    list.push_back("LFNFILENAME3.TXT");
    list.push_back("LFNFILENAME4.TXT");
    list.push_back("DIRECTORY9");
    
    int result = fat32->getChildCount();
    int ret = 0;
    
    FAT32_fileInfo* buf = new FAT32_fileInfo[fat32->getChildCount()];
    fat32->getDirList(buf);
    if (result != 24)
        ret = -1;
    
    string filename;
    for (uint32_t i = 0; i < fat32->getChildCount(); i++)
    {
        cout << buf[i].fileName << "\n";
        filename = buf[i].fileName;
        
        std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);
        if (find(list.begin(), list.end(), filename) != list.end()) {
            result--;
        }
    }
    
    delete[] buf;

    delete fat32;
    
    return ret + result;
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

int readDisk(Sector* dest, vint_arch lba, vint_arch size)
{
    disk.seekg(lba * fat32->getDiskDevice()->getBytespersector(), ios::beg);
    disk.read((char*)dest->getData(), size * fat32->getDiskDevice()->getBytespersector());
    return disk.tellg() == -1 ? 1 : 0;
}

int writeDisk(Sector* src, vint_arch lba)
{
    disk.seekp(lba * fat32->getDiskDevice()->getBytespersector(), ios::beg);
    disk.write((char*)src->getData(), src->getSectorCount() * fat32->getDiskDevice()->getBytespersector());
    return disk.tellp() == -1 ? 1 : 0;
}