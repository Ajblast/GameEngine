#pragma once
#include <atomic>
#include "common.h"

namespace GRAVEngine
{
	namespace Locks
	{
		class reentrantLock32
		{
			std::atomic<std::size_t> m_atomic;
			int32 m_refCount;

		public:
			reentrantLock32() : m_atomic(0), m_refCount(0) {}

			// Acquire the lock
			void lock();
			// Release the lock
			void unlock();

			// attempt to acquire the lock
			bool try_lock();
		};
	}
}