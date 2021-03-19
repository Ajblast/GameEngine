#include "gravpch.h"
#include "ostreamSink.h"

void GRAVEngine::Logging::Sinks::ostreamSink::sinkMessage(const Logging::logMessage& message)
{
	m_ostream.write(message.m_Payload.c_str(), message.m_Payload.size());
	if (m_ForceFlush)
		m_ostream.flush();
}

void GRAVEngine::Logging::Sinks::ostreamSink::flush_()
{
	m_ostream.flush();
}
