/* Copyright (c) 2021. kms1212(권민수)
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

uint32_t FS_FAT32::getLinkedClusterCount(uint32_t cluster)
{
	uint32_t count = 0;
	for (uint32_t temp = cluster; temp != 0xFFFFFFF8; temp = getNextCluster(temp))
		count++;
	return count;
}

int FS_FAT32::getLinkedCluster(Sector* sector, uint32_t cluster)
{
	uint32_t linkedClusterCount = getLinkedClusterCount(currentCluster);
	if (linkedClusterCount == 0)
	{
		return 1;
	}
	
	int clusterCursor = currentCluster;
	for (int i = 0; i < linkedClusterCount; i++)
	{
		Sector temp(1, dd->getBytespersector());
		dd->readDisk(&temp, resvSectorCount + fatSize32 * fatCount + clusterCursor - rootCluster, sectorPerCluster);
		
		memcpy(sector->getData() + i * dd->getBytespersector(), temp.getData(), dd->getBytespersector());
		clusterCursor = getNextCluster(clusterCursor);
		if (clusterCursor == 0xFFFFFFF7)
		{
			return 1;
		}
		if (clusterCursor == 0xFFFFFFF8)
			break;
	}
	
	return 0;
}