/*
* This file is part of the "ii-importer" project
*
* See LICENSE and MENTIONS in the root of the source tree for information
* regarding licensing.
*/

#pragma once

#include "Toolkit/Obfuscate.h"
#include <string>

namespace ii::err {
	const std::string runtime1(AY_OBFUSCATE("We could not export from module, because it was not loaded. Module: %s"));
	const std::string runtime2(AY_OBFUSCATE("Dos header is wrong of module "));
	const std::string runtime3(AY_OBFUSCATE("Could not found nt header for "));
	const std::string runtime4(AY_OBFUSCATE("Export directory could not read for "));
	const std::string runtime5(AY_OBFUSCATE("Export directory was empty for "));
	const std::string runtime6(AY_OBFUSCATE(" not found in module "));
}
