#pragma once

#include "common.h"
#include <stdexcept>

namespace GRAVEngine
{
	namespace Exceptions
	{
		class GRAVAPI fiberCreationException : public std::runtime_error
		{
		public:
			explicit fiberCreationException(const std::string& description) : std::runtime_error(description) {}
			explicit fiberCreationException(const char* description) : std::runtime_error(description) {}
			fiberCreationException(const fiberCreationException& other) noexcept : std::runtime_error(other) {}
		};

	}
}