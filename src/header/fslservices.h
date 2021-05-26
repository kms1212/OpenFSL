/* Copyright (c) 2020. kms1212(권민수)
This file is part of OpenFSL.

OpenFSL and its source code is published over BSD 3-Clause License.
See the BSD-3-Clause for more details.
<https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

*/

#ifndef __FSLSERVICES_H
#define __FSLSERVICES_H

#include <stdint.h>
#include <vector>
#include <string>

#include "stddef.h"

namespace openFSL {
		
	/**
	 *
	 * @brief Library initializer
	 * @return int: Error code 
	 *
	 */
	int        initializeLibrary();
		
	/**
	 *
	 * @brief String tokenizer
	 *
	 */
	void            fsl_strtokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);
}

#endif