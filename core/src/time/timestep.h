#pragma once

#include "common.h"
#include "durations.h"

namespace GRAVEngine
{
	namespace Time
	{
		// Simple representation of a step in time
		class GRAVAPI timestep
		{
		public:
			// Clock start and stop points
			timestep(timePoint startTick, timePoint endTick);
			// The amount of time in nanoseconds
			timestep(timeDurationCount time);
			// The amount of time in seconds
			timestep(float time = 0.0f);

			operator float() const;
			double getSeconds() const;
			double getMilliseconds() const;

		private:
			// The elapsed duration in nanoseconds
			nanoseconds m_Time;
		};
	}
}