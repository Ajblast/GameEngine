#pragma once

#include "common.h"
#include "logging/logMessage.h"
#include "logging/logVerbosity.h"

namespace GRAVEngine
{
	namespace Logging
	{
		namespace Sinks
		{
			class sink
			{
			public:
				virtual ~sink() = default;
				virtual void log(const Logging::logMessage& message) = 0;

				virtual void flush() = 0;

				inline void setVerobisty(Logging::verbosity verbosity)
				{
					m_Verbosity.store(static_cast<int>(verbosity), std::memory_order_relaxed);
				}
				inline Logging::verbosity verbosity() const
				{
					return static_cast<Logging::verbosity>(m_Verbosity.load(std::memory_order_relaxed));
				}

				inline bool shouldLog(Logging::verbosity verbosity) const
				{
					return m_Verbosity.load(std::memory_order_relaxed) >= static_cast<int>(verbosity);
				}
			protected:
				// Sink verbosity level
				verbosity_t m_Verbosity{ static_cast<int>(verbosity::trace) };
			};
		}
	}
}