#ifndef __PORT_H__
#define __PORT_H__

#include "type.h"
#include "string.h"
#include "memory.h"

uint8_t portInB(uint16_t port);
uint16_t portInW(uint16_t port);
uint32_t portInDW(uint16_t port);

void portOutB(uint16_t port, uint8_t data);
void portOutW(uint16_t port, uint16_t data);
void portOutDW(uint16_t port, uint32_t data);
#endif