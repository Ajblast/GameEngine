#pragma once
#include <atomic>

#include "common.h"

namespace GRAVEngine
{
	namespace Locks
	{
		class spinLock
		{
			std::atomic_flag m_atomic = ATOMIC_FLAG_INIT;

		public:
			spinLock() {}

			// Try acquire the lock
			bool tryAcquire();

			// Acquire the lock
			void acquire();
			// Release the lock
			void release();
		};
	}
}