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
			class fileNotFoundException : public ioException
			{
			public:
				explicit fileNotFoundException(const char* fileName, const char* description);
				explicit fileNotFoundException(const std::string& fileName, const std::string& description) : fileNotFoundException(fileName.c_str(), description.c_str()) {}
				fileNotFoundException(const fileNotFoundException& other) noexcept;

				fileNotFoundException& operator= (const fileNotFoundException& other) noexcept;

				inline const char* getFileName() const noexcept
				{
					return m_FileName;
				}

				~fileNotFoundException() noexcept;

			private:
				const char* m_FileName;

			};
		}
	}
}