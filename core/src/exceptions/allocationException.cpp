#include "gravpch.h"
#include "allocationException.h"

GRAVEngine::Exceptions::allocationException& GRAVEngine::Exceptions::allocationException::operator=(const allocationException& other) noexcept
{
	exception::operator=(other);

	if (this != &other)
	{

	}

	return *this;
}

const char* GRAVEngine::Exceptions::allocationException::what() const noexcept
{
	return m_Message.c_str();
}
