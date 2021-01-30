#include "gravpch.h"
#include "stopwatch.h"

GRAVEngine::Time::stopwatch::stopwatch(const stopwatch& other) : m_StartTick(other.m_StartTick), m_Running(false), m_StoredDuration(other.m_StoredDuration)
{
}
GRAVEngine::Time::stopwatch::stopwatch(stopwatch&& other) noexcept : m_StartTick(std::move(other.m_StartTick)), m_Running(std::move(other.m_Running)), m_StoredDuration(std::move(other.m_Running))
{
}
GRAVEngine::Time::stopwatch& GRAVEngine::Time::stopwatch::operator=(stopwatch other)
{
	m_StartTick = other.m_StartTick;
	m_Running = other.m_Running;
	m_StoredDuration = other.m_StoredDuration;

	return *this;
}
GRAVEngine::Time::stopwatch& GRAVEngine::Time::stopwatch::operator=(stopwatch&& other) noexcept
{
	if (this != &other)
	{
		m_StartTick = std::move(other.m_StartTick);
		m_Running = std::move(other.m_Running);
		m_StoredDuration = other.m_StoredDuration;
	}

	return *this;
}

void GRAVEngine::Time::stopwatch::start() noexcept
{
	// Do nothing if it is currently running
	if (m_Running)
		return;

	// Get the current time
	m_StartTick = std::chrono::steady_clock::now();

	m_Running = true;
}
void GRAVEngine::Time::stopwatch::pause() noexcept
{
	// Do nothing if it is not currently running
	if (!m_Running)
		return;

	// Get the current time
	timePoint currentTick = std::chrono::steady_clock::now();

	// Get the current duration
	nanoseconds currentDuration = std::chrono::duration_cast<nanoseconds>(currentTick - m_StartTick);

	// Add the curernt duration to the stored duration
	m_StoredDuration += currentDuration;

	m_Running = false;
}
void GRAVEngine::Time::stopwatch::reset() noexcept
{
	// Reset the stored duration
	m_StoredDuration = nanoseconds(0);

	// Set the starting tick
	m_StartTick = std::chrono::steady_clock::now();
}

GRAVEngine::Time::timeDurationCount GRAVEngine::Time::stopwatch::elapsedNanoseconds() noexcept
{
	// Get the current duration
	timeDurationCount elapsedDuration = m_StoredDuration.count();

	// Calculate the current duration if the timer is running
	if(m_Running)
	{ 
		// Get the current time
		timePoint currentTick = std::chrono::steady_clock::now();

		// Get the current duration
		nanoseconds currentDuration = std::chrono::duration_cast<nanoseconds>(currentTick - m_StartTick);

		// Add the curernt duration to the elapsed duration
		elapsedDuration += currentDuration.count();
	}

	return elapsedDuration;
}
GRAVEngine::Time::timeDurationCount GRAVEngine::Time::stopwatch::elapsedMilliseconds() noexcept
{
	// Get the current duration
	timeDurationCount elapsedDuration = std::chrono::duration_cast<milliseconds>(m_StoredDuration).count();

	// Calculate the current duration if the timer is running
	if (m_Running)
	{
		// Get the current time
		timePoint currentTick = std::chrono::steady_clock::now();

		// Get the current duration
		nanoseconds currentDuration = std::chrono::duration_cast<milliseconds>(currentTick - m_StartTick);

		// Add the curernt duration to the elapsed duration
		elapsedDuration += currentDuration.count();
	}

	return elapsedDuration;
}
GRAVEngine::Time::timeDurationCount GRAVEngine::Time::stopwatch::elapsedSeconds() noexcept
{
	// Get the current duration
	timeDurationCount elapsedDuration = std::chrono::duration_cast<seconds>(m_StoredDuration).count();

	// Calculate the current duration if the timer is running
	if (m_Running)
	{
		// Get the current time
		timePoint currentTick = std::chrono::steady_clock::now();

		// Get the current duration
		nanoseconds currentDuration = std::chrono::duration_cast<seconds>(currentTick - m_StartTick);

		// Add the curernt duration to the elapsed duration
		elapsedDuration += currentDuration.count();
	}

	return elapsedDuration;
}
GRAVEngine::Time::timeDurationCount GRAVEngine::Time::stopwatch::elapsedMinutes() noexcept
{
	// Get the current duration
	timeDurationCount elapsedDuration = std::chrono::duration_cast<minutes>(m_StoredDuration).count();

	// Calculate the current duration if the timer is running
	if (m_Running)
	{
		// Get the current time
		timePoint currentTick = std::chrono::steady_clock::now();

		// Get the current duration
		nanoseconds currentDuration = std::chrono::duration_cast<minutes>(currentTick - m_StartTick);

		// Add the curernt duration to the elapsed duration
		elapsedDuration += currentDuration.count();
	}

	return elapsedDuration;
}
GRAVEngine::Time::timeDurationCount GRAVEngine::Time::stopwatch::elapsedHours() noexcept
{
	// Get the current duration
	timeDurationCount elapsedDuration = std::chrono::duration_cast<hours>(m_StoredDuration).count();

	// Calculate the current duration if the timer is running
	if (m_Running)
	{
		// Get the current time
		timePoint currentTick = std::chrono::steady_clock::now();

		// Get the current duration
		nanoseconds currentDuration = std::chrono::duration_cast<hours>(currentTick - m_StartTick);

		// Add the curernt duration to the elapsed duration
		elapsedDuration += currentDuration.count();
	}

	return elapsedDuration;
}

