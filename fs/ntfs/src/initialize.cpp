/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "openfsl/ntfs/fs_ntfs.h"

extern void __hexdump(const uint8_t* p, size_t offset, size_t len);

error_t openfsl::NTFS::initialize() {
    error_t result = FileSystem::initialize();
    if (result)
        return result;

    // Read BPB sector
    result = iod->readSector(&vbr, getActualLBA(0), 1);
    if (result)
        return result;

    if (memcmp(vbr.vbrOEMName, "NTFS    ", 8) != 0)
        return OPENFSL_ERROR_INVALID_SIGNATURE;

    if (vbr.vbrSignature != 0xAA55)  // Check BPB signature
        return OPENFSL_ERROR_INVALID_SIGNATURE;

    currentPath = rootSymbol;

    Sector mftsect(vbr.vbrSectorsPerCluster,
        iod->getDiskParameter().bytesPerSector);

    for (cluster_t clustercursor = 0;; clustercursor++) {
        result = iod->readSector(mftsect.getData(),
            (vbr.vbrMFTStartCluster + clustercursor) * vbr.vbrSectorsPerCluster,
            vbr.vbrSectorsPerCluster);
        if (result)
            return result;

        bool isSignatureValid;

        for (uint64_t currentMftHeaderLoc = 0;
            currentMftHeaderLoc < vbr.vbrSectorsPerCluster *
                iod->getDiskParameter().bytesPerSector;) {
            MFTEntryHeader* mftheader = mftsect.getDataCast<MFTEntryHeader>();
            MFTResidentAttrHeader* attrheader;
            uint64_t currentMftCursor = mftheader->mftAttributeOffset;

            std::cout << "Current MFT Header Location(byte): " << currentMftHeaderLoc<< "\n";
            std::cout << "Current MFT Cursor(byte): " << currentMftCursor << "\n";
            isSignatureValid =
                std::memcmp(mftheader->mftSignature, "FILE", 4) == 0;
            if (isSignatureValid) {
                for (;;) {
                    attrheader =
                        mftsect.getDataCast<MFTResidentAttrHeader>
                        (currentMftHeaderLoc + currentMftCursor);
                    std::cout << currentMftCursor << " "
                        << std::hex << static_cast<int>(attrheader->mftTypeID)
                        << std::dec << ":\n";

                    //__hexdump(reinterpret_cast<uint8_t*>(attrheader), 0,
                    //    sizeof(MFTResidentAttrHeader));

                    if (attrheader->mftTypeID == MFTEntryAttribute::END) {
                        currentMftHeaderLoc += mftheader->mftSizeAllocated;

                        break;
                    } else if (attrheader->mftFormCode == 0) {
                        switch (attrheader->mftTypeID) {
                        case MFTEntryAttribute::STANDARD_INFORMATION: {
                            MFTStandardInformation* attrdata =
                                mftsect.getDataCast<MFTStandardInformation>
                                (currentMftHeaderLoc + currentMftCursor +
                                    attrheader->mftContentOffset);

                            std::cout << "============= $STANDARD_INFORMATION =============\n";
                            std::cout << "Date Created: " << attrdata->siDateCreated << "\n";
                            std::cout << "Date Modified: " << attrdata->siDateModified << "\n";
                            std::cout << "Date MFT Modified: " << attrdata->siDateMFTRecordModified << "\n";
                            std::cout << "Date Accessed: " << attrdata->siDateAccessed << "\n";
                            std::cout << "=================================================\n\n";
                            break;
                        }

                        case MFTEntryAttribute::FILE_NAME: {
                            MFTFileName* attrdata = mftsect.getDataCast<MFTFileName>
                                (currentMftHeaderLoc + currentMftCursor +
                                    attrheader->mftContentOffset);

                            char16_t* fileNameBuffer = new char16_t[attrdata->fnNameLength + 1];
                            std::memset(fileNameBuffer, 0, (attrdata->fnNameLength + 1) * sizeof(char16_t));
                            std::memcpy(fileNameBuffer, &attrdata->fnNameStart, attrdata->fnNameLength * sizeof(char16_t));

                            std::cout << "================== $FILE_NAME ===================\n";
                            std::cout << "File Name: " << __ucs2ToU8(std::u16string(fileNameBuffer)) << "\n";
                            std::cout << "File Size: " << attrdata->fnFileSize << "\n";
                            std::cout << "=================================================\n\n";

                            delete[] fileNameBuffer;
                            break;
                        }

                        case MFTEntryAttribute::DATA: {
                            uint8_t* attrdata = mftsect.getDataCast<uint8_t>
                                (currentMftHeaderLoc + currentMftCursor +
                                    attrheader->mftContentOffset);

                            std::cout << "================= $DATA resident ================\n";
                            __hexdump(attrdata, attrheader->mftContentOffset, attrheader->mftContentLength);
                            std::cout << "=================================================\n\n"; 
                            break;
                        }

                        default: {
                            std::cout << "Unrecognizable MFT Entry type\n";
                            break;
                        }
                        }
                    } else if (attrheader->mftFormCode == 1) {
                        std::cout << "Non-Resident MFT Entry\n\n";
                    } else {
                        break;
                    }

                    currentMftCursor += attrheader->mftAttributeLength;
                }
            } else {
                break;
            }
        }

        if (!isSignatureValid)
            break;
    }


    return 0;
}
