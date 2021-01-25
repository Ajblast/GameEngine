#include "common.h"

#include "logging/sinks/sinks.h"

#define NOMINMAX
#include <Windows.h>
#include <PathCch.h>
#include <Shlwapi.h>

#include <iostream>

GRAVEngine::Logging::logManager manager;

int main()
{
#pragma region PathStuff
	//wchar_t pathBuffer[MAX_PATH_LENGTH];
	char pathBuffer[MAX_PATH_LENGTH];

	//wchar_t* string = (wchar_t*) GetModuleFileName(nullptr, pathBuffer, sizeof(pathBuffer) / 2);
	GetModuleFileNameA(nullptr, pathBuffer, sizeof(pathBuffer) / 2);
	//PathCchRemoveFileSpec(string, MAX_PATH_LENGTH);

	char dirBuffer[MAX_PATH_LENGTH];

	_splitpath(pathBuffer, nullptr, dirBuffer, nullptr, nullptr);

	std::stringstream ss;
	ss << dirBuffer;
	ss << "testFile.txt";

	std::cout << ss.str().c_str() << std::endl;

	std::string filePath = ss.str();
#pragma endregion


	//GRAVEngine::Logging::Sinks::fileSink fileSink(filePath.c_str());
	
	//const char* buffer = "This is a test buffer.";
	//char* readBuffer = new char[100];
	//size_t bufferSize;
	//
	//GRAVEngine::io::file testFile(ss.str().c_str(), GRAVEngine::io::fileMode::read, true);
	//std::cout << "File Size: " << testFile.fileSize() << std::endl;;
	//testFile.readAll((void*&)readBuffer, bufferSize);
	//
	//readBuffer[bufferSize] = 0;
	//
	//std::cout << "File Contents: " << readBuffer << std::endl;
	//
	//// Reopen the file in append mode
	//testFile.reopen(GRAVEngine::io::fileMode::append);
	//
	//testFile.write((void*) buffer, strlen(buffer));
	//testFile.flush();

	auto testFile = GRAVEngine::io::file(ss.str().c_str(), GRAVEngine::io::fileMode::append, false);
	std::cout << "File Path: " << testFile.filePath() << std::endl;
	testFile.close();

	manager.startUp({ std::make_shared<GRAVEngine::Logging::Sinks::fileSink>(ss.str().c_str()) });
	//auto test = std::make_shared<GRAVEngine::Logging::Sinks::fileSink>(ss.str().c_str(), false);
	//
	//GRAVEngine::Logging::logMessage message("", GRAVEngine::Logging::verbosity::critical, "This is a test payload");
	//
	//std::cout << "Test File Path: " << test->filePath() << std::endl;
	//
	//test->log(message);




	// manager.defaultLogger()->sinks().front();

	manager.defaultLogger()->log(GRAVEngine::Logging::verbosity::critical, "This is a test", "");
	manager.defaultLogger()->flush();

	// Test Assertion
	GRAV_ASSERT(1 == 2);
}

