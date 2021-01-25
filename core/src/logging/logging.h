#pragma once

#include "logging/logManager.h"

#include <sstream>

namespace GRAVEngine
{
	namespace Logging
	{
		void reportAssertionFailure(const char* expression, const char* file, size_t line);
	}
}