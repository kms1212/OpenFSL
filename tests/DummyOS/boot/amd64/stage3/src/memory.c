#include "memory.h"

void* memset(void* dest, BYTE v, size_t s)
{
	for (size_t i = 0; i < s; i++)
		*((BYTE*)dest + i) = v;
	return dest;
}

void* memcpy(void* dest, void* src, size_t s)
{
	char* tmp = dest;
	const char *sc = src;
	while(s--)
		*tmp++ = *sc++;
	return dest;
}

int memtest(void* addr)
{
	BYTE* tmp = (BYTE*)addr;
	BYTE original = *tmp;
	int result = false;
	*tmp = 0x0F;
	if (*tmp == 0x0F)
		result = true;
	*tmp = original;
	return result;
}
