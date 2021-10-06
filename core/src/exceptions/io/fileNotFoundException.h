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
			class GRAVAPI fileNotFoundException : public ioException
			{
			public:
				explicit fileNotFoundException(const char* fileName, const char* description) : fileNotFoundException(std::string(fileName), std::string(description)) {}
				explicit fileNotFoundException(const std::string& fileName, const std::string& description) : m_FileName(fileName), ioException(description) {}
				fileNotFoundException(const fileNotFoundException& other) noexcept : m_FileName(other.m_FileName), ioException(other.what()) {}

				fileNotFoundException& operator= (const fileNotFoundException& other) noexcept;

				inline const std::string& getFileName() const noexcept
				{
					return m_FileName;
				}

				~fileNotFoundException() noexcept = default;

			private:
				std::string m_FileName;

			};
		}
	}
}