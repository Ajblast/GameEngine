#include <stdexcept>
#include <string>
#include "common.h"

namespace GRAVEngine
{
	namespace Exceptions
	{
		class unknownErrorException : public std::exception
		{
		public:
			unknownErrorException(const std::string& description) : std::exception(description.c_str()) {}
			unknownErrorException(const char* description) : std::exception(description) {}
			unknownErrorException(const unknownErrorException& other) noexcept : std::exception(other) {}
		};
	}
}