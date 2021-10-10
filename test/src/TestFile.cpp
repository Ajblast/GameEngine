#include "pch.h"
#include "common.h"

//#define NOMINMAX
//#include <Windows.h>
//#include <PathCch.h>
//#include <Shlwapi.h>

#include "jobs/jobs.h"

//GRAV_JOB_ENTRY_POINT(endingJob)
//{
//	GRAV_LOG_LINE_CRITICAL("This is a job to start shutting down the job manager");
//	GRAVEngine::Jobs::jobManager::getInstance()->startShutdown();
//}
//GRAV_JOB_ENTRY_POINT(waitJob)
//{
//	GRAV_LOG_LINE_INFO("Wait Job");
//	int counter = 10;
//	while (counter != 0)
//	{
//		GRAV_LOG_LINE_CRITICAL("Waiting...");
//		std::this_thread::sleep_for(std::chrono::seconds(1));
//		counter--;
//	}
//	GRAV_LOG_LINE_CRITICAL("Wait Job Ended");
//}


void mainMethod()
{
	//GRAV_LOG_LINE_INFO("Kick waiting jobs");
	//int jobCount = 32;
	//GRAVEngine::scope<GRAVEngine::Jobs::declaration[]> jobs = GRAVEngine::createScope<GRAVEngine::Jobs::declaration[]>(jobCount);
	//GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;

	//for (size_t i = 0; i < jobCount; i++)
	//	jobs[i] = GRAVEngine::Jobs::declaration(waitJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL);
	//GRAV_KICK_JOBS(jobs.get(), 32, counter);
	//GRAV_WAIT_COUNTER(counter, 0);


	//// Test lambda and passing value
	//GRAV_LOG_LINE_INFO("Kick lambda jobs");
	//auto job = [](uintptr_t value)
	//{
	//	GRAV_LOG_LINE_CRITICAL("This is a test job entry point: %d", value);
	//};
	//for (size_t i = 0; i < jobCount; i++)
	//	jobs[i] = GRAVEngine::Jobs::declaration(job, i);
	//GRAV_KICK_JOBS(jobs.get(), 32, counter);
	//GRAV_WAIT_COUNTER(counter, 0);
}