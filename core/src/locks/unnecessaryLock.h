#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace Locks
	{
		class unnecessaryLock
		{
			volatile bool m_locked;

		public:
			// Acquire the lock
			void acquire();
			// Release the lock
			void release();

#ifdef ASSERTIONS_ENABLED
#define BEGIN_ASSERT_LOCK_NOT_NECESSARY(L) (L).acquire()
#define END_ASSERT_LOCK_NOT_NECESSARY(L) (L).release()
#else
#define BEGIN_ASSERT_LOCK_NOT_NECESSARY(L)
#define END_ASSERT_LOCK_NOT_NECESSARY(L)
#endif
		};

		class unnecessaryLockJanitor
		{
			unnecessaryLock* m_pLock;
		public:

			explicit unnecessaryLockJanitor(unnecessaryLock& lock) : m_pLock(&lock)
			{
				m_pLock->acquire();
			}
			~unnecessaryLockJanitor()
			{
				m_pLock->release();
			}

		};

#ifdef ASSERTIONS_ENABLED
#define ASSERT_LOCK_NOT_NECESSARY(J,L) \
	unnecessaryLockJanitor J(L)
#else
#define ASSERT_LOCK_NOT_NECESSARY(J,L)
#endif 

	}
}