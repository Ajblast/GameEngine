#pragma once

#include <string>
#include "common.h"
#include "ioException.h"

namespace GRAVEngine
{
	namespace Exceptions
	{
		namespace IO
		{
			class unauthorizedAccessException : public ioException
			{
			public:
				explicit unauthorizedAccessException(const char* fileName, const char* description) : unauthorizedAccessException(std::string(fileName), std::string(description)) {}
				explicit unauthorizedAccessException(const std::string& fileName, const std::string& description) : m_FileName(fileName), ioException(description) {}
				unauthorizedAccessException(const unauthorizedAccessException& other) noexcept : m_FileName(other.m_FileName), ioException(other.what()) {}

				unauthorizedAccessException& operator= (const unauthorizedAccessException& other) noexcept;


				inline const std::string& getFileName() const noexcept
				{
					return m_FileName;
				}

				~unauthorizedAccessException() noexcept = default;

			private:
				std::string m_FileName;

			};
		}
	}
}