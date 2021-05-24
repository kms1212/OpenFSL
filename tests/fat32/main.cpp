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
#include "openfsl/fs_fat32.h"

using namespace std;
using namespace openFSL;

void hexdump(uint8_t* p, int offset, int len);
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
	
	spdlog::set_level(spdlog::level::warn);
	
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
		fat32->getDirList("::", buf);
		std::string dir;
		
		cout << "type \"exit\" to exit\n";
		while(true)
		{
			cout << fat32->getPath() << "> ";
			cin >> dir;
			
			if (dir == "exit") {
				break;
			}
			else if (dir == "dir") {
				if (buf != NULL)
					delete[] buf;
				buf = new FAT32_fileInfo[fat32->getChildCount("")];
				fat32->getDirList("", buf);
				for (int i = 0; i < fat32->getChildCount(""); i++)
				{
					cout << i << " " << buf[i].fileName << " " << (int)buf[i].fileCreateTime.time_month << "-" << (int)buf[i].fileCreateTime.time_day << "-" << (int)buf[i].fileCreateTime.time_year << " " << (int)buf[i].fileCreateTime.time_hour << ":" << (int)buf[i].fileCreateTime.time_min << ":" << (int)buf[i].fileCreateTime.time_sec << "\n";
				}
			}
			else {
				if (fat32->chdir(dir))
					cout << "Error\n";
			}
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		delete fat32->getDiskDevice();
		
		return 0;
	}
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