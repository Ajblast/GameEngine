#pragma once

#include <stdexcept>
#include <string>
#include "common.h"

namespace GRAVEngine
{
	namespace Exceptions
	{
		class invalidArgumentException : public std::invalid_argument
		{
		public:
			invalidArgumentException(const std::string& description) : std::invalid_argument(description) {}
			invalidArgumentException(const char* description) : std::invalid_argument(description) {}
			invalidArgumentException(const invalidArgumentException& other) noexcept : std::invalid_argument(other) {}
		};
	}
}