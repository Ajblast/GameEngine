#pragma once
#include "common.h"

#include "jobPriorities.h"
#include "counter.h"

namespace GRAVEngine
{
	namespace Jobs
	{
		typedef void (*entryPoint)(uintptr_t param);

		struct declaration
		{
			entryPoint m_EntryPoint;
			uintptr_t m_Param;
			jobPriority m_Priority;
			counter* m_Counter;

			// TODO: Add debug job declaration string

			declaration() : m_EntryPoint(nullptr), m_Param(0), m_Priority(jobPriority::LOW), m_Counter(nullptr) {}
			declaration(entryPoint entryPoint, uintptr_t param, jobPriority priority, counter* counter) : 
				m_EntryPoint(entryPoint), m_Param(param), m_Priority(priority), m_Counter(counter)
			{}
		};
	}
}