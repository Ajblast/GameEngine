#pragma once

#include "common.h"
#include <stdexcept>
#include <string>

namespace GRAVEngine
{
	namespace Exceptions
	{
		class agentException : public std::runtime_error
		{
		public:
			agentException(const std::string& description) : std::runtime_error(description) {}
			agentException(const char* description) : std::runtime_error(description) {}
			agentException(const agentException& other) noexcept : std::runtime_error(other) {}
		};
	}
}