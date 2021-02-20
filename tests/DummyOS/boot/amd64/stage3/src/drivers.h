#ifndef __DRIVERS_H__
#define __DRIVERS_H__

#include "type.h"
#include "string.h"
#include "memory.h"
#include "port.h"

/////////////////////////////////////////
// iotty.h
/////////////////////////////////////////

#define TAB_LENGTH 8

#pragma pack (push, 1)
typedef struct fbCharacterStruct {
	BYTE fb_ch;
	BYTE fb_attr;
} ttyFBChar;
#pragma pack (pop)

void ttyClearScreen();
void ttyPrint(const char* str);
void ttyPrinti(int val, int base);
void ttyPrintn(const char* str, size_t s);
int ttyPutchar(int ch);
void ttyEnableCursor(uint8_t start, uint8_t end);
void ttyDisableCursor();
void ttySetCursorPosition(coord pos);
coord ttyGetCursorPosition();

/////////////////////////////////////////
// ioatapio.h
/////////////////////////////////////////

#define ATAPIO_STATUS_BSY    0x80
#define ATAPIO_STATUS_RDY    0x40
#define ATAPIO_STATUS_DRQ    0x08
#define ATAPIO_STATUS_DF     0x20
#define ATAPIO_STATUS_ERR    0x01

void* ATAPIORead(void* dest, uint32_t lba, uint8_t s_s);
void* ATAPIOWrite(uint32_t lba, uint8_t s_s, void* src);
static void ATAPIOWaitBusy();
static void ATAPIOWaitDRQ();

#endif