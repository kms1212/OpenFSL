/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifdef FAT32_BUILD

#include <openfsl/fat32/fs_fat32.h>

#include <chrono>

#include "header.h"

int fat32shell(openfsl::FileBlockDevice* fbd,
    openfsl::DiskStructure diskStructure, size_t selectIndex) {
    openfsl::FAT32 fat32(fbd, "", "\\/", "::");

    error_t result;

    if (diskStructure.partTable == openfsl::PartitionTableType::MBR) {
        openfsl::MBR mbr(fbd);

        result = mbr.initialize();
        if (result)
            return result;

        std::vector<openfsl::MBR::PartitionInfo> partitionInfo;
        result = mbr.getPartitionInfo(&partitionInfo);
        if (result)
            return result;

        fat32.setFsLBAOffset(partitionInfo[selectIndex].partOffset);
    } else if (diskStructure.partTable == openfsl::PartitionTableType::GPT) {
        openfsl::GPT gpt(fbd);

        result = gpt.initialize();
        if (result)
            return result;

        std::vector<openfsl::GPT::PartitionInfo> partitionInfo;
        result = gpt.getPartitionInfo(&partitionInfo);
        if (result)
            return result;

        fat32.setFsLBAOffset(partitionInfo[selectIndex].partOffset);
    }

    result = fat32.initialize();
    if (result)
        return result;

    result = fat32.enableCache(128);
    if (result) {
        return result;
    }

    std::cout << "file:\"" + fbd->getFilename() +
        "\":" + std::to_string(selectIndex) + "(" +
        openfsl::fileSystemTypeToString(diskStructure.partList[selectIndex])
        + "), VolumeName: " + fat32.getVolumeInfo().volumeLabel + "\n";

    std::string command = "";
    std::chrono::time_point<std::chrono::steady_clock> operationStart =
        std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> operationEnd;

    std::cout << "Type \"help\" to list commands\n";
    while (true) {
        std::vector<std::string> cmd;

        std::cout << fat32.getPath() << "> ";

        std::getline(std::cin, command);
        split(command, &cmd, ' ');

        if (settings["MeasureElapsedTime"] == true) {
            operationStart = std::chrono::steady_clock::now();
        }

        if (cmd[0] == "") {
            std::cout << "\n";
            continue;
        } else if ((cmd[0] == "exit") || (cmd[0] == "quit")) {
            break;
        } else if (cmd[0] == "help") {
            if (cmd.size() == 1) {
                std::cout << " Help\n"
                          << "---------------------------------------------\n"
                          << "exit, quit:            "
                          << "Exit program\n"
                          << "set:                   "
                          << "Set properties\n"
                          << "help:                  "
                          << "Show this message.\n"
                          << "dir:                   "
                          << "List directory contents\n"
                          << "cd:                    "
                          << "Change directory\n"
                          << "info:                  "
                          << "Get file/directory information\n"
                          << "mkdir:                 "
                          << "Create directory.\n"
                          << "rmdir:                 "
                          << "Remove directory.\n"
                          << "mkfile:                "
                          << "Create file.\n"
                          << "move:                  "
                          << "Move file or directory.\n"
                          << "defaultformatoptions:  "
                          << "Get default format options.\n"
                          << "setvolname:            "
                          << "Rename volume.\n"
                          << "getvolinfo:            "
                          << "Get current volume info.\n";
            } else if (cmd.size() == 2) {
                if (cmd[1] == "set") {
                    std::cout << " Help: set\n"
                              << "-----------------------------------------\n"
                              << "Usage: "
                              << "set <Property Name> <true,t,1/false,f,0>\n"
                              << "MeasureElapsedTime: "
                              << "Measure filesystem operation time\n";
                }
            }
        } else if (cmd[0] == "dir") {
            std::vector<openfsl::FAT32::FileInfo> buf;

            int result;
            if (cmd.size() == 2)
                result = fat32.listDirectory(&buf, cmd[1]);
            else
                result = fat32.listDirectory(&buf);

            if (result != 0) {
                std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";
                continue;
            }

            size_t maxFilenameLength = 9;
            for (size_t i = 0; i < buf.size(); i++)
                if (maxFilenameLength < buf[i].fileName.size())
                    maxFilenameLength = buf[i].fileName.size();

            std::stringstream ss;
            ss << std::setfill(' ') << std::left
                << std::setw((buf.size() + 1) / 10) << "N" << " | "
                << std::setw(maxFilenameLength) << "Filename" << " | "
                << std::setw(19) << "Create Time" << " | "
                << std::setw(19) << "Modify Time" << " | "
                <<    "File Size" << "\n";

            std::cout << ss.str();

            for (size_t i = 0; i < ss.str().size(); i++)
                std::cout << '-';
            std::cout << '\n';

            for (size_t i = 0; i < buf.size(); i++) {
                std::cout << std::left << std::setfill(' ')
                    << std::setw((buf.size() + 1) / 10) << i + 1 << " | "
                    << std::setw(maxFilenameLength) << buf[i].fileName << " | "
                    << std::setfill('0') << std::right
                    << std::setw(2)
                    << static_cast<int>(buf[i].fileCreateTime.time_month) << "-"
                    << std::setw(2)
                    << static_cast<int>(buf[i].fileCreateTime.time_day) << "-"
                    << std::setw(4)
                    << static_cast<int>(buf[i].fileCreateTime.time_year) << " "
                    << std::setw(2)
                    << static_cast<int>(buf[i].fileCreateTime.time_hour) << ":"
                    << std::setw(2)
                    << static_cast<int>(buf[i].fileCreateTime.time_min) << ":"
                    << std::setw(2)
                    << static_cast<int>(buf[i].fileModTime.time_sec) << " | "
                    << std::setw(2)
                    << static_cast<int>(buf[i].fileModTime.time_month) << "-"
                    << std::setw(2)
                    << static_cast<int>(buf[i].fileModTime.time_day) << "-"
                    << std::setw(4)
                    << static_cast<int>(buf[i].fileModTime.time_year) << " "
                    << std::setw(2)
                    << static_cast<int>(buf[i].fileModTime.time_hour) << ":"
                    << std::setw(2)
                    << static_cast<int>(buf[i].fileModTime.time_min) << ":"
                    << std::setw(2)
                    << static_cast<int>(buf[i].fileModTime.time_sec) << " | ";

                if ((buf[i].fileAttr &
                    openfsl::FAT32::FileAttribute::Directory) !=
                    (openfsl::FAT32::FileAttribute)0)
                    std::cout << "<DIR>\n";
                else
                    std::cout << static_cast<int>(buf[i].fileSize) << "\n";
            }
        } else if (cmd[0] == "set") {
            if (cmd.size() == 3) {
                if (settings.find(cmd[1]) != settings.end()) {
                    if (cmd[2] == "true" || cmd[2] == "t" || cmd[2] == "1")
                        settings[cmd[1]] = true;
                    else if (cmd[2] == "false" ||
                        cmd[2] == "f" || cmd[2] == "0")
                        settings[cmd[1]] = false;
                    else
                        std::cout << "Failed to parse input.\n";
                } else {
                    std::cout << "Property is not found.\n";
                }
            }
        } else if (cmd[0] == "cd") {
            int result = fat32.changeDirectory(cmd[1]);
            if (result) {
                std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";
            }
        } else if (cmd[0] == "info") {
            std::pair<error_t, openfsl::FAT32::FileInfo> fileInfo =
                fat32.getEntryInfo(cmd[1]);

            if (fileInfo.first) {
                std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";
                continue;
            }

            if (fileInfo.second.fileName != "") {
                std::cout << "File Name: " << fileInfo.second.fileName << "\n";
                std::cout << "Create Time: ";
                std::cout << static_cast<int>
                    (fileInfo.second.fileCreateTime.time_month) << "-";
                std::cout << static_cast<int>
                    (fileInfo.second.fileCreateTime.time_day) << "-";
                std::cout << static_cast<int>
                    (fileInfo.second.fileCreateTime.time_year) << " ";
                std::cout << static_cast<int>
                    (fileInfo.second.fileCreateTime.time_hour) << ":";
                std::cout << static_cast<int>
                    (fileInfo.second.fileCreateTime.time_min) << ":";
                std::cout << static_cast<int>
                    (fileInfo.second.fileCreateTime.time_sec) << "\n";
                std::cout << "Last Access Time: ";
                std::cout << static_cast<int>
                    (fileInfo.second.fileAccessTime.time_month) << "-";
                std::cout << static_cast<int>
                    (fileInfo.second.fileAccessTime.time_day) << "-";
                std::cout << static_cast<int>
                    (fileInfo.second.fileAccessTime.time_year) << " ";
                std::cout << static_cast<int>
                    (fileInfo.second.fileAccessTime.time_hour) << ":";
                std::cout << static_cast<int>
                    (fileInfo.second.fileAccessTime.time_min) << ":";
                std::cout << static_cast<int>
                    (fileInfo.second.fileAccessTime.time_sec) << "\n";
                std::cout << "Last Modify Time: ";
                std::cout << static_cast<int>
                    (fileInfo.second.fileModTime.time_month) << "-";
                std::cout << static_cast<int>
                    (fileInfo.second.fileModTime.time_day) << "-";
                std::cout << static_cast<int>
                    (fileInfo.second.fileModTime.time_year) << " ";
                std::cout << static_cast<int>
                    (fileInfo.second.fileModTime.time_hour) << ":";
                std::cout << static_cast<int>
                    (fileInfo.second.fileModTime.time_min) << ":";
                std::cout << static_cast<int>
                    (fileInfo.second.fileModTime.time_sec) << "\n";

                if ((fileInfo.second.fileAttr &
                    openfsl::FAT32::FileAttribute::Directory) !=
                    (openfsl::FAT32::FileAttribute)0)
                    std::cout << "File type is directory.\n";
                else
                    std::cout << "File size"
                        << static_cast<int>(fileInfo.second.fileSize) << "\n";
            }
        } else if (cmd[0] == "read") {
            if (cmd.size() == 2) {
                openfsl::FAT32::FILE file(&fat32);

                error_t result = file.open(cmd[1],
                    openfsl::FSL_OpenMode::Read);

                if (result) {
                    std::cerr << "Error opening file: "
                              << openfsl::geterrorstr(result) << "\n";
                } else {  // Hexdump file
                    file.seekg(0, openfsl::FSL_SeekMode::SeekEnd);
                    size_t fileSize = file.tellg();
                    file.seekg(0, openfsl::FSL_SeekMode::SeekSet);

                    if (fileSize > 0) {
                        char* buf = new char[fileSize + 1]();
                        file.read(reinterpret_cast<uint8_t*>(buf), fileSize, 1);
                        buf[fileSize] = 0;

                        hexdump(reinterpret_cast<uint8_t*>(buf), 0, fileSize);

                        std::cout << buf << "\n";
                        std::cout << "File get pointer: " << file.tellg() << "\n";
                        std::cout << "File put pointer: " << file.tellp() << "\n";
                    } else {
                        std::cout << "File is empty.\n";
                    }

                    file.close();
                }
            }
        } else if (cmd[0] == "write") {
            if (cmd.size() == 3) {
                openfsl::FAT32::FILE file(&fat32);

                error_t result = file.open(cmd[1],
                    openfsl::FSL_OpenMode::Read |
                    openfsl::FSL_OpenMode::Write);

                if (result) {
                    std::cerr << "Error opening file: "
                              << openfsl::geterrorstr(result) << "\n";
                } else {  // Hexdump file
                    file.seekp(0, openfsl::FSL_SeekMode::SeekSet);

                    file.write(
                        reinterpret_cast<const uint8_t*>(cmd[2].c_str()),
                        1, cmd[2].length());

                    file.flush();

                    std::cout << "File get pointer: " << file.tellg() << "\n";
                    std::cout << "File put pointer: " << file.tellp() << "\n";

                    file.close();
                }
            }
        } else if (cmd[0] == "mkdir") {
            int result = fat32.makeDirectory(cmd[1]);
            if (result)
                std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";
        } else if (cmd[0] == "rmdir") {
            int result = fat32.remove(cmd[1],
                openfsl::FAT32::FileAttribute::Directory);
            if (result)
                std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";
        } else if (cmd[0] == "rm") {
            int result = fat32.remove(cmd[1],
                openfsl::FAT32::FileAttribute::Archive);
            if (result)
                std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";
        } else if (cmd[0] == "mkfile") {
            int result = fat32.makeFile(cmd[1]);
            if (result)
                std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";
        } else if (cmd[0] == "move") {
            int result = fat32.move(cmd[1], cmd[2]);
            if (result)
                std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";
        } else if (cmd[0] == "search") {
            std::vector<std::pair<std::string, openfsl::FAT32::FileInfo>> finfo;
            int result = fat32.search(&finfo, cmd[1], cmd[2],
                openfsl::FAT32::FileAttribute::Any, true);
            if (result)
                std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";

            for (std::pair<std::string, openfsl::FAT32::FileInfo> fi : finfo) {
                std::cout << fi.first << fat32.getPathSeparator().at(0)
                    << fi.second.fileName << "\n";
            }
        } else if (cmd[0] == "defaultformatoptions") {
            openfsl::FAT32::FormatOptions fo =
                openfsl::FAT32::getDefaultFormatOptions(104857600);
            std::cout << "volumeLabel: " << fo.volumeLabel << "\n";
            std::cout << std::hex
                << "volumeSerial: "
                << fo.volumeSerial << "\n"
                << "sectorsPerCluster: "
                << static_cast<int>(fo.sectorsPerCluster) << "\n"
                << "reservedSectorCount: "
                << fo.reservedSectorCount << "\n"
                << "fatAreaCount: "
                << static_cast<int>(fo.fatAreaCount) << "\n"
                << "fatMediaType: "
                << static_cast<int>(fo.fatMediaType) << "\n"
                << "fsinfoLocation: "
                << fo.fsinfoLocation << "\n"
                << "backupBSLocation: "
                << fo.backupBSLocation << "\n"
                << "biosDriveNum: "
                << static_cast<int>(fo.biosDriveNum)
                << std::dec << "\n";
        } else if (cmd[0] == "setvolname") {
            error_t result = fat32.setVolumeName(cmd[1]);
            if (result)
                std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";
        } else if (cmd[0] == "getvolinfo") {
            openfsl::FAT32::VolumeInfo info = fat32.getVolumeInfo();
            std::cout << "Volume Label: " << info.volumeLabel << "\n";
            std::cout << "Volume Serial: " << info.volumeSerial << "\n";
            std::cout << "Free Cluster Count: " << info.freeCluster << "\n";
            std::cout << "Next Free Cluster: " << info.nextFreeCluster << "\n";
            std::cout << "Sector Per Cluster: "
                << static_cast<int>(info.sectorPerCluster) << "\n";
            std::cout << "Bytes Per Sector: " << info.bytesPerSector << "\n";
            std::cout << "Filesystem Size: " << info.filesystemSize << "\n";

            std::cout << info.bytesPerSector *
                info.sectorPerCluster * info.freeCluster << " bytes free.\n";
        } else {
            std::cerr << command << ": No such command\n";
        }

        if (settings["MeasureElapsedTime"] == true) {
            operationEnd = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> elapsedTime =
                operationEnd - operationStart;

            std::cout << "Elapsed time: " << elapsedTime.count() << "ms\n";
        }
    }

    return 0;
}

#endif  // FAT32_BUILD
