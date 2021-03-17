#include "gravpch.h"
#include "logging.h"

// TODO: Allow time stamping and allowing categories
void GRAVEngine::Logging::reportAssertionFailure(const char* expression, const char* file, size_t line)
{
	// Get the logger
	auto logger = GRAVEngine::Logging::logManager::getInstance().defaultLogger();

	// Concatenate the strings
	std::stringstream ss;
	ss << "Expression: " << expression << " | File: " << file << " | Line: " << line;

	// Log the assertion
	logger->logLine(GRAVEngine::Logging::verbosity::critical, ss.str().c_str());
}