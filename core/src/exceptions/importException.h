#pragma once

#include "common.h"
#include <stdexcept>
#include <string>

namespace GRAVEngine
{
	namespace Exceptions
	{
		class importException : public std::runtime_error
		{
		public:
			importException(const std::string& description) : std::runtime_error(description) {}
			importException(const char* description) : std::runtime_error(description) {}
			importException(const importException& other) noexcept : std::runtime_error(other) {}
		};
	}
}