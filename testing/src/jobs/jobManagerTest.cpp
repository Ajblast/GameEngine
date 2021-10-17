#include "pch.h"
#include "jobManagerTest.h"
#include "jobs/jobManager.h"
#include "time/stopwatch.h"

#include "logging/sinks/ostreamSink.h"
#include <iostream>

GRAV_JOB_ENTRY_POINT(dummyJob) {}
GRAV_JOB_ENTRY_POINT(dummyWaitJob)
{
	GRAVEngine::Jobs::gravThread::sleepFor(5); 
	std::stringstream ss;
	ss << param << ": Finish Waiting" << std::endl;
	Logger::WriteMessage(ss.str().c_str());
}

void GRAVCore::Jobs::jobManager::canStartUp()
{
	// Startup the log manager
	auto coutSink = GRAVEngine::createRef<GRAVEngine::Logging::Sinks::ostreamSink>(std::cout, true);
	GRAVEngine::Logging::logger logger = GRAVEngine::Logging::logger("Default", { coutSink });
	logger.setVerbosity(GRAVEngine::Logging::verbosity::trace);
	logger.setFlushVerbosity(GRAVEngine::Logging::verbosity::trace);
	m_LogManager.startUp(GRAVEngine::createRef<GRAVEngine::Logging::logger>(logger));

	m_Options = GRAVEngine::Jobs::jobManagerOptions();
	m_JobManager.startUp(m_Options);
}
void GRAVCore::Jobs::jobManager::canShutdown()
{	
	GRAVEngine::Jobs::jobManager::getInstance()->shutDown();
	GRAVEngine::Logging::logManager::getInstance()->shutDown();
	}


void GRAVCore::Jobs::jobManager::canKickJob()
{
	checkManagers();

	try
	{
		GRAVEngine::Jobs::declaration job = { dummyJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };
		m_JobManager.kickJob(job);
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		Assert::Fail(L"Couldn't kick job as queue was full");
	}
}
void GRAVCore::Jobs::jobManager::canKickJobWithCounter()
{
	checkManagers();
	
	try
	{
		GRAVEngine::Jobs::declaration job = { dummyJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };
		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;
		m_JobManager.kickJob(job, &counter);
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		Assert::Fail(L"Couldn't kick job as queue was full");
	}
}
void GRAVCore::Jobs::jobManager::canKickJobs()
{
	checkManagers();
	
	try
	{
		size_t m_JobCount = 50;
		GRAVEngine::scope<GRAVEngine::Jobs::declaration[]> jobs = GRAVEngine::createScope<GRAVEngine::Jobs::declaration[]>(m_JobCount);
		for (size_t i = 0; i < m_JobCount; i++)
			jobs[i] = { dummyJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };

		m_JobManager.kickJobs(jobs.get(), m_JobCount);
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		Assert::Fail(L"Couldn't kick jobs as queue was full");
	}
}
void GRAVCore::Jobs::jobManager::canKickJobsWithCounter()
{
	checkManagers();
	
	try
	{
		GRAVEngine::scope<GRAVEngine::Jobs::declaration[]> jobs = GRAVEngine::createScope<GRAVEngine::Jobs::declaration[]>(m_JobCount);
		for (size_t i = 0; i < m_JobCount; i++)
			jobs[i] = { dummyJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };

		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;
		m_JobManager.kickJobs(jobs.get(), m_JobCount, &counter);
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		Assert::Fail(L"Couldn't kick jobs as queue was full");
	}
}

void GRAVCore::Jobs::jobManager::canWaitForJob()
{
	checkManagers();
	
	GRAVEngine::Time::stopwatch sp;
	try
	{
		sp.start();
		GRAVEngine::Jobs::declaration job = { dummyWaitJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };
		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;
		m_JobManager.kickJob(job, &counter);

		m_JobManager.waitForCounter(counter, 0);
		sp.pause();
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		sp.pause();
		Assert::Fail(L"Couldn't kick job as queue was full");
	}

	std::stringstream ss;
	ss << "Wait Job Time: " << sp.elapsedMilliseconds();

	Logger::WriteMessage(ss.str().c_str());
}
void GRAVCore::Jobs::jobManager::canWaitForJobs()
{
	checkManagers();
	
	GRAVEngine::Time::stopwatch sp;
	try
	{
		sp.start();
		GRAVEngine::scope<GRAVEngine::Jobs::declaration[]> jobs = GRAVEngine::createScope<GRAVEngine::Jobs::declaration[]>(m_JobCount);
		for (size_t i = 0; i < m_JobCount; i++)
			jobs[i] = { dummyWaitJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };

		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;
		m_JobManager.kickJobs(jobs.get(), m_JobCount, &counter);
		sp.pause();
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		sp.pause();
		Assert::Fail(L"Couldn't kick jobs as queue was full");
	}

	std::stringstream ss;
	ss << "Wait Jobs Time: " << sp.elapsedMilliseconds();

	Logger::WriteMessage(ss.str().c_str());
}
void GRAVCore::Jobs::jobManager::canWaitForMultipleCounters()
{
	checkManagers();
	
	GRAVEngine::Time::stopwatch sp;
	try
	{
		sp.start();
		GRAVEngine::scope<GRAVEngine::Jobs::declaration[]> jobs = GRAVEngine::createScope<GRAVEngine::Jobs::declaration[]>(m_JobCount);
		for (size_t i = 0; i < m_JobCount; i++)
			jobs[i] = { dummyWaitJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };

		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;
		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter2;
		m_JobManager.kickJobs(jobs.get(), m_JobCount, &counter);
		m_JobManager.kickJobs(jobs.get(), m_JobCount, &counter2);

		m_JobManager.waitForCounter(counter, 0);
		m_JobManager.waitForCounter(counter2, 0);
		sp.pause();
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		sp.pause();
		Assert::Fail(L"Couldn't kick jobs as queue was full");
	}

	std::stringstream ss;
	ss << "Wait Multiple Counters Time: " << sp.elapsedMilliseconds();

	Logger::WriteMessage(ss.str().c_str());
}


void GRAVCore::Jobs::jobManagerSingleThreaded::canStartUp()
{
	// Startup the log manager
	auto coutSink = GRAVEngine::createRef<GRAVEngine::Logging::Sinks::ostreamSink>(std::cout, true);
	GRAVEngine::Logging::logger logger = GRAVEngine::Logging::logger("Default", { coutSink });
	logger.setVerbosity(GRAVEngine::Logging::verbosity::trace);
	logger.setFlushVerbosity(GRAVEngine::Logging::verbosity::trace);
	m_LogManager.startUp(GRAVEngine::createRef<GRAVEngine::Logging::logger>(logger));

	m_Options = GRAVEngine::Jobs::jobManagerOptions();
	m_Options.m_NumThreads = 1;

	m_JobManager.startUp(m_Options);
}
void GRAVCore::Jobs::jobManagerSingleThreaded::canShutdown()
{
	GRAVEngine::Jobs::jobManager::getInstance()->shutDown();
	GRAVEngine::Logging::logManager::getInstance()->shutDown();
}

void GRAVCore::Jobs::jobManagerSingleThreaded::canKickJob()
{
	checkManagers();
	
	try
	{
		GRAVEngine::Jobs::declaration job = { dummyJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };
		m_JobManager.kickJob(job);
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		Assert::Fail(L"Couldn't kick job as queue was full");
	}
}
void GRAVCore::Jobs::jobManagerSingleThreaded::canKickJobWithCounter()
{
	checkManagers();

	try
	{
		GRAVEngine::Jobs::declaration job = { dummyJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };
		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;
		m_JobManager.kickJob(job, &counter);
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		Assert::Fail(L"Couldn't kick job as queue was full");
	}
}
void GRAVCore::Jobs::jobManagerSingleThreaded::canKickJobs()
{
	checkManagers();
	
	try
	{
		GRAVEngine::scope<GRAVEngine::Jobs::declaration[]> jobs = GRAVEngine::createScope<GRAVEngine::Jobs::declaration[]>(m_JobCount);
		for (size_t i = 0; i < m_JobCount; i++)
			jobs[i] = { dummyJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };

		m_JobManager.kickJobs(jobs.get(), m_JobCount);
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		Assert::Fail(L"Couldn't kick jobs as queue was full");
	}
}
void GRAVCore::Jobs::jobManagerSingleThreaded::canKickJobsWithCounter()
{
	checkManagers();
	
	try
	{
		size_t m_JobCount = 50;
		GRAVEngine::scope<GRAVEngine::Jobs::declaration[]> jobs = GRAVEngine::createScope<GRAVEngine::Jobs::declaration[]>(m_JobCount);
		for (size_t i = 0; i < m_JobCount; i++)
			jobs[i] = { dummyJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };

		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;
		m_JobManager.kickJobs(jobs.get(), m_JobCount, &counter);
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		Assert::Fail(L"Couldn't kick jobs as queue was full");
	}
}

void GRAVCore::Jobs::jobManagerSingleThreaded::canWaitForJob()
{
	checkManagers();
	
	GRAVEngine::Time::stopwatch sp;
	try
	{
		sp.start();
		GRAVEngine::Jobs::declaration job = { dummyWaitJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };
		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;
		m_JobManager.kickJob(job, &counter);

		m_JobManager.waitForCounter(counter, 0);
		sp.pause();
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		sp.pause();
		Assert::Fail(L"Couldn't kick job as queue was full");
	}

	std::stringstream ss;
	ss << "Single Threaded Wait Job Time: " << sp.elapsedMilliseconds();

	Logger::WriteMessage(ss.str().c_str());
}
void GRAVCore::Jobs::jobManagerSingleThreaded::canWaitForJobs()
{
	checkManagers();
	
	GRAVEngine::Time::stopwatch sp;
	try
	{
		sp.start();
		GRAVEngine::scope<GRAVEngine::Jobs::declaration[]> jobs = GRAVEngine::createScope<GRAVEngine::Jobs::declaration[]>(m_JobCount);
		for (size_t i = 0; i < m_JobCount; i++)
			jobs[i] = { dummyWaitJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };

		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;
		m_JobManager.kickJobs(jobs.get(), m_JobCount, &counter);
		sp.pause();
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		sp.pause();
		Assert::Fail(L"Couldn't kick jobs as queue was full");
	}

	std::stringstream ss;
	ss << "Single Threaded Wait Jobs Time: " << sp.elapsedMilliseconds();

	Logger::WriteMessage(ss.str().c_str());
}
void GRAVCore::Jobs::jobManagerSingleThreaded::canWaitForMultipleCounters()
{
	checkManagers();
	
	GRAVEngine::Time::stopwatch sp;
	try
	{
		sp.start();
		GRAVEngine::scope<GRAVEngine::Jobs::declaration[]> jobs = GRAVEngine::createScope<GRAVEngine::Jobs::declaration[]>(m_JobCount);
		for (size_t i = 0; i < m_JobCount; i++)
			jobs[i] = { dummyWaitJob, 0, GRAVEngine::Jobs::jobPriority::NORMAL };

		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter;
		GRAVEngine::ref<GRAVEngine::Jobs::counter> counter2;
		m_JobManager.kickJobs(jobs.get(), m_JobCount, &counter);
		m_JobManager.kickJobs(jobs.get(), m_JobCount, &counter2);

		m_JobManager.waitForCounter(counter, 0);
		m_JobManager.waitForCounter(counter, 0);
		sp.pause();
	}
	catch (const GRAVEngine::Exceptions::jobQueueFullException& e)
	{
		sp.pause();
		Assert::Fail(L"Couldn't kick jobs as queue was full");
	}

	std::stringstream ss;
	ss << "Single Threaded Wait Multiple Counters Time: " << sp.elapsedMilliseconds();

	Logger::WriteMessage(ss.str().c_str());
}
