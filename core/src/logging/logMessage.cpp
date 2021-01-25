#include "gravpch.h"
#include "logMessage.h"

GRAVEngine::Logging::logMessage::logMessage(std::string loggerName, Logging::verbosity verbosity, const char* payload) :
	m_LoggerName(loggerName), m_Verbosity(verbosity)
{
	STRING_COPY(m_Payload, payload, MAX_LOG_BUFFER_SIZE);
}

GRAVEngine::Logging::logMessage::logMessage(const logMessage& other) : m_LoggerName(other.m_LoggerName), m_Verbosity(other.m_Verbosity)
{
	STRING_COPY(m_Payload, other.m_Payload, MAX_LOG_BUFFER_SIZE);
}

GRAVEngine::Logging::logMessage::logMessage(logMessage&& other) noexcept : m_LoggerName(std::move(other.m_LoggerName)), m_Verbosity(other.m_Verbosity), m_Payload(other.m_Payload)
{
	// Free resources
	other.m_Payload = nullptr;
}

GRAVEngine::Logging::logMessage& GRAVEngine::Logging::logMessage::operator=(const logMessage& other)
{
	if (this != &other)
	{
		// Delete current resources
		delete[] m_Payload;

		// Copy other resources
		STRING_COPY(m_Payload, other.m_Payload, MAX_EXCEPTION_STRING_LENGTH);
	}

	return *this;
}

GRAVEngine::Logging::logMessage& GRAVEngine::Logging::logMessage::operator=(logMessage&& other) noexcept
{
	if (this != &other)
	{
		// Delete current resources
		delete[] m_Payload;

		// Steal other resources
		m_LoggerName = std::move(other.m_LoggerName);
		m_Verbosity = other.m_Verbosity;
		m_Payload = other.m_Payload;

		// Release pointers to resources
		other.m_Payload = nullptr;
	}

	return *this;
}

GRAVEngine::Logging::logMessage::~logMessage()
{
	delete[] m_Payload;
}
