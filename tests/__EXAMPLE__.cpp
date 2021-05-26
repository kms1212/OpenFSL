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

void hexdump(uint8_t* p, int offset, int len);
size_t split(const std::string &txt, std::vector<std::string> &strs, char ch);
int readDisk(Sector* dest, vint_arch lba, vint_arch size);
int openDisk();
int closeDisk();
int print(const char* str);

fstream disk;
char* filename;
FS_FAT32* fat32;

int main(int argc, char** argv) {
	if (argc < 2)
	{
		cerr << argv[0] << ": Usage: " << argv[0] << " <File Name> [-t]\n";
		return 1;
	}
	filename = argv[1];
	
	if (argc == 3 && argv[2] == "-t")
	{
		
	}
	else if (argc == 2)
	{
		
		fat32 = new FS_FAT32(new DiskDevice(), FAT32_OPTION_NONE, "\\/");
	
		fat32->getDiskDevice()->readDisk = readDisk;
		fat32->getDiskDevice()->openDisk = openDisk;
		fat32->getDiskDevice()->closeDisk = closeDisk;
		
		fat32->initialize();
		
		FAT32_fileInfo* buf = new FAT32_fileInfo[fat32->getChildCount("::")];
		fat32->getDirList(buf, "::");
		
		std::string command;
		
		cout << "type \"exit\" to exit\n";
		while(true)
		{
			vector<string> cmd;
			
			cout << fat32->getPath() << "> ";
			getline(cin, command);
			split(command, cmd, ' ');
			
			if (cmd[0] == "exit") {
				break;
			}
			else if (cmd[0] == "dir") {
				if (buf != NULL)
					delete[] buf;
				buf = new FAT32_fileInfo[fat32->getChildCount()];
				fat32->getDirList(buf);
				for (int i = 0; i < fat32->getChildCount(); i++)
				{
					cout << i << " " << buf[i].fileName << " " << (int)buf[i].fileCreateTime.time_month << "-" << (int)buf[i].fileCreateTime.time_day << "-" << (int)buf[i].fileCreateTime.time_year << " " << (int)buf[i].fileCreateTime.time_hour << ":" << (int)buf[i].fileCreateTime.time_min << ":" << (int)buf[i].fileCreateTime.time_sec << "\n";
				}
			}
			else if (cmd[0] == "cd") {
				if (fat32->chdir(cmd[1]))
					cout << "Error\n";
			}
			else if (cmd[0] == "info") {
				FAT32_fileInfo fileInfo = fat32->getFileInformation(cmd[1]);
				if (fileInfo.fileName != "") {
					cout << fileInfo.fileName << " " << (int)fileInfo.fileCreateTime.time_month << "-" << (int)fileInfo.fileCreateTime.time_day << "-" << (int)fileInfo.fileCreateTime.time_year << " " << (int)fileInfo.fileCreateTime.time_hour << ":" << (int)fileInfo.fileCreateTime.time_min << ":" << (int)fileInfo.fileCreateTime.time_sec << "\n";
				}
			}
		}

		delete fat32->getDiskDevice();
		
		return 0;
	}
}

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

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

int openDisk()
{
	disk.open(filename, ios::in | ios::binary);
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

int print(const char* str)
{
	cout << str;
	return 0;
}