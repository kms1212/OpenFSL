/* Copyright (c) 2021. kms1212(Min Su Kwon)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "../header/fs_fat32/fs_fat32.h"

using namespace openFSL;

uint32_t FS_FAT32::getNextCluster(uint32_t cluster)
{
	uint32_t temp = cluster & ~(0xF << 28);     // Ignore first 4 bits
	temp = fatClusterList[temp] & ~(0xF << 28); // Get next cluster
	if (0xFFFFFFF - temp <= 7)                  // Check bad cluster or cluster end
		return 0xFFFFFFF8;
	else if (0xFFFFFFF - temp == 8)
		return 0xFFFFFFF7;
	else
		return temp;
}

uint32_t FS_FAT32::getLinkedClusterCount(uint32_t cluster)
{
	uint32_t count = 0;
	for (uint32_t temp = cluster; (temp != 0xFFFFFFF7) && (temp != 0xFFFFFFF8); temp = getNextCluster(temp)) // Repeat getNextCluster() while end
		count++;
	return count;
}

int FS_FAT32::getLinkedCluster(Sector* sector, uint32_t cluster, uint32_t count)
{
	uint32_t linkedClusterCount = getLinkedClusterCount(cluster);
	if (linkedClusterCount == 0)
	{
		return 1;
	}
	
	int clusterCursor = cluster;
	int clusterCountToRead = ((count != 0) && count < linkedClusterCount) ? count : linkedClusterCount; // Get linked cluster count and compare with given count and set smaller value
	
	for (int i = 0; i < clusterCountToRead; i++)
	{
		Sector temp(1, dd->getBytespersector());
		dd->readDisk(&temp, resvSectorCount + fatSize32 * fatCount + clusterCursor - rootCluster, sectorPerCluster);
		
		memcpy(sector->getData() + i * dd->getBytespersector(), temp.getData(), dd->getBytespersector());
		clusterCursor = getNextCluster(clusterCursor);
		if (clusterCursor == 0xFFFFFFF7)
			return 1;
		if (clusterCursor == 0xFFFFFFF8)
			break;
	}
	
	return 0;
}