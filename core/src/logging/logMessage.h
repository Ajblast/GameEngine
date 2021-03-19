#pragma once

#include <string>
#include "logVerbosity.h"

namespace GRAVEngine
{
	namespace Logging
	{
		struct logMessage
		{
			std::string m_LoggerName;
			Logging::verbosity m_Verbosity{ GRAVEngine::Logging::verbosity::off };
			std::string m_Payload;


			explicit logMessage(std::string loggerName, Logging::verbosity verbosity, const std::string& payload) 
				: m_LoggerName(loggerName), m_Verbosity(verbosity), m_Payload(payload) {}
			explicit logMessage(const logMessage& other)
				: m_LoggerName(other.m_LoggerName), m_Verbosity(other.m_Verbosity), m_Payload(other.m_Payload) {}
			explicit logMessage(logMessage&& other) noexcept
				: m_LoggerName(std::move(other.m_LoggerName)), m_Verbosity(other.m_Verbosity), m_Payload(std::move(other.m_Payload)) {}

			logMessage& operator= (const logMessage& other);
			logMessage& operator= (logMessage&& other) noexcept;

			~logMessage() = default;
		};
	}
}