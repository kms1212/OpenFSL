/* Copyright (c) 2021. kms1212(권민수)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#include "../header/fs_fat32/fs_fat32.h"

using namespace openFSL;

uint32_t FS_FAT32::getChildCount(std::string path) {
	std::string currentPath_temp = currentPath;
	uint32_t currentCluster_temp = currentCluster;
	
	if (path != "") {
		chdir(path);
	}
	
	int count = 0;
	int countWithDeletedFiles = 0;
	
	uint32_t linkedClusterCount = getLinkedClusterCount(currentCluster); // Get linked cluster count.
	if (linkedClusterCount == 0)
	{
		currentPath = currentPath_temp;
		currentCluster = currentCluster_temp;
		return 0;
	}
	
	Sector dir_cluster(sectorPerCluster * linkedClusterCount, dd->getBytespersector()); // Get entire linked cluster of directory entry
	
	if (getLinkedCluster(&dir_cluster, currentCluster))
	{
		currentPath = currentPath_temp;
		currentCluster = currentCluster_temp;
		return 0;
	}
	
	FAT32_entry* fileEntry = (FAT32_entry*)dir_cluster.getData();
		
	for (int i = 0; i < 16 * linkedClusterCount; i++) {
		if (fileEntry[i].fileAttr == 0x00) // return if entry is blank
		{
			currentPath = currentPath_temp;
			currentCluster = currentCluster_temp;
			return count;
		}
		else {
			if (countWithDeletedFiles < 65536) {
				countWithDeletedFiles++; // Increase entry count
				if (fileEntry[i].fileName[0] != 0xE5 && (fileEntry[i].fileAttr == 0x10 || fileEntry[i].fileAttr == 0x20)) // Increase count if file is exist
					count++;
			} else { // return if entry count is over 65535
				currentPath = currentPath_temp;
				currentCluster = currentCluster_temp;
				return count;
			}
		}
	}
	
	currentPath = currentPath_temp;
	currentCluster = currentCluster_temp;
	return count;
}

uint8_t* str16to8(uint8_t* dest, const uint16_t* src, size_t size) // Temporary function to convert 16bit UCS-2 value to 8bit ASCII (drain)
{
	for (int i = 0; i < size; i++)
		dest[i] = (uint8_t)src[i];
	return dest;
}

FAT32_fileInfo* FS_FAT32::getDirList(FAT32_fileInfo* buf, std::string path) {
	std::string currentPath_temp = currentPath;
	uint32_t currentCluster_temp = currentCluster;
	
	if (path != "") {
		chdir(path);
	}
	
	uint32_t linkedClusterCount = getLinkedClusterCount(currentCluster);
	if (linkedClusterCount == 0)
	{
		currentPath = currentPath_temp;
		currentCluster = currentCluster_temp;
		return NULL;
	}
	
	Sector dir_cluster(sectorPerCluster * linkedClusterCount, dd->getBytespersector());
	
	if (getLinkedCluster(&dir_cluster, currentCluster))
	{
		currentPath = currentPath_temp;
		currentCluster = currentCluster_temp;
		return NULL;
	}
	
	int index = 0;
	
	FAT32_entry* fileEntry = (FAT32_entry*)dir_cluster.getData();
		
	for (int i = 0; i < 16 * linkedClusterCount; i++) {
		if (fileEntry[i].fileAttr == 0x00)
		{
			currentPath = currentPath_temp;
			currentCluster = currentCluster_temp;
			return buf;
		}
		
		if (fileEntry[i].fileName[0] != 0xE5)
		{
			if (fileEntry[i].fileAttr == 0x20 || fileEntry[i].fileAttr == 0x10) {
				std::string filename;
				
				if (fileEntry[i - 1].fileAttr == 0x0F){
					for (int lfnIndex = i - 1; fileEntry[lfnIndex].fileAttr == 0x0F; lfnIndex--)
					{
						char buf[14];
						FAT32_lfn* entry = (FAT32_lfn*)&fileEntry[lfnIndex];
						str16to8((uint8_t*)buf, entry->lfnFileName1, 5);
						str16to8((uint8_t*)(buf + 5), entry->lfnFileName2, 6);
						str16to8((uint8_t*)(buf + 11), entry->lfnFileName3, 2);
						filename += buf;
					}
				}
				else {
					filename = (char*)fileEntry[i].fileName;
					std::string fileext;
					if (fileext != "   ") {
						fileext = filename.substr(8, 3);
						fileext.erase(fileext.find_last_not_of(" ") + 1);
					}
					filename = filename.substr(0, 8);
					filename.erase(filename.find_last_not_of(" ") + 1);
					if (!fileext.empty()) 
						filename += "." + fileext;
				}
				
				buf[index].fileName = filename;
				buf[index].fileAttr = fileEntry[i].fileAttr;
				buf[index].fileCreateTime.time_millis = (fileEntry[i].fileCreateTenth % 10) * 100;
				buf[index].fileCreateTime.time_sec = ((fileEntry[i].fileCreateTime & 0b0000000000011111) * 2) + fileEntry[i].fileCreateTenth / 10;
				buf[index].fileCreateTime.time_min = (fileEntry[i].fileCreateTime & 0b0000011111100000) >> 5;
				buf[index].fileCreateTime.time_hour = (fileEntry[i].fileCreateTime & 0b1111100000000000) >> 11;
				buf[index].fileCreateTime.time_day = (fileEntry[i].fileCreateDate & 0b0000000000011111);
				buf[index].fileCreateTime.time_month = (fileEntry[i].fileCreateDate & 0b0000000111100000) >> 5;
				buf[index].fileCreateTime.time_year = ((fileEntry[i].fileCreateDate & 0b1111111000000000) >> 9) + 1980;
				
				buf[index].fileModTime.time_sec = (fileEntry[i].fileModTime & 0b0000000000011111) * 2;
				buf[index].fileModTime.time_min = (fileEntry[i].fileModTime & 0b0000011111100000) >> 5;
				buf[index].fileModTime.time_hour = (fileEntry[i].fileModTime & 0b1111100000000000) >> 11;
				buf[index].fileModTime.time_day = (fileEntry[i].fileModDate & 0b0000000000011111);
				buf[index].fileModTime.time_month = (fileEntry[i].fileModDate & 0b0000000111100000) >> 5;
				buf[index].fileModTime.time_year = ((fileEntry[i].fileModDate & 0b1111111000000000) >> 9) + 1980;
				
				buf[index].fileAccessTime.time_day = (fileEntry[i].fileAccessDate & 0b0000000000011111);
				buf[index].fileAccessTime.time_month = (fileEntry[i].fileAccessDate & 0b0000000111100000) >> 5;
				buf[index].fileAccessTime.time_year = ((fileEntry[i].fileAccessDate & 0b1111111000000000) >> 9) + 1980;
				
				buf[index].fileSize = fileEntry[i].fileSize;
				buf[index].fileLocation = (fileEntry[i].fileLocationHigh << 16) + fileEntry[i].fileLocationLow;
				
				index++;
			}
		}
	}
	
	currentPath = currentPath_temp;
	currentCluster = currentCluster_temp;
	
	return buf;
}

int FS_FAT32::chdir(std::string path, std::vector<std::string>* subdir) {
	std::string currentPath_temp = currentPath;
	uint32_t currentCluster_temp = currentCluster;
	
	int count = 0;
	int index = 0;
	
	if (subdir == NULL)
	{
		std::vector<std::string> subdir_tmp;
		fsl_strtokenize(path, subdir_tmp, pathSeparator);
		
		if (subdir_tmp.empty())
			return 1;
			
		if (subdir_tmp.front() == "::")
		{
			currentCluster = rootCluster;
			currentPath = subdir_tmp.front();
			if (subdir_tmp.size() <= 1)
				return 0;
			subdir_tmp.erase(subdir_tmp.begin());
			return FS_FAT32::chdir(path, &subdir_tmp);
		}
		else
			return FS_FAT32::chdir(path, &subdir_tmp);
	} 
	
	if (subdir->empty())
		return 0;
	
	uint32_t linkedClusterCount = getLinkedClusterCount(currentCluster);
	if (linkedClusterCount == 0)
	{
		currentPath = currentPath_temp;
		currentCluster = currentCluster_temp;
		return 1;
	}
	
	Sector dir_cluster(sectorPerCluster * linkedClusterCount, dd->getBytespersector());
	
	if (getLinkedCluster(&dir_cluster, currentCluster))
	{
		currentPath = currentPath_temp;
		currentCluster = currentCluster_temp;
		return 1;
	}
	
	FAT32_entry* fileEntry = (FAT32_entry*)dir_cluster.getData();
	
	std::string path_tmp = subdir->front();
	std::transform(path_tmp.begin(), path_tmp.end(), path_tmp.begin(), ::toupper);
	
	for (int i = 0; i < 16 * linkedClusterCount; i++) {
		if (fileEntry[i].fileAttr == 0x00)
			return 1;
		
		if (fileEntry[i].fileName[0] != 0xE5) {
			if (fileEntry[i].fileAttr == 0x10)
			{
				std::string filename;
				if (fileEntry[i - 1].fileAttr == 0x0F){
					for (int lfnIndex = i - 1; fileEntry[lfnIndex].fileAttr == 0x0F; lfnIndex--)
					{
						char buf[14];
						FAT32_lfn* entry = (FAT32_lfn*)&fileEntry[lfnIndex];
						str16to8((uint8_t*)buf, entry->lfnFileName1, 5);
						str16to8((uint8_t*)(buf + 5), entry->lfnFileName2, 6);
						str16to8((uint8_t*)(buf + 11), entry->lfnFileName3, 2);
						filename += buf;
					}
				}
				else {
					filename = (char*)fileEntry[i].fileName;
					filename = filename.substr(0, 8);
					filename.erase(filename.find_last_not_of(" ") + 1);
				}
				std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);
				
				std::cout << i + 1 << " " << filename << "\n";
				
				if (currentCluster == rootCluster) {
					if (path_tmp == "." || path_tmp == "..") {
						subdir->erase(subdir->begin());
						return FS_FAT32::chdir(path, subdir);
					}
				}
				
				if (path_tmp == filename)
				{ 
					if (filename == ".");
					else if (filename == "..")
						currentPath = currentPath.substr(0, currentPath.find_last_of(pathSeparator));
					 else
						currentPath += pathSeparator.at(0) + subdir->front();
					subdir->erase(subdir->begin());
					currentCluster = fileEntry[i].fileLocationHigh * 0x10000 + fileEntry[i].fileLocationLow;
					if (currentCluster == 0)
						currentCluster = rootCluster;
					return FS_FAT32::chdir(path, subdir);
				}
			}
		}
		else
			return 1;
		count++;
	}

	return 1;
}