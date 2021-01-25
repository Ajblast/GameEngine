#include "gravpch.h"
#include "unauthorizedAccessException.h"

GRAVEngine::Exceptions::IO::unauthorizedAccessException::unauthorizedAccessException(const char* fileName, const char* description) : ioException(description)
{
	STRING_COPY(m_FileName, fileName, MAX_EXCEPTION_STRING_LENGTH);
}

GRAVEngine::Exceptions::IO::unauthorizedAccessException::unauthorizedAccessException(const unauthorizedAccessException& other) noexcept : ioException(other)
{
	STRING_COPY(m_FileName, other.what(), MAX_EXCEPTION_STRING_LENGTH);
}

GRAVEngine::Exceptions::IO::unauthorizedAccessException& GRAVEngine::Exceptions::IO::unauthorizedAccessException::operator=(const unauthorizedAccessException& other) noexcept
{
	exception::operator=(other);

	if (this != &other)
	{
		// Delete the old file name
		delete[] m_FileName;

		STRING_COPY(m_FileName, other.m_FileName, MAX_EXCEPTION_STRING_LENGTH);
	}

	return *this;
}

GRAVEngine::Exceptions::IO::unauthorizedAccessException::~unauthorizedAccessException() noexcept
{
	delete[] m_FileName;
}
