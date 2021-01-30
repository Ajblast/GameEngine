#pragma once

#include "common.h"
#include <thread>

namespace GRAVEngine
{
	namespace Jobs
	{
		struct jobManagerOptions
		{
			jobManagerOptions() : m_NumThreads(std::thread::hardware_concurrency()) {}
			~jobManagerOptions() = default;

			// Threads and Fibers
			uint8 m_NumThreads;						// Number of worker threads. Default to the number of cores
			uint16 m_NumFibers = 128;				// Number of Fibers
			bool m_ThreadAffinity = true;			// Should threads be locked to cores. Only valid if the m_NumThreads == Num of cores
			bool m_AutoInitializeThreads = true;	// Should threads be spawned and initialized automatically
			bool m_ShutDownAfterMain = true;		// Should the job system shutdown after running the main method

			// Wait fiber size
			uint16 m_MaxWaitingFibersCount		= 256;

			// Job Queue Sizes
			size_t m_CriticalPriorityQueueSize	= 512;
			size_t m_HighPriorityQueueSize		= 1024;
			size_t m_NormalPriorityQueueSize	= 2048;
			size_t m_LowPriorityQueueSize		= 4096;
		};
	}
}