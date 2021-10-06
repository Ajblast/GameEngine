#include "gravpch.h"
#include "timestep.h"

// The amount of time in nanoseconds


// Clock start and stop points

GRAVEngine::Time::timestep::timestep(timePoint startTick, timePoint endTick) : m_Time(endTick - startTick) {}

GRAVEngine::Time::timestep::timestep(timeDurationCount time) : m_Time(time) {}

// The amount of time in seconds

GRAVEngine::Time::timestep::timestep(float time) : m_Time(std::chrono::duration_cast<nanoseconds>(seconds(time))) {}

GRAVEngine::Time::timestep::operator float() const
{
	return std::chrono::duration_cast<seconds>(m_Time).count();
}

double GRAVEngine::Time::timestep::getSeconds() const
{
	return std::chrono::duration_cast<seconds>(m_Time).count();
}

double GRAVEngine::Time::timestep::getMilliseconds() const
{
	return std::chrono::duration_cast<milliseconds>(m_Time).count();
}
