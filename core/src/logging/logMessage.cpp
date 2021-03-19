#include "gravpch.h"
#include "logMessage.h"


GRAVEngine::Logging::logMessage& GRAVEngine::Logging::logMessage::operator=(const logMessage& other)
{
	if (this != &other)
	{
		m_LoggerName = other.m_LoggerName;
		m_Verbosity = other.m_Verbosity;
		m_Payload = other.m_Payload;
	}

	return *this;
}

GRAVEngine::Logging::logMessage& GRAVEngine::Logging::logMessage::operator=(logMessage&& other) noexcept
{
	if (this != &other)
	{
		// Steal other resources
		m_LoggerName = std::move(other.m_LoggerName);
		m_Verbosity = other.m_Verbosity;
		m_Payload = std::move(other.m_Payload);
	}

	return *this;
}
