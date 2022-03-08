/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "header.h"

#ifdef FAT32_BUILD
#include <openfsl/fat32/fs_fat32.h>
#endif

int main(int argc, char** argv) {
    std::cout << openfsl::getFullVersionString() << "\n";
    std::cout << "Supported File systems: \n";
    std::cout << "    FAT32:   ";
#ifdef FAT32_BUILD
    std::cout << "YES\n";
#else
    std::cout << "NO\n";
#endif
    std::cout << "    NTFS:    ";
#ifdef NTFS_BUILD
    std::cout << "YES\n";
#else
    std::cout << "NO\n";
#endif
    std::cout << "    ISO9660: ";
#ifdef ISO9660_BUILD
    std::cout << "YES\n";
#else
    std::cout << "NO\n";
#endif

    std::string imageFileName;

    if (argc != 2) {
        std::cout << "Please enter image file path: ";
        std::cin >> imageFileName;
    } else {
        imageFileName = argv[1];
    }

    settings["MeasureElapsedTime"] = false;

    bool isCdrom = false;

    size_t imagedotpos = imageFileName.find_last_of('.');
    if (imagedotpos != std::string::npos) {
        std::string imageFileExt = imageFileName.substr(imagedotpos + 1);

        for (auto & c : imageFileExt) c = static_cast<char>(toupper(c));

        isCdrom = imageFileExt == "ISO";
    }

    openfsl::FileBlockDevice fbd;

    if (isCdrom) {
        openfsl::BlockDevice::DiskParameter parameter;
        parameter.bytesPerSector = 2048;
        fbd.setDiskParameter(parameter);
    } else {
        std::cout << "Reading image file parameter...\n";
        std::fstream diskInfo;
        diskInfo.open(std::string(imageFileName) + std::string(".info"),
            std::ios::in);
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

            fbd.setDiskParameter(parameter);

            diskInfo.close();
        } else {
            std::cout << "Fail to read disk parameter file. ";
            std::cout << "Default values are will be used.\n";
        }
    }

    error_t result = fbd.initialize(std::string(imageFileName),
        openfsl::FileBlockDevice::OpenMode::RW);
    if (result) {
        std::cout << openfsl::geterrorstr(result) << "\n";
        return 0;
    }

    if (isCdrom) {
#ifdef ISO9660_BUILD
        if (cdfsshell(&fbd)) {
            fbd.deinitialize();
            return EXIT_FAILURE;
        }
        goto exitProgram;
#endif
        std::cerr << "File system is not supported.\n";
    } else {
        openfsl::DiskStructure diskStructure;

        result = detectDiskStructure(&diskStructure, &fbd);
        if (result) {
            std::cout << openfsl::geterrorstr(result) << "\n";
            return 0;
        }

        size_t selectIndex = 0;
        bool format = false;

        while (true) {
            /////////////////////////////////// Menu List
            std::cout << std::endl;
            std::cout << "Partition Table Type: ";
            std::cout << partitionTableTypeToString(diskStructure.partTable)
                << "\n";

            std::cout << "File Systems: \n";
            if (diskStructure.partList.size() == 0) {
                std::cout << "              No file system found.\n";
            }

            for (size_t i = 0; i < diskStructure.partList.size(); i++) {
                std::cout << "              " << i + 1 << ": ";

                std::cout << fileSystemTypeToString(diskStructure.partList[i])
                    << "\n";
            }
            std::cout << diskStructure.partList.size() + 1 << ": Format\n";
            std::cout << diskStructure.partList.size() + 2 << ": Exit\n";
            std::cout << "Select: ";
            std::cin >> selectIndex;

            if ((selectIndex > 0) &&
                (selectIndex < diskStructure.partList.size() + 1)) {
                break;
            } else if (selectIndex == diskStructure.partList.size() + 1) {
                format = true;
                break;
            } else if (selectIndex == diskStructure.partList.size() + 2) {
                goto exitProgram;
            } else {
                std::cout << "Invalid selection.\n";
            }

            std::cin.ignore();
        }

        selectIndex--;
        if (!format) {
#ifdef FAT32_BUILD
            if (diskStructure.partList[selectIndex] ==
                openfsl::FileSystemType::FAT32) {
                if (fat32shell(&fbd, diskStructure, selectIndex)) {
                    fbd.deinitialize();
                    return EXIT_FAILURE;
                }
                goto exitProgram;
            }
#endif
#ifdef NTFS_BUILD
            if (diskStructure.partList[selectIndex] ==
                openfsl::FileSystemType::NTFS) {
                if (ntfsshell(&fbd, diskStructure, selectIndex)) {
                    fbd.deinitialize();
                    return EXIT_FAILURE;
                }
                goto exitProgram;
            }
#endif
            std::cerr << "File system is not supported.\n";
        } else {  // fat32 format
#ifdef FAT32_BUILD
            openfsl::FAT32 fat32(&fbd);

            openfsl::lba48_t fsSize;
            bool f_f;

            std::cout << "Enter file system size: ";
            std::cin >> fsSize;
            std::cout << "Fast format?: ";
            std::cin >> f_f;

            openfsl::FAT32::FormatOptions options =
                fat32.getDefaultFormatOptions(fsSize);

            result = fat32.format(0, fsSize, options, f_f);
            if (result)
                std::cerr << "Error: " << openfsl::geterrorstr(result) << "\n";
#endif
        }
    }

exitProgram:
    fbd.deinitialize();
    return 0;
}