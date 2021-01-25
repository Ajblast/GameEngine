#pragma once

#include "common.h"

namespace GRAVEngine
{
	namespace Locks
	{
		template<class LOCK>
		class scopedLock
		{
			typedef LOCK lock_t;
			lock_t* m_pLock;

		public:
			explicit scopedLock(lock_t& lock) : m_pLock(&lock)
			{
				m_pLock->acquire();
			}
			~scopedLock()
			{
				m_pLock->release();
			}
		};
	}
}