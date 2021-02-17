#include "diskdevice.hpp"

class DiskDevice : public Device {
public:
	int (*input)();
	int (*output)();
}