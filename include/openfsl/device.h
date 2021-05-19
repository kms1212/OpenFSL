#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <stddef.h>

namespace openFSL {
	class Device {	
	public:
		int (*input)();
		int (*output)();
		
		Device();
	};
}


#endif