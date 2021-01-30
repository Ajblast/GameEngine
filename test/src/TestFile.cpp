#include "common.h"

//#define NOMINMAX
//#include <Windows.h>
//#include <PathCch.h>
//#include <Shlwapi.h>

#include "jobs/jobs.h"
#include "jobs/jobManager.h"
#include "logging/sinks/sinks.h"

#include <iostream>

GRAVEngine::Logging::logManager logManager;
GRAVEngine::jobs::jobManager jobManager;


void endingJob(uintptr_t)
{
	GRAV_LOG_LINE_CRITICAL("This is a job to start shutting down the job manager");
	GRAVEngine::jobs::jobManager::getInstance()->startShutdown();
}
void waitJob(uintptr_t)
{
	int counter = 10;
	while (counter != 0)
	{
		GRAV_LOG_LINE_CRITICAL("Waiting...");
		std::this_thread::sleep_for(std::chrono::seconds(1));
		counter--;
	}
	GRAV_LOG_LINE_CRITICAL("After waiting");
	
	GRAVEngine::jobs::jobManager::getInstance()->kickJob({ endingJob, 0,  GRAVEngine::jobs::jobPriority::HIGH, nullptr });
}


void mainMethod()
{
	GRAVEngine::jobs::jobManager::getInstance()->kickJob({ waitJob, 0,  GRAVEngine::jobs::jobPriority::NORMAL, nullptr });


	auto job = [](uintptr_t value)
	{
		GRAV_LOG_LINE_CRITICAL("This is a test job entry point: %d", value);
	};
	GRAVEngine::jobs::counter* counter = new GRAVEngine::jobs::counter();

	GRAV_LOG_LINE_CRITICAL("Kick Jobs");
	for (size_t i = 0; i < 30; i++)
	{
		// Kick a job
		GRAVEngine::jobs::jobManager::getInstance()->kickJob({ job, i,  GRAVEngine::jobs::jobPriority::NORMAL, counter });
	}
	GRAV_LOG_LINE_CRITICAL("After Kick Job");

	//GRAVEngine::jobs::jobManager::getInstance()->waitForCounter(counter, 0);
	//GRAV_LOG_LINE_CRITICAL("After Waiting on counter");
	delete counter;
}

int main()
{
#pragma region PathStuff
	////wchar_t pathBuffer[MAX_PATH_LENGTH];
	//char pathBuffer[MAX_PATH_LENGTH];

	////wchar_t* string = (wchar_t*) GetModuleFileName(nullptr, pathBuffer, sizeof(pathBuffer) / 2);
	//GetModuleFileNameA(nullptr, pathBuffer, sizeof(pathBuffer) / 2);
	////PathCchRemoveFileSpec(string, MAX_PATH_LENGTH);

	//char dirBuffer[MAX_PATH_LENGTH];

	//_splitpath(pathBuffer, nullptr, dirBuffer, nullptr, nullptr);

	//std::stringstream ss;
	//ss << dirBuffer;
	//ss << "testFile.txt";

	//std::cout << ss.str().c_str() << std::endl;

	//std::string filePath = ss.str();
#pragma endregion

#pragma region SettingUpLogging
	// Create a default logger with a file sink and a sink to the msvc console
	auto fileSink = std::make_shared<GRAVEngine::Logging::Sinks::fileSink>("A:\\Users\\Austin Kincer\\Source\\Repos\\Ajblast\\GameEngine\\test\\test.txt");
	auto msvcSink = std::make_shared<GRAVEngine::Logging::Sinks::msvcSink>();
	auto coutSink = std::make_shared<GRAVEngine::Logging::Sinks::ostreamSink>(std::cout, true);

	GRAVEngine::Logging::logger logger = GRAVEngine::Logging::logger("Default", { fileSink, msvcSink, coutSink });
	// Have to set the verbosity when providing a logger for startup
	logger.setVerbosity(GRAVEngine::Logging::verbosity::trace);
	logger.setFlushVerbosity(GRAVEngine::Logging::verbosity::trace);

	// Start up the manager
	logManager.startUp(std::make_shared<GRAVEngine::Logging::logger> (logger));
	GRAV_LOG_LINE_INFO("%s: Log Manager Initialized", __FUNCTION__);
#pragma endregion

	GRAV_LOG_LINE_INFO("%s: Begin starting up managers", __FUNCTION__);

#pragma region SettingUpJobManager
	// Use default job manager options
	GRAVEngine::jobs::jobManagerOptions jobManagerOptions;
	jobManagerOptions.m_ShutDownAfterMain = false;

	// Start up the job manager
	jobManager.startUp(jobManagerOptions);
	GRAV_LOG_LINE_INFO("%s: Job Manager Initialized", __FUNCTION__);
#pragma endregion

	jobManager.runMain(mainMethod);


	GRAV_LOG_LINE_INFO("%s: Begin shutting down managers", __FUNCTION__);
	jobManager.shutDown();
	logManager.shutDown();
}

