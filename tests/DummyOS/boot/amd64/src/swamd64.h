#ifndef __SWAMD64_H__
#define __SWAMD64_H__

void getCPUID(DWORD query, DWORD* eax, DWORD* ebx, DWORD* ecx, DWORD* edx);
void switch64(void);

#endif
