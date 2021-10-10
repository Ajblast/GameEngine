#pragma once
#include <vector>

#include "common.h"
#include "jobTypes.h"
#include "fiber.h"

namespace GRAVEngine
{
	namespace Jobs
	{
		enum class fiberDestination : uint8
		{
			NONE, WAITNIG, POOL
		};

		struct tls
		{
			tls() : m_CurrentFiberIndex(GRAV_MAX_FIBER_INDEX) {}
			~tls() = default;

			// TODO: Make special threads for IO
			//bool m_IsIOThread = false;

			// The thread's personal starting fiber that is outside of the fiber pool.
			// This is used to start fiber's into the fiber pool, and allows the fiber pool to return back to the calling thread callback after execution ends
			fiber m_Fiber;

			// Current fiber
			fiberIndex m_CurrentFiberIndex;


			// Previous thread fiber
			uint16 m_PreviousFiberIndex = UINT16_MAX;
			std::atomic_bool* m_PreviousFiberStored = nullptr;
			// Where was the previous fiber going to be placed
			fiberDestination m_PreviousFiberDestination = fiberDestination::NONE;
		};
	}
}