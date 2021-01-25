#pragma once

#include <stdexcept>

namespace GRAVEngine
{
	namespace Exceptions
	{
		class jobQueueFullException : public std::runtime_error
		{
		public:
			explicit jobQueueFullException(const std::string& description) : std::runtime_error(description) {}
			explicit jobQueueFullException(const char* description) : std::runtime_error(description) {}
			jobQueueFullException(const jobQueueFullException& other) noexcept : std::runtime_error(other) {}
		};

	}
}