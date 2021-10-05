#pragma once
#include "common.h"

#include "jobPriorities.h"
#include "counter.h"
#include <functional>

namespace GRAVEngine
{
	namespace Jobs
	{
		//typedef void (*entryPoint)(uintptr_t param);

		typedef std::function<void(uintptr_t)> entryPoint;


		struct declaration
		{
			entryPoint m_EntryPoint;
			uintptr_t m_Param;
			jobPriority m_Priority;
			ref<counter> m_Counter;

			// TODO: Add debug job declaration string

			declaration() : m_EntryPoint(nullptr), m_Param(0), m_Priority(jobPriority::LOW), m_Counter(nullptr) {}
			declaration(entryPoint entryPoint, uintptr_t param, jobPriority priority, ref<counter> counter) : 
				m_EntryPoint(entryPoint), m_Param(param), m_Priority(priority), m_Counter(counter)
			{}
		};
	}
}