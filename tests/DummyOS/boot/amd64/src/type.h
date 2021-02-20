#ifndef __TYPE_H__
#define __TYPE_H__

#define true 1
#define false 0
#define NULL 0

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned long long QWORD;
typedef unsigned char BOOL;
typedef unsigned int size_t;

#pragma pack (push, 1)
typedef struct fbCharacterStruct {
	BYTE fb_ch;
	BYTE fb_attr;
} FBChar;
#pragma pack (pop)

#endif
