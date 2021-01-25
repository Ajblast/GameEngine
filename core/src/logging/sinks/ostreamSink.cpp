#include "gravpch.h"
#include "ostreamSink.h"

void GRAVEngine::Logging::Sinks::ostreamSink::sinkMessage(const Logging::logMessage& message)
{
	m_ostream.write(message.m_Payload, strlen(message.m_Payload));
	if (m_ForceFlush)
		m_ostream.flush();
}

void GRAVEngine::Logging::Sinks::ostreamSink::flush_()
{
	m_ostream.flush();
}
