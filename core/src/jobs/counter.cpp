#include "gravpch.h"
#include "counter.h"

GRAVEngine::jobs::counter::counter() //: m_MaxNumWaitingFibers(maxNumWaitingFibers)
{
	// Create waiting fiber list
	//m_WaitingFibers = new waitingFiberSlot[maxNumWaitingFibers];
	//m_FreeWaitingFibers = new std::atomic_bool[maxNumWaitingFibers];
}
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
		// Release current resources
		//delete[] m_WaitingFibers;
		//delete[] m_FreeWaitingFibers;

		// Steal resources
		//m_MaxNumWaitingFibers = other.m_MaxNumWaitingFibers;
		//m_WaitingFibers = other.m_WaitingFibers;
		//m_FreeWaitingFibers = other.m_FreeWaitingFibers;
		m_Counter.store(other.m_Counter.load(std::memory_order_acquire), std::memory_order_release);

		// Release stolen resources
		other.m_Counter.store(0, std::memory_order_release);
		//other.m_WaitingFibers = nullptr;
		//other.m_FreeWaitingFibers = nullptr;
		//other.m_MaxNumWaitingFibers = 0;
	}

	return *this;
}
GRAVEngine::jobs::counter::~counter()
{
	//delete[] m_WaitingFibers;
	//delete[] m_FreeWaitingFibers;
}


//bool GRAVEngine::jobs::counter::addWaitingFiber(fiberIndex index, counterTarget targetValue, std::atomic_bool* fiberStored)
//{
//	for (size_t i = 0; i < m_MaxNumWaitingFibers; i++)
//	{
//		// If this waiting fiber is free, obtain it so that 
//		bool expected = true;
//		if (std::atomic_compare_exchange_strong_explicit(&m_FreeWaitingFibers[i], &expected, false, std::memory_order_seq_cst, std::memory_order_relaxed) == false)
//			continue;
//
//		// This waiting fiber slot is now obtained
//		// m_FreeWaitingFibers[i] == false
//
//
//		// Setup the slot for actual use
//		auto slot = &m_WaitingFibers[i];
//		slot->m_FiberIndex = index;			// Store the index of the the waiting fiber
//		slot->m_FiberStored = fiberStored;	// Pointer to bool of whether this fiber is currently stored in a waiting list
//		slot->m_TargetValue = targetValue;	// Target value for the waiting thread
//		slot->m_InUse.store(false, std::memory_order_release);	// Is this slot in use overall?
//
//
//		// Check if the job is done already
//		// The job is not done if the slot is now in use. Thread beat us to using this slot
//		if (slot->m_InUse.load(std::memory_order_acquire))
//			return false;
//
//		// The job is not done if the target hasn't been met
//		if (slot->m_TargetValue != m_Counter.load(std::memory_order_acquire))
//			return false;
//
//		// The job is not done if slot is in use (Another thread beat this thread
//		expected = false;
//		if (std::atomic_compare_exchange_strong_explicit(&slot->m_InUse, &expected, true, std::memory_order_seq_cst, std::memory_order_relaxed) == false)
//			return false;
//
//		// This job is actually already done, so free up this slot
//		m_FreeWaitingFibers[i].store(true, std::memory_order_release);
//		return true;
//	}
//
//	// There are no free slots
//	throw GRAVEngine::Exceptions::counterWaitListFullException("Job Counter waiting slots full");
//}

//void GRAVEngine::jobs::counter::checkWaitingFibers(uint32 value)
//{
//	for (size_t i = 0; i < m_MaxNumWaitingFibers; i++)
//	{
//		// Is this waiting slot free?
//		if (m_FreeWaitingFibers[i].load(std::memory_order_acquire))
//			continue;	// Can't do anything when the slot is free
//
//						// Get the slot
//		auto waitingSlot = &m_WaitingFibers[i];
//		// This fiber is not ready to be brought out if the slot is in use
//		if (waitingSlot->m_InUse.load(std::memory_order_acquire))
//			continue;
//
//		// This fiber is not ready to be brought out if the target is still not met
//		if (waitingSlot->m_TargetValue != value)
//			continue;
//
//		// If not in use, this slot is now in use
//		// If it is in use, expected = true
//		bool expected = false;
//		if (std::atomic_compare_exchange_strong_explicit(&waitingSlot->m_InUse, &expected, true, std::memory_order_seq_cst, std::memory_order_relaxed) == false)
//			continue;
//
//		// Place this fiber into the ready fiber list
//		GRAVEngine::jobs::jobManager::getInstance()->getCurrentTLS()->m_ReadyFibers.emplace_back(waitingSlot->m_FiberIndex, waitingSlot->m_FiberStored);
//		m_FreeWaitingFibers[i].store(true, std::memory_order_release);
//	}
//}
