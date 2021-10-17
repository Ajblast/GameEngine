#include "gravpch.h"
#include "gravThread.h"
#include "exceptions/concurrency/threadCreationException.h"

GRAVEngine::Jobs::gravThread::gravThread() {}
GRAVEngine::Jobs::gravThread::~gravThread() {}

void GRAVEngine::Jobs::gravThread::spawn(threadCallback callback)
{
	// TODO: Instead of using CreateThread, turn this into _beginthreadex


	// Create the thread
	#ifdef GRAV_PLATFORM_WINDOWS
	//m_ThreadHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)callback.target<void(gravThread*)>(), this, 0, (DWORD*) &m_ThreadID);
	m_ThreadHandle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) callback, this, 0, (DWORD*) &m_ThreadID);
	#endif

	// Throw an exception if a thread was not able to be created
	if (m_ThreadHandle == NULL)
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
size_t GRAVEngine::Jobs::gravThread::getAffinity()
{
	if (isValid() == false)
		return 0;

	size_t affinity = 0;

	#ifdef GRAV_PLATFORM_WINDOWS
	size_t mask = 1;
	while (mask)
	{
		// Try each CPU to set the mask to get the original mask
		// This is done like this because the process itself could disallow a certain CPU and we need to succeed to get the original mask
		size_t old = SetThreadAffinityMask(m_ThreadHandle, mask);
		if (old)
		{
			SetThreadAffinityMask(m_ThreadHandle, old);
			affinity = old;
			break;
		}
		else
			if (GetLastError() != ERROR_INVALID_PARAMETER)
				throw Exceptions::threadAffinityException("Unable to retrieve original affinity mask");

		// Go to the next CPU
		mask <<= 1;
	}
	#endif

	// Affinities are set 0 based, so make this zero based
	return affinity - 1;
}

void GRAVEngine::Jobs::gravThread::setName(const std::wstring& name)
{
	if (isValid() == false)
		return;

	#ifdef GRAV_PLATFORM_WINDOWS
	PCWSTR threadName = (PCWSTR) name.c_str();
	SetThreadDescription(m_ThreadHandle, threadName);
	#endif
}
std::wstring GRAVEngine::Jobs::gravThread::getName() const
{
	if (isValid() == false)
		return L"";

	std::wstring threadName;

	#ifdef GRAV_PLATFORM_WINDOWS
	PWSTR data;
	HRESULT hr = GetThreadDescription(m_ThreadHandle, &data);

	if (SUCCEEDED(hr))
	{
		threadName = std::wstring(data);
		LocalFree(data);
	}
	#endif

	return threadName;
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