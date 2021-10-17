#include "gravpch.h"
#include "instrumentor.h"
#include "jobs/jobManager.h"

void GRAVEngine::instrumentor::startSession(const std::string& sessionName, const std::string& filepath)
{
	Locks::scopedLock<decltype(m_Lock)> lock(m_Lock);
	if (m_CurrentSession)
	{
		// Attempting to start a new session while one is in progress.
		//	End the current session and start a new one

		// Check to make sure the logManager has been initialized
		if (Logging::logManager::getInstance())
			GRAV_LOG_LINE_ERROR("instrumentor::startSession('%s') when session '%s already open.", sessionName.c_str(), m_CurrentSession->m_SessionName.c_str());

		// End the current session
		internalEndSession();
	}

	// Start a new session if the file was opened property
	if (m_OutputFile.tryOpen(filepath, IO::fileMode::OUTPUT | IO::fileMode::TRUNCATE, true))
	{
		if (Logging::logManager::getInstance())
			GRAV_LOG_LINE_INFO("Start instrumentation session '%s'", sessionName.c_str());

		m_CurrentSession = new instrumentationSession({ sessionName });
		writeSessionHeader();
	}
	else
	{
		if (Logging::logManager::getInstance())
			GRAV_LOG_LINE_ERROR("Instrumentor could not open the results file '%s", filepath.c_str());
	}

}
void GRAVEngine::instrumentor::endSession()
{
	Locks::scopedLock<decltype(m_Lock)> lock(m_Lock);
	// End the session
	internalEndSession();
}
void GRAVEngine::instrumentor::writeProfile(const profileResult& result)
{
	Locks::scopedLock<decltype(m_Lock)> lock(m_Lock);

	if (m_CurrentSession)
	{
		std::stringstream json;

		auto elapsedDurationCount = result.m_ElapsedDuration.count();
		auto startPoint = result.m_StartPoint.count();

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << elapsedDurationCount << ',';
		json << "\"name\":\"" << result.m_ProfileName << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.m_ThreadID << ",";
		json << "\"ts\":" << startPoint;
		json << "}";

		// Output the file if the current session is a valid one
		m_OutputFile.write(json.str());
	}
}
void GRAVEngine::instrumentor::internalEndSession()
{
	if (m_CurrentSession)
	{
		if (Logging::logManager::getInstance())
			GRAV_LOG_LINE_INFO("End instrumentation session '%s'", m_CurrentSession->m_SessionName.c_str());

		// Write the footer and close the file
		writeSessionFooter();
		m_OutputFile.close();

		// Delete the current session
		delete m_CurrentSession;
		m_CurrentSession = nullptr;
	}
}

void GRAVEngine::instrumentor::writeSessionHeader()
{
	m_OutputFile.write("{\"otherData\": {},\"traceEvents\":[{}");
}
void GRAVEngine::instrumentor::writeSessionFooter()
{
	m_OutputFile.write("]}");
}

GRAVEngine::instrumentorStopwatch::instrumentorStopwatch(const char* name) : m_Name(name)
{
	// Start the stopwatch
	stopwatch.start();
}
GRAVEngine::instrumentorStopwatch::~instrumentorStopwatch()
{
	if (stopwatch.isRunning())
	{
		stop();

		// Output the 
	}
}
void GRAVEngine::instrumentorStopwatch::stop()
{
	stopwatch.pause();

	if (instrumentor::getInstance())
		instrumentor::getInstance()->writeProfile({
			m_Name,
			Time::microseconds(stopwatch.startTick().time_since_epoch()),
			stopwatch.elapsedMicrosecondsDuration(),
			GRAVEngine::Jobs::jobManager::getInstance() == nullptr ? UINT8_MAX : GRAVEngine::Jobs::jobManager::getInstance()->getCurrentThreadIndex()
			});
}
