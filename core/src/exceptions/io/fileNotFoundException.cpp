#include "gravpch.h"
#include "fileNotFoundException.h"

GRAVEngine::Exceptions::IO::fileNotFoundException& GRAVEngine::Exceptions::IO::fileNotFoundException::operator=(const fileNotFoundException& other) noexcept
{
	exception::operator=(other);

	if (this != &other)
	{
		m_FileName = other.m_FileName;
	}

	return *this;
}
