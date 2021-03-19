#pragma once

#include "sink.h"
#include "locks/spinLock.h"
#include "locks/scopedLock.h"

namespace GRAVEngine
{
	namespace Logging
	{
		namespace Sinks
		{
			// Base sink class which handles locking. Implementations do not need to worry about locking
			class baseSink : public sink
			{
			public:
				baseSink() {}
				virtual ~baseSink() override = default;

				baseSink(const baseSink&) = delete;	// Delete copy constructor
				baseSink(baseSink&&) = delete;		// Delete move constructor

				baseSink& operator= (const baseSink&) = delete;	// Delete set constructor
				baseSink& operator= (baseSink&&) = delete;		// Delete move set constructor

				// Log a message
				void log(const Logging::logMessage& message) final;
				// Flush the sink
				void flush() final;
			protected:
				GRAVEngine::Locks::spinLock m_Lock;

				// Sink a message in the sink
				virtual void sinkMessage(const Logging::logMessage& message) = 0;
				// Virtual flush
				virtual void flush_() = 0;
			};
		}
	}
}