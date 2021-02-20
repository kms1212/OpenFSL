#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "type.h"

void* memset(void* dest, BYTE v, size_t s);
void* memcpy(void* dest, void* src, size_t s);
int memtest(void* addr);
#endif
