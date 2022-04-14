/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <cstdlib>
#include <cstring>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <codecvt>
#include <bitset>

#include "openfsl/fslutils.h"
#include "openfsl/fileblockdevice.h"
#include "openfsl/sector.h"
#include "openfsl/fat32/fs_fat32.h"

int readCount = 0;
int writeCount = 0;

size_t split(const std::string &txt, std::vector<std::string>* strs, char ch);
void hexdump(uint8_t* p, size_t offset, size_t len);

std::fstream disk;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << " [image_file] [test_name]";
        return 1;
    }

    std::string imageFileName = std::string(argv[1]);
    std::string testName = std::string(argv[2]);

    disk.open(imageFileName, std::ios::in | std::ios::out | std::ios::binary);

    openfsl::FileBlockDevice bd;

    std::cout << "Reading image file parameter...\n";
    std::fstream diskInfo;
    diskInfo.open(imageFileName + ".info", std::ios::in);
    if (!diskInfo.fail()) {
        std::string line;
        openfsl::BlockDevice::DiskParameter parameter;

        while (getline(diskInfo, line)) {
            std::cout << line << "\n";

            std::vector<std::string> value;

            split(line, &value, ' ');
            if (value[0] == "SectorPerTrack") {
                parameter.sectorPerTrack = stoi(value[1]);
            } else if (value[0] == "HeadPerCylinder") {
                parameter.headPerCylinder = stoi(value[1]);
            } else if (value[0] == "BytesPerSector") {
                parameter.bytesPerSector = stoi(value[1]);
            }
        }

        bd.setDiskParameter(parameter);

        diskInfo.close();
    } else {
        std::cout << "Fail to read disk parameter file. ";
        std::cout << "Default values are will be used.\n";
    }

    bd.initialize(imageFileName, openfsl::FileBlockDevice::OpenMode::RW);

    openfsl::FAT32 fat32(&bd, "", "\\/");

    error_t result = fat32.initialize();
    if (result) {
        disk.close();

        return result;
    }

    fat32.enableCache(512);

    /////////////////////////////////////////////
    // CHDIR TEST
    /////////////////////////////////////////////
    std::cout << "===========================\n";
    std::cout << " CHDIR TEST\n";
    std::cout << "===========================\n";

    std::vector<std::string> chdirChecklist;  // Checklist
    chdirChecklist.push_back("::/.");
    chdirChecklist.push_back("::/..");
    chdirChecklist.push_back("::/directory1");
    chdirChecklist.push_back("subdir1");
    chdirChecklist.push_back("::");
    chdirChecklist.push_back("directory1/subdir2");
    chdirChecklist.push_back("../..");
    chdirChecklist.push_back("directory9");

    for (size_t i = 0; i < chdirChecklist.size(); i++) {
        fat32.changeDirectory(chdirChecklist.at(i));
        std::cout << "chdir(\"" << chdirChecklist.at(i) << "\") -> "
             << fat32.getPath() << "\n";
    }

    /////////////////////////////////////////////
    // GETDIRLIST TEST
    /////////////////////////////////////////////
    std::cout << "\n\n";
    std::cout << "===========================\n";
    std::cout << " GETDIRLIST TEST\n";
    std::cout << "===========================\n";

    std::vector<std::string> listDirChecklistDir;  // Checklist directories
    std::vector<std::vector<std::string>> listDirChecklist;  // Checklist
    std::vector<std::string> listDirChecklistChilds;  // Checklist childs list

    listDirChecklistDir.push_back("::");  // Directory ::
    listDirChecklistChilds.push_back("DIRECTORY1");
    listDirChecklistChilds.push_back("DIRECTORY2");
    listDirChecklistChilds.push_back("DIRECTORY3");
    listDirChecklistChilds.push_back("DIRECTORY4");
    listDirChecklistChilds.push_back("DIRECTORY5");
    listDirChecklistChilds.push_back("DIRECTORY6");
    listDirChecklistChilds.push_back("DIRECTORY7");
    listDirChecklistChilds.push_back("DIRECTORY8");
    listDirChecklistChilds.push_back("FILE1.TXT");
    listDirChecklistChilds.push_back("FILE2.TXT");
    listDirChecklistChilds.push_back("FILE3.TXT");
    listDirChecklistChilds.push_back("FILE4.TXT");
    listDirChecklistChilds.push_back("FILE5.TXT");
    listDirChecklistChilds.push_back("FILE6.TXT");
    listDirChecklistChilds.push_back("FILE7.TXT");
    listDirChecklistChilds.push_back("FILE8.TXT");
    listDirChecklistChilds.push_back("LFNFILENAME1.TXT");
    listDirChecklistChilds.push_back("LFNFILENAME2.TXT");
    listDirChecklistChilds.push_back("LFNFILENAME3.TXT");
    listDirChecklistChilds.push_back("LFNFILENAME4.TXT");
    listDirChecklistChilds.push_back("DIRECTORY9");
    listDirChecklist.push_back(listDirChecklistChilds);

    listDirChecklistChilds.clear();

    listDirChecklistDir.push_back("::/directory1");  // Directory ::/directory1
    listDirChecklistChilds.push_back(".");
    listDirChecklistChilds.push_back("..");
    listDirChecklistChilds.push_back("SUBDIR1");
    listDirChecklistChilds.push_back("SUBDIR2");
    listDirChecklistChilds.push_back("SUBDIR3");
    listDirChecklistChilds.push_back("SUBDIR4");
    listDirChecklist.push_back(listDirChecklistChilds);


    for (size_t j = 0; j < listDirChecklist.size(); j++) {
        int result_temp;
        fat32.changeDirectory(listDirChecklistDir.at(j));

        std::vector<openfsl::FAT32::FileInfo> buf;
        fat32.listDirectory(&buf);

        result_temp = buf.size() != listDirChecklist.at(j).size();
        result += result_temp;
        std::cout << "(dirList.size() = " << buf.size()
            << ") != (dirChild.size() = " << listDirChecklist.at(j).size()
            << ") Returns : " << result_temp << "\n";

        std::string filename;
        for (uint32_t i = 0; i < buf.size(); i++) {
            std::cout << buf[i].fileName << ": ";
            filename = buf[i].fileName;

            for (auto& c : filename) c = static_cast<char>(toupper(c));
            if (find(listDirChecklist.at(j).begin(),
                listDirChecklist.at(j).end(), filename) ==
                listDirChecklist.at(j).end()) {
                std::cout << "not found\n";
                result++;
            } else {
                std::cout << "found\n";
            }
        }
    }

    /////////////////////////////////////////////
    // GETFILEINFORMATION TEST
    /////////////////////////////////////////////
    std::cout << "\n\n";
    std::cout << "===========================\n";
    std::cout << " GETFILEINFORMATION TEST\n";
    std::cout << "===========================\n";

    std::vector<std::string> finfoChecklist;  // Checklist
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

    for (size_t i = 0; i < finfoChecklist.size(); i++) {
        std::pair<error_t, openfsl::FAT32::FileInfo> fileInfo =
            fat32.getEntryInfo(finfoChecklist.at(i));

        if (fileInfo.first) {
            result++;
            break;
        }

        std::cout << "getFileInformation(\"" << finfoChecklist.at(i)
            << "\") Returns : \n";
        std::cout << "    File Name: " << fileInfo.second.fileName << "\n";
        std::cout << "    File Created Time: "
            << fileInfo.second.fileCreateTime.time_month << "/"
            << fileInfo.second.fileCreateTime.time_day << "/"
            << fileInfo.second.fileCreateTime.time_year << " "
            << fileInfo.second.fileCreateTime.time_hour << ":"
            << fileInfo.second.fileCreateTime.time_min << ":"
            << fileInfo.second.fileCreateTime.time_sec << "."
            << fileInfo.second.fileCreateTime.time_millis << "\n";
        std::cout << "    File Access Time: "
            << fileInfo.second.fileAccessTime.time_month << "/"
            << fileInfo.second.fileAccessTime.time_day << "/"
            << fileInfo.second.fileAccessTime.time_year << " "
            << fileInfo.second.fileAccessTime.time_hour << ":"
            << fileInfo.second.fileAccessTime.time_min << ":"
            << fileInfo.second.fileAccessTime.time_sec << "."
            << fileInfo.second.fileAccessTime.time_millis << "\n";
        std::cout << "    File Mod Time: "
            << fileInfo.second.fileModTime.time_month << "/"
            << fileInfo.second.fileModTime.time_day << "/"
            << fileInfo.second.fileModTime.time_year << " "
            << fileInfo.second.fileModTime.time_hour << ":"
            << fileInfo.second.fileModTime.time_min << ":"
            << fileInfo.second.fileModTime.time_sec << "."
            << fileInfo.second.fileModTime.time_millis << "\n";
        std::cout << "    File Location: " << fileInfo.second.fileLocation << "\n";
        std::cout << "    File Size: " << fileInfo.second.fileSize << "\n";

        std::stringstream ss;
        ss << std::hex << (unsigned int)fileInfo.second.fileAttr;
        std::cout << "    File Attribute: 0x" << ss.str() << "\n";
    }

    /////////////////////////////////////////////
    // FILE TEST
    /////////////////////////////////////////////
    std::cout << "\n\n";
    std::cout << "===========================\n";
    std::cout << " FILE TEST\n";
    std::cout << "===========================\n";

    std::vector<std::string> fileChecklist;  // Checklist
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

    for (size_t i = 0; i < fileChecklist.size(); i++) {
        std::cout << "Filename: " << finfoChecklist.at(i) << "\n";
        openfsl::FAT32::FILE file(&fat32);
        if (file.open(fileChecklist.at(i), openfsl::FSL_OpenMode::Read)) {
            result++;
            continue;
        }

        std::cout << "openFile(\"" << finfoChecklist.at(i) << "\")\n";

        file.seekg(0, openfsl::FSL_SeekMode::SeekEnd);
        size_t fileSize = file.tellg();
        file.seekg(0, openfsl::FSL_SeekMode::SeekSet);

        char* buf = new char[fileSize + 1]();
        memset(buf, 0, fileSize + 1);
        file.read(reinterpret_cast<uint8_t*>(buf), 1, fileSize);
        buf[fileSize] = 0;

        std::string buf_s(buf);
        std::string comp_s("Hello, World!\nOpenFSL\n");
        std::cout << "file.read() :\n";
        std::cout << "    Read: \n<" << buf_s << ">\n";
        hexdump(reinterpret_cast<uint8_t*>(buf), 0, fileSize);
        std::cout << "\n";

        if (buf_s != comp_s) {
            result++;
        }

        // Try to read file with seek
        memset(buf, 0, fileSize + 1);
        file.seekg(14, openfsl::FSL_SeekMode::SeekSet);
        file.read(reinterpret_cast<uint8_t*>(buf), 1, fileSize - 14);

        buf_s = std::string(buf);
        comp_s = "OpenFSL\n";
        std::cout << "file.seek(14) file.read() :\n";
        std::cout << "    Read: \n<" << buf_s << ">\n";
        hexdump(reinterpret_cast<uint8_t*>(buf), 0, fileSize - 14);

        if (buf_s != comp_s) {
            result++;
        }

        // Try to read more than the size of file.
        memset(buf, 0, fileSize + 1);
        file.seekg(14, openfsl::FSL_SeekMode::SeekSet);
        size_t ret = file.read(reinterpret_cast<uint8_t*>(buf), 1, fileSize);

        if (ret != fileSize - 14)
            result++;

        std::cout << "file.seek(14) file.read() :\n";
        std::cout << "    Returned: " << ret << "\n";

        file.close();
        std::cout << "------------------------------\n";
    }

    /////////////////////////////////////////////
    // MAKE DIRECTORY TEST
    /////////////////////////////////////////////
    std::cout << "\n\n";
    std::cout << "===========================\n";
    std::cout << " MAKE DIRECTORY TEST\n";
    std::cout << "===========================\n";

    std::vector<std::string> mkDirChecklist;  // Checklist
    fileChecklist.push_back("::/mkdirtest1");
    fileChecklist.push_back("::/mkdirtest2");
    fileChecklist.push_back("::/SFNMKDIR.1");
    fileChecklist.push_back("::/SFNMKDIR.2");

    for (std::string dirname : mkDirChecklist) {
        result += fat32.makeDirectory(dirname);

        result += fat32.changeDirectory(dirname);
        result += fat32.changeDirectory("..");
    }

    /////////////////////////////////////////////
    // REMOVE DIRECTORY TEST
    /////////////////////////////////////////////
    std::cout << "\n\n";
    std::cout << "===========================\n";
    std::cout << " REMOVE DIRECTORY TEST\n";
    std::cout << "===========================\n";

    std::vector<std::string> rmDirChecklist;  // Checklist
    fileChecklist.push_back("::/mkdirtest1");
    fileChecklist.push_back("::/mkdirtest2");
    fileChecklist.push_back("::/SFNMKDIR.1");
    fileChecklist.push_back("::/SFNMKDIR.2");

    for (std::string dirname : rmDirChecklist) {
        result += fat32.removeDirectory(dirname);

        if (!fat32.changeDirectory(dirname))
            result++;
    }

    std::cout << "Total read count: " << readCount << std::endl;
    std::cout << "Total write count: " << writeCount << std::endl;

    disk.close();

    return result;
}

size_t split(const std::string &txt,
    std::vector<std::string>* strs, char ch) {
    std::string temp = txt;
    size_t pos = temp.find(ch);
    size_t initialPos = 0;
    strs->clear();

    // Decompose statement
    while (pos != std::string::npos) {
        if (temp.at(pos - 1) != '\\') {
            strs->push_back(temp.substr(initialPos, pos - initialPos));
            initialPos = pos + 1;
        } else {
            temp.erase(temp.begin() + pos - 1);
        }

        pos = temp.find(ch, pos + 1);
    }

    // Add the last one
    strs->push_back(temp.substr(initialPos,
        std::min(pos, temp.size()) - initialPos + 1));

    return strs->size();
}

void hexdump(uint8_t* p, size_t offset, size_t len) {
    std::ios init(nullptr);
    init.copyfmt(std::cout);

    size_t address = 0;
    size_t row = 0;

    std::cout << std::hex << std::setfill('0');
    while (1) {
        if (address >= len) break;
        size_t nread = ((len - address) > 16) ? 16 : (len - address);

        // Show the address
        std::cout << std::setw(8) << address + offset;

        // Show the hex codes
        for (size_t i = 0; i < 16; i++) {
            if (i % 8 == 0) std::cout << ' ';
            if (i < nread)
                std::cout << ' ' << std::setw(2)
                    << static_cast<int>(p[16 * row + i + offset]);
            else
                std::cout << "   ";
        }

        // Show printable characters
        std::cout << "  ";
        for (size_t i = 0; i < nread; i++) {
            uint8_t ch = p[16 * row + i + offset];
            if (ch < 32 || ch > 125) std::cout << '.';
            else
                std::cout << ch;
        }

        std::cout << "\n";
        address += 16;
        row++;
    }
    std::cout.copyfmt(init);
}
