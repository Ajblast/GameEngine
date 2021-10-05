#pragma once

#include <string>
#include <sstream>
#include <iomanip>

#include "common.h"
#include "time/durations.h"
#include "time/stopwatch.h"
#include "jobs/jobManager.h"
#include "locks/spinLock.h"
#include "locks/scopedLock.h"
#include "io/textFile.h"

#include "debug/functionSignature.h"

namespace GRAVEngine
{
	struct profileResult
	{
		std::string m_ProfileName;

		Time::microseconds m_StartPoint;
		Time::microseconds m_ElapsedDuration;
		Jobs::threadID m_ThreadID;

		// TODO: Total memory used
		// TODO: Memory allocated
		// TODO: Memory deallocated
	};
	struct instrumentationSession
	{
		// TODO: Track memory allocations/deallocations
		// TODO: Potentially stop/pause a profiling session when a job is taken off and waiting for other 
		std::string m_SessionName;
	};

	class instrumentor
	{
	public:
		instrumentor() : m_CurrentSession(nullptr) {}	// Private default constructor
		~instrumentor() { endSession(); }

		instrumentor(const instrumentor&) = delete;	// Delete copy constructor
		instrumentor(instrumentor&&) = delete;		// Delete move constructor

		void startSession(const std::string& sessionName, const std::string& filepath);
		void endSession();

		void writeProfile(const profileResult& result);

		inline static instrumentor* getInstance() 
		{
			// NOTE: instrumentor uses this type of singleton appraoch because stack objects that are deconstructed still need profiling
			static instrumentor instance;
			return &instance;
		}
	private:
		void internalEndSession();

		void writeSessionHeader();
		void writeSessionFooter();
	private:
		instrumentationSession* m_CurrentSession;
		Locks::spinLock m_Lock;
		IO::textFile m_OutputFile;
	};

	class instrumentorStopwatch
	{
	public:
		instrumentorStopwatch(const char* name) : m_Name(name)
		{
			// Start the stopwatch
			stopwatch.start();
		}
		~instrumentorStopwatch()
		{
			if (stopwatch.isRunning())
			{
				stop();

				// Output the 
			}
		}

		void stop()
		{
			stopwatch.pause();

			instrumentor::getInstance()->writeProfile({
				m_Name,
				Time::microseconds(stopwatch.startTick().time_since_epoch()),
				stopwatch.elapsedMicrosecondsDuration(),
				GRAVEngine::Jobs::jobManager::getInstance() == nullptr ? UINT32_MAX : GRAVEngine::Jobs::jobManager::getInstance()->getCurrentThreadID()
				});
		}
	private:
		const char* m_Name;			// Name of instrumented line
		Time::stopwatch stopwatch;	// Timer
	};

}

#define GRAV_PROFILE_LOG_CONSOLE 0
#define GRAV_PROFILE 1
#if GRAV_PROFILE
	#define GRAV_PROFILE_START_SESSION(name, filepath) GRAVEngine::instrumentor::getInstance()->startSession(name, filepath)
	#define GRAV_PROFILE_END_SESSION() GRAVEngine::instrumentor::getInstance()->endSession()

#if GRAV_PROFILE_LOG_CONSOLE
	#define GRAV_PROFILE_SCOPE_LINE2(name, line) GRAVEngine::instrumentorStopwatch stopwatch##line(name);\
												 if (Logging::logManager::getInstance()) GRAV_LOG_LINE_TRACE("Profile Function: %s", name)
#else
	#define GRAV_PROFILE_SCOPE_LINE2(name, line) GRAVEngine::instrumentorStopwatch stopwatch##line(name);
#endif
	#define GRAV_PROFILE_SCOPE_LINE(name, line) GRAV_PROFILE_SCOPE_LINE2(name, line)
	#define GRAV_PROFILE_SCOPE(name) GRAV_PROFILE_SCOPE_LINE(name, __LINE__)
	#define GRAV_PROFILE_FUNCTION() GRAV_PROFILE_SCOPE(GRAV_CLEAN_FUNC_SIG)
#else
	#define GRAV_PROFILE_START_SESSION(name, filepath)
	#define GRAV_PROFILE_END_SESSION()
	#define GRAV_PROFILE_SCOPE(name)
	#define GRAV_PROFILE_FUNCTION()
#endif
