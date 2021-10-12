#pragma once

#include "common.h"
#include "application.h"
#include "logging/sinks/sinks.h"
#include "rendering/renderer/rendererCommand.h"
#include "ai/environmentManager.h"
#include "debug/instrumentation/instrumentor.h"
#include "time/stopwatch.h"

#include "io/input/inputManager.h"

#include <iostream>


#ifdef GRAV_PLATFORM_WINDOWS
int main(int argc, char** argv)
{
	GRAVEngine::Logging::logManager logManager;
	GRAVEngine::instrumentor instrumentor;
	//GRAVEngine::AI::trainer trainer;
	GRAVEngine::IO::inputManager inputManager;


	GRAV_PROFILE_START_SESSION("Manager Startup", "debug/profiles/GRAVEngineProfile-ManagerStartup.json");
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
	inputManager.startup();
#pragma endregion
	GRAV_PROFILE_END_SESSION();

	GRAV_PROFILE_START_SESSION("Application Startup", "debug/profiles/GRAVEngineProfile-ApplicationStartup.json");
	auto application = GRAVEngine::createApplication();
	GRAV_PROFILE_END_SESSION();

	// Run the application
	GRAV_PROFILE_START_SESSION("Runtime", "debug/profiles/GRAVEngineProfile-RunTime.json");
	application->run();
	GRAV_PROFILE_END_SESSION();

	GRAV_PROFILE_START_SESSION("Application Shutdown", "debug/profiles/GRAVEngineProfile-ApplicationShutdown.json");
	delete application;
	GRAV_PROFILE_END_SESSION();

#pragma region ShuttingDownManagers
	GRAV_PROFILE_START_SESSION("Manager Shutdown", "debug/profiles/GRAVEngineProfile-ManagerShutdown.json");
	// Deinitialize the trainer
	GRAVEngine::AI::environmentManager::instance().deinitialize();

	inputManager.shutdown();
	logManager.shutDown();
	GRAV_PROFILE_END_SESSION();
#pragma endregion
}
#endif