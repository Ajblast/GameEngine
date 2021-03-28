#include "gravpch.h"
#include "fileSink.h"

GRAVEngine::Logging::Sinks::fileSink::fileSink(const std::string& filePath, bool truncate) : m_File(filePath, GRAVEngine::IO::fileMode::OUTPUT | GRAVEngine::IO::fileMode::APPEND, false)
{
}

void GRAVEngine::Logging::Sinks::fileSink::sinkMessage(const Logging::logMessage& message)
{
	// Write the message to the file
	m_File.write(message.m_Payload);
}

void GRAVEngine::Logging::Sinks::fileSink::flush_()
{
	// Flush the file
	m_File.flush();
}
