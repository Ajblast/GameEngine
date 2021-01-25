#include "gravpch.h"
#include "fileNotFoundException.h"

GRAVEngine::Exceptions::IO::fileNotFoundException::fileNotFoundException(const char* fileName, const char* description) : ioException(description)
{
	STRING_COPY(m_FileName, fileName, MAX_EXCEPTION_STRING_LENGTH);
}

GRAVEngine::Exceptions::IO::fileNotFoundException::fileNotFoundException(const fileNotFoundException& other) noexcept : ioException(other)
{
	STRING_COPY(m_FileName, other.what(), MAX_EXCEPTION_STRING_LENGTH);
}

GRAVEngine::Exceptions::IO::fileNotFoundException& GRAVEngine::Exceptions::IO::fileNotFoundException::operator=(const fileNotFoundException& other) noexcept
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

GRAVEngine::Exceptions::IO::fileNotFoundException::~fileNotFoundException() noexcept
{
	delete[] m_FileName;
}
