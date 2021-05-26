/* Copyright (c) 2020. kms1212(권민수)
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
int openDisk();
int closeDisk();

fstream disk;
FS_FAT32* fat32;

int main(int argc, char** argv) {
	fat32 = new FS_FAT32(new DiskDevice(), FAT32_OPTION_NONE, "\\/");
	
	fat32->getDiskDevice()->readDisk = readDisk;
	fat32->getDiskDevice()->openDisk = openDisk;
	fat32->getDiskDevice()->closeDisk = closeDisk;
	
	fat32->initialize();
	
	uint32_t errorState = fat32->getState();

	delete fat32->getDiskDevice();
	
	return errorState;
}

int openDisk()
{
	disk.open("../fat32.img", ios::in | ios::binary);
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
	return 0;
}