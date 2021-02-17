#include "diskdevice.h"

class DiskDevice : public Device {
public:
	int (*input)();
	int (*output)();
}