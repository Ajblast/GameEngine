#pragma once

#include <stdexcept>

namespace GRAVEngine
{
	namespace Exceptions
	{
		class GRAVAPI counterWaitListFullException : public std::runtime_error
		{
		public:
			explicit counterWaitListFullException(const std::string& description) : std::runtime_error(description) {}
			explicit counterWaitListFullException(const char* description) : std::runtime_error(description) {}
			counterWaitListFullException(const counterWaitListFullException& other) noexcept : std::runtime_error(other) {}
		};

	}
}