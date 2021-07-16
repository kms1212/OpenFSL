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
#include "openfsl/fat32/fs_fat32.h"

using namespace std;
using namespace openFSL;

int readDisk(DiskDevice* dd, uint8_t* dest, size_t lba, size_t size);
int writeDisk(Sector* src, size_t lba);
int openDisk();
int closeDisk();

fstream disk;
FS_FAT32* fat32;

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
    argc = argc; // Ignore MSVC warning C4100
    argv = argv; 
    
    fat32 = new FS_FAT32(NULL, "\\/");
    
    fat32->getDiskDevice()->read = readDisk;
    fat32->getDiskDevice()->write = writeDisk;
    fat32->getDiskDevice()->open = openDisk;
    fat32->getDiskDevice()->close = closeDisk;
    
    int result = fat32->initialize();
    
    
    /////////////////////////////////////////////
    // CHDIR TEST
    /////////////////////////////////////////////
    cout << "\n\n";
    cout << "===========================\n";
    cout << " CHDIR TEST\n";
    cout << "===========================\n";
    
    vector<string> chdirChecklist; // Checklist
    chdirChecklist.push_back("::/.");
    chdirChecklist.push_back("::/..");
    chdirChecklist.push_back("::/directory1");
    chdirChecklist.push_back("subdir1");
    chdirChecklist.push_back("::");
    chdirChecklist.push_back("directory1/subdir2");
    chdirChecklist.push_back("../..");
    chdirChecklist.push_back("directory9");
    
    for (size_t i = 0; i < chdirChecklist.size(); i++)
    {
        int result_temp = fat32->chdir(chdirChecklist.at(i));
        cout << "chdir(\"" << chdirChecklist.at(i) << "\") -> " << fat32->getPath() << ", Returns : " << result_temp << "\n";
        result += result_temp;
    }
    
    /////////////////////////////////////////////
    // GETDIRLIST TEST
    /////////////////////////////////////////////
    cout << "\n\n";
    cout << "===========================\n";
    cout << " GETDIRLIST TEST\n";
    cout << "===========================\n";
    
    vector<string> getDirListChecklistDir; // Checklist directories
    vector<vector<string>> getDirListChecklist; // Checklist
    vector<string> getDirListChecklistChilds; // Checklist childs list
    
    getDirListChecklistDir.push_back("::"); // Directory ::
    getDirListChecklistChilds.push_back("DIRECTORY1");
    getDirListChecklistChilds.push_back("DIRECTORY2");
    getDirListChecklistChilds.push_back("DIRECTORY3");
    getDirListChecklistChilds.push_back("DIRECTORY4");
    getDirListChecklistChilds.push_back("DIRECTORY5");
    getDirListChecklistChilds.push_back("DIRECTORY6");
    getDirListChecklistChilds.push_back("DIRECTORY7");
    getDirListChecklistChilds.push_back("DIRECTORY8");
    getDirListChecklistChilds.push_back("FILE1.TXT");
    getDirListChecklistChilds.push_back("FILE2.TXT");
    getDirListChecklistChilds.push_back("FILE3.TXT");
    getDirListChecklistChilds.push_back("FILE4.TXT");
    getDirListChecklistChilds.push_back("FILE5.TXT");
    getDirListChecklistChilds.push_back("FILE6.TXT");
    getDirListChecklistChilds.push_back("FILE7.TXT");
    getDirListChecklistChilds.push_back("FILE8.TXT");
    getDirListChecklistChilds.push_back("LFNFILENAME1.TXT");
    getDirListChecklistChilds.push_back("LFNFILENAME2.TXT");
    getDirListChecklistChilds.push_back("LFNFILENAME3.TXT");
    getDirListChecklistChilds.push_back("LFNFILENAME4.TXT");
    getDirListChecklistChilds.push_back("DIRECTORY9");
    getDirListChecklist.push_back(getDirListChecklistChilds);
    
    getDirListChecklistChilds.clear();
    
    getDirListChecklistDir.push_back("::/directory1"); // Directory ::/directory1
    getDirListChecklistChilds.push_back(".");
    getDirListChecklistChilds.push_back("..");
    getDirListChecklistChilds.push_back("SUBDIR1");
    getDirListChecklistChilds.push_back("SUBDIR2");
    getDirListChecklistChilds.push_back("SUBDIR3");
    getDirListChecklistChilds.push_back("SUBDIR4");
    getDirListChecklist.push_back(getDirListChecklistChilds);
    
    
    if (getDirListChecklistDir.size() == getDirListChecklist.size())
    {
        for (size_t j = 0; j < getDirListChecklist.size(); j++)
        {
            int result_temp;
            result_temp = fat32->chdir(getDirListChecklistDir.at(j));
            result += result_temp;
    
            std::vector<FS_FAT32::FileInfo> buf;
            fat32->getDirList(&buf);
            
            result_temp = buf.size() != getDirListChecklist.at(j).size();
            result += result_temp;
            cout << "(dirList.size() = " << buf.size() << ") != (dirChild.size() = " << getDirListChecklist.at(j).size() << ") Returns : " << result_temp << "\n";
            
            string filename;
            for (uint32_t i = 0; i < buf.size(); i++)
            {
                cout << buf[i].fileName << ": ";
                filename = buf[i].fileName;
                
                for (auto & c: filename) c = (char)toupper(c);
                if (find(getDirListChecklist.at(j).begin(), getDirListChecklist.at(j).end(), filename) == getDirListChecklist.at(j).end()) {
                    cout << "not found\n";
                    result++;
                }
                else
                    cout << "found\n";
            }
            
            
        }
    }
    else
    {
        result++;
    }
    
    /////////////////////////////////////////////
    // GETFILEINFORMATION TEST
    /////////////////////////////////////////////
    cout << "\n\n";
    cout << "===========================\n";
    cout << " GETFILEINFORMATION TEST\n";
    cout << "===========================\n";
    
    vector<string> finfoChecklist; // Checklist
    finfoChecklist.push_back("::/file1.txt");
    finfoChecklist.push_back("::/file2.txt");
    finfoChecklist.push_back("::/file3.txt");
    finfoChecklist.push_back("::/file4.txt");
    finfoChecklist.push_back("::/file5.txt");
    finfoChecklist.push_back("::/file6.txt");
    finfoChecklist.push_back("::/file7.txt");
    finfoChecklist.push_back("::/file8.txt");
    finfoChecklist.push_back("::/lfnfilename1.txt");
    finfoChecklist.push_back("::/lfnfilename2.txt");
    finfoChecklist.push_back("::/lfnfilename3.txt");
    finfoChecklist.push_back("::/lfnfilename4.txt");
    
    for (size_t i = 0; i < finfoChecklist.size(); i++)
    {
        FS_FAT32::FileInfo fileInfo = fat32->getFileInformation(finfoChecklist.at(i));
        cout << "getFileInformation(\"" << finfoChecklist.at(i) << "\") Returns : \n";
        cout << "    File Name: " << fileInfo.fileName << "\n";
        cout << "    File Created Time: " << fileInfo.fileCreateTime.time_month << "/" << fileInfo.fileCreateTime.time_day << "/" << fileInfo.fileCreateTime.time_year << " " 
             << fileInfo.fileCreateTime.time_hour << ":" << fileInfo.fileCreateTime.time_min << ":" << fileInfo.fileCreateTime.time_sec << "." << fileInfo.fileCreateTime.time_millis << "\n";
        cout << "    File Access Time: " << fileInfo.fileAccessTime.time_month << "/" << fileInfo.fileAccessTime.time_day << "/" << fileInfo.fileAccessTime.time_year << " " 
             << fileInfo.fileAccessTime.time_hour << ":" << fileInfo.fileAccessTime.time_min << ":" << fileInfo.fileAccessTime.time_sec << "." << fileInfo.fileAccessTime.time_millis << "\n";
        cout << "    File Mod Time: " << fileInfo.fileModTime.time_month << "/" << fileInfo.fileModTime.time_day << "/" << fileInfo.fileModTime.time_year << " " 
             << fileInfo.fileModTime.time_hour << ":" << fileInfo.fileModTime.time_min << ":" << fileInfo.fileModTime.time_sec << "." << fileInfo.fileModTime.time_millis << "\n";
        cout << "    File Location: " << fileInfo.fileLocation << "\n";
        cout << "    File Size: " << fileInfo.fileSize << "\n";
        
        std::stringstream ss;
        ss << std::hex << (unsigned int)fileInfo.fileAttr;
        cout << "    File Attribute: 0x" << ss.str() << "\n";
    }
    
    /////////////////////////////////////////////
    // FILE TEST
    /////////////////////////////////////////////
    cout << "\n\n";
    cout << "===========================\n";
    cout << " FILE TEST\n";
    cout << "===========================\n";
    
    vector<string> fileChecklist; // Checklist
    fileChecklist.push_back("::/file1.txt");
    fileChecklist.push_back("::/file2.txt");
    fileChecklist.push_back("::/file3.txt");
    fileChecklist.push_back("::/file4.txt");
    fileChecklist.push_back("::/file5.txt");
    fileChecklist.push_back("::/file6.txt");
    fileChecklist.push_back("::/file7.txt");
    fileChecklist.push_back("::/file8.txt");
    fileChecklist.push_back("::/lfnfilename1.txt");
    fileChecklist.push_back("::/lfnfilename2.txt");
    fileChecklist.push_back("::/lfnfilename3.txt");
    fileChecklist.push_back("::/lfnfilename4.txt");
    
    for (size_t i = 0; i < fileChecklist.size(); i++)
    {
        cout << "Filename: " << finfoChecklist.at(i) << "\n";
        FSL_File<FS_FAT32>* file = fat32->openFile(fileChecklist.at(i), FSL_OpenMode::read);
        cout << "openFile(\"" << finfoChecklist.at(i) << "\")\n";
        
        if (file == NULL)
        {
            result++;
        }
        else {
            uint32_t fileSize = file->getFileController()->getFileInfo().fileSize;
            char* buf = new char[fileSize + 1]();
            memset(buf, 0, fileSize + 1);
            file->read((uint8_t*)buf, 1, fileSize);
            buf[fileSize + 1] = 0;
            
            string buf_s(buf);
            string comp_s("Hello, World!\nOpenFSL\n");
            cout << "file->read() :\n";
            cout << "    Read: \n<" << buf_s << ">\n";
            hexdump((uint8_t*)buf, 0, fileSize + 1);
            cout << "\n";
            
            if (buf_s != comp_s) {
                result++;
            }
            
            memset(buf, 0, fileSize + 1);
            file->seek(14);
            file->read((uint8_t*)buf, 1, fileSize - 14);
            
            buf_s = string(buf);
            comp_s = "OpenFSL\n";
            cout << "file->seek(14) file->read() :\n";
            cout << "    Read: \n<" << buf_s << ">\n";
            hexdump((uint8_t*)buf, 0, fileSize - 13);
        }
        cout << "------------------------------\n";
    }

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
    if (disk.read((char*)dest, size * dd->getBytespersector()))
        return 1;
    
    return disk.tellg() == -1 ? 1 : 0;
}

int writeDisk(Sector* src, size_t lba)
{
    disk.seekp(lba * fat32->getDiskDevice()->getBytespersector(), ios::beg);
    if (disk.write((char*)src->getData(), src->getSectorCount() * fat32->getDiskDevice()->getBytespersector()))
        return 1;
    return disk.tellp() == -1 ? 1 : 0;
}