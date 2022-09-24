/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <wx/wx.h>

#include "FAT32Command.h"

error_t FAT32Command::Initialize(openfsl::DiskStructure diskStructure, size_t selectIndex) {
    fat32 = new openfsl::fat32::FAT32(fbd, "", "/", "::");
    error_t result;
    
    if (diskStructure.partTable == openfsl::PartitionTableType::MBR) {
        openfsl::MBR mbr(fbd);
        result = mbr.initialize();
        if (result) {
            wxMessageBox(openfsl::geterrorstr(result), "Error");
            return result;
        }
        std::vector<openfsl::MBR::PartitionInfo> partitionInfo;
        result = mbr.getPartitionInfo(&partitionInfo);
        if (result) {
            wxMessageBox(openfsl::geterrorstr(result), "Error");
            return result;
        }
        fat32->setFsLBAOffset(partitionInfo[selectIndex].partOffset);
    } else if (diskStructure.partTable == openfsl::PartitionTableType::GPT) {
        openfsl::GPT gpt(fbd);
        result = gpt.initialize();
        if (result) {
            wxMessageBox(openfsl::geterrorstr(result), "Error");
            return result;
        }
        std::vector<openfsl::GPT::PartitionInfo> partitionInfo;
        result = gpt.getPartitionInfo(&partitionInfo);
        if (result) {
            wxMessageBox(openfsl::geterrorstr(result), "Error");
            return result;
        }
        fat32->setFsLBAOffset(partitionInfo[selectIndex].partOffset);
    }

    result = fat32->initialize();
    if (result) {
        wxMessageBox(openfsl::geterrorstr(result), "Error");
        return result;
    }

    result = fat32->enableCache(128);
    if (result) {
        wxMessageBox(openfsl::geterrorstr(result), "Error");
        return result;
    }


    return OPENFSL_SUCCESS;
}

error_t FAT32Command::CreateFile() {
    wxMessageBox("FAT32 option is not enabled.", "Error");
    return OPENFSL_SUCCESS;
}

error_t FAT32Command::RemoveFile() {
    wxMessageBox("FAT32 option is not enabled.", "Error");
    return OPENFSL_SUCCESS;
}

error_t FAT32Command::GetVolumeInfo() {
    wxMessageBox("FAT32 option is not enabled.", "Error");
    return OPENFSL_SUCCESS;
}

error_t FAT32Command::SetVolumeName() {
    wxMessageBox("FAT32 option is not enabled.", "Error");
    return OPENFSL_SUCCESS;
}

error_t FAT32Command::GetPathSeparator(std::string* separator) {
    *separator = fat32->getPathSeparator();
    return 0;
}

error_t FAT32Command::CreateDirectory() {
    wxMessageBox("FAT32 option is not enabled.", "Error");
    return OPENFSL_SUCCESS;
}

error_t FAT32Command::RemoveDirectory() {
    wxMessageBox("FAT32 option is not enabled.", "Error");
    return OPENFSL_SUCCESS;
}

error_t FAT32Command::GetCurrentDirectory(std::string* outPath) {
    *outPath = fat32->getPath();
    return OPENFSL_SUCCESS;
}

error_t FAT32Command::NavigateDirectory(const std::string dest) {
    error_t result = fat32->changeDirectory(dest);
    if (result) {
        wxMessageBox(openfsl::geterrorstr(result), "Error");
        return result;
    }
    return OPENFSL_SUCCESS;
}

error_t FAT32Command::GetFileInformation() {
    wxMessageBox("FAT32 option is not enabled.", "Error");
    return OPENFSL_SUCCESS;
}

error_t FAT32Command::ListDirectoryChild(std::vector<FileInfo>* outList, const std::string path) {
    std::vector<openfsl::fat32::FAT32::FileInfo> buf;
    outList->clear();

    error_t result = fat32->listDirectory(&buf, path);
    if (result) {
        wxMessageBox(openfsl::geterrorstr(result), "Error");
        return result;
    }

    for (openfsl::fat32::FAT32::FileInfo file : buf) {
        FileInfo outFile;
        outFile.fileName = file.fileName;
        outFile.fileCreateTime = file.fileCreateTime;
        outFile.fileAccessTime = file.fileAccessTime;
        outFile.fileModTime = file.fileModTime;
        outFile.fileSize = file.fileSize;
        if ((file.fileAttr & openfsl::fat32::FAT32::FileAttribute::Archive) != (openfsl::fat32::FAT32::FileAttribute)0) {
            outFile.fileType = FileType::File;
        } else if ((file.fileAttr & openfsl::fat32::FAT32::FileAttribute::Directory) != (openfsl::fat32::FAT32::FileAttribute)0) {
            outFile.fileType = FileType::Directory;
        }

        outList->push_back(outFile);
    }

    return OPENFSL_SUCCESS;
}

error_t FAT32Command::Search() {
    wxMessageBox("FAT32 option is not enabled.", "Error");
    return OPENFSL_SUCCESS;
}

openfsl::fat32::FAT32* FAT32Command::getFileSystem() {
    return fat32;
}

void FAT32Command::Deinitialize() {
    delete fat32;
}
