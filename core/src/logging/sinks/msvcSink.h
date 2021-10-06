#pragma once

#include "common.h"
#include "baseSink.h"

#ifdef GRAV_PLATFORM_WINDOWS
namespace GRAVEngine
{
	namespace Logging
	{
		namespace Sinks
		{

			class GRAVAPI msvcSink : public baseSink
			{
			public:
				msvcSink() = default;

			protected:
				// Inherited via baseSink
				virtual void sinkMessage(const Logging::logMessage& message) final;
				virtual void flush_() final;
			};
		}
	}
}
#endif