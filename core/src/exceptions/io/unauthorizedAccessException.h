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
				explicit unauthorizedAccessException(const char* fileName, const char* description);
				explicit unauthorizedAccessException(const std::string& fileName, const std::string& description) : unauthorizedAccessException(fileName.c_str(), description.c_str()) {}
				unauthorizedAccessException(const unauthorizedAccessException& other) noexcept;

				unauthorizedAccessException& operator= (const unauthorizedAccessException& other) noexcept;


				inline const char* getFileName() const noexcept
				{
					return m_FileName;
				}

				~unauthorizedAccessException() noexcept;

			private:
				const char* m_FileName;

			};
		}
	}
}