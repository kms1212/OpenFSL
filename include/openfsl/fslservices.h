#ifndef __FSLSERVICES_H__
#define __FSLSERVICES_H__

#include "fixedint.h"
#include "stddef.h"

namespace openFSL {
	/* Essential variables */
	extern uint32_t bytespersector;
	
	/* Library initializer */
	uint32_t initializeLibrary();
	
	/* Memory allocation function wrapper */
	extern void* (*fsl_malloc)(size_t); // size_t size
	
	/* Free memory function wrapper */
	extern void (*fsl_free)(void*); // void* ptr
	
	/* Copy memory function wrapper */
	extern void* (*fsl_memcpy)(void*, const void*, size_t); // void* dest, const void* src, size_t num
}

#endif