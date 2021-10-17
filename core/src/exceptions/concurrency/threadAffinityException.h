#pragma once

#include "common.h"
#include <stdexcept>

namespace GRAVEngine
{
	namespace Exceptions
	{
		class GRAVAPI threadAffinityException : public std::runtime_error
		{
		public:
			explicit threadAffinityException(const std::string& description) : std::runtime_error(description) {}
			explicit threadAffinityException(const char* description) : std::runtime_error(description) {}
			threadAffinityException(const threadAffinityException& other) noexcept : std::runtime_error(other) {}
		};

	}
}