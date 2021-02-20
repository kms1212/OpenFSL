#include "io.h"

void clear() {
	FBChar* charCurs = (FBChar*) 0xB8000;
	for (int i = 0; i < 80 * 25; i++)
	{
		charCurs[i].fb_ch = '\0';
		charCurs[i].fb_attr = 0x07;
	}
	coord pos = { 0, 0 };
	setCursorPosition(pos);
}

void print(const char* str) {
	char ch;
    while((ch = *str++) != 0) {
        putchar(ch);
	}
}

void printi(int val, int base) {
	char buf[20];
	itoa(buf, base, val);
	print(buf);
}

void printn(const char* str, size_t s) {
    for(int i = 0; i < s; i++) {
        putchar(str[i]);
		if (!str[i])
			break;
	}
}

int putchar(int ch) {
	FBChar* charCurs = (FBChar*) 0xB8000;
	
	coord pos = getCursorPosition();
	
	if (ch == '\n')
	{
		pos.y++;
	}
	else if (ch == '\r')
	{
		pos.x = 0;
	}
	else if (ch == '\t')
	{
		uint16_t tmp = pos.x % TAB_LENGTH;
		pos.x += TAB_LENGTH - tmp;
	}
	else
	{
		charCurs[pos.x + pos.y * 80].fb_ch = ch;
		pos.x++;
	}
	
	if (pos.x + pos.y * 80 >= 80 * 25)
	{
		for(uint64_t i = 1; i < 25; i++)
			memcpy((void*)((i - 1) * 80 * 2 + 0xB8000), (void*)(i * 80 * 2 + 0xB8000), 80 * 2);
		for(uint64_t i = 0; i < 80; i++) {
			charCurs[24 * 80 + i].fb_ch = '\0';
			charCurs[24 * 80 + i].fb_attr = 0x07;
		}
		pos.x = 0;
		pos.y --;
	}
	setCursorPosition(pos);
}

void enableCursor(uint8_t start, uint8_t end)
{
	portOutB(0x03D4, 0x0A);
	portOutB(0x03D5, start);
 
	portOutB(0x03D4, 0x0B);
	portOutB(0x03D5, end);
}

void disableCursor()
{
	portOutB(0x03D4, 0x0A);
	portOutB(0x03D5, 0x20);
}

coord getCursorPosition() {
	uint16_t pos = 0;
	coord ret;
    portOutB(0x3D4, 0x0F);
    pos |= portInB(0x3D5);
    portOutB(0x3D4, 0x0E);
    pos |= ((uint16_t)portInB(0x3D5)) << 8;
	ret.x = pos % 80;
	ret.y = (pos - ret.x) / 80;
    return ret;
}

void setCursorPosition(coord pos) {
	int offset = (pos.x + pos.y * 80);
	portOutB(0x03D4, 14);
	portOutB(0x03D5, offset >> 8);
	
	portOutB(0x03D4, 15);
	portOutB(0x03D5, offset & 0xFF);
}