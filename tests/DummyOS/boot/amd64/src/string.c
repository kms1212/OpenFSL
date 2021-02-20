#include "string.h"

void printString(int x, int y, const char* str)
{
	FBChar* charCurs = (FBChar*) 0xB8000;
	int i;

	charCurs += (y * 80) + x;
	for (i = 0; str[i] != 0; i++)
	{
		charCurs[i].fb_ch = str[i];
	}
}

char *strncpy(char *dest, const char *src, size_t count)
{
        char *tmp = dest;

        while (count) {
                if ((*tmp = *src) != 0)
                        src++;
                tmp++;
                count--;
        }
        return dest;
}

int strncmp(const char *s1, const char *s2, size_t count)
{
	for (size_t i = 0; i < count - 1; i++) {
		if(*s1 != *s2) break;
		if(*s1 == 0) break;
		s1++;
		s2++;
	}
	if(*s1 == *s2) return 0;
	else if (*s1 > *s2) return 1;
	return -1;
}