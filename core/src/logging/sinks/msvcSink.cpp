#include "gravpch.h"
#include "msvcSink.h"

#ifdef GRAV_PLATFORM_WINDOWS
#ifndef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h> // For OutputDebugString()


void GRAVEngine::Logging::Sinks::msvcSink::sinkMessage(const Logging::logMessage& message)
{
	OutputDebugStringA(message.m_Payload);
}

void GRAVEngine::Logging::Sinks::msvcSink::flush_()
{
}

#endif