#ifndef __TYPE_H__
#define __TYPE_H__

#include <stdint.h>

#define true 1
#define false 0
#define NULL 0

#define SIZE_SECTOR 512

typedef uint8_t   BYTE;
typedef uint16_t  WORD;
typedef uint32_t  DWORD;
typedef uint64_t  QWORD;
typedef void*     PNTR;

typedef unsigned char bool;
typedef unsigned int size_t;

#pragma pack (push, 1)
typedef struct characterCoordStruct {
	uint16_t x;
	uint16_t y;
}coord;
#pragma pack (pop)

#endif
