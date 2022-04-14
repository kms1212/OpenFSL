/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
Check the full BSD-3-Clause license for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_NTFS_INCLUDE_OPENFSL_NTFS_NTFS_STRUCTURES_H_
#define FS_NTFS_INCLUDE_OPENFSL_NTFS_NTFS_STRUCTURES_H_

#ifndef FS_NTFS_H_INTERNAL_
#error This file has to be included by fs_ntfs.h.
#endif  // FS_NTFS_H_INTERNAL_

    /**
     * @brief NTFS VBR
     * @details
     * Type       | Name                     | Offset
     * -----------|--------------------------|----------
     * uint8_t    | vbrJumpCode[3]           | 0x0000
     * uint8_t    | vbrOEMName[8]            | 0x0003
     * uint16_t   | vbrBytesPerSector        | 0x000B
     * uint8_t    | vbrSectorsPerCluster     | 0x000D
     * uint16_t   | vbrReservedSectorCount   | 0x000E
     * uint8_t    | __Reserved1[5]           | 0x0010
     * uint8_t    | vbrMediaType             | 0x0015
     * uint8_t    | __Reserved2[18]          | 0x0016
     * uint64_t   | vbrTotalSectors          | 0x0028
     * uint64_t   | vbrMFTStartCluster       | 0x0030
     * uint64_t   | vbrMFTMirrorStartCluster | 0x0038
     * uint8_t    | vbrClustersPerRecord     | 0x0040
     * uint8_t    | __Reserved3[3]           | 0x0041
     * uint8_t    | vbrClustersPerIndex      | 0x0044
     * uint8_t    | __Reserved4[3]           | 0x0045
     * uint64_t   | vbrVolumeSerial          | 0x0048
     * uint8_t    | __Reserved5[4]           | 0x0050
     * uint8_t    | vbrBootCode[426]         | 0x0054
     * uint16_t   | vbrSignature             | 0x01FE
     */
#pragma pack(push, 1)
    typedef struct VBRSector {
        uint8_t        vbrJumpCode[3];
        uint8_t        vbrOEMName[8];
        uint16_t       vbrBytesPerSector;
        uint8_t        vbrSectorsPerCluster;
        uint16_t       vbrReservedSectorCount;
        uint8_t        __Reserved1[5];
        uint8_t        vbrMediaType;
        uint8_t        __Reserved2[18];
        uint64_t       vbrTotalSectors;
        uint64_t       vbrMFTStartCluster;
        uint64_t       vbrMFTMirrorStartCluster;
        uint8_t        vbrClustersPerRecord;
        uint8_t        __Reserved3[3];
        uint8_t        vbrClustersPerIndex;
        uint8_t        __Reserved4[3];
        uint64_t       vbrVolumeSerial;
        uint8_t        __Reserved5[4];
        uint8_t        vbrBootCode[426];
        uint16_t       vbrSignature;
    } VBRSector;
#pragma pack(pop)

    /**
     * @brief NTFS MFT Flag
     * @details
     * Name            | Value
     * ----------------|---------
     * mftUsing        | 0x1
     * mftDirectory    | 0x2
     */
    enum class MFTFlags : uint16_t {
        mftUsing = 0x1,
        mftDirectory = 0x2
    };

    friend inline NTFS::MFTFlags operator|
        (NTFS::MFTFlags a, NTFS::MFTFlags b)
    { return (NTFS::MFTFlags)((uint16_t)a | (uint16_t)b); }

    friend inline NTFS::MFTFlags operator&
        (NTFS::MFTFlags a, NTFS::MFTFlags b)
    { return (NTFS::MFTFlags)((uint16_t)a & (uint16_t)b); }

    /**
     * @brief NTFS MFT $STANDARD_INFORMATION Flags
     * @details
     * Name                | Value
     * --------------------|---------
     * siReadOnly          | 0x0001
     * siHidden            | 0x0002
     * siSystem            | 0x0004
     * siArchive           | 0x0020
     * siDevice            | 0x0040
     * siNormal            | 0x0080
     * siTemporary         | 0x0100
     * siSparseFile        | 0x0200
     * siReparsePoint      | 0x0400
     * siCompressed        | 0x0800
     * siOffline           | 0x1000
     * siNotContentIndexed | 0x2000
     * siEncrypted         | 0x4000
     */
    enum class StdInfoFlags : uint32_t {
        siReadOnly = 0x0001,
        siHidden = 0x0002,
        siSystem = 0x0004,
        siArchive = 0x0020,
        siDevice = 0x0040,
        siNormal = 0x0080,
        siTemporary = 0x0100,
        siSparseFile = 0x0200,
        siReparsePoint = 0x0400,
        siCompressed = 0x0800,
        siOffline = 0x1000,
        siNotContentIndexed = 0x2000,
        siEncrypted = 0x4000
    };

    friend inline NTFS::StdInfoFlags operator|
        (NTFS::StdInfoFlags a, NTFS::StdInfoFlags b)
    { return (NTFS::StdInfoFlags)((uint32_t)a | (uint32_t)b); }

    friend inline NTFS::StdInfoFlags operator&
        (NTFS::StdInfoFlags a, NTFS::StdInfoFlags b)
    { return (NTFS::StdInfoFlags)((uint32_t)a & (uint32_t)b); }

    /**
     * @brief NTFS MFT $FILE_NAME Flags
     * @details
     * Name                | Value
     * --------------------|-----------
     * fnReadOnly          | 0x00000001
     * fnHidden            | 0x00000002
     * fnSystem            | 0x00000004
     * fnArchive           | 0x00000020
     * fnDevice            | 0x00000040
     * fnNormal            | 0x00000080
     * fnTemporary         | 0x00000100
     * fnSparseFile        | 0x00000200
     * fnReparsePoint      | 0x00000400
     * fnCompressed        | 0x00000800
     * fnOffline           | 0x00001000
     * fnNotContentIndexed | 0x00002000
     * fnEncrypted         | 0x00004000
     * fnDirectory         | 0x10000000
     * fnIndexView         | 0x20000000
     */
    enum class FileNameFlags : uint32_t {
        fnReadOnly = 0x0001,
        fnHidden = 0x0002,
        fnSystem = 0x0004,
        fnArchive = 0x0020,
        fnDevice = 0x0040,
        fnNormal = 0x0080,
        fnTemporary = 0x0100,
        fnSparseFile = 0x0200,
        fnReparsePoint = 0x0400,
        fnCompressed = 0x0800,
        fnOffline = 0x1000,
        fnNotContentIndexed = 0x2000,
        fnEncrypted = 0x4000,
        fnDirectory = 0x10000000,
        fnIndexView = 0x20000000
    };

    friend inline NTFS::FileNameFlags operator|
        (NTFS::FileNameFlags a, NTFS::FileNameFlags b)
    { return (NTFS::FileNameFlags)((uint32_t)a | (uint32_t)b); }

    friend inline NTFS::FileNameFlags operator&
        (NTFS::FileNameFlags a, NTFS::FileNameFlags b)
    { return (NTFS::FileNameFlags)((uint32_t)a & (uint32_t)b); }

    /**
     * @brief NTFS File Reference Address
     */
    typedef struct FileRefAddr {
        uint64_t mftEntryAddress : 48;
        uint64_t sequenceNumber : 16;
    } FileRefAddr;

    /**
     * @brief NTFS MFT Entry Header
     * @details
     * Type       | Name                     | Offset
     * -----------|--------------------------|----------
     * uint8_t    | mftSignature[4]          | 0x0000
     * uint16_t   | mftFixupArrayOffset      | 0x0004
     * uint16_t   | mftFixupArrayCount       | 0x0006
     * uint64_t   | mftLogSequenceNum        | 0x0008
     * uint16_t   | mftSequenceValue         | 0x0010
     * uint16_t   | mftHardLinkCount         | 0x0012
     * uint16_t   | mftAttributeOffset       | 0x0014
     * uint16_t   | mftFlags                 | 0x0016
     * uint32_t   | mftSizeUsed              | 0x0018
     * uint32_t   | mftSizeAllocated         | 0x001C
     * uint64_t   | mftBaseMFTFileReference  | 0x0020
     * uint16_t   | mftNextAttributeID       | 0x0028
     * uint16_t   | mftFixupPattern          | 0x002A
     * uint32_t   | mftRecordNumber          | 0x002C
     */
#pragma pack(push, 1)
    typedef struct MFTEntryHeader {
        uint8_t        mftSignature[4];
        uint16_t       mftFixupArrayOffset;
        uint16_t       mftFixupArrayCount;
        uint64_t       mftLogSequenceNum;
        uint16_t       mftSequenceValue;
        uint16_t       mftHardLinkCount;
        uint16_t       mftAttributeOffset;
        uint16_t       mftFlags;
        uint32_t       mftSizeUsed;
        uint32_t       mftSizeAllocated;
        uint64_t       mftBaseMFTFileReference;
        uint16_t       mftNextAttributeID;
        uint16_t       mftFixupPattern;
        uint32_t       mftRecordNumber;
    } MFTEntryHeader;
#pragma pack(pop)

    /**
     * @brief NTFS MFT Entry Attribute
     */
    enum class MFTEntryAttribute : uint32_t {
        STANDARD_INFORMATION = 0x10,
        ATTRIBUTE_LIST = 0x20,
        FILE_NAME = 0x30,
        VOLUME_VERSION = 0x40,
        OBJECT_ID = 0x40,
        SECURITY_DESCRIPTOR = 0x50,
        VOLUME_NAME = 0x60,
        VOLUME_INFORMATION = 0x70,
        DATA = 0x80,
        INDEX_ROOT = 0x90,
        INDEX_ALLOCATION = 0xA0,
        BITMAP = 0xB0,
        SYMBOLIC_LINK = 0xC0,
        REPARSE_POINT = 0xC0,
        EA_INFORMATION = 0xD0,
        EA = 0xE0,
        LOGGED_UTILITY_STREAM = 0xF0,
        END = 0xFFFFFFFF
    };

    /**
     * @brief NTFS File Name Type
     */
    enum class FileNameType : uint8_t {
        POSIX = 0,
        Win32 = 1,
        DOS = 2,
        Win32_DOS = 3
    };

    /**
     * @brief NTFS MFT Resident Entry Header
     * @details
     * Type              | Name                     | Offset
     * ------------------|--------------------------|----------
     * MFTEntryAttribute | mftTypeID                | 0x0000
     * uint32_t          | mftAttributeLength       | 0x0004
     * uint8_t           | mftFormCode              | 0x0008
     * uint8_t           | mftNameLength            | 0x0009
     * uint16_t          | mftNameOffset            | 0x000A
     * uint16_t          | mftFlags                 | 0x000C
     * uint16_t          | mftAttribID              | 0x000E
     * uint32_t          | mftContentLength         | 0x0010
     * uint16_t          | mftContentOffset         | 0x0014
     * uint8_t           | __Reserved1[2]           | 0x0016
     */
#pragma pack(push, 1)
    typedef struct MFTResidentAttrHeader {
        MFTEntryAttribute mftTypeID;
        uint32_t          mftAttributeLength;
        uint8_t           mftFormCode;
        uint8_t           mftNameLength;
        uint16_t          mftNameOffset;
        uint16_t          mftFlags;
        uint16_t          mftAttribID;
        uint32_t          mftContentLength;
        uint16_t          mftContentOffset;
        uint8_t           __Reserved1[2];
    } MFTResidentAttrHeader;
#pragma pack(pop)

    /**
     * @brief NTFS MFT Non-Resident Entry Header
     * @details
     * Type              | Name                     | Offset
     * ------------------|--------------------------|----------
     * MFTEntryAttribute | mftTypeID                | 0x0000
     * uint32_t          | mftAttributeLength       | 0x0004
     * uint8_t           | mftFormCode              | 0x0008
     * uint8_t           | mftNameLength            | 0x0009
     * uint16_t          | mftNameOffset            | 0x000A
     * uint16_t          | mftFlags                 | 0x000C
     * uint16_t          | mftAttribID              | 0x000E
     * uint64_t          | mftStartVirtualCluster   | 0x0010
     * uint64_t          | mftEndVirtualCluster     | 0x0018
     * uint16_t          | mftRunlistOffset         | 0x0020
     * uint16_t          | mftCompressionUnitSize   | 0x0022
     * uint8_t           | __Reserved1[4]           | 0x0024
     * uint64_t          | mftContentSize           | 0x0028
     * uint64_t          | mftDiskContentSize       | 0x0030
     * uint64_t          | mftInitilizedContentSize | 0x0038
     */
#pragma pack(push, 1)
    typedef struct MFTNonResidentAttrHeader {
        MFTEntryAttribute mftTypeID;
        uint32_t          mftAttributeLength;
        uint8_t           mftFormCode;
        uint8_t           mftNameLength;
        uint16_t          mftNameOffset;
        uint16_t          mftFlags;
        uint16_t          mftAttribID;
        uint64_t          mftStartVirtualCluster;
        uint64_t          mftEndVirtualCluster;
        uint16_t          mftRunlistOffset;
        uint16_t          mftComporessionUnitSize;
        uint8_t           __Reserved1[4];
        uint64_t          mftContentSize;
        uint64_t          mftDiskContentSize;
        uint64_t          mftInitializedContentSize;
    } MFTNonResidentAttrHeader;
#pragma pack(pop)

    /**
     * @brief NTFS MFT $STANDARD_INFORMATION Attribute
     * @details
     * Type       | Name                     | Offset
     * -----------|--------------------------|----------
     * filetime_t | siDateCreated            | 0x0000
     * filetime_t | siDateModified           | 0x0008
     * filetime_t | siDateMFTRecordModified  | 0x0010
     * filetime_t | siDateAccessed           | 0x0018
     * uint32_t   | siFlags                  | 0x0020
     * uint32_t   | siMaxVersions            | 0x0024
     * uint32_t   | siVersionNumber          | 0x0028
     * uint32_t   | siClassID                | 0x002C
     * uint32_t   | siOwnerID                | 0x0030
     * uint32_t   | siSecurityID             | 0x0034
     * uint64_t   | siQuotaCharged           | 0x0038
     * uint64_t   | siUsn                    | 0x0040
     */
#pragma pack(push, 1)
    typedef struct MFTStandardInformation {
        filetime_t   siDateCreated;
        filetime_t   siDateModified;
        filetime_t   siDateMFTRecordModified;
        filetime_t   siDateAccessed;
        StdInfoFlags siFlags;
        uint32_t     siMaxVersions;
        uint32_t     siVersionNumber;
        uint32_t     siClassID;
        uint32_t     siOwnerID;
        uint32_t     siSecurityID;
        uint64_t     siQuotaCharged;
        uint64_t     siUsn;
    } MFTStandardInformation;
#pragma pack(pop)

    /**
     * @brief NTFS MFT $FILE_NAME Attribute
     * @details
     * Type         | Name                     | Offset
     * -------------|--------------------------|----------
     * FileRefAddr  | fnParentDirectory        | 0x0000
     * filetime_t   | fnDateCreated            | 0x0008
     * filetime_t   | fnDateModified           | 0x0010
     * filetime_t   | fnDateMFTRecordModified  | 0x0018
     * filetime_t   | fnDateAccessed           | 0x0020
     * uint64_t     | fnFileSize               | 0x0028
     * uint64_t     | fnAllocatedFileSize      | 0x0030
     * uint32_t     | fnFlags                  | 0x0038
     * uint32_t     | fnReparseValue           | 0x003C
     * uint8_t      | fnNameLength             | 0x0040
     * FileNameType | fnNameType               | 0x0041
     * uint16_t     | fnNameStart              | 0x0042
     */
#pragma pack(push, 1)
    typedef struct MFTFileName {
        FileRefAddr   fnParentDirectory;
        filetime_t    fnDateCreated;
        filetime_t    fnDateModified;
        filetime_t    fnDateMFTRecordModified;
        filetime_t    fnDateAccessed;
        uint64_t      fnFileSize;
        uint64_t      fnAllocatedFileSize;
        FileNameFlags fnFlags;
        uint32_t      fnReparseValue;
        uint8_t       fnNameLength;
        FileNameType  fnNameType;
        uint16_t      fnNameStart;
    } MFTFileName;
#pragma pack(pop)

#endif  // FS_NTFS_INCLUDE_OPENFSL_NTFS_NTFS_STRUCTURES_H_
