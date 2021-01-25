#include "gravpch.h"
#include "gravThread.h"

GRAVEngine::jobs::gravThread::gravThread()
{
}

void GRAVEngine::jobs::gravThread::spawn(threadCallbackFunction callback)
{
	//m_ThreadHandle = nullptr;
	//m_ID = UINT32_MAX;
	//m_CallbackFunction = callback;

	// Create a thread 
	//m_Thread = std::thread((void*)callback, this);
	//m_Thread = std::thread(
	//	[callback, this](threadCallbackFunction a, GRAVEngine::jobs::gravThread* b)
	//	{
	//		a(b);
	//	}
	//);
	m_Thread = std::thread(
		[callback, this]()
		{
			callback(this);
		}
	);
}

void GRAVEngine::jobs::gravThread::setAffinity(size_t affinity)
{
#ifdef _WIN32
	// Mask the CPU and set the affinity
	DWORD_PTR mask = 1ull << affinity;
	SetThreadAffinityMask(m_Thread.native_handle(), mask);
#endif
}

void GRAVEngine::jobs::gravThread::join()
{
	m_Thread.join();
}

void GRAVEngine::jobs::gravThread::sleepFor(uint32 ms)
{
#ifdef _WIN32
	Sleep(ms);
#elif LINUX
	sleep(ms);
#endif
}