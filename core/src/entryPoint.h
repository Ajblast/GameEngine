#pragma once

#include "common.h"
#include "application.h"
#include "jobs/jobManager.h"
#include "logging/sinks/sinks.h"
#include "rendering/renderer/rendererCommand.h"
#include <iostream>

#include "debug/instrumentation/instrumentor.h"

#include "time/stopwatch.h"

#ifdef GRAV_PLATFORM_WINDOWS
int main(int argc, char** argv)
{
	GRAVEngine::Logging::logManager logManager;
	GRAVEngine::Jobs::jobManager jobManager;
	GRAVEngine::instrumentor instrumentor;

	GRAV_PROFILE_START_SESSION("Manager Startup", "GRAVEngineProfile-ManagerStartup.json");
#pragma region SettingUpLogging
	// Create a default logger with a file sink and a sink to the msvc console
	auto fileSink = GRAVEngine::createRef<GRAVEngine::Logging::Sinks::fileSink>("A:\\Development\\source\\Ajblast\\GameEngine\\test\\test.log");
	auto msvcSink = GRAVEngine::createRef<GRAVEngine::Logging::Sinks::msvcSink>();
	auto coutSink = GRAVEngine::createRef<GRAVEngine::Logging::Sinks::ostreamSink>(std::cout, true);

	GRAVEngine::Logging::logger logger = GRAVEngine::Logging::logger("Default", { fileSink, msvcSink, coutSink });
	// Have to set the verbosity when providing a logger for startup
	logger.setVerbosity(GRAVEngine::Logging::verbosity::trace);
	logger.setFlushVerbosity(GRAVEngine::Logging::verbosity::trace);

	// Start up the manager
	logManager.startUp(GRAVEngine::createRef<GRAVEngine::Logging::logger>(logger));
	GRAV_LOG_LINE_INFO("%s: Log Manager Initialized", GRAV_CLEAN_FUNC_SIG);
#pragma endregion

#pragma region StartingUpManagers
	// Use default job manager options
	GRAVEngine::Jobs::jobManagerOptions jobManagerOptions;

	// Start up the job manager
	GRAV_LOG_LINE_INFO("%s: Start Up JobManager", GRAV_CLEAN_FUNC_SIG);
	jobManager.startUp(jobManagerOptions);
	GRAV_LOG_LINE_INFO("%s: JobManager Initialized", GRAV_CLEAN_FUNC_SIG);
#pragma endregion
	GRAV_PROFILE_END_SESSION();

	GRAV_PROFILE_START_SESSION("Application Startup", "GRAVEngineProfile-ApplicationStartup.json");
	auto application = GRAVEngine::createApplication();
	GRAV_PROFILE_END_SESSION();

	// Run the application
	GRAV_PROFILE_START_SESSION("Runtime", "GRAVEngineProfile-RunTime.json");
	//jobManager.runMain( []() { });
	application->run();
	GRAV_PROFILE_END_SESSION();

	GRAV_PROFILE_START_SESSION("Application Shutdown", "GRAVEngineProfile-ApplicationShutdown.json");
	delete application;
	GRAV_PROFILE_END_SESSION();

#pragma region ShuttingDownManagers
	GRAV_PROFILE_START_SESSION("Manager Shutdown", "GRAVEngineProfile-ManagerShutdown.json");
	jobManager.startShutdown();
	jobManager.shutDown();
	logManager.shutDown();
	GRAV_PROFILE_END_SESSION();
#pragma endregion
}
#endif