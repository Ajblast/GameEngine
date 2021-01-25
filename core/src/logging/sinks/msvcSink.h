#pragma once

#include "baseSink.h"

#ifdef _WIN32
namespace GRAVEngine
{
	namespace Logging
	{
		namespace Sinks
		{

			class msvcSink : public baseSink
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