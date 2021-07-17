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

using namespace std;
using namespace openFSL;

int readDisk(DiskDevice* dd, uint8_t* dest, size_t lba, size_t size);
int writeDisk(Sector* src, size_t lba);
int openDisk();
int closeDisk();

fstream disk;

void hexdump(uint8_t* p, size_t offset, size_t len)
{
    ios init(NULL);
    init.copyfmt(cout);
    
    size_t address = 0;
    size_t row = 0;
 
    std::cout << std::hex << std::setfill('0');
    while (1) {
        if (address >= len) break;
        size_t nread = ((len - address) > 16) ? 16 : (len - address);
        
        // Show the address
        std::cout << std::setw(8) << address + offset;
 
        // Show the hex codes
        for (size_t i = 0; i < 16; i++)
        {
            if (i % 8 == 0) std::cout << ' ';
            if (i < nread)
                    std::cout << ' ' << std::setw(2) << (int)p[16 * row + i + offset];
            else
                std::cout << "   ";
        }
 
        // Show printable characters
        std::cout << "  ";
        for (size_t i = 0; i < nread; i++)
        {
            uint8_t ch = p[16 * row + i + offset];
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
    DiskDevice* dd = new DiskDevice();
    dd->read = readDisk;
    dd->write = writeDisk;
    dd->open = openDisk;
    dd->close = closeDisk;
    
    dd->initialize();
    
    int result = 0;
	
    cout << "===========================\n";
    cout << " DISKDEEVICE TEST\n";
    cout << "===========================\n";
    
    for (uint8_t i = 0; i < 4; i++)
    {
        cout << "Reading sector " << (int)i << "...\n";
        Sector sector(1, dd->getBytespersector());
        
        if (dd->readDisk((uint8_t*)sector.getData(), i, 1))
            return 1;
        
        if (((uint8_t*)sector.getData())[3] != i + 1)
        {
            result++;
            cout << (int)((uint8_t*)sector.getData())[3] << " != " << (int)i + 1 << "Fail\n";
        }
        else
            cout << "Success\n";
        
        hexdump((uint8_t*)sector.getData(), 0, 32);
    }
    
    return result;
}

int openDisk()
{
    disk.open("base_test.img", ios::in | ios::out | ios::binary | ios::ate);
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
    if (disk.read((char*)dest, size * dd->getBytespersector()))
        return 1;
    
    return disk.tellg() == -1 ? 1 : 0;
}

int writeDisk(Sector* src, size_t lba)
{
    return 1;
}