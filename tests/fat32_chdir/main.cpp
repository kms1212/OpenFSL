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

int main(int argc, char** argv) {
    fat32 = new FS_FAT32(NULL, "\\/");
    
    fat32->getDiskDevice()->read = readDisk;
    fat32->getDiskDevice()->write = writeDisk;
    fat32->getDiskDevice()->open = openDisk;
    fat32->getDiskDevice()->close = closeDisk;
    
    int result = fat32->initialize();
    if (result)
        return result;
    
    result += fat32->chdir("::");
    cout << result << endl;
    result += fat32->chdir("::/.");
    cout << result << endl;
    result += fat32->chdir("::/..");
    cout << result << endl;
    result += fat32->chdir("::/directory1/..");
    cout << result << endl;
    result += fat32->chdir("::/directory1/subdir1");
    cout << result << endl;
    result += fat32->chdir("../..");
    cout << result << endl;
    result += fat32->chdir("::/directory9/..");
    cout << result << endl;

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