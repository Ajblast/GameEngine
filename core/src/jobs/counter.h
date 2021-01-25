#pragma once
#include <atomic>
#include "common.h"
#include "fiber.h"

#include "exceptions/exceptions.h"

namespace GRAVEngine
{
	namespace jobs
	{
		typedef uint32 counterTarget;

		class counter
		{
		public:
			counter();
			counter(const counter& other) = delete;				// Not allowed to copy counters
			counter(counter&& other) noexcept;

			counter& operator= (const counter& other) = delete;	// Not allowed to copy counters
			counter& operator= (counter&& other) noexcept;
			~counter();

			counterTarget increment()
			{
				// Fetch the previous value and increment it
				uint32 previousValue = m_Counter.fetch_add(1, std::memory_order::memory_order_seq_cst);
				return previousValue;
				// Check if there are any fibers associated with this counter that should be removed from the waiting list
				//checkWaitingFibers(previousValue + 1);
			}
			counterTarget decrement()
			{
				uint32 previousValue = m_Counter.fetch_sub(1, std::memory_order::memory_order_seq_cst);
				return previousValue;
				// Check if there are any fibers associated with this counter that should be removed from the waiting list
				//checkWaitingFibers(previousValue - 1);
			}

			uint32 getValue() const
			{
				return m_Counter.load(std::memory_order_seq_cst);
			}

			// Add a waiting fiber. Returns whether the waiting is already done
			//bool addWaitingFiber(fiberIndex index, counterTarget targetValue, std::atomic_bool* fiberStored);
			// Check if there are any fiber in the waiting list that need to be brought out of the list and be ran
			//void checkWaitingFibers(uint32 value);

		private:
			// Counter starts with 0
			std::atomic<counterTarget> m_Counter = 0;

			// Waiting Lists
			//uint8 m_MaxNumWaitingFibers;
			//
			//
			//waitingFiberSlot* m_WaitingFibers;
			//std::atomic_bool* m_FreeWaitingFibers;

		};
	}
}