#include "gravpch.h"
#include "counter.h"
#include "jobManager.h"

GRAVEngine::Jobs::counter::counter(jobManager* manager, counterTarget initialValue, fiberIndex maxWaitingFibers) : 
	m_JobManager(manager), m_FiberCount(maxWaitingFibers), m_Counter(initialValue),
	m_FreeFibers(createScope<std::atomic_bool[]>(maxWaitingFibers)),
	m_WaitingFibers(createScope<waitingFiberSlot[]>(maxWaitingFibers)),
	m_WaitingFiberInUse(createScope<std::atomic_bool[]>(maxWaitingFibers))
{
	// TODO: Implement checks to make sure that the allocations are the proper sizes

	for (size_t i = 0; i < m_FiberCount; i++)
	{
		// Fibers start as free
		m_FreeFibers[i].store(true);

		// Waiting fibers are considered to be in use when the slot is free because checkWaitingFibers will throw some garbage if not
		m_WaitingFiberInUse[i].store(true);
	}
}
GRAVEngine::Jobs::counter::~counter()
{
	// Keep this counter in memory while anything is interacting with it
	while (m_DestructionLock.load(std::memory_order_seq_cst) > 0)
		std::this_thread::yield();
}

GRAVEngine::Jobs::counterTarget GRAVEngine::Jobs::counter::increment()
{
	m_DestructionLock.fetch_add(1, std::memory_order_seq_cst);

	// Fetch the previous value and increment it
	counterTarget previousValue = m_Counter.fetch_add(1, std::memory_order::memory_order_seq_cst);
	checkWaitingFibers(previousValue + 1);

	m_DestructionLock.fetch_sub(1, std::memory_order_seq_cst);

	return previousValue;
}
GRAVEngine::Jobs::counterTarget GRAVEngine::Jobs::counter::decrement()
{
	m_DestructionLock.fetch_add(1, std::memory_order_seq_cst);
	
	// Fetch the previous value and decrement it
	counterTarget previousValue = m_Counter.fetch_sub(1, std::memory_order::memory_order_seq_cst);
	checkWaitingFibers(previousValue - 1);

	m_DestructionLock.fetch_sub(1, std::memory_order_seq_cst);

	return previousValue;
}
GRAVEngine::Jobs::counterTarget GRAVEngine::Jobs::counter::getValue()
{
	m_DestructionLock.fetch_add(1, std::memory_order_seq_cst);
	
	// Fetch the current value
	counterTarget value = m_Counter.load(std::memory_order::memory_order_seq_cst);

	m_DestructionLock.fetch_sub(1, std::memory_order_seq_cst);

	return value;
}

bool GRAVEngine::Jobs::counter::addWaitingFiber(fiberBundle* bundle, counterTarget target)
{
	GRAV_PROFILE_FUNCTION();

	for (size_t i = 0; i < m_FiberCount; i++)
	{
		// Determine if this indexed fiber is free, and obtain it if it is free
		bool expected = true;
		if (std::atomic_compare_exchange_strong_explicit(&m_FreeFibers[i], &expected, false, std::memory_order_seq_cst, std::memory_order_relaxed) == false)
			continue;

		// Free fiber is now found
		m_WaitingFibers[i].m_Bundle = bundle;
		m_WaitingFibers[i].m_TargetValue = target;
		m_WaitingFiberInUse[i].store(false, std::memory_order_seq_cst);	// Waiting fiber slot is currently not use as data has been written already


		// Check if the target was found while checking everything
		counterTarget loadedTarget = m_Counter.load(std::memory_order_relaxed);
		// Check if the waiting fiber is in use. Return false if it is because the current fiber isn't even done writing
		if (m_WaitingFiberInUse[i].load(std::memory_order_acquire) == true)
			return false;

		// Check the target now if the waiting fiber is free
		if (m_WaitingFibers[i].m_TargetValue == loadedTarget)
		{
			// The waiting fiber actually met the target and is currently not in use

			// Recheck if the waiting fiber is still not in use because there is a possible premption at the previous if statement
			expected = false;
			if (std::atomic_compare_exchange_strong_explicit(&m_WaitingFiberInUse[i], &expected, true, std::memory_order_seq_cst, std::memory_order_relaxed) == false)
				return false;	// Return false because another thread has already said that the previously free waiting fiber is now in use

			// The waiting fiber was not in use, but it is now.
			// The fiber slot is also now free as the fiber had finished execution
			m_FreeFibers[i].store(true, std::memory_order_release);

			return true;
		}

		// Waiting fiber's target wasn't met
		return false;
	}

	// There are no free slots, so die.
	throw GRAVEngine::Exceptions::counterWaitListFullException("Job Counter waiting slots full");
}

void GRAVEngine::Jobs::counter::checkWaitingFibers(counterTarget value)
{
	GRAV_PROFILE_FUNCTION();

	for (size_t i = 0; i < m_FiberCount; i++)
	{
		// Check if the slot is not in use. Don't check the slot if there isn't anything there
		if (m_FreeFibers[i].load(std::memory_order_acquire))
			continue;

		// Check if the waiting fiber is being potentially modified, and don't check it if so.
		if (m_WaitingFiberInUse[i].load(std::memory_order_acquire) == true)
			continue;

		// Check if the waiting fiber's target was met
		if (m_WaitingFibers[i].m_TargetValue == value)
		{
			// The waiting fiber actually met the target and is currently not in use

			// Recheck if the waiting fiber is still not in use because there is a possible premption at the previous if statement
			bool expected = false;
			if (std::atomic_compare_exchange_strong_explicit(&m_WaitingFiberInUse[i], &expected, true, std::memory_order_seq_cst, std::memory_order_relaxed) == false)
				continue;

			// Add this waiting fiber to the job manager as a now ready fiber
			m_JobManager->addReadyFiber(m_WaitingFibers[i].m_Bundle);

			// Set this fiber slot to be free as the waiting fiber is now gone
			// Keep the waiting fiber as still being in use so that the check fiber won't do extraneous readying
			m_FreeFibers[i].store(true, std::memory_order_release);
		}

	}
}

