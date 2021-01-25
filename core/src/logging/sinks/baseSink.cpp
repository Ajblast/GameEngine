#include "gravpch.h"
#include "baseSink.h"


void GRAVEngine::Logging::Sinks::baseSink::log(const Logging::logMessage& message)
{
	GRAVEngine::Locks::scopedLock<decltype(m_Lock)> janitor(m_Lock);	// Create the lock
	sinkMessage(message);							// Sink the message
}

void GRAVEngine::Logging::Sinks::baseSink::flush()
{
	GRAVEngine::Locks::scopedLock<decltype(m_Lock)> janitor(m_Lock);	// Create the lock
	flush_();										// Flush the sink
}
