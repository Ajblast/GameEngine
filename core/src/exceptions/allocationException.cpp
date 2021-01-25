#include "gravpch.h"
#include "allocationException.h"

GRAVEngine::Exceptions::allocationException::allocationException(const char* message)
{
	STRING_COPY(m_Message, message, MAX_EXCEPTION_STRING_LENGTH);
}

GRAVEngine::Exceptions::allocationException::allocationException(const allocationException& other) noexcept
{
	STRING_COPY(m_Message, other.what(), MAX_EXCEPTION_STRING_LENGTH);
}

GRAVEngine::Exceptions::allocationException& GRAVEngine::Exceptions::allocationException::operator=(const allocationException& other) noexcept
{
	exception::operator=(other);

	if (this != &other)
	{
		// Delete the old file name
		delete[] m_Message;

		STRING_COPY(m_Message, other.m_Message, MAX_EXCEPTION_STRING_LENGTH);
	}

	return *this;
}

const char* GRAVEngine::Exceptions::allocationException::what() const noexcept
{
	return m_Message;
}

GRAVEngine::Exceptions::allocationException::~allocationException() noexcept
{
	delete[] m_Message;
}
