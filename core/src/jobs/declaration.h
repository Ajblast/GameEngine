#pragma once
#include "common.h"

#include "jobPriorities.h"
#include "counter.h"
#include <functional>

#define GRAV_JOB_ENTRY_POINT(jobName)				void jobName(uintptr_t param)

namespace GRAVEngine
{
	namespace Jobs
	{
		typedef std::function<void(uintptr_t)> entryPoint;	// A job's entry point

		// A job declaration
		struct GRAVAPI declaration
		{
			entryPoint m_EntryPoint;	// The job function pointer
			uintptr_t m_Arg;			// Pointer to the job's arguments
			jobPriority m_Priority;		// The priority of the job

		public:
			declaration() : m_Arg(0), m_Priority(jobPriority::NORMAL) {}
			declaration(entryPoint entryPoint, uintptr_t arg, jobPriority priority = jobPriority::NORMAL) : m_EntryPoint(entryPoint), m_Arg(arg), m_Priority(priority) {}
		};
	}
}