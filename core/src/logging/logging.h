#pragma once

#include "logging/logManager.h"

namespace GRAVEngine
{
	namespace Logging
	{
		void reportAssertionFailure(const char* expression, const char* file, size_t line);
	}
}

#define GRAV_LOG_CRITICAL(format, ...)	GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->log(GRAVEngine::Logging::verbosity::critical,	format, __VA_ARGS__)
#define GRAV_LOG_ERROR(format, ...)		GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->log(GRAVEngine::Logging::verbosity::error,		format, __VA_ARGS__)
#define GRAV_LOG_WARN(format, ...)		GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->log(GRAVEngine::Logging::verbosity::warn,		format, __VA_ARGS__)
#define GRAV_LOG_INFO(format, ...)		GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->log(GRAVEngine::Logging::verbosity::info,		format, __VA_ARGS__)
#define GRAV_LOG_DEBUG(format, ...)		GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->log(GRAVEngine::Logging::verbosity::debug,		format, __VA_ARGS__)
#define GRAV_LOG_TRACE(format, ...)		GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->log(GRAVEngine::Logging::verbosity::trace,		format, __VA_ARGS__)

#define GRAV_LOG_LINE_CRITICAL(format, ...)	GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->logLine(GRAVEngine::Logging::verbosity::critical,	format, __VA_ARGS__)
#define GRAV_LOG_LINE_ERROR(format, ...)	GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->logLine(GRAVEngine::Logging::verbosity::error,		format, __VA_ARGS__)
#define GRAV_LOG_LINE_WARN(format, ...)		GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->logLine(GRAVEngine::Logging::verbosity::warn,		format, __VA_ARGS__)
#define GRAV_LOG_LINE_INFO(format, ...)		GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->logLine(GRAVEngine::Logging::verbosity::info,		format, __VA_ARGS__)
#define GRAV_LOG_LINE_DEBUG(format, ...)	GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->logLine(GRAVEngine::Logging::verbosity::debug,		format, __VA_ARGS__)
#define GRAV_LOG_LINE_TRACE(format, ...)	GRAVEngine::Logging::logManager::getInstance()->defaultLogger()->logLine(GRAVEngine::Logging::verbosity::trace,		format, __VA_ARGS__)