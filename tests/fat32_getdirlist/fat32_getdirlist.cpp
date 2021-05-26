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
	if (result != 21)
		ret = -1;
	
	string filename;
	for (int i = 0; i < fat32->getChildCount(); i++)
	{
		filename = buf[i].fileName;
		std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);
		if (find(list.begin(), list.end(), filename) != list.end()) {
			result--;
		}
	}
	
	delete[] buf;
	delete fat32->getDiskDevice();
	
	return ret + result;
}

int openDisk()
{
	disk.open("fat32.img", ios::in | ios::binary);
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