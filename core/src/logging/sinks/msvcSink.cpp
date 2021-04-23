#include "gravpch.h"
#include "msvcSink.h"

#ifdef GRAV_PLATFORM_WINDOWS
#ifndef	WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NOMINMAX
#include <Windows.h> // For OutputDebugString()


void GRAVEngine::Logging::Sinks::msvcSink::sinkMessage(const Logging::logMessage& message)
{
	OutputDebugStringA(message.m_Payload.c_str());
}

void GRAVEngine::Logging::Sinks::msvcSink::flush_()
{
}

#endif