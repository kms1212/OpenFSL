#ifndef __STRING_H__
#define __STRING_H__

#include "type.h"

char *strncpy(char *dest, const char *src, size_t count);
int strncmp(const char *s1, const char *s2, size_t count);
void itoa(uint8_t *buf, uint32_t base, uint32_t d);
#endif
