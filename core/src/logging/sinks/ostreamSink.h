#pragma once

#include "baseSink.h"

namespace GRAVEngine
{
	namespace Logging
	{
		namespace Sinks
		{
			class GRAVAPI ostreamSink final : public baseSink
			{
			public:
				explicit ostreamSink(std::ostream& os, bool forceFlush = false) : m_ostream(os), m_ForceFlush(forceFlush) {}

				ostreamSink(const ostreamSink&) = delete;				// Delete copy constructor
				ostreamSink& operator= (const ostreamSink&) = delete;	// Delete set constructor

			protected:
				virtual void sinkMessage(const Logging::logMessage& message) override;
				virtual void flush_() override;

				std::ostream& m_ostream;
				bool m_ForceFlush;
			};
		}
	}
}