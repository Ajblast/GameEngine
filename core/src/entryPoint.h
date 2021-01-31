#pragma once

#include "common.h"
#include "application.h"
#include "jobs/jobManager.h"
#include "logging/sinks/sinks.h"
#include <iostream>

#ifdef GRAV_PLATFORM_WINDOWS
int main(int argc, char** argv)
{
	GRAVEngine::Logging::logManager logManager;
	GRAVEngine::Jobs::jobManager jobManager;

#pragma region SettingUpLogging
	// Create a default logger with a file sink and a sink to the msvc console
	auto fileSink = std::make_shared<GRAVEngine::Logging::Sinks::fileSink>("A:\\Users\\Austin Kincer\\Source\\Repos\\Ajblast\\GameEngine\\test\\test.log");
	auto msvcSink = std::make_shared<GRAVEngine::Logging::Sinks::msvcSink>();
	auto coutSink = std::make_shared<GRAVEngine::Logging::Sinks::ostreamSink>(std::cout, true);

	GRAVEngine::Logging::logger logger = GRAVEngine::Logging::logger("Default", { fileSink, msvcSink, coutSink });
	// Have to set the verbosity when providing a logger for startup
	logger.setVerbosity(GRAVEngine::Logging::verbosity::trace);
	logger.setFlushVerbosity(GRAVEngine::Logging::verbosity::trace);

	// Start up the manager
	logManager.startUp(std::make_shared<GRAVEngine::Logging::logger>(logger));
	GRAV_LOG_LINE_INFO("%s: Log Manager Initialized", __FUNCTION__);
#pragma endregion

#pragma region StartingUpManagers
	GRAV_LOG_LINE_INFO("%s: Begin starting up managers", __FUNCTION__);

	// Use default job manager options
	GRAVEngine::Jobs::jobManagerOptions jobManagerOptions;

	// Start up the job manager
	GRAV_LOG_LINE_INFO("%s: Start Up JobManager", __FUNCTION__);
	jobManager.startUp(jobManagerOptions);
	GRAV_LOG_LINE_INFO("%s: JobManager Initialized", __FUNCTION__);
#pragma endregion

	GRAV_LOG_LINE_INFO("%s: Create the Application", __FUNCTION__);
	auto application = GRAVEngine::createApplication();

	// Run the application
	GRAV_LOG_LINE_INFO("%s: Run the application", __FUNCTION__);
	//jobManager.runMain( []() { });
	application->run();

	GRAV_LOG_LINE_INFO("%s: Delete the Application", __FUNCTION__);
	delete application;

#pragma region ShuttingDownManagers
	GRAV_LOG_LINE_INFO("%s: Begin shutting down managers", __FUNCTION__);
	jobManager.startShutdown();
	jobManager.shutDown();
	logManager.shutDown();
#pragma endregion
}
#endif