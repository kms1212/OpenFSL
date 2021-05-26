/* Copyright (c) 2020. kms1212(권민수)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "../header/fs_fat32/fs_fat32.h"

using namespace openFSL;

uint32_t FS_FAT32::getNextCluster(uint32_t cluster)
{
	uint32_t temp = cluster & ~(0xF << 28);
	temp = fatClusterList[temp] & ~(0xF << 28);
	if (0xFFFFFFF - temp <= 7)
		return 0xFFFFFFF8;
	else if (0xFFFFFFF - temp == 8)
		return 0xFFFFFFF7;
	else
		return temp;
}