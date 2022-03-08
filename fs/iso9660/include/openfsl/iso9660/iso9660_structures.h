/* Copyright (c) 2021. kms1212(Minsu Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef FS_ISO9660_INCLUDE_OPENFSL_ISO9660_ISO9660_STRUCTURES_H_
#define FS_ISO9660_INCLUDE_OPENFSL_ISO9660_ISO9660_STRUCTURES_H_

#ifndef FS_ISO9660_H_INTERNAL_
#error This file has to be included by fs_ntfs.h.
#endif  // FS_ISO9660_H_INTERNAL_

    /**
     * @brief ISO9660 Volume Descriptor for determining partition type
     */
#pragma pack(push, 1)
    typedef struct VolumeDescriptor {
        uint8_t        descType;
        uint8_t        descSignature[5];
        uint8_t        descVersion;
        uint8_t        descData[2041];
    } VolumeDescriptor;
#pragma pack(pop)

#endif  // FS_ISO9660_INCLUDE_OPENFSL_ISO9660_ISO9660_STRUCTURES_H_
