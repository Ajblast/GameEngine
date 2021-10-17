#pragma once
#include <atomic>
#include "common.h"
#include "jobTypes.h"

#define MAX_NUM_WAITING_FIBERS 4

namespace GRAVEngine
{
	namespace Jobs
	{
		// Forward define the job manager so that we can store a pointer to it
		class jobManager;

		class GRAVAPI counter
		{
			friend jobManager;	// Job manager is a friend to make it easier to add a waiting fiber

		public:
			//counter(jobManager* manager, fiberIndex maxWaitingFibers = MAX_NUM_WAITING_FIBERS);
			counter(jobManager* manager, counterTarget initialValue, fiberIndex maxWaitingFibers = MAX_NUM_WAITING_FIBERS);
			~counter();

			counter(const counter& other) = delete;					// Not allowed to copy construct
			counter(counter&& other) noexcept = delete;				// Not allowed to move construct
			counter& operator= (const counter& other) = delete;		// Not allowed to copy set
			counter& operator= (counter&& other) noexcept = delete;	// Not allowed to move set

			// Atomically increment the counter
			counterTarget increment();
			// Atomically decrement the counter
			counterTarget decrement();
			// Atomically get the value in the counter
			counterTarget getValue();

		private:
			// A slot in the waiting fiber array
			struct waitingFiberSlot
			{
				fiberBundle* m_Bundle;							// The fiber bundle
				//fiberIndex m_FiberIndex = GRAV_MAX_FIBER_INDEX;	// The fiber index
				counterTarget m_TargetValue = 0;				// The target value the fiber is waiting for
			};

			// Add a fiber to this counter's waiting list. Returns whether the waiting was already done while add this fiber to the waiting list
			bool addWaitingFiber(fiberBundle* bundle, counterTarget target);
			// Check if there are any fiber in the waiting list that need to be brought out of the list and be ran because their target == value
			void checkWaitingFibers(counterTarget value);

		private:
			std::atomic<counterTarget> m_Counter = 0;		// Counter starts with 0, but will be initialized to the initial value
			std::atomic_uint16_t m_DestructionLock = 0;		// A single atomic counter that makes sure that the counter can't be destructed while incrementing/decrementing
			jobManager* m_JobManager;						// Reference to the jobManager to easily readd waiting fibers back

			fiberIndex m_FiberCount;						// The maximum number of fibers for waiting
			scope<std::atomic_bool[]> m_FreeFibers;			// Which fibers are free
			scope<waitingFiberSlot[]> m_WaitingFibers;		// The slots where a fiber is placed when it needs to wait
			scope<std::atomic_bool[]> m_WaitingFiberInUse;	// An array holding which waiting fiber is being used to add/remove data.

		};
	}
}