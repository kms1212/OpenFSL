/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/fat32/fs_fat32.h"

error_t openfsl::FAT32::__forEachEntry(
    std::function<bool(FileInfo)> forEachFunction, const std::string path) {
    std::string tempPath = currentPath;
    cluster_t tempCluster = currentCluster;

    if (path != "") {  // Change directory if path is set
        int errcode = __chdir(path);
        if (errcode) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            return errcode;
        }
    }

    LinkedCluster lcluster(this, currentCluster);
    FileEntry* fileEntry = new FileEntry();

    std::string lfnBuf = "";
    size_t entrySize = 0;

    if (lcluster.getClusterList()->size() * 16 > 65536)
        return OPENFSL_ERROR_TOO_LARGE_ENTRY_LIST;

    // Add entries to buffer
    for (size_t i = 0; i < lcluster.getClusterList()->size() * 16; i++) {
        error_t result = lcluster.read(
            fileEntry, i * sizeof(FileEntry), sizeof(FileEntry));

        if (result) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            delete fileEntry;
            return result;
        }

        // Return if search hits bottom of the directory file entry list
        if (fileEntry->fileName[0] == 0x00) {
            currentPath = tempPath;
            currentCluster = tempCluster;
            delete fileEntry;
            return 0;
        } else if ((fileEntry->fileName[0] != 0xE5)) {
            // If file is not deleted
            entrySize++;

            if (fileEntry->fileAttr == FileAttribute::LFN) {
                // If entry attribute is LFN, add data to lfn buffer

                std::stringstream filenamebuf;
                LFNEntry* lfnEntry = reinterpret_cast<LFNEntry*>(fileEntry);

                size_t copySize = 0;
                for (copySize = 0; copySize < 5; copySize++)
                    if (lfnEntry->lfnFileName1[copySize] == 0xFFFF)
                        break;
                if (copySize) {
                    char16_t* nametmpseq = new char16_t[copySize + 1];
                    memcpy(nametmpseq, lfnEntry->lfnFileName1,
                        copySize * sizeof(char16_t));
                    nametmpseq[copySize] = 0;
                    std::u16string nametmpstr(nametmpseq);

                    filenamebuf << __ucs2ToU8(nametmpseq);

                    delete[] nametmpseq;
                }

                for (copySize = 0; copySize < 6; copySize++)
                    if (lfnEntry->lfnFileName2[copySize] == 0xFFFF)
                        break;
                if (copySize) {
                    char16_t* nametmpseq = new char16_t[copySize + 1];
                    memcpy(nametmpseq, lfnEntry->lfnFileName2,
                        copySize * sizeof(char16_t));
                    nametmpseq[copySize] = 0;
                    std::u16string nametmpstr(nametmpseq);

                    filenamebuf << __ucs2ToU8(nametmpseq);

                    delete[] nametmpseq;
                }

                for (copySize = 0; copySize < 2; copySize++)
                    if (lfnEntry->lfnFileName3[copySize] == 0xFFFF)
                        break;
                if (copySize) {
                    char16_t* nametmpseq = new char16_t[copySize + 1];
                    memcpy(nametmpseq, lfnEntry->lfnFileName3,
                        copySize * sizeof(char16_t));
                    nametmpseq[copySize] = 0;
                    std::u16string nametmpstr(nametmpseq);

                    filenamebuf << __ucs2ToU8(nametmpseq);

                    delete[] nametmpseq;
                }

                lfnBuf.insert(0, filenamebuf.str());
            } else {
                bool isLfn = false;
                std::string filename;
                FileInfo fileInfo;  // Copy information to buf

                if (lfnBuf != "") {
                    filename = lfnBuf;
                    lfnBuf = "";
                    fileInfo.fileName = filename;
                    isLfn = true;
                }

                filename = reinterpret_cast<char*>(fileEntry->fileName);
                std::string fileext;
                if (fileext != "   ") {
                    fileext = filename.substr(8, 3);
                    fileext.erase(fileext.find_last_not_of(" ") + 1);
                }
                filename = filename.substr(0, 8);
                filename.erase(filename.find_last_not_of(" ") + 1);
                if (!fileext.empty())
                    filename += "." + fileext;

                if (!isLfn)  // if current file name is SFN
                    fileInfo.fileName = filename;
                fileInfo.fileSFNName = filename;
                fileInfo.fileAttr = fileEntry->fileAttr;

                fileInfo.fileCreateTime = toFslTime(
                    fileEntry->fileCreateTime, fileEntry->fileCreateDate);

                fileInfo.fileModTime = toFslTime(
                    fileEntry->fileModTime, fileEntry->fileModDate);

                fileInfo.fileAccessTime = toFslTime(fileEntry->fileAccessDate);

                fileInfo.fileSize = leToSystem<uint32_t>(fileEntry->fileSize);
                fileInfo.fileLocation =
                    static_cast<cluster_t>(
                        (leToSystem<uint16_t>(
                            fileEntry->fileLocationHigh) << 16) +
                        leToSystem<uint16_t>(fileEntry->fileLocationLow));
                fileInfo.fileEntryIndex = i;
                fileInfo.fileEntrySize = entrySize;

                if (forEachFunction(fileInfo)) {
                    break;
                }

                entrySize = 0;
            }
        }
    }

    currentPath = tempPath;
    currentCluster = tempCluster;

    delete fileEntry;
    return 0;
}
