#pragma once

#include "common.h"
#include <stdexcept>

namespace GRAVEngine
{
	namespace Exceptions
	{
		class GRAVAPI fiberCallbackException : public std::runtime_error
		{
		public:
			explicit fiberCallbackException(const std::string& description) : std::runtime_error(description) {}
			explicit fiberCallbackException(const char* description) : std::runtime_error(description) {}
			fiberCallbackException(const fiberCallbackException& other) noexcept : std::runtime_error(other) {}
		};

	}
}