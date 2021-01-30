#pragma once

#include <chrono>

#include "common.h" 
#include "durations.h"

namespace GRAVEngine
{
	namespace Time
	{
		// TODO: Allow stopwatches to use custom clocks. Maybe

		class stopwatch
		{
		public:
			stopwatch() = default;
			stopwatch(const stopwatch& other);
			stopwatch(stopwatch&& other) noexcept;

			stopwatch& operator=(stopwatch other);
			stopwatch& operator=(stopwatch&& other) noexcept;

			~stopwatch() = default;

			void start() noexcept;
			void pause() noexcept;
			void reset() noexcept;

			timeDurationCount elapsedNanoseconds() noexcept;
			timeDurationCount elapsedMilliseconds() noexcept;
			timeDurationCount elapsedSeconds() noexcept;
			timeDurationCount elapsedMinutes() noexcept;
			timeDurationCount elapsedHours() noexcept;


		private:
			timePoint m_StartTick;
			nanoseconds m_StoredDuration;

			bool m_Running = false;
		};

	}
}