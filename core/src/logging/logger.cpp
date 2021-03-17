#include "gravpch.h"
#include "logger.h"

GRAVEngine::Logging::logger::logger(std::string name) : m_Name(std::move(name)), m_Sinks()
{}
GRAVEngine::Logging::logger::logger(std::string name, ref<Sinks::sink> sinkPtr) 
	: m_Name(std::move(name)), m_Sinks(1, sinkPtr), m_Verbosity(static_cast<int>(Logging::verbosity::trace)), m_FlushVerbosity(static_cast<int>(Logging::verbosity::trace))
{}
GRAVEngine::Logging::logger::logger(const logger& other) :
	m_Name(other.m_Name), 
	m_Sinks(other.m_Sinks),
	m_Verbosity(other.m_Verbosity.load(std::memory_order_relaxed)), 
	m_FlushVerbosity(other.m_FlushVerbosity.load(std::memory_order_relaxed))
{}
GRAVEngine::Logging::logger::logger(logger&& other) noexcept :
	m_Name(std::move(other.m_Name)),
	m_Sinks(std::move(other.m_Sinks)),
	m_Verbosity(other.m_Verbosity.load(std::memory_order_relaxed)),
	m_FlushVerbosity(other.m_FlushVerbosity.load(std::memory_order_relaxed))
{}

GRAVEngine::Logging::logger& GRAVEngine::Logging::logger::operator=(logger other)
{
	this->swap(other);
	return *this;
}

GRAVEngine::Logging::logger& GRAVEngine::Logging::logger::operator=(logger&& other) noexcept
{
	if (this != &other)
	{
		// Steal Resources
		m_Name = std::move(other.m_Name);
		m_Sinks = std::move(other.m_Sinks);

		// Swap Verbosity
		auto otherVerbosity = other.m_Verbosity.load();
		auto curVerbosity = m_Verbosity.exchange(otherVerbosity);
		other.m_Verbosity.store(curVerbosity);

		// Swap FlushVerbosity
		auto otherFlushVerbosity = other.m_FlushVerbosity.load();
		auto curFlushVerbosity = m_FlushVerbosity.exchange(otherFlushVerbosity);
		other.m_FlushVerbosity.store(curFlushVerbosity);
	}

	return *this;
}

void GRAVEngine::Logging::logger::swap(logger& other)
{
	m_Name.swap(other.m_Name);
	m_Sinks.swap(other.m_Sinks);

	// Swap Verbosity
	auto otherVerbosity = other.m_Verbosity.load();
	auto curVerbosity = m_Verbosity.exchange(otherVerbosity);
	other.m_Verbosity.store(curVerbosity);

	// Swap FlushVerbosity
	auto otherFlushVerbosity = other.m_FlushVerbosity.load();
	auto curFlushVerbosity = m_FlushVerbosity.exchange(otherFlushVerbosity);
	other.m_FlushVerbosity.store(curFlushVerbosity);

}


int GRAVEngine::Logging::logger::log(GRAVEngine::Logging::verbosity verbosity, const char* format, ...)
{
	int charsWritten = 0;

	bool logEnabled = shouldLog(verbosity);

	if (!logEnabled)
		return 0;

	va_list argList;
	va_start(argList, format);

	charsWritten += log(verbosity, logEnabled, format, argList);

	va_end(argList);

	return charsWritten;
}

int GRAVEngine::Logging::logger::logLine(Logging::verbosity verbosity, const char* format, ...)
{
	int charsWritten = 0;

	bool logEnabled = shouldLog(verbosity);

	if (!logEnabled)
		return 0;

	// Get the list of arguments
	va_list argList;
	va_start(argList, format);

	std::stringstream ss;
	ss << format << "\n";

	charsWritten = log(verbosity, logEnabled, ss.str().c_str(), argList);

	va_end(argList);

	return charsWritten;
}

void GRAVEngine::Logging::logger::flush()
{
	for (auto& sink : m_Sinks)
	{
		sink->flush();
	}

}

int GRAVEngine::Logging::logger::log(Logging::verbosity verbosity, bool logEnabled, const char* format, va_list args)
{
	int charsWritten = 0;

	// TODO: Determine if this needs to be removed. Probably so
	char s_buffer[MAX_LOG_BUFFER_SIZE];

	charsWritten = vsnprintf(s_buffer, MAX_LOG_BUFFER_SIZE, format, args);

	// Create the message object
	Logging::logMessage message(m_Name, verbosity, s_buffer);

	// Log the message
	logMessage(message, logEnabled);

	return charsWritten;
}

void GRAVEngine::Logging::logger::logMessage(Logging::logMessage& message, bool logEnabled)
{
	if (logEnabled)
		sinkMessage(message);
}

void GRAVEngine::Logging::logger::sinkMessage(Logging::logMessage& message)
{
	for (auto& sink : m_Sinks)
	{
		if (sink->shouldLog(message.m_Verbosity))
		{
			sink->log(message);
		}
	}

	if (shouldFlush(message))
	{
		flush();
	}
}