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

int main(int argc, char** argv) {
    fat32 = new FS_FAT32(NULL, FAT32_OPTION_NONE, "\\/");
    
    fat32->getDiskDevice()->readDisk = readDisk;
    fat32->getDiskDevice()->writeDisk = writeDisk;
    fat32->getDiskDevice()->openDisk = openDisk;
    fat32->getDiskDevice()->closeDisk = closeDisk;
    
    fat32->initialize();
    
    uint32_t result = fat32->getState();
    if (result)
        return result;
    
    result = 0;
    result += fat32->chdir("::/.");
    result += fat32->chdir("::/..");
    result += fat32->chdir("::/directory1/..");
    result += fat32->chdir("::/directory1/subdir1");
    result += fat32->chdir("../..");
    result += fat32->chdir("::/directory9/..");

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