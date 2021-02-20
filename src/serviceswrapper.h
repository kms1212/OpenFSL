#ifndef __SERVICESWRAPPER_H__
#define __SERVICESWRAPPER_H__

namespace FSLServices {
	/* Memory allocation function wrapper */
	void* (*malloc)(size_t);
	
	/* Free memory function wrapper */
	void (*free)(void*);
	
	/* Copy memory function wrapper */
	void (*memcpy)(void*, const void*, size_t);
}

#endif