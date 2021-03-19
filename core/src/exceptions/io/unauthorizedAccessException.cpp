#include "gravpch.h"
#include "unauthorizedAccessException.h"

GRAVEngine::Exceptions::IO::unauthorizedAccessException& GRAVEngine::Exceptions::IO::unauthorizedAccessException::operator=(const unauthorizedAccessException& other) noexcept
{
	exception::operator=(other);

	if (this != &other)
	{
		m_FileName = other.m_FileName;
	}

	return *this;
}
