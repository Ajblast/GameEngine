#pragma once

#include <chrono>

#include "common.h"
#include "durations.h"

// TODO: Finish implementation of timer class
// TODO: Allow function ality to fire off automatically when the timer "Finishes"

namespace GRAVEngine
{
	namespace Time
	{
		class timer
		{
		public:
			void start() noexcept;
			void pause() noexcept;
			void reset() noexcept;

		private:
			uint64 m_StartTick;
			uint64 m_EndTick;
		};

	}
}