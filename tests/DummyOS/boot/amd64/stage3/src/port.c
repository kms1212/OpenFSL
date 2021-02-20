#include "port.h"

uint8_t portInB(uint16_t port) { uint8_t ret; __asm__("in %%dx, %%al" : "=a" (ret) : "d" (port)); return ret; }
uint16_t portInW(uint16_t port) { uint16_t ret; __asm__("in %%dx, %%ax" : "=a" (ret) : "d" (port)); return ret; }
uint32_t portInDW(uint16_t port) { uint32_t ret; __asm__("in %%dx, %%eax" : "=a" (ret) : "d" (port)); return ret; }

void portOutB(uint16_t port, uint8_t data) { __asm__("out %%al, %%dx" : : "a" (data), "d" (port)); }
void portOutW(uint16_t port, uint16_t data) { __asm__("out %%ax, %%dx" : : "a" (data), "d" (port)); }
void portOutDW(uint16_t port, uint32_t data) { __asm__("out %%eax, %%dx" : : "a" (data), "d" (port)); }