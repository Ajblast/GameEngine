#pragma once

#include "base.h"
#include "jobs/jobManager.h"
#include "logging/logManager.h"

namespace GRAVCore
{
	namespace Jobs
	{
		TEST_CLASS(jobManager)
		{
		public:
			TEST_METHOD_INITIALIZE(canStartUp);
			TEST_METHOD_CLEANUP(canShutdown);

			TEST_METHOD(canKickJob);
			TEST_METHOD(canKickJobWithCounter);
			TEST_METHOD(canKickJobs);
			TEST_METHOD(canKickJobsWithCounter);

			TEST_METHOD(canWaitForJob);
			TEST_METHOD(canWaitForJobs);
			TEST_METHOD(canWaitForMultipleCounters);

		private:
			inline void checkManagers() const
			{
				if (m_LogManager.isValid() == false)
					Assert::Fail(L"Log Manager invalid");
				else if (m_JobManager.isValid() == false)
					Assert::Fail(L"Job Manager invalid");
			}
		private:
			GRAVEngine::Jobs::jobManager m_JobManager;
			GRAVEngine::Jobs::jobManagerOptions m_Options;
			size_t m_JobCount = 10;
			GRAVEngine::Logging::logManager m_LogManager;
		};

		TEST_CLASS(jobManagerSingleThreaded)
		{
		public:
			TEST_METHOD_INITIALIZE(canStartUp);
			TEST_METHOD_CLEANUP(canShutdown);
			
			TEST_METHOD(canKickJob);
			TEST_METHOD(canKickJobWithCounter);
			TEST_METHOD(canKickJobs);
			TEST_METHOD(canKickJobsWithCounter);

			TEST_METHOD(canWaitForJob);
			TEST_METHOD(canWaitForJobs);
			TEST_METHOD(canWaitForMultipleCounters);

		private:
			inline void checkManagers() const
			{
				if (m_LogManager.isValid() == false)
					Assert::Fail(L"Log Manager invalid");
				else if (m_JobManager.isValid() == false)
					Assert::Fail(L"Job Manager invalid");
			}

		private:
			GRAVEngine::Jobs::jobManager m_JobManager;
			GRAVEngine::Jobs::jobManagerOptions m_Options;
			size_t m_JobCount = 10;
			GRAVEngine::Logging::logManager m_LogManager;
		};
	}
}