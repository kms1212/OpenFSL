/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include <regex>

#include "openfsl/fat32/fs_fat32.h"

size_t __getdigitcount(size_t n) {
    size_t ret;
    for (ret = 1;; ret++) {
        if (n >= 10) {
            ret++;
            n /= 10;
        } else {
            break;
        }
    }
    return ret;
}

error_t openfsl::FAT32::__createEntry(
    const std::string path, const FileInfo createFileInfo) {
    std::string tempPath = currentPath;
    cluster_t tempCluster = currentCluster;

    if (createFileInfo.fileSize > std::numeric_limits<uint32_t>::max())
        return OPENFSL_ERROR_TOO_LARGE_FILE_SIZE;

    if (path != "") {  // Change directory if path is set
        int errcode = __chdir(path);
        if (errcode) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return errcode;
        }
    }

    // Validate file name and get required entry count
    size_t requiredEntrySize =
            getEntryCount(createFileInfo.fileName, true);
    size_t dotpos = createFileInfo.fileName.find_last_of('.');

    if (requiredEntrySize == 0) {
        return OPENFSL_ERROR_INVALID_FILE_NAME;
    }
    if (dotpos == std::string::npos) {
        dotpos = createFileInfo.fileName.size();
    }

    // Check if file exists
    std::vector<FileInfo> fileList;
    error_t result = __listDir(&fileList, FileAttribute::All);
    if (result) {
        currentPath = tempPath;
        currentCluster = tempCluster;
        return result;
    }

    std::string searchFilename = createFileInfo.fileName;
    size_t dupEntrySize = 2;

    size_t lfnDuplicateFileCountInSfn = 0;

    for (auto & c : searchFilename) c = static_cast<char>(toupper(c));

recalc:
    for (size_t i = 0; i < fileList.size(); i++) {
        std::string filename = fileList.at(i).fileName;
        for (auto & c : filename) c = static_cast<char>(toupper(c));

        if (filename.substr(0, 8 - dupEntrySize) ==
            searchFilename.substr(0, 8 - dupEntrySize))
            lfnDuplicateFileCountInSfn++;

        if (__getdigitcount(lfnDuplicateFileCountInSfn) != dupEntrySize - 1) {
            dupEntrySize++;
            goto recalc;
        }

        if (searchFilename == filename) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return OPENFSL_ERROR_FILE_OR_DIR_EXISTS;
        }
    }

    // Find free entry
    LinkedCluster lcluster(this, currentCluster);
    size_t entryPlace = 0;
    size_t blankPlaceCount = 0;
    size_t totalEntryCount = lcluster.getClusterList()->size() * 16;

    for (size_t i = 0; i < totalEntryCount + 1; i++) {
        if (i == totalEntryCount) {
            result = lcluster.addCluster(1);
            if (result) {
                currentPath = tempPath;
                currentCluster = tempCluster;
                return result;
            }
            totalEntryCount = lcluster.getClusterList()->size() * 16;
        }

        uint8_t deletionMark;
        result = lcluster.read(&deletionMark, i * 32, 1);
        if (result) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return result;
        }

        if ((deletionMark == 0xE5) || (deletionMark == 0x00))
            blankPlaceCount++;
        else
            blankPlaceCount = 0;

        if (blankPlaceCount >= requiredEntrySize) {
            entryPlace = i;
            break;
        }
    }

    // Create entry
    FileEntry fileEntry;
    fileEntry.fileAttr = createFileInfo.fileAttr;
    fileEntry.fileLocationHigh = (uint16_t)(createFileInfo.fileLocation >> 16);
    fileEntry.fileLocationLow = (uint16_t)createFileInfo.fileLocation;
    fileEntry.fileSize = (uint32_t)createFileInfo.fileSize;

    fileEntry.fileCreateDate = __dateToWord(createFileInfo.fileCreateTime);
    fileEntry.fileCreateTime = __timeToWord(createFileInfo.fileCreateTime);

    fileEntry.fileAccessDate = __dateToWord(createFileInfo.fileAccessTime);

    fileEntry.fileModDate = __dateToWord(createFileInfo.fileModTime);
    fileEntry.fileModTime = __timeToWord(createFileInfo.fileModTime);

    if (requiredEntrySize > 1) {  // If file name is LFN
        // Initialize SFN entry to whitespace
        memset(fileEntry.fileName, ' ', 11);

        std::u16string filename16 = __u8ToUCS2(createFileInfo.fileName);

        // Convert LFN name to SFN

        // Copy first 8 chars from filename to entry and replace every
        // non-ASCII character to underscore
        std::string filename_sfn;
        for (auto c : filename16.substr(0, dotpos > 8 ? 8 : dotpos))
            filename_sfn
                .push_back((c < 0x80) && (c != '.') ?
                    static_cast<char>(toupper(c)) : '_');
        memcpy(fileEntry.fileName, filename_sfn.c_str(), filename_sfn.size());

        // Append duplicate count string to filename
        if (dotpos > 8) {
            std::string sfnDuplicateCountString = std::string("~") +
                std::to_string(lfnDuplicateFileCountInSfn + 1);
            memcpy(fileEntry.fileName + 8 - sfnDuplicateCountString.size(),
                   sfnDuplicateCountString.c_str(),
                   sfnDuplicateCountString.size());
        }

        // Copy first 3 characters from file extension to entry and replace
        // every non-ASCII character to underscore
        if (dotpos != filename16.size()) {
            std::string fileext_sfn;
            size_t extsize = filename16.size() - dotpos;
            for (auto c :
                filename16.substr(dotpos + 1, extsize > 3 ? 3 : extsize))
                fileext_sfn
                    .push_back(c < 0x80 ? static_cast<char>(toupper(c)) : '_');

            memcpy(fileEntry.fileExt, fileext_sfn.c_str(), fileext_sfn.size());
        }

        uint8_t checksum = __chksumSFN(fileEntry.fileName);

        // Write SFN entry
        result = lcluster.write(&fileEntry,
            entryPlace * sizeof(FileEntry), sizeof(FileEntry));
        if (result) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return result;
        }
        requiredEntrySize--;

        // Create lfn entries
        LFNEntry* lfnEntry = reinterpret_cast<LFNEntry*>(&fileEntry);
        lfnEntry->lfnSeqNum = 0;

        for (entryPlace--;; entryPlace--) {
            memset((reinterpret_cast<uint8_t*>(&fileEntry)) + 1, 0,
                sizeof(LFNEntry) - 1);
            lfnEntry->lfnSeqNum++;
            if (requiredEntrySize < 2)
                lfnEntry->lfnSeqNum += 0x40;
            lfnEntry->lfnAttr = FileAttribute::LFN;
            lfnEntry->lfnCheckSum = checksum;

            if (filename16.size() >= 5) {
                memcpy(lfnEntry->lfnFileName1, filename16.c_str(), 10);
                filename16 = filename16.substr(5);
            } else if (filename16.size() > 0) {
                memset(lfnEntry->lfnFileName1, 0xFF, 10);
                memcpy(lfnEntry->lfnFileName1, filename16.c_str(),
                    filename16.length() * sizeof(char16_t));
                filename16.clear();
            } else {
                memset(lfnEntry->lfnFileName1, 0xFF, 10);
            }

            if (filename16.size() >= 6) {
                memcpy(lfnEntry->lfnFileName2, filename16.c_str(), 12);
                filename16 = filename16.substr(6);
            } else if (filename16.size() > 0) {
                memset(lfnEntry->lfnFileName2, 0xFF, 12);
                memcpy(lfnEntry->lfnFileName2, filename16.c_str(),
                    filename16.length() * sizeof(char16_t));
                filename16.clear();
            } else {
                memset(lfnEntry->lfnFileName2, 0xFF, 12);
            }

            if (filename16.size() >= 2) {
                memcpy(lfnEntry->lfnFileName3, filename16.c_str(), 4);
                filename16 = filename16.substr(2);
            } else if (filename16.size() > 0) {
                memset(lfnEntry->lfnFileName3, 0xFF, 4);
                memcpy(lfnEntry->lfnFileName3, filename16.c_str(),
                    filename16.length() * sizeof(char16_t));
                filename16.clear();
            } else {
                memset(lfnEntry->lfnFileName3, 0xFF, 4);
            }

            // Write current LFN entry
            result = lcluster.write(&fileEntry,
                entryPlace * sizeof(FileEntry), sizeof(FileEntry));
            if (result) {
                currentPath = tempPath;
                currentCluster = tempCluster;
                return result;
            }

            if (requiredEntrySize < 2)
                break;

            requiredEntrySize--;
        }
    } else {  // If file name is SFN
        std::string createFilename = createFileInfo.fileName;
        for (auto & c : createFilename) c = static_cast<char>(toupper(c));
        std::string filename_str = createFilename.substr(0, dotpos);
        std::string fileext_str = "";

        if (createFilename == ".") {
            memset(fileEntry.fileName, ' ', 8);
            memset(fileEntry.fileExt, ' ', 3);
            fileEntry.fileName[0] = '.';
        }  else if (createFilename == "..") {
            memset(fileEntry.fileName, ' ', 8);
            memset(fileEntry.fileExt, ' ', 3);
            fileEntry.fileName[0] = '.';
            fileEntry.fileName[1] = '.';
        } else {
            memcpy(fileEntry.fileName, filename_str.c_str(), dotpos);
            memset(fileEntry.fileName + filename_str.size(),
                ' ', 8 - filename_str.size());

            if (dotpos != createFilename.size()) {
                fileext_str = createFilename.substr(dotpos + 1);
            }
            memcpy(fileEntry.fileExt, fileext_str.c_str(), fileext_str.size());
            memset(fileEntry.fileExt + fileext_str.size(),
                ' ', 3 - fileext_str.size());
        }

        result = lcluster.write(&fileEntry,
            entryPlace * sizeof(FileEntry), sizeof(FileEntry));
        if (result) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return result;
        }
    }

    result = lcluster.flush();
    if (result) {
        currentPath = tempPath;
        currentCluster = tempCluster;
        return result;
    }

    currentPath = tempPath;
    currentCluster = tempCluster;

    return 0;
}
