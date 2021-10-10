#pragma once

#include "common.h"
#include <atomic>

#define GRAV_MAX_THREAD_ID UINT32_MAX
#define GRAV_MAX_THREAD_INDEX UINT8_MAX
#define GRAV_MAX_FIBER_INDEX UINT16_MAX

namespace GRAVEngine
{
	namespace Jobs
	{
		typedef uint16 fiberIndex;		// A fiber index
		typedef void* fiberHandle;		// A handle to a fiber

		typedef uint32 counterTarget;	// A target for a counter

		typedef uint32 threadID;		// A thread ID
		typedef uint8 threadIndex;		// A thread index
		typedef void* threadHandle;		// A thread handle

		struct fiberBundle
		{
		public:
			fiberIndex m_Index;					// The fiber's index
			std::atomic_bool m_FiberIsSwitched;	// Has the fiber been switch out of and cleaned up
		};

	}
}