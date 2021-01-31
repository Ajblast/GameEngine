#include "gravpch.h"
#include "gravThread.h"
#include "exceptions/threadCreationException.h"

GRAVEngine::Jobs::gravThread::gravThread() {}

void GRAVEngine::Jobs::gravThread::spawn(threadCallbackFunction callback)
{
	// Create the thread
#ifdef GRAV_PLATFORM_WINDOWS
	m_ThreadHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)callback, this, 0, (DWORD*) &m_ThreadID);
#endif

	// Throw an exception if a thread was not able to be created
	if (m_ThreadHandle == nullptr)
		throw GRAVEngine::Exceptions::threadCreationException("Unable to spawn thread.");
}

void GRAVEngine::Jobs::gravThread::setAffinity(size_t affinity)
{
	if (isValid() == false)
		return;

#ifdef GRAV_PLATFORM_WINDOWS
	// Mask the CPU and set the affinity
	DWORD_PTR mask = 1ull << affinity;
	SetThreadAffinityMask(m_ThreadHandle, mask);
#endif
}

void GRAVEngine::Jobs::gravThread::join()
{
	if (isValid() == false)
		return;

	// Join the thread handle
#ifdef GRAV_PLATFORM_WINDOWS
	WaitForSingleObject(m_ThreadHandle, INFINITE);
#endif
}

void GRAVEngine::Jobs::gravThread::initializeFromCurrentThread()
{
	// Get the current thread information
#ifdef GRAV_PLATFORM_WINDOWS
	m_ThreadHandle = GetCurrentThread();
	m_ThreadID = GetCurrentThreadId();
#endif
}

void GRAVEngine::Jobs::gravThread::sleepFor(uint32 ms)
{
#ifdef GRAV_PLATFORM_WINDOWS
	Sleep(ms);
#elif LINUX
	sleep(ms);
#endif
}