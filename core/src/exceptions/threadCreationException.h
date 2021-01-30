#pragma once

#include <stdexcept>

namespace GRAVEngine
{
	namespace Exceptions
	{
		class threadCreationException : public std::runtime_error
		{
		public:
			explicit threadCreationException(const std::string& description) : std::runtime_error(description) {}
			explicit threadCreationException(const char* description) : std::runtime_error(description) {}
			threadCreationException(const threadCreationException& other) noexcept : std::runtime_error(other) {}
		};

	}
}