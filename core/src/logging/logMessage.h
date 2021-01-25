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
			const char* m_Payload;


			explicit logMessage(std::string loggerName, Logging::verbosity verbosity, const char* payload);
			explicit logMessage(const logMessage& other);
			explicit logMessage(logMessage&& other) noexcept;

			logMessage& operator= (const logMessage& other);
			logMessage& operator= (logMessage&& other) noexcept;

			~logMessage();
		};
	}
}