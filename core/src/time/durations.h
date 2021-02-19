#pragma once

#include <chrono>
#include "common.h"

namespace GRAVEngine
{
	namespace Time
	{
		typedef uint64 timeDurationCount;
		typedef std::chrono::steady_clock::time_point timePoint;

		typedef std::chrono::duration<timeDurationCount, std::nano>	nanoseconds;
		typedef std::chrono::duration<float, std::milli>			milliseconds;
		typedef std::chrono::duration<float>						seconds;
		typedef std::chrono::duration<float, std::ratio<60>>		minutes;
		typedef std::chrono::duration<float, std::ratio<3600>>		hours;

		constexpr timeDurationCount clockFrequency()
		{
			return std::chrono::steady_clock::period::den;
		}
	}
}