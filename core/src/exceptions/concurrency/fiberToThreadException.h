#pragma once

#include "common.h"
#include <stdexcept>

namespace GRAVEngine
{
	namespace Exceptions
	{
		class GRAVAPI fiberToThreadException : public std::runtime_error
		{
		public:
			explicit fiberToThreadException(const std::string& description) : std::runtime_error(description) {}
			explicit fiberToThreadException(const char* description) : std::runtime_error(description) {}
			fiberToThreadException(const fiberToThreadException& other) noexcept : std::runtime_error(other) {}
		};

	}
}