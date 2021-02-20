#include "type.h"
#include "string.h"
#include "memory.h"
#include "drivers.h"
#include "fat.h"

int main(void) {
	ttyPrint("--Stage 3 bootloader--\n");
	
	while(1) {
		__asm__("HLT");
	}
}