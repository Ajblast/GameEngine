#pragma once

#include <memory>
#include <unordered_map>
#include "common.h"
#include "logger.h"
#include "locks/scopedLock.h"
#include "locks/spinLock.h"

namespace GRAVEngine
{
	namespace Logging
	{
		/*std::shared_ptr<Logging::logger> create(std::string loggerName, std::string path)
		{
			auto sink = std::make_shared<Logging::Sinks::fileSink>(path);
			auto logger = std::make_shared<Logging::logger>(std::move(loggerName));

			Logging::logManager::getInstance()->initializeLogger(logger);
			return logger;
		}*/

		// TODO: Allow asynchronous logging
		// TODO: Allow flushing of the sinks in interval time

		class logManager
		{
		public:
			explicit logManager() {}
			logManager(const logManager&) = delete;				// Delete copy constructor
			logManager& operator= (const logManager&) = delete;	// Delete set constructor
			~logManager() = default;

			// Startup the log manager
			void startUp(std::shared_ptr<Logging::Sinks::sink> defaultLoggerSink);
			// Startup the log manager with a default logger
			void startUp(std::shared_ptr<Logging::logger> defaultLogger);
			// Shutdown the log manager
			void shutDown();

			// Register a logger
			void registerLogger(std::shared_ptr<logger> logger);
			// Initialze a logger with the default verbosity and flush verbosity
			void initializeLogger(std::shared_ptr<logger> logger);

			// Get a logger based on its name
			std::shared_ptr<logger> get(const std::string& loggerName);
			// Get the default logger
			std::shared_ptr<logger> defaultLogger();

			// Set the default logger
			void setDefaultLogger(std::shared_ptr<logger> logger);

			// Set the verbosity of all loggers
			void setVerbosity(Logging::verbosity verbosity);
			// Set the flush verbosity of all loggers
			void setFlushVerbosity(Logging::verbosity verbosity);
			// Flush all of the loggers
			void flushAll();

			// Remove a logger by its name
			void removeLogger(const std::string& loggerName);
			// Remove all of the loggers
			void removeAll();

			// Should loggers be automatically registered after initialization?
			void setAutomaticRegistration(bool automaticRegistraction);
			// Set verbosities for all existing/future loggers. Unspecified verbosities can be set or not. Specified loggers do not have to exist yet.
			void setVerbosities(std::unordered_map<std::string, Logging::verbosity> verbosities, bool setUnspecified, Logging::verbosity& verbosity);

			// Statically get the logging instance
			inline static logManager* getInstance()
			{
				return s_Instance;
			}
		private:
//			logManager();
			//~logManager() = default;

		private:
			Locks::spinLock m_MapLock;

			Logging::verbosity_t m_FlushVerbosity;
			Logging::verbosity_t m_Verbosity;

			std::shared_ptr<logger> m_DefaultLogger;
			std::unordered_map<std::string, std::shared_ptr<logger>> m_Loggers;
			std::unordered_map<std::string, Logging::verbosity> m_LoggerVerbosities;

			bool m_AutomaticRegistration = true;

			static logManager* s_Instance;
		};
	}
}