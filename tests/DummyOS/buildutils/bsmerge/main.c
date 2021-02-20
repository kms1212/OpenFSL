#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
	if (argc < 4) {
		printf("Usage : %s <dest> <src(file to merge from)> <src:offset(hex)> <src(file to merge to)>\n", argv[0]);
		return 1;
	}
	
	int srcoffset = (int)strtol(argv[3], NULL, 16);
	
	FILE* fp;
	int srcflen;
	uint8_t* srcbuf;

	// Read src file "merge to"
	fp = fopen(argv[4], "rb");
	if (fp == NULL) {
		perror("fopen()");
		return EXIT_FAILURE;
	}
	
	
	fseek(fp, 0, SEEK_END);
	srcflen = ftell(fp);
	srcbuf = (uint8_t*)malloc(srcflen * sizeof(uint8_t));
	
	fseek(fp, 0, SEEK_SET);
	fread(srcbuf, sizeof(uint8_t), srcflen, fp);
	
	fclose(fp);
	
	// Read src file "merge from"
	uint8_t* destbuf;
	int destflen;

	fp = fopen(argv[2], "rb");
	if (fp == NULL) {
		perror("fopen()");
		return EXIT_FAILURE;
	}

	fseek(fp, 0, SEEK_END);
	destflen = ftell(fp);
	destbuf = (uint8_t*)malloc(srcflen * sizeof(uint8_t));

	fseek(fp, 0, SEEK_SET);
	fread(destbuf, sizeof(uint8_t), destflen, fp);

	fclose(fp);

	// Process files
	if (srcflen + srcoffset < destflen)
	{
		fprintf(stderr, "checklen: \"merge to\" file + offset is smaller then \"merge from\" file.\n");
		return EXIT_FAILURE;
	}
	
	short isMergeState = 0;

	for (int i = 0; i < srcflen; i++)
	{
		if (destflen > i)
		{		
			if (destbuf[i] == '_')
			{
				if (strncmp(destbuf + i, "__ext~", 6) == 0)
				{
					isMergeState = 1;
					destbuf[i + 5] = 0;
				}
			}
			if (isMergeState == 1)
			{
				if (destbuf[i] == '~')
				{
					destbuf[i] = srcbuf[i];
					isMergeState = 0;
					continue;
				}
				destbuf[i] = srcbuf[i];
			}
		}
	}

	memcpy(srcbuf + srcoffset, destbuf, destflen);
	
	fp = fopen(argv[1], "wb");
	if (fp == NULL) {
		perror("fopen()");
		return EXIT_FAILURE;
	}
	
	fwrite(srcbuf, sizeof(uint8_t), srcflen, fp);
	fclose(fp);


	free(destbuf);
	free(srcbuf);
}
