#include "gravpch.h"
#include "counter.h"

GRAVEngine::jobs::counter::counter() {}
GRAVEngine::jobs::counter::counter(counter&& other) noexcept //: m_MaxNumWaitingFibers(other.m_MaxNumWaitingFibers), m_WaitingFibers(other.m_WaitingFibers), m_FreeWaitingFibers(other.m_FreeWaitingFibers)
{
	// Steal other resources
	m_Counter.store(other.m_Counter.load(std::memory_order_acquire), std::memory_order_release);

	// Release the stolen resources
	other.m_Counter.store(0, std::memory_order_release);
	//other.m_WaitingFibers = nullptr;
	//other.m_FreeWaitingFibers = nullptr;
	//other.m_MaxNumWaitingFibers = 0;
}
GRAVEngine::jobs::counter& GRAVEngine::jobs::counter::operator=(counter&& other) noexcept
{
	if (this != &other)
	{
		m_Counter.store(other.m_Counter.load(std::memory_order_acquire), std::memory_order_release);

		// Release stolen resources
		other.m_Counter.store(0, std::memory_order_release);
		//other.m_WaitingFibers = nullptr;
		//other.m_FreeWaitingFibers = nullptr;
		//other.m_MaxNumWaitingFibers = 0;
	}

	return *this;
}
GRAVEngine::jobs::counter::~counter() {}

