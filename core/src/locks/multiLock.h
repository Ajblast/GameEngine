#pragma once
#include "common.h"
#include "standardLocking.h"
#include <mutex>

namespace GRAVEngine
{
	namespace Locks
	{
		// A wrapper around a set of mutex locks and will acquire all of the locks at a time
		template<typename... L>
		class GRAVAPI multiLock
		{
		public:
			explicit multiLock(L&... locks) : m_Locks(locks...) {}

			// Try acquire all of the locks. Returns which lock failed
			bool try_lock()
			{
				//std::apply(Locks::lock, m_Locks);
				std::apply([](L&... locks) {std::try_lock(locks...); }, m_Locks);
			}

			// Acquire the lock
			void lock()
			{
				std::apply([](L&... locks) {std::lock(locks...); }, m_Locks);

				//std::apply(Locks::lock, m_Locks);
			}
			// Release the lock
			void unlock()
			{
				std::apply([](L&... locks) { (..., (void)locks.unlock()); }, m_Locks);

				//std::apply([](auto&&... args) {Locks::unlock(args...); }, m_Locks);
			}
		private:
			std::tuple<L&...> m_Locks;
		};
	}
}