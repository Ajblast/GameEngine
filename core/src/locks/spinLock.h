#pragma once
#include <atomic>

#include "common.h"

namespace GRAVEngine
{
	namespace Locks
	{
		class GRAVAPI spinLock
		{
			std::atomic_flag m_atomic = ATOMIC_FLAG_INIT;

		public:
			spinLock() {}

			// Try acquire the lock
			bool try_lock();

			// Acquire the lock
			void lock();
			// Release the lock
			void unlock();
		};
	}
}