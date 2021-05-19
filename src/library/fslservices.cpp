#include "../header/fslservices.h"

using namespace openFSL;

namespace openFSL {
	uint32_t bytespersector = 512;
	void* (*fsl_malloc)(size_t) = NULL;
	void (*fsl_free)(void*) = NULL;
	void* (*fsl_memcpy)(void*, const void*, size_t) = NULL;
}

uint32_t openFSL::initializeLibrary() {
	
}