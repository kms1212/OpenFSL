#ifndef __STRING_H__
#define __STRING_H__

#include "type.h"

void printString(int x, int y, const char* str);
char *strncpy(char *dest, const char *src, size_t count);
int strncmp(const char *s1, const char *s2, size_t count);
#endif
