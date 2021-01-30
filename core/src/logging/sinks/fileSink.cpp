#include "gravpch.h"
#include "fileSink.h"

GRAVEngine::Logging::Sinks::fileSink::fileSink(const char* filePath, bool truncate) : m_File(GRAVEngine::IO::file(filePath, GRAVEngine::IO::fileMode::append, false))
{
	// Open the file
	//m_File = ;
}

void GRAVEngine::Logging::Sinks::fileSink::sinkMessage(const Logging::logMessage& message)
{
	// Write the message to the file
	m_File.write((void*)message.m_Payload, strlen(message.m_Payload));
}

void GRAVEngine::Logging::Sinks::fileSink::flush_()
{
	// Flush the file
	m_File.flush();
}
