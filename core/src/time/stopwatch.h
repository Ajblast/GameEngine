#pragma once

#include <chrono>

#include "common.h" 
#include "durations.h"

namespace GRAVEngine
{
	namespace Time
	{
		// TODO: Allow stopwatches to use custom clocks. Maybe

		class GRAVAPI stopwatch
		{
		public:
			stopwatch();
			stopwatch(const stopwatch& other);
			stopwatch(stopwatch&& other) noexcept;

			stopwatch& operator=(stopwatch other);
			stopwatch& operator=(stopwatch&& other) noexcept;

			~stopwatch() = default;

			void start() noexcept;
			void pause() noexcept;
			void reset() noexcept;

			timeDurationCount elapsedNanoseconds() noexcept;
			double elapsedMicroseconds() noexcept;
			double elapsedMilliseconds() noexcept;
			double elapsedSeconds() noexcept;
			double elapsedMinutes() noexcept;
			double elapsedHours() noexcept;

			nanoseconds elapsedNanosecondsDuration() noexcept;
			microseconds elapsedMicrosecondsDuration() noexcept;
			milliseconds elapsedMillisecondsDuration() noexcept;
			seconds elapsedSecondsDuration() noexcept;
			minutes elapsedMinutesDuration() noexcept;
			hours elapsedHoursDuration() noexcept;

			inline timePoint startTick() { return m_StartTick; }
			inline timePoint endTick() { return m_EndTick; }
			inline bool isRunning() { return m_Running; }
		private:
			timePoint m_StartTick;
			timePoint m_EndTick;
			nanoseconds m_StoredDuration;

			bool m_Running = false;
		};

	}
}