#ifndef __FSLSERVICES_H
#define __FSLSERVICES_H

#include <stdint.h>
#include <vector>
#include <string>

#include "stddef.h"

namespace openFSL {
	
	/* Library initializer */
	uint32_t        initializeLibrary();
	
	/* String tokenizer */
	void            fsl_strtokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);
}

#endif