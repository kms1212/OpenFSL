#ifndef __DRIVERS_H__
#define __DRIVERS_H__

#include "type.h"
#include "port.h"

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