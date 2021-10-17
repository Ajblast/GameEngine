#pragma once

#include "common.h"
#include <stdexcept>

namespace GRAVEngine
{
	namespace Exceptions
	{
		class GRAVAPI fiberQuitCallbackException : public std::runtime_error
		{
		public:
			explicit fiberQuitCallbackException(const std::string& description) : std::runtime_error(description) {}
			explicit fiberQuitCallbackException(const char* description) : std::runtime_error(description) {}
			fiberQuitCallbackException(const fiberQuitCallbackException& other) noexcept : std::runtime_error(other) {}
		};

	}
}