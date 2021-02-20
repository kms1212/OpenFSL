#include "type.h"
#include "string.h"
#include "memory.h"
#include "driver_ata_pio.h"
#include "io.h"
#include "fat.h"
#include "elf.h"

FILE* findFile(BPB* bpb, uint32_t offset, uint32_t sector, char* fileName, uint8_t attrib);

void main_64(void)
{
	enableCursor(0xCD, 0xED);
	coord pos = {24, 6};
	
	setCursorPosition(pos);
	print("OK");

	pos.x = 0;
	pos.y = 7;
	setCursorPosition(pos);
	print("Detecting volume... ");
	
	int offset = 0;
	uint16_t* flag = (uint16_t*)0x3101FC;
	do {
		ATAPIORead((void*)0x310000, offset, 1);
		offset++;
	} while (*flag != 0x1122);
	offset--;
	print("OK\r\n");
	print("\r\n");
	
	memset((void*)0x310000, 0x00, 512);
	
	BPB* bpb = (BPB*)0x7C00;
	FILE* file;
	print("Reading Root Directory : LBA ");
	printi(offset + bpb->bpbReservedSectors + (bpb->ebpbSectorsPerFAT32 * bpb->bpbFATCount) * bpb->bpbSectorPerCluster, 10);
	print("\r\n");
	
	
	file = findFile(bpb, offset, offset + bpb->bpbReservedSectors + (bpb->ebpbSectorsPerFAT32 * bpb->bpbFATCount) * bpb->bpbSectorPerCluster, "BOOT       ", 0x10);
	uint32_t location = (file->fileLocationHigh << 16) + file->fileLocationLow - 2;
	print("\r\n");
	
	if (location != 0)
	{
		print("Reading BOOT Directory : LBA ");
		printi(offset + bpb->bpbReservedSectors + (bpb->ebpbSectorsPerFAT32 * bpb->bpbFATCount + location) * bpb->bpbSectorPerCluster, 10);
		print("\r\n");
		
		file = findFile(bpb, offset, offset + bpb->bpbReservedSectors + (bpb->ebpbSectorsPerFAT32 * bpb->bpbFATCount + location) * bpb->bpbSectorPerCluster, "LOADER  BIN", 0x20);
		location = (file->fileLocationHigh << 16) + file->fileLocationLow - 2;
		print("\r\n");
		if (location != 0)
		{
			ATAPIORead((void*)0x310000, offset + bpb->bpbReservedSectors + (bpb->ebpbSectorsPerFAT32 * bpb->bpbFATCount + location) * bpb->bpbSectorPerCluster, file->fileSize / bpb->bpbBytesPerSector + 1);
			print("Jumping to loaded bootloader file...\r\n");
			int errorCode = ((int (*)(void))0x310000)();
			print("\r\nAn error occured while boot sequence. please restart the computer.\r\n0x");
			printi(errorCode, 16);
		}
		else
			print("Bootloader file is missing");
	}

	while (1);
}

FILE* findFile(BPB* bpb, uint32_t offset, uint32_t sector, char* fileName, uint8_t attrib)
{
	ATAPIORead((void*)0x310000, sector, bpb->bpbSectorPerCluster);
	print("FILENAME EXT\tLOC \tSIZE\r\n");
	for(int i = 0, j = 0; ; i++)
	{
		if (i * 0x20 > bpb->bpbSectorPerCluster * bpb->bpbBytesPerSector)
		{
			j += bpb->bpbSectorPerCluster;
			ATAPIORead((void*)0x310000, sector + j, bpb->bpbSectorPerCluster);
		}
		FILE* file = (FILE*)(uint64_t)(0x310000 + i * 0x20);
		if (!file->fileName)
			break;
		if (file->fileAttr == 0x0F)
			continue;
		
		printn(file->fileName, 8);
		print(" ");
		printn(file->fileExt, 3);
		print("\t");
		
		uint32_t fileLoc = (file->fileLocationHigh << 16) + file->fileLocationLow;
		printi(fileLoc, 10);
		print("\t");
		
		uint32_t fileSize = file->fileSize;
		printi(fileSize, 10);
		
		if ((strncmp(file->fileName, fileName, 11) == 0) && (file->fileAttr & attrib))
		{
			print ("\t<\r\n");
			return file;
		}
		print("\r\n");
	}
	return 0;
}