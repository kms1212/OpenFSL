#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <stddef.h>

class Device {	
public:
	int (*input)();
	int (*output)();
	
	Device();
};

#endif