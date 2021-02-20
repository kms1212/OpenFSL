#ifndef __IO_H__
#define __IO_H__

#include "type.h"
#include "string.h"
#include "memory.h"
#include "port.h"

#define TAB_LENGTH 8

#pragma pack (push, 1)
typedef struct characterCoordStruct {
	uint16_t x;
	uint16_t y;
}coord;
#pragma pack (pop)

#pragma pack (push, 1)
typedef struct fbCharacterStruct {
	BYTE fb_ch;
	BYTE fb_attr;
} FBChar;
#pragma pack (pop)

void clearScreen();
void print(const char* str);
void printi(int val, int base);
void printn(const char* str, size_t s);
int putchar(int ch);
void enableCursor(uint8_t start, uint8_t end);
void disableCursor();
void setCursorPosition(coord pos);
coord getCursorPosition();
#endif