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
	nanoseconds elapsedDuration = m_StoredDuration;

	// Calculate the current duration if the timer is running
	if(m_Running)
	{ 
		// Add to the duration if the stopwatch is running
		elapsedDuration += (std::chrono::steady_clock::now() - m_StartTick);
	}

	return std::chrono::duration_cast<nanoseconds>(elapsedDuration).count();
}
double GRAVEngine::Time::stopwatch::elapsedMilliseconds() noexcept
{
	// Get the current duration
	nanoseconds elapsedDuration = m_StoredDuration;

	// Calculate the current duration if the timer is running
	if (m_Running)
	{
		// Add to the duration if the stopwatch is running
		elapsedDuration += (std::chrono::steady_clock::now() - m_StartTick);
	}

	return std::chrono::duration_cast<milliseconds>(elapsedDuration).count();
}
double GRAVEngine::Time::stopwatch::elapsedSeconds() noexcept
{
	// Get the current duration
	nanoseconds elapsedDuration = m_StoredDuration;

	// Calculate the current duration if the timer is running
	if (m_Running)
	{
		// Add to the duration if the stopwatch is running
		elapsedDuration += (std::chrono::steady_clock::now() - m_StartTick);
	}

	return std::chrono::duration_cast<seconds>(elapsedDuration).count();
}
double GRAVEngine::Time::stopwatch::elapsedMinutes() noexcept
{
	// Get the current duration
	nanoseconds elapsedDuration = m_StoredDuration;

	// Calculate the current duration if the timer is running
	if (m_Running)
	{
		// Add to the duration if the stopwatch is running
		elapsedDuration += (std::chrono::steady_clock::now() - m_StartTick);
	}

	return std::chrono::duration_cast<minutes>(elapsedDuration).count();
}
double GRAVEngine::Time::stopwatch::elapsedHours() noexcept
{
	// Get the current duration
	nanoseconds elapsedDuration = m_StoredDuration;

	// Calculate the current duration if the timer is running
	if (m_Running)
	{
		// Add to the duration if the stopwatch is running
		elapsedDuration += (std::chrono::steady_clock::now() - m_StartTick);
	}

	return std::chrono::duration_cast<hours>(elapsedDuration).count();
}

