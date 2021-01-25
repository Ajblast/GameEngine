#include "gravpch.h"
#include "reentrantLock32.h"

void GRAVEngine::Locks::reentrantLock32::acquire()
{
	std::hash<std::thread::id> hasher;
	std::size_t tid = hasher(std::this_thread::get_id());

	// If this thread doesn't already hold the lock
	if (m_atomic.load(std::memory_order_relaxed) != tid)
	{
		// Spin wait until lock acquired
		std::size_t unlockValue = 0;
		while (!m_atomic.compare_exchange_weak(unlockValue, tid, std::memory_order_relaxed, std::memory_order_relaxed))
		{
			unlockValue = 0;
			std::this_thread::yield();
		}
	}

	// Increment reference count so we can verify that acquire and release are calle din pairs
	++m_refCount;

	// Use an acquire fence to ensure all subsequent reads by this thread will be valid
	std::atomic_thread_fence(std::memory_order_acquire);
}

void GRAVEngine::Locks::reentrantLock32::release()
{
	// Use release semantics to ensure that all prior write have been fully committed before we unlock
	std::atomic_thread_fence(std::memory_order_release);

	std::hash<std::thread::id> hasher;
	std::size_t tid = hasher(std::this_thread::get_id());
	std::size_t actual = m_atomic.load(std::memory_order_relaxed);
	GRAV_ASSERT(actual == tid);

	--m_refCount;
	if (m_refCount == 0)
	{
		// Release Lock, which is safe since it is owned
		m_atomic.store(0, std::memory_order_relaxed);
	}
}

bool GRAVEngine::Locks::reentrantLock32::tryAcquire()
{
	std::hash<std::thread::id> hasher;
	std::size_t tid = hasher(std::this_thread::get_id());

	bool acquired = false;

	if (m_atomic.load(std::memory_order_relaxed) == tid)
		acquired = true;
	else
	{
		std::size_t unlockValue = 0;
		acquired = m_atomic.compare_exchange_strong(unlockValue, tid, std::memory_order_relaxed, std::memory_order_relaxed); // Fence below!
	}

	if (acquired)
	{
		++m_refCount;
		std::atomic_thread_fence(std::memory_order_acquire);
	}

	return acquired;
}
