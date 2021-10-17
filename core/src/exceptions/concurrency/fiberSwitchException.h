#pragma once

#include "common.h"
#include <stdexcept>

namespace GRAVEngine
{
	namespace Exceptions
	{
		class GRAVAPI fiberSwitchException : public std::runtime_error
		{
		public:
			explicit fiberSwitchException(const std::string& description) : std::runtime_error(description) {}
			explicit fiberSwitchException(const char* description) : std::runtime_error(description) {}
			fiberSwitchException(const fiberSwitchException& other) noexcept : std::runtime_error(other) {}
		};

	}
}