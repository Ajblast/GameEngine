#pragma once

#include <atomic>

#define GRAVEngine_VERBOSITY_OFF		0
#define GRAVEngine_VERBOSITY_CRITICAL	1
#define GRAVEngine_VERBOSITY_ERROR		2
#define GRAVEngine_VERBOSITY_WARN		3
#define GRAVEngine_VERBOSITY_INFO		4
#define GRAVEngine_VERBOSITY_DEBUG		5
#define GRAVEngine_VERBOSITY_TRACE		6

#define GRAVEngine_VERBOSITY_STRING_OFF			"OFF"
#define GRAVEngine_VERBOSITY_STRING_CRITICAL	"CRITICAL"
#define GRAVEngine_VERBOSITY_STRING_ERROR		"ERROR"
#define GRAVEngine_VERBOSITY_STRING_WARN		"WARN"
#define GRAVEngine_VERBOSITY_STRING_INFO		"INFO"
#define GRAVEngine_VERBOSITY_STRING_DEBUG		"DEBUG"
#define GRAVEngine_VERBOSITY_STRING_TRACE		"TRACE"

namespace GRAVEngine
{
	namespace Logging
	{
		typedef std::atomic<int> verbosity_t;

		enum class verbosity
		{
			off			= GRAVEngine_VERBOSITY_OFF,
			critical	= GRAVEngine_VERBOSITY_CRITICAL,
			error		= GRAVEngine_VERBOSITY_ERROR,
			warn		= GRAVEngine_VERBOSITY_WARN,
			info		= GRAVEngine_VERBOSITY_INFO,
			debug		= GRAVEngine_VERBOSITY_DEBUG,
			trace		= GRAVEngine_VERBOSITY_TRACE
		};

		static const char* verbosityNames[]
		{
			GRAVEngine_VERBOSITY_STRING_OFF,
			GRAVEngine_VERBOSITY_STRING_CRITICAL,
			GRAVEngine_VERBOSITY_STRING_ERROR,
			GRAVEngine_VERBOSITY_STRING_WARN,
			GRAVEngine_VERBOSITY_STRING_INFO,
			GRAVEngine_VERBOSITY_STRING_DEBUG,
			GRAVEngine_VERBOSITY_STRING_TRACE
		};

		GRAVAPI inline const char* toString(GRAVEngine::Logging::verbosity verbosity)
		{
			return verbosityNames[static_cast<int>(verbosity)];
		}

		GRAVAPI inline GRAVEngine::Logging::verbosity logVerbosityToEnum(const char* name)
		{
			int level = 0;

			for (auto levelstr : verbosityNames)
			{
				if (strcmp(levelstr, name))
					return static_cast<GRAVEngine::Logging::verbosity>(level);

				level++;
			}

			return GRAVEngine::Logging::verbosity::off;
		}
	}
}