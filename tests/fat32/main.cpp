#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>

#include "fslservices.h"
#include "diskdevice.h"
#include "logdevice.h"
#include "sector.h"
#include "size_t.h"

using namespace std;
using namespace openFSL;

#if defined(_M_X64) || defined(__amd64__)
void readDisk(Sector* dest, uint64_t lba, uint64_t size);
#else
void readDisk(Sector* dest, uint32_t lba, uint32_t size);
#endif
void* memcpyWrapper(void* dest, const void* src, size_t num);
void hexdump(uint8_t* p, int offset, int len);
void openDisk();
void closeDisk();

fstream disk;
char* filename;

int main(int argc, char** argv) {
	if (argc < 2)
	{
		cerr << argv[0] << ": Usage: " << argv[0] << " [File Name]\n";
	}
	filename = argv[1];
	
	openFSL::fsl_malloc = malloc;
	openFSL::fsl_free = free;
	openFSL::fsl_memcpy = memcpyWrapper;
	
	DiskDevice* dd = new DiskDevice();
	LogDevice* ld = new LogDevice();
	Sector* sector = new Sector(1);
	
	dd->readDisk = readDisk;
	dd->openDisk = openDisk;
	dd->closeDisk = closeDisk;
	dd->initialize();
	if (dd->getState() != 0) {
		cerr << argv[0] << ": Error: DiskDevice";
		return 1;
	}
	
	for (int i = 0; i < 5; i++)
	{
		cout << "=================== " << i << " ===================\n";
		
		dd->readDisk(sector, i, 1);
		hexdump(sector->getData(), 0, 512);
	}
	
	dd->close();
	delete dd;
	delete ld;
	delete sector;
	
	return 0;
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
            if (p[16 * row + i + offset] < 32) std::cout << '.';
            else std::cout << p[16 * row + i + offset];
        }
 
        std::cout << "\n";
        address += 16;
        row++;
    }
	cout.copyfmt(init);
}

void openDisk()
{
	disk.open(filename, ios::in | ios::binary);
}

void closeDisk()
{
	disk.close();
}

#if defined(_M_X64) || defined(__amd64__)
void readDisk(Sector* dest, uint64_t lba, uint64_t size)
#else
void readDisk(Sector* dest, uint32_t lba, uint32_t size)
#endif
{
	disk.seekg(lba * bytespersector, ios::beg);
	disk.read((char*)dest->getData(), size * bytespersector);
}

void* memcpyWrapper(void* dest, const void* src, size_t num)
{
	return memcpy(dest, src, (long unsigned int)num);
}