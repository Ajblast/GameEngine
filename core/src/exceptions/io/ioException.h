#pragma once

#include <stdexcept>

namespace GRAVEngine
{
	namespace Exceptions
	{
		namespace IO
		{
			class ioException : public std::runtime_error
			{
			public:
				explicit ioException(const std::string& description) : std::runtime_error(description) {}
				explicit ioException(const char* description) : std::runtime_error(description) {}
				ioException(const ioException& other) noexcept : std::runtime_error(other) {}
			};
					

			//class directoryNotFoundException : public ioException
			//{
			//
			//};
			//
			//class driveNotFoundException : public ioException
			//{
			//
			//};
			//
			//class pathTooLongException : public ioException
			//{
			//
			//};
			//
			//class unauthorizedAccessException : public ioException
			//{
			//
			//};
		}
	}
}