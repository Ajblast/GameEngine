#pragma once

#include "common.h"
#include "durations.h"

namespace GRAVEngine
{
	namespace Time
	{
		// Simple representation of a step in time
		class timestep
		{
		public:
			timestep(timePoint startTick, timePoint endTick) : m_Time(endTick - startTick) {}
			timestep(timeDurationCount time = 0) : m_Time(time) {}
			timestep(float time = 0.0f) : m_Time(std::chrono::duration_cast<nanoseconds>(seconds(time))) {}

			operator float() const
			{
				return std::chrono::duration_cast<seconds>(m_Time).count();
			}

			double getSeconds() const
			{
				return std::chrono::duration_cast<seconds>(m_Time).count();
			}
			double getMilliseconds() const
			{
				return std::chrono::duration_cast<milliseconds>(m_Time).count();
			}

		private:
			// The elapsed duration in nanoseconds
			nanoseconds m_Time;
		};
	}
}