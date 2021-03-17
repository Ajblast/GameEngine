#include "gravpch.h"
#include "logManager.h"
#include "logging/sinks/fileSink.h"

GRAVEngine::Logging::logManager* GRAVEngine::Logging::logManager::s_Instance = nullptr;

void GRAVEngine::Logging::logManager::startUp(ref<Logging::Sinks::sink>defaultLoggerSink)
{
	// Only one instance possible
	GRAV_ASSERT(s_Instance == nullptr);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);

	// Create a default file logger
	const char* defaultLoggerName = "";
	//m_DefaultLogger = std::make_shared<Logging::logger>(defaultLoggerName, defaultLoggerSink);
	m_DefaultLogger = createRef<Logging::logger>(defaultLoggerName, defaultLoggerSink);
	m_DefaultLogger->setVerbosity(GRAVEngine::Logging::verbosity::trace);
	m_DefaultLogger->setFlushVerbosity(GRAVEngine::Logging::verbosity::trace);

	//auto fileSink = std::make_shared<Logging::Sinks::fileSink>();
	//m_DefaultLogger = std::make_shared<Logging::logger>(defaultLoggerName, fileSink);
	m_Loggers[defaultLoggerName] = m_DefaultLogger;


	// Set the static instance
	s_Instance = this;
}

void GRAVEngine::Logging::logManager::startUp(ref<Logging::logger> defaultLogger)
{
	// Only one instance possible
	GRAV_ASSERT(s_Instance == nullptr);

	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);

	// Set the default logger
	m_DefaultLogger = defaultLogger;
	m_Loggers[defaultLogger->name()] = defaultLogger;

	// Set the static instance;
	s_Instance = this;
}

void GRAVEngine::Logging::logManager::shutDown()
{
	GRAV_LOG_LINE_DEBUG("Shutting down Log Manager");

	GRAV_ASSERT(s_Instance);

	// Remove all of the loggers
	removeAll();

	// Remove the instance
	s_Instance = nullptr;
}

void GRAVEngine::Logging::logManager::registerLogger(ref<logger> logger)
{
	GRAV_ASSERT(s_Instance);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);

	std::string loggerName = logger->name();

	// Assert that logger of the same name is not being registered
	GRAV_ASSERT(get(loggerName) == nullptr);

	m_Loggers[loggerName] = std::move(logger);
}

void GRAVEngine::Logging::logManager::initializeLogger(ref<logger> logger)
{
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);

	GRAV_ASSERT(s_Instance);

	// TODO: Error handling

	// Set new level according to previously configured level or default level
	auto it = m_LoggerVerbosities.find(logger->name());
	Logging::verbosity newVerbosity = static_cast<Logging::verbosity>(
		it != m_LoggerVerbosities.end() ? static_cast<int>(it->second) : m_Verbosity.load(std::memory_order_relaxed)
	);

	logger->setVerbosity(newVerbosity);
	logger->setFlushVerbosity(
		static_cast<Logging::verbosity>(m_FlushVerbosity.load(std::memory_order_relaxed)
	));

	if (m_AutomaticRegistration)
		registerLogger(std::move(logger));
}

GRAVEngine::ref<GRAVEngine::Logging::logger> GRAVEngine::Logging::logManager::get(const std::string& loggerName)
{
	GRAV_ASSERT(s_Instance);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);
	auto found = m_Loggers.find(loggerName);
	return found == m_Loggers.end() ? nullptr : found->second;
}

GRAVEngine::ref<GRAVEngine::Logging::logger> GRAVEngine::Logging::logManager::defaultLogger()
{
	GRAV_ASSERT(s_Instance);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);
	return m_DefaultLogger;
}

void GRAVEngine::Logging::logManager::setDefaultLogger(ref<logger> logger)
{
	GRAV_ASSERT(s_Instance);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);

	// Remove previous default logger from the map
	if (m_DefaultLogger != nullptr)
		m_Loggers.erase(m_DefaultLogger->name());

	if (logger != nullptr)
		m_Loggers[logger->name()] = logger;

	m_DefaultLogger = std::move(logger);
}

void GRAVEngine::Logging::logManager::setVerbosity(Logging::verbosity verbosity)
{
	GRAV_ASSERT(s_Instance);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);

	for (auto& logger : m_Loggers)
		logger.second->setVerbosity(verbosity);

	m_Verbosity.store(static_cast<int>(verbosity));
}

void GRAVEngine::Logging::logManager::setFlushVerbosity(Logging::verbosity verbosity)
{
	GRAV_ASSERT(s_Instance);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);

	for (auto& logger : m_Loggers)
		logger.second->setFlushVerbosity(verbosity);

	m_FlushVerbosity.store(static_cast<int>(verbosity));
}

void GRAVEngine::Logging::logManager::flushAll()
{
	GRAV_ASSERT(s_Instance);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);

	for (auto& logger : m_Loggers)
		logger.second->flush();
}

void GRAVEngine::Logging::logManager::removeLogger(const std::string& loggerName)
{
	GRAV_ASSERT(s_Instance);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);

	m_Loggers.erase(loggerName);

	if (m_DefaultLogger && m_DefaultLogger->name() == loggerName)
		m_DefaultLogger.reset();
}

void GRAVEngine::Logging::logManager::removeAll()
{
	GRAV_ASSERT(s_Instance);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);
	m_Loggers.clear();
	m_DefaultLogger.reset();
}

void GRAVEngine::Logging::logManager::setAutomaticRegistration(bool automaticRegistraction)
{
	GRAV_ASSERT(s_Instance);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);

	m_AutomaticRegistration = automaticRegistraction;
}

void GRAVEngine::Logging::logManager::setVerbosities(std::unordered_map<std::string, Logging::verbosity> verbosities, bool setUnspecified, Logging::verbosity& verbosity)
{
	GRAV_ASSERT(s_Instance);
	
	Locks::scopedLock<decltype(m_MapLock)> lock(m_MapLock);

	m_LoggerVerbosities = std::move(verbosities);

	m_Verbosity.store(static_cast<int>(verbosity));

	for (auto& logger : m_Loggers)
	{
		auto loggerEntry = m_LoggerVerbosities.find(logger.first);

		// Set the verbosity of known loggers
		if (loggerEntry != m_LoggerVerbosities.end())
			logger.second->setVerbosity(loggerEntry->second);
		else if (setUnspecified)
			// Set the verbosity of unspecified loggers to the given verbosity
			logger.second->setVerbosity(verbosity);
	}
}
