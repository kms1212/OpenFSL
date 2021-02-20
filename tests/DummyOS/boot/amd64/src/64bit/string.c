#include "string.h"

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

void itoa(uint8_t *buf, uint32_t base, uint32_t d)
{
	uint8_t *p = buf;
	uint8_t *p1, *p2;
	uint32_t ud = d;
	uint32_t divisor = 10;
	
	if (base == 10 && d < 0) {
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 16)
		divisor = 16;
	
	do {
		uint32_t remainder = ud % divisor;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	} while (ud /= divisor);
	
	*p = 0;
	p1 = buf;
	p2 = p - 1;
	while(p1 < p2) {
		uint8_t tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}